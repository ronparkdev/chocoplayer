#include "ScrBattleStateManager.h"

#include "../../COMMON/ComMath.h"

ScrBattleStateMgr::ScrBattleStateMgr( GameMain *main, ScrBattleBattlerMgr *battler )
	:m_main(main),
	 m_battler(battler)
{

}

void ScrBattleStateMgr::setState(int id, int stateID, bool val) {
	if (id < 0 || id >= SCR_BATTLE_MAX_ENEMIES + SCR_BATTLE_MAX_HEROS)
		return;

	int lastTopState = m_battler->getTopState(id);
	m_battler->setState(id, stateID, val);

	if (m_battler->getTopState(id) != lastTopState)
		m_TurnElasped[id] = 0;

}

void ScrBattleStateMgr::doAttack( int actID, int tgtID, bool isSecondWeapon)
{
	if (!m_battler->isHero(actID))
		return;

	GlbEquipment equip = m_battler->getEquip(actID);
	
	LdbItem *item;
	if (!isSecondWeapon)
		item = &(m_main->ldb.dItem[equip.weapon_id-1]);
	else
		item = &(m_main->ldb.dItem[equip.shield_id-1]);

	for(int i=0; i<item->state_effects.GetLength() &&
		i<m_main->ldb.dCond.GetLength(); i++) {
			if (item->state_effects[i] && RANDF() < item->state_chance/100.0f) {
				setState(tgtID, i+1, item->state_effect);//TODO
			}
	}
}

bool ScrBattleStateMgr::isAttackEffect( int actID, int cur_state_id, bool isSecondWeapon)
{
	if (!m_battler->isHero(actID))
		return false;

	GlbEquipment equip = m_battler->getEquip(actID);

	LdbItem *item;
	if (!isSecondWeapon)
		item = &(m_main->ldb.dItem[equip.weapon_id-1]);
	else
		item = &(m_main->ldb.dItem[equip.shield_id-1]);
	
	return (0<cur_state_id && cur_state_id <= item->state_effects.GetLength() && 
		item->state_effects[cur_state_id-1]);
}


void ScrBattleStateMgr::doSkill( int tgtID, int skillID )
{
	LdbSkill &skill = m_main->ldb.dSkill[skillID-1];

	for(int i=0; i<skill.state_effects.GetLength() &&
		i<m_main->ldb.dCond.GetLength(); i++) {
			if (skill.state_effects[i]) {
				setState(tgtID, i+1, skill.state_effect);//TODO
			}
	}

	int tgtState = m_battler->getTopState(tgtID);

	if (tgtState <= 0)
		return;

	LdbCondition &cond = m_main->ldb.dCond[tgtState-1];

	if (RANDF() < cond.release_by_damage/100.0f * skill.pdef_f / 10.0f) {
		setState(tgtID, tgtState, false);
	}
}

void ScrBattleStateMgr::doEndTurn()
{
	for(int i=0; i<SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES; i++) {
		if (m_battler->getID(i) <= 0)
			continue;
		
		int turn = m_TurnElasped[i]++;

		int stateID = m_battler->getTopState(i);

		if (stateID <= 0)
			continue;

		LdbCondition &cond = m_main->ldb.dCond[stateID-1];

		if (turn >= cond.hold_turn && RANDF() < cond.auto_release_prob/100.0f)
			setState(i, stateID, false);

		int maxHP = m_battler->get(i, MAXHP);
		int maxMP = m_battler->get(i, MAXMP);
		int curHP = m_battler->get(i, HP);

		int effHP = maxHP * cond.hp_change_max / 100 + cond.hp_change_val;
		int effMP = maxMP * cond.mp_change_max / 100 + cond.mp_change_val;
	
		if (curHP > 0 && curHP - effHP <= 0)
			effHP = curHP-1;

		if (effHP > 0) {
			switch(cond.hp_change_type) {
				case 1:m_battler->add(i, HP, effHP);break;
				case 2:m_battler->add(i, HP, -effHP);break;
			}
		}

		if (effMP > 0) {
			switch(cond.mp_change_type) {
			case 1:m_battler->add(i, MP, effMP);break;
			case 2:m_battler->add(i, MP, -effMP);break;
			}
		}
	}
}
