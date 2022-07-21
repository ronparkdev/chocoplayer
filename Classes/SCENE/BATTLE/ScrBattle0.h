#ifndef __R2K_SCENE_BATTLE_FOR_2000__
#define __R2K_SCENE_BATTLE_FOR_2000__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "ScrBattleType.h"
#include "ScrBattleBgInfo.h"
#include "ScrBattleBattlerManager.h"
#include "ScrBattleEffectValues.h"
#include "ScrBattleStateManager.h"
#include "ScrBattleHueManager.h"
#include "ScrBattle0EnemyNode.h"
#include "ScrBattle0ActionQueue.h"
#include "../ScreenLayer.h"
#include "../../GameMain.h"
#include "../../EVENT/EvtMain.h"
#include "../../GRAPHIC/GrpFontL.h"
#include "../../GRAPHIC/GrpMenu.h"
#include "../../GRAPHIC/GrpAniBat.h"
#include "../../GRAPHIC/OBJECT/GrpMsgbox.h"

class ScrBattle0 : public ScreenLayer
{
	//Default Structure
	GameMain *pGM;

	ScrBattleBattlerMgr battler;
	ScrBattleEffectValues effVals;
	ScrBattleStateMgr state;
	ScrBattleHueMgr hue;

	//Battle Setting
	int batId;
	BatType batType;
	BatPos batPos;
	bool isBatAutoPos;

	TString bgName;
	bool isBatEscapeable;	//TODO NOT USE
	bool isBatLoseToMap;
	bool isBatFirstAtk;

	bool isBatStart;
	bool isBatEnd;

	//MenuMoving
	bool isMenuModeRight;
	int menuMoveLeftFrame;

	//BackGround And Menus Object
	CCSprite *pSprBg;

	CCNode *pScreen;

	GrpMenu *pMenuAuto;
	GrpMenu *pMenuHero;

	GrpMenu *pMenuAction;
	
	GrpMenu *pMenuEnemy;
	ArrayList<int> menuEnemyId;

	GrpMenu *pMenuSub;
	ArrayList<int> menuSubId;
	GrpMenu *menuSubCaption;
	int menuSubLastRes;

	GrpMsgbox *pMenuProcBg;
	GrpFont *pMenuProcRes;
	GrpFontL *pMenuProcRun[4];

	//Menu
	BatMenuPos curMenu;
	BatMenuPos lastMenu;

	//Animation
	GrpAniBat *pAnis[SCR_BATTLE_MAX_ENEMIES];

	// Hero/Enemy Object/Data
	int nodeHeroLen;

	ScrBattle0EnemyNode *pNodeEnemy[SCR_BATTLE_MAX_ENEMIES];
	int nodeEnemyLen;

	int turnCnt;
	int curSelHero;

	float escapeChanceHero;
	float escapeChanceEnemy;//TODO NOT USED

	//Event
	LsdMapEventStack evtStack;
	EvtMain *pEvtMain;
	bool isEventNeedCheck;
	int evtPage;

	int lastBatCmdActType;


	//Action
	ArrayList<int> heroCmdType;
	ArrayList<int> heroCmdTgt;
	ArrayList<int> heroCmdUsing;
	ScrBattle0ActionQueue heroActQueue[SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES];

	//Turn Queue
	ArrayList<int> turnQueue;
	ArrayList<ScrBattle0ActionQueue> actQueue;
	ScrBattle0ActionQueue actCurNode;
	bool isActHeroAuto;
	bool isActHeroEscape;
	int actFrameLeft;
	
	int curMsgRow;

	//Finish
	BatFinishType finishType;
	bool isBatEscaped;

	GlbMusic lastMusic;//TODO NOT USED
public:
	ScrBattle0( GameMain *main, int battle_pos, int battle_id, ScrBattleBgInfo bg_info, bool escape_to_map, bool lose_to_map, bool first_attack);
	~ScrBattle0();

	void Init();
	void reloadShaders();

	void Draw(int framedelta);
	void chkKeys();

private:
	
	void initPeople();

	//Menu
	void initMenus();
	void chkMenuKeys();
	void doMenuStart();
	void doMenuFinish();
	void doChangeMenu(BatMenuPos menu);

	void updateMenuPos(int framedelta);
	void updateAutoMenu();
	void updateHeroMenu();
	void updateActionMenu();
	void updateEnemyMenu();
	void updateSkillMenu();
	void updateItemMenu();
	void updateSubCaption( int result );

	bool setNextActiveHero();
	bool setPrevActiveHero();

	//BG
	void setBackGround( TString bg_name );

	//Turn
	int getRandomPeople(int myID, bool isHero);

	void chkTurnPriority();
	void updateAction();
	void updateEnemyAction( int id );

	//Action
	bool doAction();
	void doAction(int actID, int tgtID, ScrBattle0ActionType type, int subVal);
	void doActionEffect( int actID, int tgtID, ScrBattle0ActionType type, int val, int subVal);

	//Queue
	void addQueueAnimation(int tgtID, int usingID);
	void addQueueAction( int actID, int tgtID, ScrBattle0ActionType type, int val, int subVal );
	void addQueueMessage(TString str, bool wait = false);
	void addQueueMessageClear();
	void addQueueSound(const GlbSound &sound);
	void addQueueDamaged(int tgtID);
	bool updateQueue(int framedelta);

	//Finish
	void initBattleFinish();
	bool chkBattleFinish(int framedelta);
	void doEndByEvent();

	//Event
	void initEvent();
	void drawEvent(int framedelta);

	int GetPara(GlbEventNode &edata, int index);

	bool chkEvent();
	bool chkEventCond(LdbEnemyGroupEventCondition *c);
	bool chkTurn(int val, int a, int b);
};

#endif
