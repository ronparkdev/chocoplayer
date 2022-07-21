#ifndef __R2K_SCREEN_APP_MAIN__
#define __R2K_SCREEN_APP_MAIN__

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GameMain.h"
#include "../../DATA/ArrayList.h"
#include "ScrAppMainGameNode.h"
#include "ScrAppMainListNode.h"

enum GameListState{
	GameListStateIdle,
	GameListStateListLoading,
	GameListStateGameLoading
};

class ScrAppMain : public ScreenLayer
{
private:
	static ArrayList<ScrAppMainGameNode> listGame;

	ArrayList<ScrAppMainListNode> listNode;
	int nodeHeight;
	int nodeLen;
	float scrollY;
	float nodeStartY;

	int touchStartX;
	int touchStartY;
	int touchLastID;
	int touchLastY;

	CCNode *pLayer;

	CCSprite *pSprBg;

	CCSprite *pSprTitleBar;
	CCSprite *pSprTitleIcon;
	CCSprite *pSprTitleShadow;
	CCSprite *pSprHider;
	CCMenu *pSprTitleMenu;

	CCMenu *pSprAdd;

	CCLabelTTF *pLabelCaption;

	GameListState state;
public:

	ScrAppMain(GameMain *main);
	~ScrAppMain();

	void Init();
	void reloadShaders();
	void reloadSetting();
	void Draw(int framedelta);
	void chkKeys();
	bool isFullScreen();
	void onTouch(Touch_Type type, int id, float x, float y );
	void setTouchEnabled(bool isEnabled);

	void setState(GameListState state);
	GameListState getState();

	void initTitlebar();
	void initList();
	static ArrayList<ScrAppMainGameNode>& getList();

	void setPosition( int pos );

	void cbOpenMenu(CCObject *pSender);
	void cbOpenGameAdd(CCObject *pSender);
	void cbOpenNotice(CCNode* c, void* s);

	void requestRemove( const TString &spath );
	void requestRefresh( const TString &spath );
	void requestRefreshAll();
};

#endif