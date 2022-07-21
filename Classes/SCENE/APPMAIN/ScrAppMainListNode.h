#ifndef __R2K_SCREEN_APP_MAIN_LIST_NODE__
#define __R2K_SCREEN_APP_MAIN_LIST_NODE__

#include "cocos2d.h"
USING_NS_CC;

#include "ScrAppMainGameNode.h"

class ScrAppMainListNode 
{
private:
	CCNode *layer;

	int index;
	int gamePos;
	int startY;
	int nodeHeight;
	int scrWidth;
	int scrHeight;
	bool isShowing;
	bool isTitleLoaded;
	bool isGameStartAniFinished;
	int resGameStart;

	float loadedTime;

	ScrAppMainGameNode* gameData;

	CCSprite *pSprLine;
	CCSprite *pSprBG;
	CCSprite *pSprMain;
	CCSprite* pSprMainBG;
	CCLabelTTF *pTxtTitle;
	CCMenu* pSprMenu;
	int menuHeight;

	CCGLProgram *glProgram;
	int ul_time;

public:
	void init( CCNode *layer, int i, int startY, int nodeHeight, int scrWidth, int scrHeight );
	void draw(float y);
	void load( int gamePos, ScrAppMainGameNode & g );
	void clear();

	void loadTitle(ScrAppMainGameNode & g);
	
	int getGamePos();
	ScrAppMainGameNode* getGameData();

	void setLoaded();
	void setGameLoadResult( int result );
	void setGameStartAniFinished();

	bool isLoaded();
	bool isUseable(float y);
	bool isInside(int i, float y);
	bool isCursorInstide(float scrollY, float x, float y);
	void reloadShaders();

	void cbOpenMenu(CCObject *pSender);
	void cbGameStartAniFinished(CCNode* sender, void* data);

	void gameStart();
	void setTouchEnabled( bool isEnabled );
};

#endif