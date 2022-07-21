#include "ScrBattle3.h"

#include "../../COMMON/ComMath.h"
#include "../../COMMON/MgrItem.h"
#include "../../COMMON/MgrSkill.h"
#include "../../COMMON/MgrState.h"

void ScrBattle3::showDamage(int tgtID, int dam) {
	for(int i=0; i<SCR_BATTLE3_MAX_NUMBER; i++) {
		if (pDmgNum[i]->isSet())
			continue;

		float posX, posY;

		if (battler.isHero(tgtID)) {
			pNodeHero[tgtID]->getPosition(&posX, &posY);
		} else {
			pNodeEnemy[tgtID-SCR_BATTLE_MAX_HEROS]->getPosition(&posX, &posY);
		}

		pDmgNum[i]->set(posX, posY, dam);
		break;
	}
}

void ScrBattle3::doAttack(int actID, int tgtID, bool isSecondWeapon) {
	int dmg = effvals.getAttackEffVal(actID, tgtID);
	float chance = effvals.getAttackEffChance(actID, tgtID, isSecondWeapon);

	if (chance > RANDF() && dmg > 0) {
		showDamage(tgtID, dmg);

		if (battler.isHero(tgtID))
			pGM->snd.PlaySound(pGM->lsd.System.se_actor_damaged);
		else
			pGM->snd.PlaySound(pGM->lsd.System.se_enemy_damaged);

		battler.add(tgtID, HP, -dmg);
		state.doAttack(actID, tgtID, isSecondWeapon);
	} else {
		pGM->snd.PlaySound(pGM->lsd.System.se_dodge);
	}

}

void ScrBattle3::doSkill(int actID, int tgtID, int skillID) {

	MgrSkill mskill(pGM);
	LdbSkill &skill = pGM->ldb.dSkill[skillID-1];

	int dmg = effvals.getSkillEffVal(actID, tgtID, skillID, skill.ignore_dmg);
	float chance = effvals.getSkillEffChance(actID, tgtID, skillID);
	bool isEffectMyTeam = mskill.isEffMyTeam(skillID);

	if (isEffectMyTeam)
		dmg *= -1;

	bool isHPMPSuccessed = false;

	if (skill.affect_hp && chance > RANDF()) {
		showDamage(tgtID, dmg);
		
		battler.add(tgtID, HP, -dmg);

		if (skill.absorb_dmg && !isEffectMyTeam)
			battler.add(actID, HP, dmg);

		isHPMPSuccessed = true;
	}

	if (skill.affect_mp && chance > RANDF()) {
		battler.add(tgtID, MP, -dmg);

		if (skill.absorb_dmg && !isEffectMyTeam)
			battler.add(actID, MP, dmg);

		isHPMPSuccessed = true;
	}

	if ((!skill.affect_hp && !skill.affect_mp) || isHPMPSuccessed) {

		state.doSkill(tgtID, skillID);

		if (skill.affect_attack && chance > RANDF())
			battler.add(tgtID, ATK, -dmg);

		if (skill.affect_defence && chance > RANDF())
			battler.add(tgtID, DEF, -dmg);

		if (skill.affect_spirit && chance > RANDF())
			battler.add(tgtID, SPT, -dmg);

		if (skill.affect_agility && chance > RANDF())
			battler.add(tgtID, AGI, -dmg);
	}

}

void ScrBattle3::doItem(int actID, int tgtID, int itemID) {
	LdbItem &item = pGM->ldb.dItem[itemID-1];
	MgrItem mitem(pGM);

	pGM->snd.PlaySound(pGM->lsd.System.se_item);

	switch(item.Type) {
		case 6://Medicine
		{
			if (!mitem.isAllEffect(itemID))
				doItemOne(tgtID, itemID);
			else for(int i=0; i<nodeHeroLen; i++)
				doItemOne(i, itemID);
			break;
		}

		case 9://Skill
		{
			break;
		}

		case 10://Switch
		{
			MgrState mstate(pGM);
			mstate.setSw(item.switch_id, true);
			break;
		}
	}

	mitem.useItem(itemID);
}

void ScrBattle3::doItemOne(int tgtID, int itemID)
{
	LdbItem &item = pGM->ldb.dItem[itemID-1];
	MgrCharacter mchar(pGM);

	switch(item.Type) {
	case 6:
		{//Poison
			int maxHP = battler.get(itemID, MAXHP);
			int maxMP = battler.get(itemID, MAXMP);
			int curHP = battler.get(itemID, HP);
			int curMP = battler.get(itemID, MP);

			int healHP = maxHP*item.recover_hp/100 + item.recover_hp_rate;
			int healMP = maxMP*item.recover_mp/100 + item.recover_mp_rate;

			if (healHP>0 && curHP < maxHP) {
				battler.add(itemID, HP, healHP);
				
				showDamage(tgtID, healHP);
			}
			if (healMP>0 && curMP < maxMP)
				battler.add(itemID, MP, healMP);

			for(int n=0; n<item.state_effects.GetLength(); n++)
				if (item.state_effects[n] && battler.getState(tgtID, n))
					mchar.setState(tgtID, n, false);

			break;
		}
	}
}