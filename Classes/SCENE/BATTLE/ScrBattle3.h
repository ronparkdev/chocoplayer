#ifndef __R2K_SCENE_BATTLE_FOR_2003__
#define __R2K_SCENE_BATTLE_FOR_2003__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "ScrBattleType.h"
#include "ScrBattleBgInfo.h"
#include "ScrBattleBattlerManager.h"
#include "ScrBattleEffectValues.h"
#include "ScrBattleStateManager.h"
#include "ScrBattleHueManager.h"
#include "ScrBattle3HeroNode.h"
#include "ScrBattle3EnemyNode.h"
#include "ScrBattle3WeaponNode.h"
#include "ScrBattle3ActionQueue.h"
#include "../ScreenLayer.h"
#include "../../GameMain.h"
#include "../../EVENT/EvtMain.h"
#include "../../GRAPHIC/GrpFontL.h"
#include "../../GRAPHIC/GrpMenu.h"
#include "../../GRAPHIC/OBJECT/GrpGaze.h"
#include "../../GRAPHIC/GrpAniBat.h"
#include "../../GRAPHIC/OBJECT/GrpBatNum.h"
#include "../../GRAPHIC/OBJECT/GrpBatDam.h"
#include "../../GRAPHIC/OBJECT/GrpFace.h"

#define SCR_BATTLE3_MAX_NUMBER 15

class ScrBattle3 : public ScreenLayer
{
	//Default Structure
	GameMain *pGM;

	ScrBattleBattlerMgr battler;
	ScrBattleEffectValues effvals;
	ScrBattleStateMgr state;
	ScrBattleHueMgr hue;

	//Battle Setting
	int batId;
	BatType batType;
	BatPos batPos;
	bool isBatAutoPos;

	ScrBattleBgInfo bgName;
	bool isBatEscapeable;
	bool isBatLoseToMap;
	bool isBatFirstAtk;

	bool isBatStart;
	bool isBatEnd;

	//MenuMoving
	bool isMenuModeRight;
	int menuMoveLeftFrame;
	GrpFace *pMenuFade[4];

	//BackGround
	CCSpriteBatchNode *bgSprs[2];

	int bgW[2], bgH[2];
	int bgR[2], bgC[2];
	int bgX[2], bgY[2];


	//Menus Object
	GrpMenu *pMenuCaption;

	GrpMenu *pMenuAuto;
	GrpMenu *pMenuHero;

	GrpMenu *pMenuAction;
	ArrayList<int> pMenuActionId;

	GrpMenu *pMenuEnemy;
	ArrayList<int> menuEnemyId;

	GrpMenu *pMenuSub;
	ArrayList<int> menuSubId;
	int menuSubSel;
	GrpMenu *pMenuSubCaption;
	int menuSubLastRes;

	int lastItemId;

	BatMenuPos curMenu;
	BatMenuPos lastMenu;

	//Animation
	GrpAniBat *pAnis[SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES];

	ScrBattle3WeaponNode *pNodeWeapon[SCR_BATTLE_MAX_ENEMIES];

	// Hero/Enemy Object/Data
	ScrBattle3HeroNode *pNodeHero[SCR_BATTLE_MAX_HEROS];
	int nodeHeroLen;

	ScrBattle3EnemyNode *pNodeEnemy[SCR_BATTLE_MAX_ENEMIES];
	int nodeEnemyLen;

	float turnPer[SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES];

	//Gage Object
	GrpGaze *pGazeHp[SCR_BATTLE_MAX_HEROS];
	GrpGaze *pGazeMp[SCR_BATTLE_MAX_HEROS];
	GrpGaze *pGazeTn[SCR_BATTLE_MAX_HEROS];
	GrpBatNum *pGazeNum[SCR_BATTLE_MAX_HEROS*2];

	//Num Object
	GrpBatDam *pDmgNum[SCR_BATTLE3_MAX_NUMBER];

	//Turn Queue
	ArrayList<ScrBattle3ActionQueue> actQueue;
	int actFrameLeft;
	ArrayList<int> heroReadyQueue;//For TypeA


	bool isBatAuto;	//TODO
	int curSelHero;
	int curHanding;
	int turnCnt;

	float escapeChanceHero;
	float escapeChanceEnemy;

	//Event
	LsdMapEventStack evtStack;
	EvtMain *pEvtMain;
	bool isEventNeedCheck;
	int evtPage;

	int lastBatCmdActType;

	//BattleFinished
	ArrayList<TString> batEndMsg;
	int batEndMsgRow;

	GrpMsgbox *pBatEndMsgbox;
	GrpSysDown *pBatEndMsgboxDown;
	GrpFont *pBatEndFont;
	int batEndLastKey;
	bool isBatEndReturningToMap;
	int curFrame;
	bool isBatEscaped;
	int menuCaptionHideLeftFrame;

	BatFinishType endType;
	GlbMusic lastMusic;
public:
	ScrBattle3( GameMain *main, int battle_pos, int battle_id, const ScrBattleBgInfo &bg_info, bool escape_to_map, bool lose_to_map, bool first_attack);
	~ScrBattle3();

	void initPeople();
	void initMenus();

	void Init();
	void reloadShaders();

	void Draw(int framedelta);
	void chkKeys();

private:
	//Menu

	bool isGameRunning();

	void initMenuKeys();
	void chkMenuKeys();
	void doMenuFinish();

	void updateMenuPos(int framedelta);
	void updateAutoMenu();
	void updateHeroMenu();
	void updateActionMenu();
	void updateEnemyMenu();
	void updateSkillMenu( int skillType );
	void updateItemMenu();
	void updateSubCaption( int result );

	void setHand(int id);

	void updateTurnPer(int timedelta);
	void doTurnFull( int id );
	void doEnemyAction( int id );
	void chkTurn();

	int getRandomPeople(int myID, bool isHero);

	//Finish
	void initBattleFinish();
	void chkBattleFinish();
	void updateBattleFinish(int framedelta);
	void doEndByEvent();

private:
	//Actions
	void initBackGround();
	void setBackGround(const ScrBattleBgInfo &bg_info);
	void drawBackGround(int framedelta);

	void addAction(int actID, int tgtID, ScrBattle3ActionType type, int etcID);

	void updateQueue(int framedelta);
	void addQueue(const ScrBattle3ActionQueue &node);
	void doQueue(ScrBattle3ActionQueue &node);
	void removeQueue(int id);

	bool isWeaponRangeAni( int actLdbID, LdbItem * item );
	void addWeaponRangeAni(int actLdbID, LdbItem *item, bool isGoing, int actID, int tgtID);
	void addWait(int frame);

	bool isAttackAllInOther(int actLdbID, LdbItem *item);

	void doAttack(int actorID, int targetID, bool isSecondWeapon = false);
	void doSkill(int actorID, int targetID, int skillID);
	void doItem(int actorID, int targetID, int itemID);
	void doItemOne(int targetID, int itemID);
	void showDamage(int tgtID, int dam);

	void setCaption(TString str);
	void updateCaption(int framedelta);
	
private:
	//Event
	void initEvent();
	bool chkEventRun();
	bool chkEventCond(LdbEnemyGroupEventCondition *cond);
	bool chkTurn(int val, int a, int b);
	bool isEventRunning();
	void chkEvent(int framedelta);

	int GetPara(GlbEventNode &edata, int index);

private:
	//Position
	void initPosition();
	void updatePositionBattleStart();
	void updatePositionAction(int actID, int tgtID);
	bool isRightOnAction(int actID, int tgtID);
};

#endif
