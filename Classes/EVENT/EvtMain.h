#ifndef __R2K_EVENT_MAIN__
#define __R2K_EVENT_MAIN__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "EvtCodes.h"
#include "../GameMain.h"
#include "../GRAPHIC/GrpFont.h"
#include "../GRAPHIC/OBJECT/GrpMsgbox.h"
#include "../GRAPHIC/OBJECT/GrpSelection.h"
#include "../GRAPHIC/OBJECT/GrpSysDown.h"
#include "../GRAPHIC/OBJECT/GrpFace.h"
#include "../COMMON/MgrState.h"
#include "../COMMON/MgrCharacter.h"
#include "../COMMON/MgrTeleport.h"
#include "../COMMON/MgrItem.h"
#include "../SCENE/BATTLE/ScrBattleBattlerManager.h"
#include "../SCENE/BATTLE/ScrBattleType.h"

#define SCREEN_TRANSITION_TIME 1
#define EVT_GRAPHIC_MSGBOX_TIME 10

enum EvtMsgType{
	EMT_ONLY_MSG,
	EMT_NUMBER,
	EMT_CHOICE
};

enum EvtMsgReq{
	EMR_WAIT			= -15,
	EMR_ANIMATION		= -14,
	EMR_REQUEST_MSG		= -13,
	EMR_WAIT_RETURN		= -12,
	EMR_GO_TO_NEXT_EVT	= -11
};

enum EvtMsgState{
	EMS_NOT_VISIBLE,
	EMS_OPENING,
	EMS_VISIBLE,
	EMS_CLOSING,
};

class EvtMain 
{
public:
	static int g_event_run_count;

private:
	int mapEventRealId[10000];
	GameMain *pGM;
	int curMapId;
	LmuMain *pMapData;
	CCNode *pLayer;
	LsdMapEventStack *pEvtStack;
	
	ScrBattleBattlerMgr *pBatrMgr;

	MgrCharacter mch;
	MgrItem mit;
	MgrState mst;
	MgrTeleport mtele;

	bool isBattle;

	GrpFont *pFont;
	GrpFont *pFontNum[6];
	GrpMsgbox *pMsgBox;
	GrpSelection *pMsgSel;
	GrpSelection *pMsgSelNum;
	GrpSysDown *pMsgContinue;
	
	GrpFace *pMsgFace;

	LsdMapEventStack *msgCalledEvtStack;
	LsdMapEventData *msgCalledEvtData;

	EvtMsgType msgType;
	EvtMsgState msgState;
	int msgReqVal;
	LsdMapEventStack *msgLastEvtStack;
	int msgLastEvtPos;

	bool isEvtSerialCommonTurn;

	int msgEvtPos;
	int msgFrameLeft;
	int msgPosY;

	int msgNonSelCnt;
	int msgSelCnt;
	int msgSelPos;
	int msgNumVal;
	bool isMsgCancelable;

	int msgLastPos;
	bool isMsgEndChecking;
	
	int lastKeyEsc;
	int lastKeyArrow;
	
	CCLabelTTF* pTransFont;
	CCLabelTTF *pDebugLabel;

	int batLastActId;
	int batLastActType[SCR_BATTLE_MAX_HEROS];
	int batLastTgtId[SCR_BATTLE_MAX_HEROS];

	bool isInnUsed;
public:
	
	EvtMain(GameMain *main, int mapid, LmuMain *mapdata, CCNode *layer);
	EvtMain(GameMain *main, LsdMapEventStack *stack, CCNode *layer, ScrBattleBattlerMgr *battler );

	~EvtMain();


	void reloadShaders();

	void doEvents(int framedelta);
	void doEvent(GlbEvent &event);
	void doTouchByHero( int eventid, int pageid );
	void doTouchByEvent( int eventid, int pageid );
	void doClickEvent( int eventid, int pageid );
	bool isRuningSerial();

	void setLastActID(int actID);
	void setLastAction(int actID, int actType, int tgtID);

	LsdMapEventStack *getLastStack(evtcode code);
private:

	//Debug
	void drawDebug();

	//Event
	void addEventInStack(int eventid, LsdMapEventStack &stack, GlbEvent &event);
	void chkParallelEventCommon(int framedelta);
	void chkParallelEventMap(int framedelta);
	void findSerialEvent();
	bool doEvent(LsdMapEventStack *stack);

	//Graphic
	void initDebug();
	void initMsg();
	void destroyMsg();

	int getRequest(LsdMapEventStack *pEvent, int eventpos);

	void drawMsg(int framedelta);

	void reqtMessage(const TString &str);
	void reqtChoice	(const TString &str, int msg_select_count, bool canCancel, int msg_nonselect_count = 0);
	void reqtNumber(const TString &str, int int_count, int msg_nonselect_count = 0 );
	void reqtTranslate( const TString & str, int sel_cnt );

	bool endMsg	(LsdMapEventStack *pEvent);
	void setMessagePosition(int eventtype);
	bool usingMsg();

	void changeSystem(TString name);

	//Face
	void showFace();
	void hideFace();
	
	//Information
	void addMsgInStack();
	void addMsgInStack(LsdMapEventData &edatas, GlbEventNode &edata, ArrayList<TString> &msg);
	LsdCharacter* getCharacter(int index);
	LsdObjectCommon* getEvent(int my_eventid, int finding_eventid);
	int getEventID( int n );
	inline int GetPara(GlbEventNode &edata, int index);
	LsdMapEventStack* getLastStack( evtcode code, LsdMapEventStack *stack);

	//Etc
	void doInnEvent(int type,int cost);
public:
	static LsdObjectCommon* getEvent(LsdMain *lsd, int finding_eventid);
	static void setSw( LsdMain *main, int index, bool val );
	int getRunningSerialMapID();
};

TString getString(evtcode code);
#endif 
