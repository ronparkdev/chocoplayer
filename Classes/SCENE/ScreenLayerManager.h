#ifndef __R2K_SCREEN_LAYER_MANAGER__
#define __R2K_SCREEN_LAYER_MANAGER__
#include "../COMMON/DebugingCode.h"

#include "ScreenLayer.h" 
#include "../COMMON/ScreenCommon.h"
#include "../CONTROL/CCLayerCTL.h"
#include "../CONTROL/ctlMain.h"
#include "../DATA/ArrayList.h"

#include "APPJOY/ScrAppJoy.h"
#include "APPVKEY/ScrAppVKey.h"
#include "BATTLE/ScrBattleBgInfo.h"

#define SCREEN_HIDE_TIME 0.5f
#define MENU_CHANGE_TIME 0.2f

class IBuffer;

class GameMain;

class ScreenLayerManager : public CCLayerCTL{
private:
	static ScreenLayerManager *gInstance;

	GameMain *pGM;

	ScreenType scrLastType;
	ScreenType scrType;

	ScreenLayer *pCurLayer;

	int changeFrameTotal;
	int changeFrameLeft;

	bool isFrameNeedRefresh;

	CCSprite *pSprBlack[2];

	CCSprite *pCurScreenSpr;
	int mosaicPattern[1200];
	bool mosaicBuffer[1200];
	int mosaicDataIn[150];

	char *lastGLSLFileName;
	CCGLProgram *pGlProg;
	int glProgCtl;
	int lastAniType;

	//Timer
	int limitFrameLeft;
	CCLabelTTF *pLimitTimeLeft;

	//SubMenu
	bool isSubVKeyOpened;
	bool isSubJoyOpened;

	ScrAppJoy *pSubJoy;
	ScrAppVKey *pSubVKey;

	//Screen Changing
	bool isScrChange;
	bool isScrHided;
	bool isMapHided;
	bool isFirst;

	int aniTypeShow;
	int aniTypeHide;

	int chkFrame;
	float changeTime;
	int totFrame;
	int lastFrameDelta;

	int gcFrameLeft;

	//Action
	int incomingAction;

	//Screen Pause
	bool isScrPaused;
	CCSprite* pSprPausedFade;
	CCSprite* pSprPausedCaption;
	int lastKeyMenu;
	
	//Screen Setting
	int sgMap_mapId;
	int sgMap_posX;
	int sgMap_posY;
	int sgMap_direct;
	bool sgMap_isInit;

	ScrMenuType sgMenu_type;
	int sgMenu_arg1;
	int sgMenu_arg2;
	bool sgMenu_arg3;

	bool sgSave_isSave;
	bool sgSave_isFromTitle;
	
	int sgBat_batPos;
	int sgBat_batId;
	ScrBattleBgInfo sgBat_bgInfo;
	bool sgBat_isEscapeToMap;
	bool sgBat_isLoseToMap;
	bool sgBat_isFirstAttack;
	
	int sgShop_workType;
	int sgShop_langType;
	ArrayList<int> sgShop_itemList;

	TString *sgName_defStr;
public:
	static ScreenLayerManager * getInstance();

	virtual bool init();  

	virtual void onTouch(Touch_Type type, int id, float x, float y);

	static CCScene* scene( ScreenLayerManager *slm, GameMain *main);
	void draw();
	CREATE_FUNC(ScreenLayerManager);

	ScreenLayerManager();
	void Init( GameMain * main );
	void reloadShaders();

	~ScreenLayerManager();

	void fixTotalFrame();

	void setScreen(ScreenType type, float changetime = SCREEN_HIDE_TIME);

	bool Hide(int type);
	bool Show(int type);

	bool isHided();
	bool isChanging();

	void setDefaultPosition();

	ScreenLayer* getCurLayer();

	void setSettingScrMap();
	void setSettingScrMap( int mapid, int posx, int posy, int chardirect, bool init);
	void setSettingScrMenu( ScrMenuType type = scr_menu_main, int arg1 = 0, int arg2 = 0 , bool arg3 = false );
	void setSettingScrSave( bool isSave, bool isfromTitle );
	void setSettingScrBattle( int battlepos, int battleid, const ScrBattleBgInfo &bg_info, bool escapemap, bool losetomap, bool firstattack);
	void setSettingScrShop( int worktype, int langtype, ArrayList<int> items );
	void setSettingScrNameInput( TString *defstr );

	void onPause();
	void onResume();
	void onChangedOrientation();

	void doAction( int type );
	void setSceneVKeyVisible(bool isVisible);
	void setSceneJoyVisible(bool isVisible);

	void refreshScrPaused();
	void setScrPaused(bool isPaused);

	void initTimeLimit();
	void startTimeLimit();
	void checkTimeLimit(int scrType);
	void drawTimeLimit(int framedelta);
private:
	int getFrameDelta();

	void setAniType( ScreenType type, ScreenType lastType );
	void addScreen( ScreenType type );

	void doReadyAni(int animation_type);
	void doFinAni();
	void doRunAni(float p);

	void getMosaic(int *d, int x, int y);
	void getMosaicUp(int *d, int x, int y);
	void getMosaicDown(int *d, int x, int y);
	
	void windowsKeyInput();
	void keyMenuClicked();
	void doScanAndHide(bool doevent);
	void openSceneVKey();
};

#endif

/*
	01전체에 랜덤 블록*
	02위에서 랜덤 블록*
	03아래서 랜덤 블록*
	17모자이크*
	19순간 삭제*

	00페이드인
	04블라인드 오픈
	05상하에서 스트라이프
	06좌우에서 스트라이프

	07바깥쪽에서 중심에 a
	08중심에서 바깥쪽에 a
	09아래에서 스크롤 
	10위에서 스크롤
	11오른쪽에서 스크롤 
	12왼쪽에서 스크롤

	13상하에서 결합
	14좌우에서 결합
	15상하좌우에서 결합
	16줌 아웃
	18래스터 스크롤
*/