#ifndef __R2K_SCR_BATTLE_STATE_MANAGER__
#define __R2K_SCR_BATTLE_STATE_MANAGER__
#include "../../COMMON/DebugingCode.h"

#include "ScrBattleBattlerManager.h"
#include "../../GameMain.h"

class ScrBattleStateMgr{
	GameMain * m_main;
	ScrBattleBattlerMgr *m_battler;

	int m_TurnElasped[SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES];
public:
	ScrBattleStateMgr( GameMain *main, ScrBattleBattlerMgr *battler );

	void setState(int id, int stateID, bool val);
	void doAttack(int actID, int tgtID, bool isSecondWeapon);
	void doSkill(int tgtID, int skillID);
	void doEndTurn();
	bool isAttackEffect( int actID, int cur_state_id, bool isSecondWeapon);
};

#endif
