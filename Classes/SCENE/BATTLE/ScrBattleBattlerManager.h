#ifndef __R2K_SCR_BATTLE_BATTER_MANAGER__
#define __R2K_SCR_BATTLE_BATTER_MANAGER__
#include "../../COMMON/DebugingCode.h"

#include "ScrBattleType.h"
#include "../../GameMain.h"
#include "../../DATA/ArrayList.h"
#include "../../COMMON/ComMgrCMD.h"
#include "../../COMMON/MgrCharacter.h"

#define SCR_BATTLE_MAX_HEROS 4
#define SCR_BATTLE_MAX_ENEMIES 8

#define EFFECT_ALL_HEROS -1
#define EFFECT_ALL_ENEMIES -2

class ScrBattleBattlerInfo{
public:
	int invisible;//for Enemy
	
	int data_id;

	int mhp;
	int mmp;
	int chp;
	int cmp;
	int catk;
	int cdef;
	int cspt;
	int cagi;

	int defence_type;
	int critical_chance;
	int is_power_up;

	int def_pos_x;
	int def_pos_y;
	int turn_count;

	ArrayList<bool> ccond;
};

class ScrBattleBattlerMgr{
	GameMain * m_main;
	ScrBattleBattlerInfo data[SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES];

	MgrCharacter mchar;

	int m_hero_length;
	int m_enemy_length;
	ArrayList<GlbEventNode> m_event_queue;
	ArrayList<byte> m_last_added_skills;
public:
	ScrBattleBattlerMgr( GameMain *main, int battle_id, BatPos pos);
	int getID( int id );
	void setID( int id, int mID );

	int get(int id, CMGR_CMD cmd);
	TString getS(int id, CMGR_CMD cmd);
	int add(int id, CMGR_CMD cmd, int val);
	int getTopState(int id);
	TString getStateStr(int id);
	bool getState(int id, int stateID);
	void setState(int id, int stateID, bool val);
	void clearState(int id);

	GlbEquipment getEquip( int id );

	CCPoint getDefPos( int id );
	void setDefPos( int id, const CCPoint &point );

	bool isHero( int id );
	bool isPreventCritical( int id );
	bool isRaiseEvasion( int id );
	bool isStateEff(int id, int stateID, bool isDeffence);
	bool isAlive( int id );

	void doEvent(const GlbEventNode &edata );
	bool getEvent(GlbEventNode &node);

	void getLastLevelUpMsg(ArrayList<TString>& msg);
private:
	void set( int id, CMGR_CMD cmd, int val );
	int chk(int id, CMGR_CMD cmd, int val);

	int getLevel(int hid, int exp);
};

#endif
