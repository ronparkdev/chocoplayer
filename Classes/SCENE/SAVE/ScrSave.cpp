#include "ScrSave.h"

#include "../../DATA/SysTime.h"
#include "../../DATA/LSD/LsdOldLoader.h"
#include "../../DATA/LSDCHK/LsdChk.h"
#include "../../FILEIO/TFolder.h"
#include "../../GRAPHIC/OBJECT/GrpMsgbox.h"

ScrSave::ScrSave( GameMain *main , bool isSave , bool fromTitle )
{
	this->pGM = main;
	this->isSave = isSave;
	this->isFromTitle = fromTitle;

	LdbSystem &sys = pGM->ldb.dSys;
	LdbVoca &voca = pGM->ldb.dVoca;

	CCSize screenSize	= CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin		= CCDirector::sharedDirector()->getVisibleOrigin();
	CCPoint poscenter	= ccp(screenSize.width/2, screenSize.height/2);

	int texID = IBuffer::getInstance()->get(imgSystem, pGM->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	CCNode *pNode;

	pNode = new GrpMsgbox(pGM, 320, 32);
	pNode->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(16)));
	this->addChild(pNode,0);

	for(int i=0; i<3; i++) {
		pNode = new GrpMsgbox(pGM, 320, 64);
		pNode->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(64*(i+1) + 8)));
		this->addChild(pNode,0);
	}

	pFontCaption = new GrpFontL(pGM);
	pFontCaption->setPosition(SCR_POS_X(8), SCR_POS_Y(8));

	if (isSave)
		pFontCaption->setText(voca.save_game_message);
	else
		pFontCaption->setText(voca.load_game_message);

	this->addChild(pFontCaption, 1);

	for(int i=0; i<3; i++) {
		float posY = 32 + 64 * i + 8;
		pFontFile[i] = new GrpFontL(pGM);
		pFontFile[i]->setPosition(SCR_POS_X(8), SCR_POS_Y(posY+8));
		//m_pFontFile[i]->setText("Test");
		this->addChild(pFontFile[i],2);

		pFontHero[i] = new GrpFontL(pGM);
		pFontHero[i]->setPosition(SCR_POS_X(8), SCR_POS_Y(posY+20));
		//m_pFontHero[i]->setText("Test");
		this->addChild(pFontHero[i],2);

		pFontLevel[i] = new GrpFontL(pGM);
		pFontLevel[i]->setPosition(SCR_POS_X(8), SCR_POS_Y(posY+32));
		//m_pFontLevel[i]->setText("Test");
		this->addChild(pFontLevel[i],2);

		pFontHP[i] = new GrpFontL(pGM);
		pFontHP[i]->setPosition(SCR_POS_X(8 + 32), SCR_POS_Y(posY+32));
		//m_pFontHP[i]->setText("Test");
		this->addChild(pFontHP[i],2);

		pFontDate[i] = new GrpFontL(pGM);
		pFontDate[i]->setPosition(SCR_POS_X(8), SCR_POS_Y(posY+44));
		//m_pFontDate[i]->setText("Date");
		this->addChild(pFontDate[i],4);

		for(int j=0; j<4; j++) {
			CCNode *node = pFace[i*4 + j] = new GrpFace(pGM);
			node->setPosition(ccp(SCR_POS_X(8 + 96 + 64 * j), SCR_POS_Y(posY + 32)));
			this->addChild(node, 3);
		}
	}

	pSel = new GrpSelection(pTex, 64, 16);
	this->addChild(pSel, 1);

	double maxtime = 0;

	curIndex = 0;
	curPage = -1;

	StructLoader sl;
	for(int i=0; i<SCR_SAVE_COUNT; i++) {
		TString path = getPath(i);

		isLsdLastVer[i] = false;

		if (sl.open(true, path)) {
			lsds[i].link(&(pGM->ldb));

			sl.setInitMode();
			lsds[i].serialize(sl);	
			sl.setInit(false);

			lsds[i].serialize(sl);	
			if (maxtime < lsds[i].Preview.Time) {
				maxtime = lsds[i].Preview.Time;
				curIndex = i;
			}

			//it seem 1.x save file
			if (lsds[i].Preview.Time == 0.0f) {
				sl.setInitMode();
				lsds[i].serialize(sl);	
				sl.setInit(false);
				
				LsdOldLoader ol;
				try{
					ol.ReadData(path, &pGM->ldb, &(lsds[i]));
				}catch(...) {
					sl.close();
					isLsdLoaded[i] = false;
					continue;
				}
				isLsdLastVer[i] = true;
			}

			sl.close();
			isLsdLoaded[i] = true;
		} else {
			isLsdLoaded[i] = false;
		}
	}

	ctlMain *ctl = pGM->ctl;

	int key_enter	= ctl->getKey(rkc_enter);
	int key_esc		= ctl->getKey(rkc_esc);
	int key_arrow	= ctl->getKey(rkc_arrow);

	keyLastEnter	= key_enter;
	keyLastEsc		= key_esc;
	keyLastArrow	= key_arrow;

	refreshPage();
	refreshPos();

	pGM->lsd.System.screen = scr_save;

	debugInt = 0;
}

ScrSave::~ScrSave()
{
	this->removeChild(pFontCaption);
	
	for(int i=0; i<3; i++) {
		this->removeChild(pFontFile[i]);
		this->removeChild(pFontHero[i]);
		this->removeChild(pFontLevel[i]);
		this->removeChild(pFontHP[i]);
		this->removeChild(pFontDate[i]);
	}

	for(int i=0; i<12; i++)
		this->removeChild(pFace[i]);

	this->removeChild(pSel);

	this->removeAllChildren();
}

void ScrSave::reloadShaders()
{
	this->removeChild(pFontCaption);
	pFontCaption->reloadShaders();

	for(int i=0; i<3; i++) {
		pFontFile[i]->reloadShaders();
		pFontHero[i]->reloadShaders();
		pFontLevel[i]->reloadShaders();
		pFontHP[i]->reloadShaders();
		pFontDate[i]->reloadShaders();
	}
}

void ScrSave::Draw(int framedelta)
{
	if (pGM->lsd.System.screen != scr_save)
		return;

	if (framedelta > 0)
		chkKey();
}

void ScrSave::refreshPage()
{
	LdbVoca &voca = pGM->ldb.dVoca;

	int curpage =  curIndex / SCR_SAVE_COUNTPERPAGE;
	if (curpage != curPage) {

		int pos = 0;
		for(int i=curpage * SCR_SAVE_COUNTPERPAGE ; 
			i<(curpage+1) * SCR_SAVE_COUNTPERPAGE; 
			i++)
		{
			TString path;
			path.setText(i+1, 2);
			path = pGM->pathGame + "/Save" + path + ".lsd";

			pFontFile[pos]->setText(voca.file + (i+1));

			if (isLsdLoaded[i]) {

				LsdPreview &pv = lsds[i].Preview;

				struct tm* t = SysTime::getTM(pv.Time);
				TString strDate;
				strDate = _ST(t->tm_year+1900) + "/" + (t->tm_mon+1) + "/" + t->tm_mday + " " + t->tm_hour + ":" + t->tm_min + ":" + t->tm_sec;


				pFontHero[pos]	->setText(pv.Hero1Name);
				pFontLevel[pos]	->setText(voca.lvl_short + " " + pv.Hero1Level);
				pFontHP[pos]		->setText(pv.Hero1HP);
				pFontDate[pos]	->setText(strDate);

				for(int i=0; i<4; i++)
					pFace[pos*4+i]->set(pv.HeroFaceName[i], pv.HeroFaceID[i]);

			} else {

				pFontHero[pos]	->setText("");
				pFontLevel[pos]	->setText("");
				pFontHP[pos]		->setText("");
				pFontDate[pos]	->setText("");
				for(int i=0; i<4; i++)
					pFace[pos*4+i]->setVisible(false);

			}

			pos++;
		}

		curPage = curpage;
	}
}


void ScrSave::refreshPos()
{
	int pos = curIndex % SCR_SAVE_COUNTPERPAGE;

	pSel->setPosition(SCR_POS_X(8+32), SCR_POS_Y(48 + 8 + 64 * pos));
}

void ScrSave::chkKey()
{
	ctlMain *ctl = pGM->ctl;

	int key_enter	= ctl->getKey(rkc_enter);
	int key_esc		= ctl->getKey(rkc_esc);
	int key_arrow	= ctl->getKey(rkc_arrow);

	//if ((m_debug_int++) == 60)key_enter = 14;//FORDEBUG

	if (keyLastEnter != key_enter) {
		keyLastEnter = key_enter;

		if (key_enter > 0) {
			if (!isSave) {
				if (isLsdLoaded[curIndex]) {
					pGM->lsd = lsds[curIndex];
					pGM->lsdbuf.Load();

					LsdObjectCommon *obj;
					LsdObjectCommon *objhero = &(pGM->lsd.LocHero);
					switch(pGM->lsd.LocHero.ride_type) {
					case 0:obj = objhero;break;
					case 1:obj = &(pGM->lsd.LocRide[0]);break;
					case 2:obj = &(pGM->lsd.LocRide[1]);break;
					case 3:obj = &(pGM->lsd.LocRide[2]);break;
					}

					pGM->slm->setSettingScrMap(
						obj->map_id, obj->posX, obj->posY,
						obj->direction, false);
					pGM->slm->setScreen(scr_map);

					if (isLsdLastVer[curIndex])
						doToast("Loaded from ChocoPlayer 1.X Save file");
				}
			} else {
				StructLoader sl;
				TString path = getPath(curIndex);
				
				TFolder::chkFolder(path);

				pGM->lsdbuf.Save();

				if (sl.open(false, path)) {
					pGM->lsd.link(&(pGM->ldb));
					pGM->lsd.serialize(sl);	

					if (!sl.close()) {
						CCLOG((path + _ST(" Save Failed")).getTextUTF8());
						doToast("Save Failed");
					}
				}

				LsdMain lsd;

				if (sl.open(true, path)) {
					lsd.link(&(pGM->ldb));

					sl.setInitMode();
					lsd.serialize(sl);	
					sl.setInit(false);

					lsd.serialize(sl);	

					sl.close();
				}

				LsdChk::chk(pGM->lsd, lsd);

				pGM->slm->setSettingScrMap();
				pGM->slm->setScreen(scr_map, SCREEN_HIDE_TIME);
			}
		}
	}

	if (keyLastEsc != key_esc) {
		keyLastEsc = key_esc;

		if (key_esc > 0) {
			if (isFromTitle)
				pGM->slm->setScreen(scr_title);
			else{
				pGM->slm->setSettingScrMap();
				pGM->slm->setScreen(scr_map, MENU_CHANGE_TIME);
			}
		}
	}

	if (keyLastArrow != key_arrow) {
		keyLastArrow = key_arrow;

		int newindex = curIndex;
		if (key_arrow == rkca_up) {
			if (newindex == 0)
				newindex = SCR_SAVE_COUNT-1;
			else
				newindex--;
		} else if (key_arrow == rkca_down) {
			if (newindex == SCR_SAVE_COUNT-1)
				newindex = 0;
			else
				newindex++;
		} else {
			return;
		}

		curIndex = newindex;

		refreshPage();
		refreshPos();
	}
}

TString ScrSave::getPath(int i) {
	TString path, num;
	num.setText(i+1, 2);
	
	if (pGM->isArchive) {
		int pos_slush = pGM->pathGame.lastIndexOf('/');
		int pos_point = pGM->pathGame.lastIndexOf('.');

		if (pos_point == -1)
			pos_point = pGM->pathGame.getLength();

		path = pGM->pathGame.substring(0, pos_slush) + "/SAVE/" + \
			pGM->pathGame.substring(pos_slush+1, pos_point) + ".s" + num;
	} else {
		path = pGM->pathGame + "Save" + num + ".lsd";
	}
	return path;
}
