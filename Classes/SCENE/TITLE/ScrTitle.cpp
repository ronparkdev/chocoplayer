#include "ScrTitle.h"

#include "../../GRAPHIC/ImageType.h"
#include "../../FILEIO/UnzipCTL.h"
#include "../../COMMON/MultiOutter.h"

ScrTitle::ScrTitle(GameMain *main)
{
	pGM = main;

	const LdbSystem &sys = pGM->ldb.dSys;
	const LdbVoca &voca = pGM->ldb.dVoca;

	CCSize screenSize	= CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin		= CCDirector::sharedDirector()->getVisibleOrigin();
	CCPoint poscenter	= ccp(SCR_POS_X(GAME_SCREEN_WIDTH/2), SCR_POS_Y(GAME_SCREEN_HEIGHT/2));

	pGM->lsd.System.system_name = sys.system_name;

	int texTitleId = IBuffer::getInstance()->get(imgTitle, IMAGE_BUFFER_NOALPHA_PRESTR + sys.title_name);
	CCTexture2D *pTitleSys = IBuffer::getInstance()->get(imgTitle, texTitleId);

	pSprTitle = CCSprite::createWithTexture(pTitleSys);
	pSprTitle->setPosition(poscenter);
	this->addChild(pSprTitle);

	int texID = IBuffer::getInstance()->get(imgSystem, sys.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	ArrayList<TString> menu;
	menu.Add(voca.new_game);
	menu.Add(voca.load_game);
	menu.Add(voca.exit_game);

	int c, maxWidth = 0, width = 0;
	for(int i=0; i<3; i++) {
		TString str = menu[i];
		
		width = 0;
		for(int j=0; j<str.getLength(); j++) {
			c = str.charAt(j);
			if (0<= c && c < 128)
				width += 6;
			else
				width += 14;
		}

		maxWidth = max(width, maxWidth);
	}

	if (!MultiOutter::isProI())
		menu[1] = "\\C[3]" + menu[1];
    
	pMenu = new GrpMenu(pGM, 3, 1, maxWidth, true);
	pMenu->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(180)));

	pMenu->refreshNodes(menu);
	pMenu->setInControl(true);

	this->addChild(pMenu);

	pGM->snd.PlayMusic(sys.title_music);


	pGM->lsd.link(&(pGM->ldb));
	pGM->lsd.newgame(pGM->ldb, pGM->lmt);


	pGM->lsd.System.screen = scr_title;	


	debugInt = 0;
}

ScrTitle::~ScrTitle() {
	this->removeChild(pSprTitle);
	this->removeChild(pMenu);
}

void ScrTitle::reloadShaders()
{
	pMenu->reloadShaders();
}

void ScrTitle::Draw(int framedelta)
{
	if (framedelta <= 0)
		return;

	int result = pMenu->getInput();

	//if ((m_debug_int++) == 60)result = 1;//FORDEBUG

	if (result == GRP_MENU_CANCEL) {
		pMenu->setIndex(2);
	} else if (result != GRP_MENU_NOTWORK) {
		switch(result) {
		case 0:
			pGM->lsd.link(&(pGM->ldb));
			pGM->lsd.newgame(pGM->ldb, pGM->lmt);
			
			pGM->lsdbuf.Load();

			pGM->slm->setSettingScrMap(
					pGM->lmt.Info.party_map_id, 
					pGM->lsd.LocHero.posX, 
					pGM->lsd.LocHero.posY, 
					pGM->lsd.LocHero.direction, true);
			
			pGM->snd.StopMusic();
			pGM->slm->setScreen(scr_map, MENU_CHANGE_TIME);

			ScreenLayerManager::getInstance()->startTimeLimit();

			break;

		case 1:
			pGM->snd.StopMusic();
			pGM->slm->setSettingScrSave(false, true);
			pGM->slm->setScreen(scr_save, MENU_CHANGE_TIME);
			break;


		case 2:
			pGM->snd.StopMusic();
			pGM->appstart();
			break;
		}
	}
}
