#include "ScrGameover.h"

ScrGameover::ScrGameover( GameMain *main )
{
	pGM = main;

	LdbSystem &sys = pGM->ldb.dSys;
	LdbVoca &voca = pGM->ldb.dVoca;

	CCSize screenSize	= CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin		= CCDirector::sharedDirector()->getVisibleOrigin();
	CCPoint poscenter	= ccp(SCR_POS_X(GAME_SCREEN_WIDTH/2), SCR_POS_Y(GAME_SCREEN_HEIGHT/2));

	int texBGID = IBuffer::getInstance()->get(imgGameOver, IMAGE_BUFFER_NOALPHA_PRESTR + sys.gameover_name);
	CCTexture2D *pTexBG = IBuffer::getInstance()->get(imgGameOver, texBGID);

	pSprBG = CCSprite::createWithTexture(pTexBG);
	pSprBG->setPosition(poscenter);
	this->addChild(pSprBG);
	
	int texSysID = IBuffer::getInstance()->get(imgSystem, pGM->lsd.System.system_name); 
	CCTexture2D *pTexSys = IBuffer::getInstance()->get(imgSystem, texSysID);

	pMenu = new GrpMenu(pGM, 1, 1, 70);
	pMenu->setVisible(false);

	ArrayList<TString> menu;
	menu.Add("blabla_you_are_dead!!");

	pMenu->refreshNodes(menu);
	pMenu->setInControl(true);

	this->addChild(pMenu);

	pGM->snd.PlayMusic(pGM->ldb.dSys.gameover_music);

	pGM->lsd.System.screen = scr_gameover;		
}

ScrGameover::~ScrGameover()
{
	this->removeChild(pSprBG);
	this->removeChild(pMenu);
}

void ScrGameover::reloadShaders()
{
	pMenu->reloadShaders();
}

void ScrGameover::Draw( int framedelta )
{
	int result = pMenu->getInput();
	if (result != GRP_MENU_NOTWORK) {
		pGM->slm->setScreen(scr_title);
	}
}

