#include "ScrBattle0.h"

#include "../../COMMON/ComMath.h"
#include "../../COMMON/MgrSkill.h"
#include "../../COMMON/MgrItem.h"
bool ScrBattle0::doAction() {
	if (turnQueue.GetLength() == 0)
		return false;

	LdbVoca &voca = pGM->ldb.dVoca;

	int actID = turnQueue[0];
	int tgtID = heroActQueue[actID].target_id;
	int usingID = heroActQueue[actID].using_id;

	int actLdbID = battler.getID(actID);
	bool actIsHero = battler.isHero(actID);
	TString actName = battler.getS(actID, NAME);
	//bool isEffAll = false;

	turnQueue.Remove(0);

	if (!battler.isAlive(actID))
		return true;

	ScrBattle0ActionQueue &act = heroActQueue[actID];

	GlbEquipment equip;

	switch(act.type) {
	case B0AT_ACT_DOUBLEATTACK:
	case B0AT_ACT_ATTACK:
		{
			if (!battler.isAlive(tgtID)) {
				tgtID = getRandomPeople(actID, !battler.isHero(actID));
				if (tgtID == actID)
					return true;
			}

			bool isUseTwoHands = false;
			bool isUseRight = false;
			bool isUseLeft = false;
			bool isAttackAll = false; 

			int atkCntR = 1,	atkCntL = 1;
			int atkAniR = 1,	atkAniL = 1;
			int atkTimeR = 0,	atkTimeL = 0;

			LdbItem *item;

			addQueueMessageClear();
			addQueueMessage(actName + voca.attacking);

			if (actIsHero) {
				LdbCharacter &ldbCh = pGM->ldb.dChar[actLdbID-1];

				if (ldbCh.use_two_hand && equip.shield_id > 0)
					isUseTwoHands = true;

				atkAniL = atkAniR = pGM->ldb.dChar[actLdbID-1].unarmed_ani;
				equip = battler.getEquip(actID);
			}

			if (actIsHero && equip.weapon_id > 0) {
				isUseRight = true;
				item = &(pGM->ldb.dItem[equip.weapon_id-1]);

				atkAniR = item->animation_id;
				atkTimeR = GrpAniOne::getAnimationFrame(pGM, atkAniR);

				if (item->dual_attack)atkCntR *= 2;
				if (item->attack_all)isAttackAll = true;
			}

			if (actIsHero && isUseTwoHands && equip.shield_id > 0) {
				isUseLeft = true;
				item = &(pGM->ldb.dItem[equip.shield_id-1]);

				atkAniL = item->animation_id;
				atkTimeL = GrpAniOne::getAnimationFrame(pGM, atkAniL);

				if (item->dual_attack)atkCntL *= 2;
				if (item->attack_all)isAttackAll = true;
			}

			if (act.type == B0AT_ACT_DOUBLEATTACK) {
				atkCntL *= 2;
				atkCntR *= 2;
			}

			if (isAttackAll) {
				if (actIsHero)
					tgtID = EFFECT_ALL_ENEMIES;
				else
					tgtID = EFFECT_ALL_HEROS;
			}

			if (isUseRight || (!isUseLeft && !isUseRight))
				for(int i=0; i<atkCntR; i++)
					addQueueAnimation(tgtID, atkAniR);

			if (isUseLeft)
				for(int i=0; i<atkCntL; i++)
					addQueueAnimation(tgtID, atkAniL);

			if (isAttackAll) {
				if (tgtID == EFFECT_ALL_HEROS) {
					for(int i=0; i<nodeHeroLen; i++) {
						if (!battler.isAlive(i))
							continue;

						doAction(actID, i, B0AT_ACT_ATTACK, isUseLeft?1:0);
					}
				} else if (tgtID == EFFECT_ALL_ENEMIES) {
					for(int i=0; i<nodeEnemyLen; i++) {
						int id = i + SCR_BATTLE_MAX_HEROS;
						if (!battler.isAlive(id))
							continue;

						doAction(actID, id, B0AT_ACT_ATTACK, isUseLeft?1:0);
					}
				}
			} else {
				doAction(actID, tgtID, B0AT_ACT_ATTACK, isUseLeft?1:0);
			}

			break;
		}

	case B0AT_ACT_SKILL:
		{
			int skillID = heroActQueue[actID].using_id;
			LdbSkill &skill = pGM->ldb.dSkill[skillID - 1];

			MgrSkill mskill(pGM);

			bool isAttackAll = mskill.isAllEffect(skillID);

			if (isAttackAll) {
				if (actIsHero)
					tgtID = EFFECT_ALL_ENEMIES;
				else
					tgtID = EFFECT_ALL_HEROS;
			}

			addQueueMessageClear();
			addQueueMessage(skill.using_msg_1);
			if (skill.using_msg_2.getLength() > 0)
				addQueueMessage(skill.using_msg_2);

			addQueueAnimation(tgtID, skill.ani_id);

			if (isAttackAll) {
				if (tgtID == EFFECT_ALL_HEROS) {
					for(int i=0; i<nodeHeroLen; i++) {
						if (!battler.isAlive(i))
							continue;

						doAction(actID, i, B0AT_ACT_SKILL, skillID);
					}
				} else if (tgtID == EFFECT_ALL_ENEMIES) {
					for(int i=0; i<nodeEnemyLen; i++) {
						int id = i + SCR_BATTLE_MAX_HEROS;
						if (!battler.isAlive(id))
							continue;

						doAction(actID, id, B0AT_ACT_SKILL, skillID);
					}
				}
			} else {
				doAction(actID, tgtID, B0AT_ACT_SKILL, skillID);
			}

			break;
		}

	case B0AT_ACT_ITEM:
		{
			int itemID = heroActQueue[actID].using_id;
			MgrItem mitem(pGM);

			addQueueMessageClear();
			addQueueMessage(actName + "은 " + mitem.getName(itemID) + voca.use_item, true);

			if (mitem.isAllEffect(itemID)) {
				for(int i=0; i<nodeHeroLen; i++) {
					if (!battler.isAlive(i))
						continue;

					doAction(actID, i, B0AT_ACT_ITEM, itemID);
				}
			} else {
				doAction(actID, tgtID, B0AT_ACT_ITEM, itemID);
			}

			break;
		}

	case B0AT_ACT_NOTHING:
		{

			break;
		}

	default:
		{
			doAction(actID, tgtID, act.type, usingID);
			break;
		}
	}

	return true;
}

void ScrBattle0::doAction(int actID, int tgtID, ScrBattle0ActionType type, int subVal) {
	LdbVoca &voca = pGM->ldb.dVoca;
	LsdSystem &sys = pGM->lsd.System;

	GlobalLangInGame &lang = pGM->inlang;

	bool actIsHero = battler.isHero(actID);
	TString actName = battler.getS(actID, NAME);
	TString tgtName = battler.getS(tgtID, NAME);

	switch(type) {
	case B0AT_ACT_ATTACK:
		{		
			battler.add(actID, BAT_DEFENCE_TYPE, 0);
			battler.add(actID, BAT_POWER_UP, 0);

			int val = effVals.getAttackEffVal(actID, tgtID);

			if (effVals.isLastAttackCritical()) {
				if (battler.isHero(actID))
					addQueueMessage(voca.actor_critical, true);
				else
					addQueueMessage(voca.enemy_critical, true);
			}

			if (RANDF() < effVals.getAttackEffChance(actID, tgtID)) {
				if (battler.isHero(actID)) {
					if (val > 0) {
						addQueueSound(sys.se_actor_damaged);
						addQueueMessage(tgtName + lang.getLang(Battle_HeroTakeDamage) + val + voca.actor_damaged, true);
					} else {
						addQueueSound(sys.se_dodge);
						addQueueMessage(tgtName + lang.getLang(Battle_HeroTakeDamage) + voca.actor_undamaged, true);
					}
				} else {
					if (val > 0) {
						addQueueSound(sys.se_enemy_damaged);
						addQueueMessage(tgtName + lang.getLang(Battle_EnemyTakeDamage) + val + voca.enemy_damaged, true);
					} else {
						addQueueSound(sys.se_dodge);
						addQueueMessage(tgtName + lang.getLang(Battle_EnemyTakeDamage) + voca.enemy_undamaged, true);
					}
				}
			} else {
				addQueueSound(sys.se_dodge);
				addQueueMessage(tgtName + voca.dodge, true);
			}

			addQueueAction(actID, tgtID, B0AT_ACT_ATTACK, val, 0);

			break;
		}


	case B0AT_ACT_SKILL:
		{
			battler.add(actID, BAT_DEFENCE_TYPE, 0);

			int skillID = subVal;

			LdbSkill &skill = pGM->ldb.dSkill[skillID-1];

			MgrSkill mskill(pGM);

			int val = effVals.getSkillEffVal(actID, tgtID, subVal, skill.ignore_dmg);

			if (RANDF() > effVals.getSkillEffChance(actID, tgtID, subVal) || val <= 0) {
				addQueueMessage(skill.fail_msg);
				break;
			}

			if (skill.affect_hp && !skill.absorb_dmg) {
				if (battler.isHero(actID)) {
					addQueueSound(sys.se_actor_damaged);
					addQueueMessage(tgtName + " " + val + voca.actor_damaged);
				} else {
					addQueueSound(sys.se_enemy_damaged);
					addQueueMessage(tgtName + " " + val + voca.enemy_damaged);
				}
			} else {
				ArrayList<TString> att;

				if (skill.affect_hp)
					att.Add(voca.health_points);
				if (skill.affect_mp)
					att.Add(voca.magic_points);
				if (skill.affect_attack)
					att.Add(voca.attack);
				if (skill.affect_defence)
					att.Add(voca.defense);
				if (skill.affect_agility)
					att.Add(voca.agility);
				if (skill.affect_spirit)
					att.Add(voca.spirit);

				for(int i=0; i<att.GetLength(); i++) {
					if (!skill.absorb_dmg) {
						if (mskill.isEffMyTeam(skillID))
							addQueueMessage(tgtID + lang.getLang(Battle_Status) + att[i] + lang.getLang(Battle_Status2) + val + voca.parameter_increase, true);
						else
							addQueueMessage(tgtID + lang.getLang(Battle_Status) + att[i] + lang.getLang(Battle_Status2) + val + voca.parameter_decrease, true);
					} else {
						if (battler.isHero(tgtID))
							addQueueMessage(tgtName + lang.getLang(Battle_AbsorbHeroStatus) + att[i] + lang.getLang(Battle_AbsorbHeroStatus2) + val + voca.actor_hp_absorbed, true);
						else
							addQueueMessage(tgtName + lang.getLang(Battle_AbsorbEnemyStatus) + att[i] + lang.getLang(Battle_AbsorbEnemyStatus2) + val + voca.actor_hp_absorbed, true);
					}
				}
			}

			addQueueAction(actID, tgtID, B0AT_ACT_SKILL, val, skillID);

			break;
		}

	case B0AT_ACT_ITEM:
		{
			battler.add(actID, BAT_DEFENCE_TYPE, 0);

			int itemID = subVal;
			addQueueAction(actID, tgtID, B0AT_ACT_ITEM, 0, itemID);
			break;
		}

	case B0AT_ACT_DEFENCE:
		{
			int deffence_type = 1;
			if (battler.isHero(actID)) {
				int actLdbID = battler.getID(actID);
				LdbCharacter &c = pGM->ldb.dChar[actLdbID-1];

				if (c.super_guard)
					deffence_type = 2;
			}
			battler.add(actID, BAT_DEFENCE_TYPE, deffence_type);
			addQueueMessage(actName + voca.defending, true);
			break;
		}

	case B0AT_ACT_OBSERVING:
		{
			battler.add(actID, BAT_DEFENCE_TYPE, 0);
			addQueueMessage(actName + voca.observing, true);
			break;
		}

	case B0AT_ACT_GATHER_POWER:
		{
			battler.add(actID, BAT_DEFENCE_TYPE, 0);
			battler.add(actID, BAT_POWER_UP, 1);
			addQueueMessage(actName + voca.gather_power, true);
			break;
		}

	case B0AT_ACT_SELFDEST:
		{
			battler.add(actID, BAT_DEFENCE_TYPE, 0);
			addQueueMessage(actName + voca.self_dest, true);
			break;
		}

	case B0AT_ACT_ESCAPE:
		{
			battler.add(actID, BAT_DEFENCE_TYPE, 0);
			battler.add(actID, BAT_INVISIBLE, true);
			addQueueMessage(actName + voca.enemy_escape, true);
			break;
		}

	case B0AT_ACT_TRANSFORM:
		{
			addQueueMessage(actName + voca.enemy_transform, true);

			int id = tgtID - SCR_BATTLE_MAX_HEROS;

			pNodeEnemy[id]->change(subVal);
			break;
		}
	}
}

void ScrBattle0::doActionEffect( int actID, int tgtID, ScrBattle0ActionType type, int effVal, int usingID)
{
	LdbVoca &voca = pGM->ldb.dVoca;
	LsdSystem &sys = pGM->lsd.System;
	GlobalLangInGame &lang = pGM->inlang;

	int last_state_id, cur_state_id, last_state_priority, cur_state_priority;

	last_state_id = battler.getTopState(tgtID);

	TString tgtName = battler.getS(tgtID, NAME);

	ArrayList<ScrBattle0ActionQueue> queueBackup = actQueue;
	actQueue.RemoveAll();

	switch(type) {
	case B0AT_ACT_ATTACK:
		{
			battler.add(tgtID, HP, -effVal);
			state.doAttack(actID, tgtID, false);
			if (usingID != 0)state.doAttack(actID, tgtID, true);
			addQueueDamaged(tgtID);
			break;
		}

	case B0AT_ACT_SKILL:
		{
			int skillID = usingID;
			LdbSkill &skill = pGM->ldb.dSkill[skillID-1];
			MgrSkill mskill(pGM);

			bool isEffectMyTeam = mskill.isEffMyTeam(skillID);

			state.doSkill(tgtID, usingID);

			if (isEffectMyTeam)
				effVal *= -1;

			if (skill.affect_hp) {
				battler.add(tgtID, HP, -effVal);

				if (skill.absorb_dmg && !isEffectMyTeam)
					battler.add(actID, HP, effVal);
				addQueueDamaged(tgtID);
			}

			if (skill.affect_mp) {
				battler.add(tgtID, MP, -effVal);

				if (skill.absorb_dmg && !isEffectMyTeam)
					battler.add(actID, MP, effVal);
			}

			if (skill.affect_attack) {
				addQueueMessage(
					tgtName + lang.getLang(Battle_Status) + voca.attack + abs(effVal) + 
					lang.getLang(Battle_Status2) + ((effVal>0)?voca.parameter_decrease:voca.parameter_increase), true);
				battler.add(tgtID, ATK, -effVal);
			}
			if (skill.affect_defence) {
				addQueueMessage(
					tgtName + lang.getLang(Battle_Status) + voca.defense + abs(effVal) + 
					lang.getLang(Battle_Status2) + ((effVal>0)?voca.parameter_decrease:voca.parameter_increase), true);
				battler.add(tgtID, DEF, -effVal);
			}
			if (skill.affect_spirit) {
				addQueueMessage(
					tgtName + lang.getLang(Battle_Status) + voca.spirit + abs(effVal) + 
					lang.getLang(Battle_Status2) + ((effVal>0)?voca.parameter_decrease:voca.parameter_increase), true);
				battler.add(tgtID, SPT, -effVal);
			}
			if (skill.affect_agility) {
				addQueueMessage(
					tgtName + lang.getLang(Battle_Status) + voca.agility + abs(effVal) + 
					lang.getLang(Battle_Status2) + ((effVal>0)?voca.parameter_decrease:voca.parameter_increase), true);
				battler.add(tgtID, AGI, -effVal);
			}
			break;
		}

	case B0AT_ACT_ITEM:
		{
			int itemID = usingID;
			LdbItem &item = pGM->ldb.dItem[itemID-1];
			MgrItem mitem(pGM);

			switch(item.Type) {
			case 6:
				{//Poison
					int maxHP = battler.get(tgtID, MAXHP);
					int maxMP = battler.get(tgtID, MAXMP);
					int curHP = battler.get(tgtID, HP);
					int curMP = battler.get(tgtID, MP);

					int healHP = maxHP*item.recover_hp/100 + item.recover_hp_rate;
					int healMP = maxMP*item.recover_mp/100 + item.recover_mp_rate;

					if (healHP>0 && curHP < maxHP) {
						addQueueMessage(tgtName + lang.getLang(Battle_Status) + voca.health_points + healHP + voca.hp_recovery, true);
						battler.add(tgtID, HP, healHP);
					}
					if (healMP>0 && curMP < maxMP) {
						addQueueMessage(tgtName + lang.getLang(Battle_Status) + voca.magic_points + healMP + voca.hp_recovery, true);
						battler.add(tgtID, MP, healMP);
					}
					for(int n=0; n<item.state_effects.GetLength(); n++) {
						if (item.state_effects[n] && battler.getState(tgtID, n)) {
							battler.setState(tgtID, n, false);
						}
					}
					break;
				}

			case 10:
				{
					MgrState mstate(pGM);
					mstate.setSw(item.switch_id, true);
				}
			}
		}
	}


	cur_state_id = battler.getTopState(tgtID);

	ArrayList<LdbCondition> &conds = pGM->ldb.dCond;

	if (last_state_id != cur_state_id) {

		last_state_priority = cur_state_priority = 0;

		if (last_state_id > 0)
			last_state_priority = conds[last_state_id-1].priority;

		if (cur_state_id > 0)
			cur_state_priority = conds[cur_state_id-1].priority;

		if ((last_state_priority < cur_state_priority) ||
			(last_state_priority == cur_state_priority && last_state_id > cur_state_id)) {
				if (battler.isHero(tgtID))
					addQueueMessage(tgtName + conds[cur_state_id-1].message_actor, true);
				else
					addQueueMessage(tgtName + conds[cur_state_id-1].message_enemy, true);
		} else if ((last_state_priority > cur_state_priority) ||
			(last_state_priority == cur_state_priority && last_state_id < cur_state_id)) {
				addQueueMessage(tgtName + conds[last_state_priority-1].message_recovery, true);
		}

	} else if (cur_state_id > 0) {
		if (state.isAttackEffect(actID, cur_state_id, false))
			addQueueMessage(tgtName + conds[cur_state_id-1].message_already, true);
		else if (conds[cur_state_id-1].message_affected.getLength() > 0)
			addQueueMessage(tgtName + conds[cur_state_id-1].message_affected, true);
	}

	if (!battler.isHero(tgtID) && cur_state_id == 1)
		addQueueSound(sys.se_enemy_death);

	for(int i=0; i<queueBackup.GetLength(); i++)
		actQueue.Add(queueBackup[i]);
}