#include "EvtMain.h"

#include "../COMMON/MgrSetting.h"
#include "../COMMON/ScreenCommon.h"
#include "../COMMON/GlobalLangFont.h"
#include "../GRAPHIC/OBJECT/GrpFace.h"

void EvtMain::initMsg()
{
	pFont = new GrpFont(pGM);
	pLayer->addChild(pFont, sz_msgbox+10);

	float fontsize = ScreenCommon::game_h/8.0f;
	pTransFont = CCLabelTTF::create("", 
		GlobalLangFont::getTransFontName().getTextUTF8(), 
		fontsize, 
		CCSize(GAME_SCREEN_WIDTH * ScreenCommon::game_s * 0.9f, GAME_SCREEN_HEIGHT * ScreenCommon::game_s / 3.0f * 0.9f),
		kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
	pTransFont->setPositionX(SCR_POS_X(160));
	pTransFont->setScale(1.0f/ScreenCommon::game_s);
	pTransFont->enableStroke(ccc3(0, 0, 0), fontsize/20.0f);
	pLayer->addChild(pTransFont, sz_msgbox );

	for(int i=0; i<6; i++) {
		pFontNum[i] = new GrpFont(pGM);
		pLayer->addChild(pFontNum[i], sz_msgbox+10);
	}

	int texID = IBuffer::getInstance()->get(imgSystem, pGM->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	pMsgBox = new GrpMsgbox(pGM, 320,80);
	pMsgBox->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(200)));
	pMsgBox->setVisible(false);
	pMsgBox->setScaleY(0.0f);
	pLayer->addChild(pMsgBox, sz_msgbox );

	pMsgSel = new GrpSelection(pTex, 305,16);
	pMsgSel->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(0)));
	pMsgSel->setVisible(false);
	pLayer->addChild(pMsgSel, sz_msgbox+5 );

	pMsgSelNum = new GrpSelection(pTex, 16,16);
	pMsgSelNum->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(0)));
	pMsgSelNum->setVisible(false);
	pLayer->addChild(pMsgSelNum, sz_msgbox+5 );

	pMsgFace = new GrpFace(pGM);
	pMsgFace->setVisible(false);
	pLayer->addChild(pMsgFace, sz_msgbox+10);

	msgState = EMS_NOT_VISIBLE;
	msgFrameLeft = 0;
	msgCalledEvtStack = NULL;
	msgCalledEvtData = NULL;
	pMsgContinue = NULL;

	isEvtSerialCommonTurn = false;
}

void EvtMain::changeSystem(TString name) {
	if (pGM->lsd.System.system_name != name) {
		pGM->lsd.System.system_name = name;

		int texID = IBuffer::getInstance()->get(imgSystem, IMAGE_BUFFER_NOALPHA_PRESTR + name); 
		CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

		int texAID = IBuffer::getInstance()->get(imgSystem, pGM->lsd.System.system_name);
		CCTexture2D *pTexA = IBuffer::getInstance()->get(imgSystem, texAID);

		CCNode *pTmp;

		pTmp = new GrpMsgbox(pGM, 320,80);
		pTmp->setPosition(pMsgBox->getPosition());
		pTmp->setVisible(pMsgBox->isVisible());
		pTmp->setScaleY(pMsgBox->getScaleY());
		pLayer->removeChild(pMsgBox);		
		pMsgBox = (GrpMsgbox*)pTmp;
		pLayer->addChild(pMsgBox, sz_msgbox );

		pMsgContinue = new GrpSysDown(pTexA);
		pMsgContinue->setPosition(ccp(SCR_POS_X(GAME_SCREEN_WIDTH/2), SCR_POS_Y(80-8/2)));
		pMsgBox->addChild(pMsgContinue);

		pTmp = new GrpSelection(pTex, 305,16);
		pTmp->setPosition(pMsgSel->getPosition());
		pTmp->setVisible(pMsgSel->isVisible());
		pLayer->removeChild(pMsgSel);	
		pMsgSel = (GrpSelection *)pTmp;
		pLayer->addChild(pMsgSel, sz_msgbox+5 );

		pTmp = new GrpSelection(pTex, 16,16);
		pTmp->setPosition(pMsgSelNum->getPosition());
		pTmp->setVisible(pMsgSelNum->isVisible());
		pLayer->removeChild(pMsgSelNum);	
		pMsgSelNum = (GrpSelection *)pTmp;
		pLayer->addChild(pMsgSelNum, sz_msgbox+5 );
	}
}

void EvtMain::destroyMsg()
{
	pLayer->removeChild(pFont);

	for(int i=0; i<6; i++)
		pLayer->removeChild(pFontNum[i]);

	pLayer->removeChild(pMsgBox);
	pLayer->removeChild(pMsgSel);
	pLayer->removeChild(pMsgSelNum);
	pLayer->removeChild(pMsgFace);
}

int EvtMain::getRequest(LsdMapEventStack *pEvent, int eventpos) {
	int frame = pGM->lsd.System.frame_count;

	bool visible = (pGM->lsd.System.msg_visiable == 0);

	pMsgBox->setVisible(visible);

	if (msgState == EMS_NOT_VISIBLE) {
		msgLastEvtStack = pEvent;
		msgLastEvtPos = eventpos;
		msgState = EMS_OPENING;

		if (visible)
			msgFrameLeft = EVT_GRAPHIC_MSGBOX_TIME;
		else
			msgFrameLeft = 1;

		return EMR_ANIMATION;

	} else if (msgState == EMS_OPENING || msgState == EMS_CLOSING)
		return EMR_ANIMATION;

	//else if (m_msg_state == EMS_VISIBLE)
	if (msgCalledEvtData != pEvent->data.GetTail()) {
		//if another stack, wait until close the message in another stack
		return EMR_WAIT;

	} else if (eventpos != msgEvtPos) {
		//if same stack and another line, request message
		msgLastEvtStack = pEvent;
		msgLastEvtPos = eventpos;
		return EMR_REQUEST_MSG;

	} else if (msgEvtPos == -1 || msgReqVal == -100) {
		return EMR_WAIT_RETURN;

	} else {
		return msgReqVal;

	}
}

void EvtMain::drawMsg(int framedelta)
{
	float per;

	msgFrameLeft -= framedelta;
	if (msgFrameLeft <= 0)
		per = 1.0f;
	else
		per = (float)(EVT_GRAPHIC_MSGBOX_TIME - msgFrameLeft)/EVT_GRAPHIC_MSGBOX_TIME;

	if (msgState == EMS_OPENING) {
		pMsgBox->setScaleY(per);

		if (per >= 1.0f) {
			msgCalledEvtStack = msgLastEvtStack;
			msgCalledEvtData = msgLastEvtStack->data.GetTail();
			msgEvtPos = -1;
			msgState = EMS_VISIBLE;

			//CCLOG("OPENED");
		}
	} else if (msgState == EMS_CLOSING) {
		pMsgBox->setScaleY(1.0f - per);

		if (per >= 1.0f) {
			msgCalledEvtStack = NULL;
			msgCalledEvtData = NULL;
			msgEvtPos = -1;
			msgState = EMS_NOT_VISIBLE;

			//CCLOG("CLOSED");
		}
	} else if (msgState == EMS_VISIBLE) {
		long frame = pGM->lsd.System.frame_count;

		bool keyEnter = pFont->refresh(frame, (msgType == EMT_ONLY_MSG));

		if (!keyEnter) {
			int key_arrow = pGM->ctl->getKey(rkc_arrow);
			int key_esc	= pGM->ctl->getKey(rkc_esc);

			if (msgType == EMT_CHOICE) {
				pMsgSel->setPositionY(SCR_POS_Y(msgPosY - ((5-msgSelPos-msgNonSelCnt)*17-1)));

				if (key_arrow != lastKeyArrow) {
					if (key_arrow == rkca_down) {
						if (msgSelPos < msgSelCnt)
							msgSelPos++;
						else
							msgSelPos = 1;
					} else if (key_arrow == rkca_up) {
						if (msgSelPos > 1)
							msgSelPos--;
						else
							msgSelPos = msgSelCnt;
					}
					lastKeyArrow = key_arrow;
				}


				if (lastKeyEsc != key_esc && key_esc>0 && isMsgCancelable)
					msgReqVal = -1;

				lastKeyEsc = key_esc;

			} else if (msgType == EMT_NUMBER) {
				pMsgSelNum->setPositionX(SCR_POS_X(16+msgSelPos*16));
				pMsgSelNum->setPositionY(SCR_POS_Y(msgPosY - ((4-msgNonSelCnt)*17-1)));

				if (key_arrow != lastKeyArrow) {
					int val = msgNumVal;
					int valadd = 1;
					for(int i=0; i<msgSelCnt-msgSelPos-1; i++)
						valadd *= 10;
					val = (val / valadd) % 10;

					if (key_arrow == rkca_left) {
						if (msgSelPos == 0)
							msgSelPos = msgSelCnt - 1;
						else
							msgSelPos--;
						pMsgSelNum->setPositionX(SCR_POS_X(16+msgSelPos*16));
					} else if (key_arrow == rkca_right) {
						if (msgSelPos == msgSelCnt - 1)
							msgSelPos = 0;
						else
							msgSelPos++;
						pMsgSelNum->setPositionX(SCR_POS_X(16+msgSelPos*16));
					} else if (key_arrow == rkca_down) {
						if (val == 0) {
							msgNumVal += valadd*9;
							val = 9;
						} else {
							msgNumVal -= valadd;
							val--;
						}
						pFontNum[msgSelPos]->setText(val);
						pFontNum[msgSelPos]->showAll();
					} else if (key_arrow == rkca_up) {
						if (val == 9) {
							msgNumVal -= valadd*9;
							val = 0;
						} else {
							msgNumVal += valadd;
							val++;
						}
						pFontNum[msgSelPos]->setText(val);
						pFontNum[msgSelPos]->showAll();
					}
					lastKeyArrow = key_arrow;
				}
			}
		} else {
			if (msgType == EMT_ONLY_MSG)
				msgReqVal = 0;
			else if (msgType == EMT_CHOICE)
				msgReqVal = msgSelPos;
			else //if (m_msg_type == EMT_NUMBER)
				msgReqVal = msgNumVal;
		}
	}

}

void EvtMain::reqtMessage(const TString &str)
{
	int frame = pGM->lsd.System.frame_count;

	reqtTranslate(str, 0);
	setMessagePosition(0);
	showFace();

	msgType = EMT_ONLY_MSG;
	msgReqVal = -100;
	msgEvtPos = msgLastEvtPos;

	if (str.indexOf("\\^") == -1)
		pFont->setText(str + "\\!");//Wait Key
	else
		pFont->setText(str + "\\|");//Auto Skip
	pFont->showStart(frame);

	pMsgBox->setVisible(pGM->lsd.System.msg_visiable == 0);
	pMsgSel->setVisible(false);
	pMsgSelNum->setVisible(false);
	for(int i=0; i<6; i++)
		pFontNum[i]->setVisible(false);

	drawMsg(0);
}

void EvtMain::reqtChoice( const TString &str, int msg_select_count, bool canCancel, int msg_nonselect_count /*= 0*/)
{

	int frame = pGM->lsd.System.frame_count;

	reqtTranslate(str, msg_select_count);
	setMessagePosition(1);
	showFace();

	msgType = EMT_CHOICE;
	msgReqVal = -100;
	msgEvtPos = msgLastEvtPos;

	msgNonSelCnt = msg_nonselect_count;
	msgSelCnt = msg_select_count;
	msgSelPos = 1;
	isMsgCancelable = canCancel;

	pFont->setText(str + "\\!");
	pFont->showStart(frame);

	pMsgBox->setVisible(pGM->lsd.System.msg_visiable == 0);
	pMsgSel->setVisible(true);
	pMsgSelNum->setVisible(false);
	for(int i=0; i<6; i++)
		pFontNum[i]->setVisible(false);

	lastKeyEsc		= pGM->ctl->getKey(rkc_esc);

	drawMsg(0);
}

void EvtMain::reqtNumber(const TString &str, int int_count, int msg_nonselect_count )
{
	int frame = pGM->lsd.System.frame_count;

	reqtTranslate(str, 0);
	setMessagePosition(1);
	showFace();

	msgType = EMT_NUMBER;
	msgReqVal = -100;
	msgEvtPos = msgLastEvtPos;

	msgNonSelCnt = msg_nonselect_count;
	msgSelCnt = int_count;
	msgNumVal = 0;
	msgSelPos = 0;

	pFont->setText(str + "\\!");
	pFont->showStart(frame);

	pMsgBox->setVisible(pGM->lsd.System.msg_visiable == 0);
	for(int i=0; i<6; i++)
		pFontNum[i]->setVisible(false);

	for(int i=0; i<msgSelCnt; i++) {
		pFontNum[i]->setVisible(true);
		pFontNum[i]->setText("0");
		pFontNum[i]->showAll();
		pFontNum[i]->setPositionX(SCR_POS_X(12+i*16));
		pFontNum[i]->setPositionY(SCR_POS_Y(msgPosY - ((4-msg_nonselect_count)*17-1) - 8));
	}

	pMsgSel->setVisible(false);
	pMsgSelNum->setVisible(true);

	lastKeyArrow = pGM->ctl->getKey(rkc_arrow);

	drawMsg(0);
}

void EvtMain::reqtTranslate( const TString & str, int sel_cnt )
{
	TString tstr;
	if (pGM->trans.getMsg(str, tstr, sel_cnt)) {
		pTransFont->setString(tstr.getTextUTF8());
		pTransFont->setVisible(true);
	} else {
		pTransFont->setVisible(false);
	}
}

bool EvtMain::endMsg(LsdMapEventStack *pEvent) {
	if (msgCalledEvtStack == pEvent) {
		if (msgState == EMS_CLOSING)
			return false;

		bool visible = (pGM->lsd.System.msg_visiable == 0);

		isMsgEndChecking = false;
		msgState = EMS_CLOSING;
		if (visible)
			msgFrameLeft = EVT_GRAPHIC_MSGBOX_TIME;
		else
			msgFrameLeft = 1;

		hideFace();

		pTransFont->setVisible(false);

		pFont->removeText();
		pMsgSel->setVisible(false);
		pMsgSelNum->setVisible(false);
		for(int i=0; i<6; i++)
			pFontNum[i]->removeText();

		return false;
	}
	return true;
}

bool EvtMain::usingMsg() {
	return (msgState != EMS_NOT_VISIBLE);
}

void EvtMain::setMessagePosition(int eventtype) {
	LsdSystem &sys = pGM->lsd.System;

	//eventtype 0:msg 1:choice 2:input
	int posx = 5, posy;

	int wantpos = pGM->lsd.System.msg_pos;

	if (isBattle)
		wantpos = (pGM->getVersion() == RPG2000)?2:0;

	msgPosY = posy = (wantpos+1) * 80;

	if (wantpos == 2) {
		pTransFont->setVerticalAlignment(kCCVerticalTextAlignmentBottom);
		pTransFont->setPositionY(SCR_POS_Y(160-40));
	} else if (wantpos == 1) {
		pTransFont->setVerticalAlignment(kCCVerticalTextAlignmentTop);
		pTransFont->setPositionY(SCR_POS_Y(240-40));
	} else {
		pTransFont->setVerticalAlignment(kCCVerticalTextAlignmentTop);
		pTransFont->setPositionY(SCR_POS_Y(160-40));
	}

	pMsgBox	->setPositionY(SCR_POS_Y(posy - 40));
	pMsgFace	->setPositionY(SCR_POS_Y(posy - 40));
	if (!sys.face_name.isEmpty() && !sys.face_right)
		pFont	->setPosition(ccp(SCR_POS_X(posx+80)	, SCR_POS_Y(posy - 75)));
	else
		pFont	->setPosition(ccp(SCR_POS_X(posx)	, SCR_POS_Y(posy - 75)));
}

void EvtMain::showFace() {
	LsdSystem &sys = pGM->lsd.System;

	if (!sys.face_name.isEmpty()) {
		pMsgFace->set(sys.face_name, sys.face_index, sys.face_flip);
		pMsgFace->setPositionX(sys.face_right?SCR_POS_X(ScreenCommon::game_w-40):SCR_POS_X(40));
		pMsgFace->setVisible(true);
	} else {
		hideFace();
	}
}

void EvtMain::hideFace() {
	pMsgFace->setVisible(false);
}
