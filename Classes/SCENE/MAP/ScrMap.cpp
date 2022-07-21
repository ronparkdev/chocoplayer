#include "ScrMap.h"

#include "../../AppMultiTime.h"
#include "../../DATA/LMU/LmuEventPage.h"
#include "../../GRAPHIC/OBJECT/GrpMsgbox.h"
#include "../../GRAPHIC/OBJECT/GrpSelection.h"
#include "../../COMMON/GlobalLangFont.h"
#include "../../COMMON/MgrSetting.h"

ScrMap::ScrMap( GameMain *main , int mapid , bool init)
	:pMapTileChecked(NULL) ,mapTileLen(0)
	,scrX(0) ,scrY(0)
{
	pGM	= main;
	curMapId = mapid;
	isInited	= init;

	TString path;
	path.setText(mapid, 4);
	path = "Map" + path + ".lmu";

	char *data;
	int datasize;
	try{
		StructLoader sl;
		if (	pGM->fctl.getData(fileFolder, path, &data, &datasize) &&
			sl.openWithNoFree(data, datasize)) {
			mapData.serialize(sl);
			sl.close();
		}
	}catch(...) {
		return;
	}

	main->lmtmgr.doMapChanged(mapid);

	if (init) {
		StructLoader sl_init;
		sl_init.setInitMode();

		ArrayList<LsdMapEventData> &mapserial = pGM->lsd.EventMap.data;
		for(int i=0; i<mapserial.GetLength(); i++)
			mapserial[i].event_id = 0;

		ArrayList<LsdMapEvent> &mapevent = main->lsd.Map.events; 
		mapevent.SetLength(mapData.Events.GetLength());
		for(int i=0; i<mapevent.GetLength(); i++)
			mapevent[i].eventstack.data.RemoveAll();

// 		ArrayList<LsdCommonEvent> &commonevent = main->lsd.EventCommon; 
// 		commonevent.SetLength(main->ldb.dComEvt.GetLength());
// 		for(int i=0; i<commonevent.GetLength(); i++)
// 			commonevent[i].eventstack.data.RemoveAll();

		main->lsd.Map.serialize(sl_init);

		for(int i=0; i<main->lsd.Picture.GetLength(); i++)
			main->lsd.Picture[i].picName = "";

		LmtMapInfo &minfo = main->lmtmgr.getInfo();

		main->lsd.System.able_escape	= (minfo.escape == 1);
		main->lsd.System.able_save		= (minfo.save == 1);
		main->lsd.System.able_teleport	= (minfo.teleport == 1);

		if (minfo.music_type == 2)
			main->snd.PlayMusic(minfo.music);


		LsdScreen &screen = pGM->lsd.Screen;

		screen.screen_adjust_x = 0;
		screen.screen_adjust_y = 0;
	} else {
		main->snd.PlayMusic(pGM->lsd.System.music_current);

		for(int i=0; i<GRP_PICTURE_MAX; i++)
			pGM->lsd.Picture[i].need_picname_check = true;
	}

	pGM->lsd.LocHero.map_id = mapid;

	pEvt = new EvtMain(pGM, mapid, &mapData, this);

// 	ScreenCommon::game_screen_x = 0.0f;
// 	ScreenCommon::game_screen_y = 0.0f;
	lastOriginalScrX = 0.0f;
	scrX = scrY = 0.0;
	isScrNeedInit = true;

	createMapChipset();
	createMapEvents(init); 
	createBackGround();
	initEffect(init);
	initWalk(init);

	pPic = new GrpPicture(pGM);
	this->addChild(pPic, sz_picture);

	pAni = new GrpAniMap(pGM);
	this->addChild(pAni, sz_animation);

	pTimer = new GrpComTimer(pGM);
	this->addChild(pTimer, sz_timer);

	pGM->lsd.System.screen = scr_map;

	debug_time_doevents = debug_time_checkkeys = debug_time_drawevents = debug_time_drawchipset
		= debug_time_drawevents2 = debug_time_draweffect = debug_time_drawbg
		= debug_time_chkride = debug_time_drwanimation = debug_time_drawtimer = 0;
	//initDebug();
	reloadSetting();
}


void ScrMap::Init()
{
	initMapChipset();
	initBackGround(isInited);
	lastOriginalScrX = 0.0f;
}

void ScrMap::reloadShaders()
{
	CCLog("ScrMap::reloadShaders()");
	bgShader.reload();

	mapShader.reload();

	for(int i=0; i<mapEvtSprs.GetLength(); i++)
		mapEvtSprs[i].reloadShaders();

	pAni->reloadShaders();
	pEvt->reloadShaders();
}

void ScrMap::reloadSetting()
{
// 	MgrSetting *s = MgrSetting::getInstance();
// 	m_debuglabel->setVisible(s->getBool(RST_ETC_DEBUG));
}

ScrMap::~ScrMap()
{
	delete pEvt;

	this->removeChild(pPic);
	this->removeChild(pAni);
	this->removeChild(pTimer);

	destroyMapEvents();

	destroyMapChipset();

	destroyBackGround();

	destroyEffect();

	this->removeChild(m_debuglabel);
}

void ScrMap::Draw(int framedelta)
{
	int frame = (pGM->lsd.System.frame_count += framedelta);

	initClock();

	if (framedelta > 0 && chipsetId > 0) {
		pEvt->doEvents(framedelta);			debug_time_doevents += defClock();
		chkKeys();								debug_time_checkkeys += defClock();
	}

	if (pGM->slm->getCurLayer() != this)
		return;

	drawEvents(framedelta);						debug_time_drawevents += defClock();
	drawChipSet(framedelta);					debug_time_drawchipset += defClock();
	drawEventsAfterChipset();					debug_time_drawevents2 += defClock();
	drawEffect(framedelta);						debug_time_draweffect += defClock();

	drawBackGround(framedelta);					debug_time_drawbg += defClock();

	updateRide();								debug_time_chkride += defClock();

	pPic->draw(framedelta);					debug_time_drawpicture += defClock();
	pAni->Draw(framedelta);					debug_time_drwanimation += defClock();
	pTimer->draw(framedelta);					debug_time_drawtimer += defClock();

	//drawDebug();
}
void ScrMap::chkKeys()
{
	int key_enter = pGM->ctl->getKey(rkc_enter);
	int key_esc = pGM->ctl->getKey(rkc_esc);
	int key_qsave = pGM->ctl->getKey(rkc_qsave);

	if (keyLastQSave != key_qsave && key_qsave > 0) {
		pGM->slm->setSettingScrSave(true, false);
		pGM->slm->setScreen(scr_save, MENU_CHANGE_TIME);
	}
	keyLastQSave = key_qsave;

	if (pEvt->isRuningSerial())
		return;

	if (keyLastEnter != key_enter && key_enter > 0)
		chkEventClick();
	keyLastEnter = key_enter;

	MgrState mstate(pGM);
	if (keyLastEsc != key_esc && key_esc > 0 && mstate.getAbleMenu()) {
		pGM->slm->setSettingScrMenu();
		pGM->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
	}
	keyLastEsc = key_esc;

}

void ScrMap::chkEventClick()
{
	LsdObjectCommon *obj;

	switch(getRideType()) {
	case 0:obj = &(pGM->lsd.LocHero);break;
	case 1:obj = &(pGM->lsd.LocRide[0]);break;
	case 2:obj = &(pGM->lsd.LocRide[1]);break;
	case 3:obj = &(pGM->lsd.LocRide[2]);break;
	}

	ArrayList<LsdMapEvent> &events = pGM->lsd.Map.events;
	int direction = obj->direction;
	int posX = obj->posX; 
	int posY = obj->posY; 

	int posXd = obj->posX + ObjectDirectX[direction]; 
	int posYd = obj->posY + ObjectDirectY[direction];
	bool posdMoveable = chkPos(posXd, posYd);

	int posXd2 = obj->posX + ObjectDirectX[direction] * 2; 
	int posYd2 = obj->posY + ObjectDirectY[direction] * 2;
	bool posd2EventRunable = chkPos(posXd2, posYd2);

	if (posdMoveable && posd2EventRunable) {
		LdbChipset &c = pGM->ldb.dChip[chipsetId-1];

		int UID = mapData.upperMap[posXd + posYd * mapWidth] ;

		int upperCID = getChipsetID(UID);

		int upperflag;

		if (upperCID < c.passableDataUpper.GetLength())
			upperflag = c.passableDataUpper[upperCID];
		else
			upperflag = 15;

		posd2EventRunable = ((upperflag & 0x40) != 0);
	}

	doRide();

	for(int i=0; i<events.GetLength(); i++) {
		LsdMapEvent &eventlsd = events[i];

		if (	(eventlsd.draw_priority != drawpri_mid && eventlsd.posX == posX && eventlsd.posY == posY) ||
			(eventlsd.draw_priority == drawpri_mid && eventlsd.posX == posXd && eventlsd.posY == posYd && posdMoveable) ||
			(eventlsd.draw_priority == drawpri_mid && eventlsd.posX == posXd2 && eventlsd.posY == posYd2 && posd2EventRunable)) {

				int pageID = ScrMapPublic::getEventPage(pGM, mapData.Events[i]);
				if (pageID == -1)
					continue;

				LmuEventPage &page = mapData.Events[i].Page[pageID];
				pEvt->doClickEvent(i, pageID);

				if (isFixedDirection(&eventlsd)) {
					eventlsd.direction_event = 0;
				} else {
					eventlsd.direction_event = ((direction - 1 + 2) % 4) + 1;
				}
		}
	}
}
