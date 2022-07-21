#include "ScrAppMain.h"

#include "../AppScaleTools.h"
#include "../../DATA/TThread.h"
#include "../../GameMain.h"
#include "../../FILEIO/GraphicIO.h"

#include "../APPVKEY/ScrAppVKey.h"
#include "../../COMMON/MultiOutter.h"
#include "../../COMMON/MgrSetting.h"
#include "../../COMMON/GlobalLangFile.h"
#include "../../COMMON/GlobalLangFont.h"

ArrayList<ScrAppMainGameNode> ScrAppMain::listGame;

ScrAppMain::ScrAppMain( GameMain *main )
{

}


ScrAppMain::~ScrAppMain() {
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate((CCTouchDelegate*)this);
}

void ScrAppMain::Init()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate((CCTouchDelegate*)this, 0);

	initTitlebar();
	initList();
}

bool ScrAppMain::isFullScreen() {
	return true;
}


void ScrAppMain::reloadShaders()
{
	for(int i=0; i<listNode.GetLength(); i++)
		listNode[i].reloadShaders();
}

void ScrAppMain::reloadSetting()
{

}

void ScrAppMain::initTitlebar()
{
	float s = AppScaleTools::getScale();
	CCSize sz = CCDirector::sharedDirector()->getVisibleSize();

	float w = sz.width;
	float h = sz.height;

	float barW = AppScaleTools::dpToPx(SCALE_TOOL_WIDTH_FULL);
	float barH = AppScaleTools::dpToPx(48.75f);

	pSprTitleBar = CCSprite::create("bg_white.png");
	pSprTitleBar->setScaleX(barW);
	pSprTitleBar->setScaleY(barH);
	pSprTitleBar->setPositionX(barW * 0.5f);
	pSprTitleBar->setPositionY(sz.height - barH * 0.5f);
	pSprTitleBar->setColor(ccc3(252, 74, 74));
	this->addChild(pSprTitleBar, 100);

	pSprTitleIcon = CCSprite::create("gamelist_ic_title.png");
	pSprTitleIcon->setScale(s);
	pSprTitleIcon->setPositionX(w * 0.5f);
	pSprTitleIcon->setPositionY(h - barH * 0.5f);
	this->addChild(pSprTitleIcon, 101);

	CCMenuItem *itemTitleMenu = CCMenuItemImage::create("gamelist_btn_menu.png", "gamelist_btn_menu_push.png", this, menu_selector(ScrAppMain::cbOpenMenu));
	itemTitleMenu->setScale(s);
	pSprTitleMenu = CCMenu::createWithItem(itemTitleMenu);
	pSprTitleMenu->setPositionX(w - barH * 0.5f);
	pSprTitleMenu->setPositionY(h - barH * 0.5f);
	pSprTitleMenu->setTouchPriority(0);
	this->addChild(pSprTitleMenu, 101);

	pSprTitleIcon = CCSprite::create("gamelist_ic_title.png");
	pSprTitleIcon->setScale(s);
	pSprTitleIcon->setPositionX(w * 0.5f);
	pSprTitleIcon->setPositionY(h - barH * 0.5f);
	this->addChild(pSprTitleIcon, 101);

	pSprTitleShadow = CCSprite::create("gamelist_bg_shadow.png");
	pSprTitleShadow->setScaleX(w / 10.0f);
	pSprTitleShadow->setScaleY(s);
	pSprTitleShadow->setPositionX(w * 0.5f);
	pSprTitleShadow->setPositionY(h - barH - s * pSprTitleShadow->getContentSize().height * 0.5f);
	this->addChild(pSprTitleShadow, 99);

	pSprHider = CCSprite::create("bg_black.png");
	pSprHider->setScaleX(w);
	pSprHider->setScaleY(h);
	pSprHider->setPositionX(w/2);
	pSprHider->setPositionY(h/2);
	pSprHider->runAction(CCSequence::createWithTwoActions(CCFadeOut::create(0.3f), CCCallFuncND::create(this, callfuncND_selector(ScrAppMain::cbOpenNotice), (void *)NULL)));
	this->addChild(pSprHider, 900);

	float bottom = 0.0f;
	if (!MultiOutter::isProA())// && MgrSetting::getInstance()->getBool(RST_ETC_IS_VERTICAL))
		bottom = AppScaleTools::dpToPx(50);

	CCMenuItem *itemAdd = CCMenuItemImage::create("gamelist_btn_add.png", "gamelist_btn_add_push.png", this, menu_selector(ScrAppMain::cbOpenGameAdd));
	itemAdd->setScale(s);

	float a = s * itemAdd->getContentSize().width * 0.6f;
	pSprAdd = CCMenu::createWithItem(itemAdd);
	pSprAdd->setPositionX(w - a);
	pSprAdd->setPositionY(a + bottom);
	pSprAdd->setTouchPriority(0);
	this->addChild(pSprAdd, 101);


	pLabelCaption = CCLabelTTF::create();
	pLabelCaption->setColor(ccc3(0, 0, 0));
	pLabelCaption->setPosition(ccp(w/2, h/2));
	pLabelCaption->setString(GlobalLangFile::getInstance()->getStringForKey("SCR_APPMAIN_NOGAME"));
	pLabelCaption->setFontName(GlobalLangFont::getDefFontName());
	pLabelCaption->setFontSize(30.0f);
	pLabelCaption->setDimensions(CCSizeMake(w/3, h));
	pLabelCaption->setHorizontalAlignment(kCCTextAlignmentCenter);
	pLabelCaption->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	pLabelCaption->setVisible(false);
	this->addChild(pLabelCaption, 200);

	nodeStartY = barH;
}

void ScrAppMain::cbOpenMenu(CCObject *pSender) {
	MultiOutter::openMenu("|MAINMENU");
}

void ScrAppMain::cbOpenGameAdd(CCObject *pSender) {
	MultiOutter::openGameAdd();
}

void ScrAppMain::cbOpenNotice(CCNode* c, void* s) {
	if (GameMain::getInstance()->isNeedNoticeRTP) {
		MultiOutter::openMenu("|NOTICEMENU|RTP");
		GameMain::getInstance()->isNeedNoticeRTP = false;
	}
}

void startFindGames(ScrAppMain *data);

void ScrAppMain::initList() {
	CCSize sz = CCDirector::sharedDirector()->getVisibleSize();

	pSprBg = CCSprite::create("bg_white.png");
	pSprBg->setScaleX(sz.width);
	pSprBg->setScaleY(sz.height);
	pSprBg->setPositionX(sz.width * 0.5f);
	pSprBg->setPositionY(sz.height * 0.5f);
	pSprBg->setColor(ccc3(239, 239, 239));
	this->addChild(pSprBg, 0);

	nodeHeight = AppScaleTools::dpToPx(105.0f);
	nodeLen = (sz.height / nodeHeight) + 2;
	scrollY = 0.0f;
	
	listNode.ChangeMode(ArrayMode);
	listNode.SetLength(nodeLen);

	for(int i=0; i<nodeLen; i++)
		listNode[i].init(this, i, nodeStartY, nodeHeight, sz.width, sz.height - nodeStartY);

	setState(GameListStateListLoading);

	startFindGames(this);
}


ArrayList<ScrAppMainGameNode>& ScrAppMain::getList()
{
	return listGame;
}

void ScrAppMain::setPosition( int pos )
{

}

void ScrAppMain::Draw(int framedelta)
{
	float yMax = -nodeHeight * (listGame.GetLength() - 1);

	if (scrollY <= yMax)
		scrollY = yMax;

	if (scrollY >= 0)
		scrollY = 0;

	float y = scrollY;
	int gID, nID;
	for(gID=0; gID<listGame.GetLength(); gID++) {
		ScrAppMainGameNode &g = listGame[gID];
		if (g.getState() == GameNodeTitlePrepared)
			g.loadTitleWithMainThread();

		if (listNode[0].isInside(gID, y)) {
			for(nID=0; nID<nodeLen; nID++)
				if (listNode[nID].getGamePos() == gID) {
					if (g.getState() == GameNodeTitleLoaded) {
						listNode[nID].loadTitle(g);
					}

					break;
				}

			if (nID == nodeLen) {
				for(nID=0; nID<nodeLen; nID++)
					if (listNode[nID].isUseable(y))
						break;

				if (nID < nodeLen) {
					listNode[nID].load(gID, g);
				}
			}
		}
	}

	pLabelCaption->setVisible(listGame.GetLength() == 0);

	for(nID=0; nID<nodeLen; nID++)
		listNode[nID].draw(y);
}

void ScrAppMain::onTouch(Touch_Type type, int id, float x, float y ) {
	switch(type) {
	case touchtype_down:
		touchLastID = id;
		touchStartX = x;
		touchStartY = y;
		touchLastY = y;

		if (pLabelCaption->isVisible()) {
			float dx = abs(pLabelCaption->getPositionX() - x) * 2.0f;
			float dy = abs(pLabelCaption->getPositionY() - y) * 2.0f;

			if (dx < pLabelCaption->getContentSize().width &&
				dy < pLabelCaption->getContentSize().height) {
					MultiOutter::openCPWeb("manual.jsp#Install_Games");
			}
		}

		break;

	case touchtype_move:
		if (touchLastID == id) {
			if (touchStartX != -1) {
				float dx = touchStartX - x;
				float dy = touchStartY - y;
				float d = sqrt(dx * dx + dy * dy);

				if (d > AppScaleTools::dpToPx(4.0f)) {
					touchStartX = -1;
				}
			} else {
				scrollY += (y - touchLastY) * 1.3f;
			}

			touchLastY = y;
		}
		break;

	case touchtype_err:
	case touchtype_up:
		if (touchStartX != -1) {
			for(int i=0; i<nodeLen; i++) {
				ScrAppMainListNode &n = listNode[i];

				if (!n.isCursorInstide(scrollY, touchStartX, touchStartY))
					continue;

				if (!n.isLoaded())
					continue;

				if (getState() != GameListStateIdle)
					continue;

				ScrAppMainGameNode *g;
				if ((g = n.getGameData()) == NULL)
					break;

				if (g->getEncID() == -1) {
					MultiOutter::openMenu("|ENCODINGMENU|" + g->getPath() + (g->isArchive()?"|Y":"|N"));
				} else {
					setState(GameListStateGameLoading);
					pSprHider->stopAllActions();
					pSprHider->runAction(CCFadeIn::create(1.0f));
					n.gameStart();
				}

				break;
			}
		}
		
		if (touchLastID == id)
			touchLastID = -1;

		break;
	}
}

void ScrAppMain::setTouchEnabled( bool isEnabled )
{
	//CCLayer::setTouchEnabled(isEnabled);

	pSprTitleMenu->setTouchEnabled(isEnabled);
	pSprAdd->setTouchEnabled(isEnabled);

	for(int i=0; i<nodeLen; i++)
		listNode[i].setTouchEnabled( isEnabled );
}

void ScrAppMain::setState( GameListState state )
{
	this->state = state;	
}

GameListState ScrAppMain::getState()
{
	return state;
}

void ScrAppMain::requestRemove( const TString &spath )
{
	CCLog("requestRemove");
	int i;

	for(i=0; i<listGame.GetLength(); i++) {
		ScrAppMainGameNode &g = listGame[i];
		if (g.getPath() != spath)
			continue;

		listGame.Remove(i);
		break;
	}

	for(i=0; i<listNode.GetLength(); i++)
		listNode[i].clear();
}

void ScrAppMain::requestRefresh( const TString &spath )
{
	CCLog("requestRefresh");
	int i, j;
	for(i=0; i<listGame.GetLength(); i++) {
		ScrAppMainGameNode &g = listGame[i];
		if (g.getPath() != spath)
			continue;

		g.reload();
		
		for(j=0; j<listNode.GetLength(); j++) {
			ScrAppMainListNode &node = listNode[j];

			if (node.getGameData() != &g)
				continue;

			node.clear();

			break;
		}

		break;
	}
}

void ScrAppMain::requestRefreshAll()
{
	CCLog("requestRefreshAll");
	int i;
	for(i=0; i<listNode.GetLength(); i++)
		listNode[i].clear();
	
	startFindGames(this);
}
