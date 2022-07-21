#include "ScrAppMainListNode.h"

#include "ScrAppMain.h"
#include "../AppScaleTools.h"
#include "../../GameMain.h"
#include "../../AppMultiTime.h"
#include "../../COMMON/GlobalLangFont.h"
#include "../../COMMON/MultiOutter.h"
#include "../../DATA/TThread.h"

#define NODE_SHOWING_TIME 0.6f
#define NODE_BG_ALPHA 120


#define NODE_TITLE_PERCENT 0.8f

#define RESULT_GAMESTART_NONE -1
#define RESULT_GAMESTART_OPENING -2
#define RESULT_GAMESTART_FINISH -3
#define RESULT_GAMESTART_CHANGE_SCENE -4

_THREAD_H g_appmain_thread = NULL;
bool g_appmain_thread_running = false;

void ScrAppMainListNode::init( CCNode *layer,  int i, int startY, int nodeHeight, int scrWidth, int scrHeight )
{
	this->layer = layer;
	this->index = i;
	this->startY = startY;
	this->nodeHeight = nodeHeight;
	this->scrWidth = scrWidth;
	this->scrHeight = scrHeight;

	gamePos = -1;
	loadedTime = 0.0f;
	resGameStart = RESULT_GAMESTART_NONE;
	gameData = NULL;
	isShowing = false;

	float titleH = nodeHeight * 0.8f;
	float titleW = titleH * 1.3333333f;
	float titleBgW = nodeHeight * 1.3333333f;
	float s = AppScaleTools::getScale();

	pSprBG = CCSprite::create("bg_white.png");
	layer->addChild(pSprBG, 50);

	pSprLine = CCSprite::create("bg_white.png");
	pSprLine->setScaleX(scrWidth);
	pSprLine->setScaleY(AppScaleTools::dpToPx(3.5f));
	pSprLine->setPositionX(scrWidth/2);
	pSprLine->setColor(ccc3(252, 74, 74));
	pSprLine->setVisible(false);
	layer->addChild(pSprLine, 52);

	pSprMainBG = CCSprite::create("gamelist_bg_title.png");
	pSprMainBG->setScale(s);
	pSprMainBG->setPositionX(AppScaleTools::getScale()*pSprMainBG->getContentSize().width/2);
	pSprMainBG->setVisible(false);
	layer->addChild(pSprMainBG, 52);

	pSprMain = CCSprite::create("bg_noise.png");
	//pSprMain->getTexture()->setAliasTexParameters();
	pSprMain->setPositionX(pSprMainBG->getPositionX());
	pSprMain->setTextureRect(CCRect(0,0,320,240));
	pSprMain->setVisible(false);
	layer->addChild(pSprMain, 53);

	pTxtTitle = CCLabelTTF::create();
	pTxtTitle->setString("ERROR");
	pTxtTitle->setFontName(GlobalLangFont::getDefFontName());
	pTxtTitle->setFontSize(s * 100.0f);
	pTxtTitle->setDimensions(CCSize(scrWidth - titleBgW, nodeHeight));
	pTxtTitle->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	pTxtTitle->setHorizontalAlignment(kCCTextAlignmentLeft);
	pTxtTitle->setPositionX(titleBgW + (scrWidth - titleBgW) * 0.5f);
	pTxtTitle->setVisible(false);

#ifndef WIN32
	pTxtTitle->enableStroke(ccc3(0, 0, 0), 1.0f * s);
#endif

	layer->addChild(pTxtTitle, 54);

	CCMenuItem *itemMenu = CCMenuItemImage::create("gamelist_btn_submenu.png", "gamelist_btn_submenu.png", layer, menu_selector(ScrAppMainListNode::cbOpenMenu));
	itemMenu->setScale(s);
	itemMenu->setTag((long)this);//TODO
	
	menuHeight = itemMenu->getContentSize().height;
	pSprMenu = CCMenu::createWithItem(itemMenu);
	pSprMenu->setPositionX(scrWidth - itemMenu->getContentSize().width*0.5f * s);
	pSprMenu->setVisible(false);
	pSprMenu->setTouchPriority(100);
	layer->addChild(pSprMenu, 56);
}

int ScrAppMainListNode::getGamePos()
{
	return gamePos;
}

void ScrAppMainListNode::setGameLoadResult( int result )
{
	resGameStart = result;
}

ScrAppMainGameNode* ScrAppMainListNode::getGameData()
{
	return gameData;
}

bool ScrAppMainListNode::isUseable(float scrollY)
{
	return ((gamePos == -1) || !isInside(gamePos, scrollY));
}

bool ScrAppMainListNode::isInside(int i, float scrollY) {
	int yStart = i * nodeHeight + scrollY;
	int yEnd = (i + 1) * nodeHeight + scrollY;

	return ((0 <= yEnd) && (yStart <= scrHeight));
}

bool ScrAppMainListNode::isCursorInstide(float scrollY, float x, float y) {
	int yStart = gamePos * nodeHeight + startY + scrollY;
	int yEnd = (gamePos + 1) * nodeHeight + startY  + scrollY;

	return (yStart <= y && y <= yEnd);
}

void ScrAppMainListNode::draw( float y )
{
	if (gamePos != -1 && gameData != NULL) {
		bool v = isInside(gamePos, y);
		
		/*
		if ((gamePos != -1) && (pSprLine->isVisible() ^ v))
			CCLog("%dGame %s", gamePos, v?"Show":"Hide");
		*/
		
		isShowing = v;

		if (v) {
			int yStart = scrHeight - gamePos * nodeHeight - y;
			int yEnd = scrHeight - (gamePos + 1) * nodeHeight - y;
			int yMid = (yStart + yEnd)/2;

			float s = AppScaleTools::getScale();

			float per = (getTime() - loadedTime) / NODE_SHOWING_TIME;

			if (per > 1.0f)
				per = 1.0f;

			pSprLine->setPositionY(yEnd);
			pSprLine->setVisible(true);

			pSprBG->setPositionY(yMid);
			pSprBG->setOpacity(per * NODE_BG_ALPHA);

			if (resGameStart == RESULT_GAMESTART_NONE) {
				pSprMain->setPositionY(yMid);
				pSprMain->setOpacity(per * 255);
				pSprMain->setVisible(true);
			} else {
				switch(resGameStart) {
					case RESULT_GAMESTART_OPENING:
					case RESULT_GAMESTART_FINISH:
					case RESULT_GAMESTART_CHANGE_SCENE:
						break;

					case SSR_LOAD_SUCCESS:
						GameMain::getInstance()->openGameWithMainThread();
						resGameStart = RESULT_GAMESTART_FINISH;
						break;

					case SSR_LOAD_FAIL_FILE_CTL:
					case SSR_LOAD_FAIL_LDB:
					case SSR_LOAD_FAIL_LMT:
						if (resGameStart == SSR_LOAD_FAIL_FILE_CTL)
							MultiOutter::openMenu("|NOTICEMENU|FAIL|FILE");
						else if (resGameStart == SSR_LOAD_FAIL_LDB)
							MultiOutter::openMenu("|NOTICEMENU|FAIL|LDB");
						else if (resGameStart == SSR_LOAD_FAIL_LMT)
							MultiOutter::openMenu("|NOTICEMENU|FAIL|LMT");

						ScreenLayerManager::getInstance()->setScreen(scr_app_main);
						break;
				}

				if (resGameStart == RESULT_GAMESTART_FINISH && isGameStartAniFinished) {
					ScreenLayerManager::getInstance()->setScreen(scr_title);
					resGameStart = RESULT_GAMESTART_CHANGE_SCENE;
				}
			}

			pSprMainBG->setPositionY(yMid);
			pSprMainBG->setVisible(true);

			pTxtTitle->setPositionY(yMid);
			pTxtTitle->setVisible(true);

			pSprMenu->setPositionY(yEnd + (menuHeight * s * 0.5f));
			pSprMenu->setVisible(true);

			if (isTitleLoaded) {
				pSprBG->setVisible(true);
			} else {
				pSprMain->setTextureRect(CCRect(rand() % 320,rand() % 240,320,240));
			}
			return;
		}
	}

	pSprLine->setVisible(false);
	pSprBG->setVisible(false);
	pSprMain->setVisible(false);
	pSprMainBG->setVisible(false);
	pTxtTitle->setVisible(false);
	pSprMenu->setVisible(false);
}

void ScrAppMainListNode::load( int gamePos, ScrAppMainGameNode & g )
{
	if (this->gamePos != gamePos) {
		//CCLOG("%dNode %dGame", index, gamePos);

		this->gameData = &g;
		this->gamePos = gamePos;
		isTitleLoaded = false;
		pSprBG->setVisible(false);
		pSprMain->initWithTexture(CCTextureCache::sharedTextureCache()->addImage("bg_noise.png"));
		pSprMain->setScale((nodeHeight * NODE_TITLE_PERCENT) / 240.0f);
		pSprMain->setVisible(true);
		pTxtTitle->setString(g.getShortPath().getTextUTF8());
	}
}

void ScrAppMainListNode::clear()
{
	isTitleLoaded = false;
	gamePos = -1;
	gameData = NULL;
}

void ScrAppMainListNode::loadTitle(ScrAppMainGameNode & g) {
	if (gamePos != -1 && !isTitleLoaded) {
		loadedTime = g.getLoadedTime();

		CCTexture2D *tex = g.getTitleTex();

		float imgw = tex->getContentSize().width;
		float imgh = tex->getContentSize().height;

		float scaleBG = scrWidth / imgw;
		float scaleMain = nodeHeight * NODE_TITLE_PERCENT / imgh;
		float rawBGH = nodeHeight / scaleBG;

		pSprBG->initWithTexture(tex);
		pSprBG->setTextureRect(CCRect(0, (imgh-rawBGH)/2, imgw, rawBGH));
		pSprBG->setPositionX(scrWidth/2);
		pSprBG->setPositionY(-nodeHeight);
		pSprBG->setScale(scaleBG);
		pSprBG->setOpacity(NODE_BG_ALPHA);
		pSprBG->setVisible(false);

		pSprMain->initWithTexture(tex);
		pSprMain->setPositionX(pSprMainBG->getPositionX());
		pSprMain->setPositionY(-nodeHeight);
		pSprMain->setScale(scaleMain);
		//pSprMain->setVisible(false);

		pTxtTitle->setString(g.getName().getTextUTF8());
		pTxtTitle->runAction(CCFadeIn::create(0.3f));

		isTitleLoaded = true;

		//CCLOG("NODE IMAGE LOAD %s TEX 0x%X", g.getShortPath().getTextUTF8(), tex);
	}
}

void ScrAppMainListNode::reloadShaders()
{

}

void ScrAppMainListNode::cbOpenMenu( CCObject *pSender )
{
	ScrAppMainListNode *n = (ScrAppMainListNode *)(((CCMenuItem *)pSender)->getTag());

	ScrAppMainGameNode *g = n->getGameData();

	CCLOG("CLICKED %d", n->getGamePos());
	if (g != NULL) {
		MultiOutter::openMenu("|DETAILMENU|" + g->getPath() + (g->isArchive()?"|Y":"|N"));

		//MultiOutter::openMenu("|DETAILMENU|" + g->getPath() + (g->isArchive()?"|Y":"|N") + "|DEL|YES");
	}
}

_THREAD_RETVALUE thStartOpenGame(void* data);

bool ScrAppMainListNode::isLoaded() {
	return (isShowing && gameData != NULL);
}

void ScrAppMainListNode::gameStart() {
	CCSize sz = CCDirector::sharedDirector()->getVisibleSize();

	float x = ScreenCommon::game_x + GAME_SCREEN_WIDTH * ScreenCommon::game_s * 0.5f;
	float y = ScreenCommon::game_y + GAME_SCREEN_HEIGHT * ScreenCommon::game_s * 0.5f;

	isGameStartAniFinished = false;

	layer->reorderChild(pSprMain, 1000);
	pSprMain->runAction(CCFadeTo::create(1.0f, 255));
	pSprMain->runAction(CCEaseSineInOut::create(CCMoveTo::create(1.0f, ccp(x, y))));
	pSprMain->runAction(CCEaseSineInOut::create(CCScaleTo::create(1.0f, ScreenCommon::game_s)));
	pSprMain->runAction(CCSequence::createWithTwoActions(
		CCDelayTime::create(1.0f), 
		CCCallFuncND::create(NULL, callfuncND_selector(ScrAppMainListNode::cbGameStartAniFinished), (void*) this)
	));
	isGameStartAniFinished;
	resGameStart = RESULT_GAMESTART_OPENING;

	_THREAD_ID id;
	_THREAD_RUN(thStartOpenGame, (void *)this, id, g_appmain_thread);
}

void ScrAppMainListNode::cbGameStartAniFinished(CCNode* sender, void* data) 
{
	((ScrAppMainListNode *)data)->setGameStartAniFinished();
}

void ScrAppMainListNode::setGameStartAniFinished()
{
	isGameStartAniFinished = true;
}

void ScrAppMainListNode::setTouchEnabled( bool isEnabled )
{
	pSprMenu->setTouchEnabled(false);
}

void stopFindGames();

_THREAD_RETVALUE thStartOpenGame(void* data) {
	do{	
		stopFindGames();

		ScrAppMainListNode *n = (ScrAppMainListNode *)data;

		ScrAppMainGameNode *g = n->getGameData();

		if (g == NULL)
			break;

		int result = GameMain::getInstance()->openGameWithBackground(*g);

		n->setGameLoadResult(result);

		g_appmain_thread = NULL;
		g_appmain_thread_running = false;
	}while(false);

	_THREAD_END;

	return NULL;
}
