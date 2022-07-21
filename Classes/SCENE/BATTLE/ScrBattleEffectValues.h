#ifndef __R2K_SCR_BATTLE_EFFECT_VALUES__
#define __R2K_SCR_BATTLE_EFFECT_VALUES__
#include "../../COMMON/DebugingCode.h"

#include "ScrBattleBattlerManager.h"
#include "../../GameMain.h"

class ScrBattleEffectValues{
	GameMain * pGM;
	ScrBattleBattlerMgr *pBattler;
	bool isLastAttackIsCritical;
public:
	ScrBattleEffectValues( GameMain *main, ScrBattleBattlerMgr *battler );

	int getAttackEffVal(int actID, int targetID);
	float getAttackEffChance( int actID, int tgtID, bool isSecondWeapon = false);
	int getSkillEffVal( int actID, int tgtID, int skillID, bool isIgnoreDef);
	float getSkillEffChance(int actID, int targetID, int skillID);

	bool isSkillUseable(int actID, int skillID);
	int getSkillSpendMp(int actID, int skillID);

	float getEscapePer(bool isHero);

	bool isAttackAll(int actID);
	bool isSkillAll(int actID, int skillID);

	bool isLastAttackCritical();
};

#endif
