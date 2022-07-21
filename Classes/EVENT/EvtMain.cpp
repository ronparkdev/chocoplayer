#include "EvtMain.h"

#include "../COMMON/ScreenCommon.h"
#include "../DATA/LMU/LmuEventPage.h"
#include "../DATA/LSD/LsdMapEventStack.h"
#include "../SCENE/MAP/ScrMapPublic.h"
#include "../SCENE/MAP/ScrMap.h"
#include "../COMMON/GlobalLangFont.h"
#include "../COMMON/MgrSetting.h"

int EvtMain::g_event_run_count = 0;

#define EVT_RUNNING_ON_FRAME 3300

//#define EVT_MAIN_DEBUG

EvtMain::EvtMain( GameMain *main, int mapid, LmuMain *mapdata, CCNode *layer )
	:mst(main), mch(main), mit(main), mtele(main)

	,pGM(main)
	,curMapId(mapid)
	,pMapData(mapdata)
	,pLayer(layer)
	,pEvtStack(&(pGM->lsd.EventMap))
	,isBattle(false)
	,isMsgEndChecking(false)
{
	initMsg();
	initDebug();

	memset(mapEventRealId, -1, sizeof(mapEventRealId));
	for(int i=0; i<pMapData->Events.GetLength(); i++) {
		int index = pMapData->Events[i].getIndex();

		if (0<= index && index < 10000) 
			mapEventRealId[index] = i;
	}
}

EvtMain::EvtMain( GameMain *main, LsdMapEventStack *stack, CCNode *layer, ScrBattleBattlerMgr *battler )
	:mst(main), mch(main), mit(main), mtele(main)
	,pGM(main)
	,pLayer(layer)
	,pEvtStack(stack)
	,pBatrMgr(battler)
	,isBattle(true)
	,isMsgEndChecking(false)
	,batLastActId(0)
{
	for(int i=0; i<SCR_BATTLE_MAX_HEROS; i++) {
		batLastActType[i] = 0;
		batLastTgtId[i] = 0;
	}

	initMsg();
	initDebug();
}

EvtMain::~EvtMain() {
	destroyMsg();

	pLayer->removeChild(pDebugLabel);
}


void EvtMain::initDebug()
{
	pDebugLabel = CCLabelTTF::create("", GlobalLangFont::getDefFontName(), 9);
	pDebugLabel->setPosition(ccp(100/ScreenCommon::game_s+1, ScreenCommon::game_h - (100)/ScreenCommon::game_s));
	pDebugLabel->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	pDebugLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	pDebugLabel->setDimensions(CCSize(200, 200));
	pDebugLabel->setScale(1.0f/ScreenCommon::game_s);
	pDebugLabel->setVisible(true);
	pLayer->addChild(pDebugLabel, sz_controler+100);
}

void EvtMain::reloadShaders()
{
	pFont->reloadShaders();

	for(int i=0; i<6; i++)
		pFontNum[i]->reloadShaders();
}


void EvtMain::doEvents(int framedelta) {
	LsdMain &lsd = pGM->lsd;

	bool result;

	initClock();

	drawMsg(framedelta);

	chkParallelEventCommon(framedelta);	
#ifdef EVT_MAIN_DEBUG
	defClockPrint("Event_Parallel_Common");
#endif
	
	drawDebug();

#ifdef EVT_MAIN_DEBUG
	defClockPrint("Event_PrintDebug");
#endif
	int running_count = 0;

	while(true) {
		if (pEvtStack->data.GetLength() != 0) {
			if (msgCalledEvtStack == pEvtStack && isMsgEndChecking) {
				endMsg(pEvtStack);
			}
			if (pEvtStack->wait_left > 0) {
				pEvtStack->wait_left -= framedelta;
				break;
			}
			if (msgCalledEvtStack != pEvtStack || !isMsgEndChecking) {
				while(pEvtStack->data.GetLength() != 0) {
					result = doEvent(pEvtStack);
					if (!result || ++running_count >= EVT_RUNNING_ON_FRAME) {
						return;
					}
				}
			}
			if (isMsgEndChecking)
				break;
		}
		if (pEvtStack->data.GetLength() == 0) {
			if (!isBattle && curMapId == pGM->curMapId)
				findSerialEvent();
			if (pEvtStack->data.GetLength() == 0)
				break;
		}
	}
#ifdef EVT_MAIN_DEBUG
	defClockPrint("Event_Serial");
#endif

	chkParallelEventMap(framedelta);	
#ifdef EVT_MAIN_DEBUG
	defClockPrint("Event_Parallel_Map");
#endif
}

void EvtMain::findSerialEvent() {

	//if (m_main->curmapid != m_mapid)
	//	return;

	LsdMapEventStack &stack = pGM->lsd.EventMap;
	ArrayList<LmuEvent> &eventsM = pMapData->Events;
	ArrayList<LsdMapEvent> &eventsS = pGM->lsd.Map.events;
	
	if (isEvtSerialCommonTurn) {
		ArrayList<LdbCommonEvent> &eventsC = pGM->ldb.dComEvt;
		for(int i=0; i<eventsC.GetLength(); i++) {
			LdbCommonEvent &event = eventsC[i];
			if (event.trigger == CommonEvtTrigger_Automatic && 
				(!event.switch_flag || mst.getSw(event.switch_id))) {
					addEventInStack(-i, stack, event.event_data);
					break;
			}
		}
	} else {
		//for(int i=eventsM.GetLength()-1; i>=0; i--) {
		for(int i=0; i<eventsM.GetLength(); i++) {
			LmuEvent &event = eventsM[i];

			int pageid = eventsS[i].serial_event_page-1;
			if (pageid == -1)
				pageid = ScrMapPublic::getEventPage(pGM, eventsM[i]);

			if (eventsS[i].isUsed && 0 <= pageid) {
				LmuEventPage &page = event.Page[pageid];

				if ((page.trigger_type == MapEvtTrigger_Autometic || eventsS[i].serial_event_queue)) {

					eventsS[i].serial_event_page = 0;
					eventsS[i].serial_event_queue = false;

					if (page.event_data.data.GetLength() > 1) {
						addEventInStack(eventsM[i].getIndex(), stack, page.event_data);
						break;
					}
				}
			}
		}
	}

	isEvtSerialCommonTurn = !isEvtSerialCommonTurn;
}

void EvtMain::addEventInStack(int eventid, LsdMapEventStack &stack, GlbEvent &event) {
	stack.data.ChangeMode(ListMode);

	bool isHaveGarbage = stack.data.isHaveGarbage();
	LsdMapEventData &save = stack.data.Add();
	if (!isHaveGarbage || save.pevent != &event) {//150507 2.15 optimization
		save.event = event;
		save.pevent = &event;
		save.event_length = event.data.GetLength();
	}
	save.event_id = eventid;
	save.eventpos = 0;

	if (stack.data.GetLength() == 1)
		stack.wait_left = 0;

	stack.m_wait_key = 0;
}

void EvtMain::addMsgInStack(LsdMapEventData &edatas, GlbEventNode &edata, ArrayList<TString> &msg) {
	GlbEventNode evtnode;
	edatas.pevent = NULL;
	evtnode.indent = edata.indent;
	int startpos = edatas.eventpos + 1;
	for(int i=0; i<msg.GetLength(); i++) {
		evtnode.code = (i%4==0)?ecShowMessage:ecShowMessage_2;
		evtnode.string = msg[i];
		edatas.event.data.Add(startpos, evtnode);
		startpos++;
	}

	edatas.event_length += msg.GetLength();
}

void EvtMain::chkParallelEventCommon(int framedelta) {
	int running_count;
	bool result;

	ArrayList<LdbCommonEvent> &eventsC = pGM->ldb.dComEvt;
	for(int i=0; i<eventsC.GetLength(); i++) {
		LdbCommonEvent &event = eventsC[i];
		LsdMapEventStack *stack = &(pGM->lsd.EventCommon[i].eventstack);

		if (stack->data.GetLength() == 0 &&
			event.trigger == MapEvtTrigger_Parallel && 
			(!event.switch_flag || mst.getSw(event.switch_id))) {
				addEventInStack(-i-1, *stack, event.event_data);
		}

		running_count = 0;
		while(stack->data.GetLength() != 0 && running_count < EVT_RUNNING_ON_FRAME) {
			if (msgCalledEvtStack == stack && isMsgEndChecking)
				endMsg(stack);

			if (stack->wait_left > 0) {
				if (event.switch_flag & !mst.getSw(event.switch_id)) {
					//FIXED 2.04 : Only In CommonEvent, Shutdown event when trigger is off
					stack->data.RemoveAll();
				}
				stack->wait_left -= framedelta;
				break;
			}
			if (msgCalledEvtStack != stack || !isMsgEndChecking) {
				while(stack->data.GetLength() != 0) {
					result = doEvent(stack);
					if (!result && event.switch_flag & !mst.getSw(event.switch_id)) {
						//FIXED 2.04 : Only In CommonEvent, Shutdown event when trigger is off
						stack->data.RemoveAll();
					}
					if (!result || ++running_count >= EVT_RUNNING_ON_FRAME) {
						running_count = EVT_RUNNING_ON_FRAME;
						break;
					}
				}
			}

			if (isMsgEndChecking)
				break;
		}

		if (msgCalledEvtStack == stack && stack->data.GetLength() == 0)
			endMsg(stack);
	}

	//defClockPrint("Event_Parallel_Common");

}

void EvtMain::chkParallelEventMap(int framedelta) {
	int running_count;
	bool result;

	if (!isBattle) {
		ArrayList<LmuEvent> &eventsM = pMapData->Events;
		ArrayList<LsdMapEvent> &eventsS = pGM->lsd.Map.events;
		for(int i=0; i<eventsM.GetLength(); i++) {
			LmuEvent &event = eventsM[i];
			LsdMapEventStack *stack = &(pGM->lsd.Map.events[i].eventstack);

			int pageid = ScrMapPublic::getEventPage(pGM, eventsM[i]);

			if (eventsS[i].isUsed && stack->data.GetLength() == 0 && 0 <= pageid) {
				LmuEventPage &page = event.Page[pageid];

				if (page.trigger_type == MapEvtTrigger_Parallel) {
					addEventInStack(event.getIndex(), *stack, page.event_data);
					eventsS[i].parallel_event_page = pageid + 1;
				}
			}

			running_count = 0;
			while(stack->data.GetLength() != 0 && running_count < EVT_RUNNING_ON_FRAME) {
				if (msgCalledEvtStack == stack && isMsgEndChecking)
					endMsg(stack);

				if (stack->wait_left > 0) {
					if (eventsS[i].parallel_event_page != pageid + 1) {
						//FIXED 2.15 : Shutdown event when trigger is not correct
						stack->data.RemoveAll();
					}
					stack->wait_left -= framedelta;
					break;
				}
				if (msgCalledEvtStack != stack || !isMsgEndChecking) {
					while(stack->data.GetLength() != 0) {
						result = doEvent(stack);
						if (!result && msgCalledEvtStack == stack && 
							(eventsS[i].parallel_event_page != pageid + 1)) {
								//FIXED 2.15 : Shutdown event when trigger is not correct
								stack->data.RemoveAll();
						}
						if (!result || (++running_count >= EVT_RUNNING_ON_FRAME) || 
							(eventsS[i].parallel_event_page != pageid + 1)) {
								//FIXED 2.15 : Shutdown event when trigger is not correct

								running_count = EVT_RUNNING_ON_FRAME;
								break;
						}
					}
				}


				if (isMsgEndChecking)
					break;
			}
			if (msgCalledEvtStack == stack && stack->data.GetLength() == 0)
				endMsg(stack);

			if (curMapId != pGM->curMapId)
				break;
		}
	}
}

bool EvtMain::isRuningSerial()
{
	return (pEvtStack->data.GetLength() > 0 || usingMsg());
}

void EvtMain::doEvent( GlbEvent &event )
{
	addEventInStack(0, *pEvtStack, event);
}

void EvtMain::doTouchByHero( int eventid, int pageid )
{
	if (pEvtStack->data.GetLength() > 0 && 
		pEvtStack->data[0].event_id == eventid)
			return;

	LsdMapEventStack &stack = pGM->lsd.EventMap;
	LmuEventPage & page = pMapData->Events[eventid].Page[pageid];
	switch(page.trigger_type) {
	case MapEvtTrigger_touchevent:
	case MapEvtTrigger_touchhero:
		LsdMapEvent &mapevent = pGM->lsd.Map.events[eventid];
		mapevent.serial_event_queue = true;
		mapevent.serial_event_page = pageid + 1;
	}
}

void EvtMain::doTouchByEvent( int eventid, int pageid )
{
	if ((pEvtStack->data.GetLength() > 0 && pEvtStack->data[0].event_id == eventid) ||
		eventid < 0 || pageid < 0)
		return;

	LsdMapEventStack &stack = pGM->lsd.EventMap;
	LmuEventPage & page = pMapData->Events[eventid].Page[pageid];
	if (page.trigger_type == MapEvtTrigger_touchevent) {
		LsdMapEvent &mapevent = pGM->lsd.Map.events[eventid];
		mapevent.serial_event_queue = true;
		mapevent.serial_event_page = pageid + 1;
	}
}

void EvtMain::doClickEvent( int eventid, int pageid )
{
	if ((pEvtStack->data.GetLength() > 0 && pEvtStack->data[0].event_id == eventid) ||
		eventid < 0 || pageid < 0)
			return;

	LsdMapEventStack &stack = pGM->lsd.EventMap;

	LmuEventPage & page = pMapData->Events[eventid].Page[pageid];
	if (page.trigger_type == MapEvtTrigger_key ||
		(page.draw_priority == drawpri_mid && 
		(page.trigger_type == MapEvtTrigger_touchevent || page.trigger_type == MapEvtTrigger_touchhero))) {
			LsdMapEvent &mapevent = pGM->lsd.Map.events[eventid];
			mapevent.serial_event_queue = true;
			mapevent.serial_event_page = pageid + 1;
	}
}

void EvtMain::setLastActID(int actID) {
	batLastActId = actID;
}

void EvtMain::setLastAction( int actID, int actType, int tgtID )
{
	if (actID < 0 || actID >= SCR_BATTLE_MAX_HEROS)
		return;

	batLastActType[actID] = actType;
	batLastTgtId[actID]= tgtID;
}

int EvtMain::getRunningSerialMapID()
{
	if (pEvtStack->data.GetLength() == 0) {
		return -1;
	} else {
		int index = pEvtStack->data[0].event_id;
		for(int i=0; i<pMapData->Events.GetLength(); i++) {
			if (pMapData->Events[i].getIndex() == index)
				return i+4;
		}
	}
	
	return 0;
}

LsdMapEventStack* EvtMain::getLastStack( evtcode code )
{
	LsdMapEventStack *stack;
	if ((stack = getLastStack(code, &(pGM->lsd.EventMap))) != NULL)
		return stack;

	ArrayList<LdbCommonEvent> &eventsC = pGM->ldb.dComEvt;
	for(int i=0; i<eventsC.GetLength(); i++) {
		LdbCommonEvent &event = eventsC[i];
		LsdMapEventStack *stack = &(pGM->lsd.EventCommon[i].eventstack);

		if ((stack = getLastStack(code, pEvtStack)) != NULL)
			return stack;
	}

	ArrayList<LsdMapEvent> &eventsS = pGM->lsd.Map.events;
	for(int i=0; i<pGM->lsd.Map.events.GetLength(); i++) {
		LsdMapEventStack *stack = &(pGM->lsd.Map.events[i].eventstack);

		if ((stack = getLastStack(code, pEvtStack)) != NULL)
			return stack;
	}

	return NULL;
}

LsdMapEventStack* EvtMain::getLastStack( evtcode code, LsdMapEventStack *stack) {
	if (stack == NULL || stack->data.GetLength() == 0)
		return NULL;

	LsdMapEventData *data = stack->data.GetTail();

	if (data->event.data[data->eventpos].code == code)
		return stack;
	else
		return NULL;
}

void EvtMain::drawDebug()
{
	MgrSetting *setting = MgrSetting::getInstance();

	bool visible = setting->getBool(RST_ETC_DEBUG);

	if (!visible) {
		pDebugLabel->setVisible(false);
	} else {
		if (pEvtStack->data.GetLength() > 0) {
			TString debugnum;
			if (pEvtStack->data[0].event_id >= 0) {
				int eventid = pEvtStack->data[0].event_id;
				debugnum.setText(eventid, 3);
				pDebugLabel->setString(("M" + debugnum + "/" + pEvtStack->data[0].eventpos).getTextUTF8());
			} else {
				debugnum.setText((-pEvtStack->data[0].event_id), 3);
				pDebugLabel->setString(("C" + debugnum + "/" + pEvtStack->data[0].eventpos).getTextUTF8());
			}
			pDebugLabel->setVisible(true);
		} else {
			if (msgCalledEvtData != NULL) {
				TString debugnum;
				debugnum.setText(-msgCalledEvtData->event_id, 3);
				pDebugLabel->setString(("K" + debugnum + "/" + msgCalledEvtData->eventpos).getTextUTF8());

				pDebugLabel->setVisible(true);
			} else {
				pDebugLabel->setVisible(false);
			}
		}
	}
}
