#include "ScrBattleEffectValues.h"

#include "../../COMMON/MgrEquip.h"
#include "../../COMMON/MgrSkill.h"
#include "../../COMMON/ComMath.h"

ScrBattleEffectValues::ScrBattleEffectValues( GameMain *main, ScrBattleBattlerMgr *battler )
{
	pGM = main;
	pBattler = battler;
}

int ScrBattleEffectValues::getAttackEffVal( int actID, int tgtID)
{
	int actAtk = pBattler->get(actID, ATK);
	int tgtDef = pBattler->get(tgtID, DEF);

	int val = actAtk/2 - tgtDef/4;

	float criticalChance = pBattler->get(actID, BAT_CRITICAL_CHANCE) / 100.0f;

	if (RANDF() < criticalChance && !pBattler->isPreventCritical(tgtID)) {
		val *= 3;
		isLastAttackIsCritical = true;
	} else {
		isLastAttackIsCritical = false;
	}

	if (pBattler->get(actID, BAT_POWER_UP) == 1)
		val *= 2;

	switch(pBattler->get(tgtID, BAT_DEFENCE_TYPE)) {
		case 1:val /= 2;break;
		case 2:val /= 4;break;
	}

	val += (int)(val * (RANDF() * 2.0f - 1.0f) * 0.2f);

	if (val < 0)
		val = 0;

	ArrayList<byte> *tgtAtts;
	int tgtLdbID = pBattler->getID(tgtID);
	if (pBattler->isHero(tgtID))
		tgtAtts = &(pGM->ldb.dChar[tgtLdbID-1].attribute_ranks);
	else
		tgtAtts = &(pGM->ldb.dEnemy[tgtLdbID-1].attribute_ranks);

	bool attWeaponF = false, attMagicF = false;
	float attWeapon = 1.0f, attMagic = 1.0f;
	int attVal;
	bool equiped_stateeff;
	for(int i=0; i<pGM->ldb.dAttr.GetLength(); i++) {

			LdbAttribute &att = pGM->ldb.dAttr[i];

			equiped_stateeff = pBattler->isStateEff(actID, i, false);

			if (equiped_stateeff && i<tgtAtts->GetLength())
				attVal = (*tgtAtts)[i];
			else
				attVal = 2;

			if (attVal > 0 && pBattler->isStateEff(tgtID, i, true))
				attVal--;

			float tval = 0.0f;

			switch(attVal) {
			case 0:tval = att.a_rate/100.0f;break;
			case 1:tval = att.b_rate/100.0f;break;
			case 2:tval = att.c_rate/100.0f;break;
			case 3:tval = att.d_rate/100.0f;break;
			case 4:tval = att.e_rate/100.0f;break;
			}

			if (att.type == AttrType_Physical) {
				//Effect to Weapon
				if (equiped_stateeff && !attWeaponF || attWeapon < tval) {
					attWeapon = tval;
					attWeaponF = true;
				}
			} else {
				//Effect to Magic
				if (!attMagicF || attMagic < tval) {
					attMagic = tval;
					attMagicF = true;
				}
			}
	}

	
	//TODO 2.2.2 need check
	val *= attWeapon;

	return val;
}

float ScrBattleEffectValues::getAttackEffChance( int actID, int tgtID, bool isSecondWeapon)
{
	float val = 1.0f;

	int actAgi = pBattler->get(actID, AGI);
	int tgtAgi = pBattler->get(tgtID, AGI);

	int actState = pBattler->getTopState(tgtID);
	int tgtState = pBattler->getTopState(tgtID);

	bool is_ignore_evasion = false;
	int chance = 100;
	if (pBattler->isHero(actID)) {
		GlbEquipment equip = pBattler->getEquip(actID);
		LdbItem *item;
		if (!isSecondWeapon) {
			item = &(pGM->ldb.dItem[equip.weapon_id-1]);
		} else {
			item = &(pGM->ldb.dItem[equip.shield_id-1]);
		}

		chance = item->hit;
		is_ignore_evasion = item->ignore_evasion;
	} else {
		int enemyID = pBattler->getID(tgtID);
		if (pGM->ldb.dEnemy[enemyID-1].miss)
			chance = 50;
		else
			chance = 100;

		if (pBattler->isRaiseEvasion(tgtID))
			val *= 0.75f;
	}

	if (actState > 0) {
		val = pGM->ldb.dCond[actState-1].reduce_hit_ratio/100.0f;
	}

	if (tgtState > 0) {
		if (pGM->ldb.dCond[tgtState-1].avoid_attacks)
			val = 0.0f;
	}

	if (tgtState > 0 && pGM->ldb.dCond[tgtState-1].restriction == 1) {
		//Limit Action State
		val *= 1.0f;
	} else if (is_ignore_evasion) {
		val *= chance/100.0f;
	} else {
		val *= (100 - (100-chance)*(1+(tgtAgi/actAgi)/2))/100.0f;
	}

	return val;
}

int ScrBattleEffectValues::getSkillEffVal( int actID, int tgtID, int skillID, bool isIgnoreDef)
{
	MgrSkill mskill(pGM);
	LdbSkill *skill = &(pGM->ldb.dSkill[skillID-1]);
	int actAtk, actSpt, tgtDef, tgtSpt;

	actAtk = pBattler->get(actID, ATK);
	actSpt = pBattler->get(actID, SPT);
	tgtDef = pBattler->get(tgtID, DEF);
	tgtSpt = pBattler->get(tgtID, SPT);

	int val = skill->power + 
			actAtk * skill->pdef_f/20 +
			actSpt * skill->mdef_f/40;

	if (pBattler->isHero(actID) != pBattler->isHero(tgtID) && !isIgnoreDef) {
		int def = tgtDef*skill->pdef_f/40 + tgtSpt*skill->mdef_f/80;

		if (!skill->ignore_dmg)
			val-= def;

		switch(pBattler->get(tgtID, BAT_DEFENCE_TYPE)) {
			case 1:val /= 2;break;
			case 2:val /= 4;break;
		}
	}

	int actLdbID = pBattler->getID(actID);
	int tgtLdbID = pBattler->getID(tgtID);
	ArrayList<byte> *tgtAtts;
	if (pBattler->isHero(tgtID)) {
		tgtAtts = &(pGM->ldb.dChar[tgtLdbID-1].attribute_ranks);
	} else {
		tgtAtts = &(pGM->ldb.dEnemy[tgtLdbID-1].attribute_ranks);
	}

	val += (int)(val * (RANDF() * 2.0f - 1.0f) * 0.2f);

	if (val<0)val = 0;



	ArrayList<bool> &skillAtts = skill->attribute_effects;

	bool attWeaponF = false, attMagicF = false;
	float attWeapon = 1.0f, attMagic = 1.0f;
	int attVal;

	for(int i=0; i<skillAtts.GetLength() && 
		i<pGM->ldb.dAttr.GetLength(); i++) {
		
		if (!skillAtts[i])
			continue;
		
		LdbAttribute &att = pGM->ldb.dAttr[i];

		if (i<tgtAtts->GetLength())
			attVal = (*tgtAtts)[i];
		else
			attVal = 2;

		if (attVal > 0 && pBattler->isStateEff(tgtID, i, true))
			attVal--;

		if (skill->affect_attr_defence)
			if (mskill.isEffMyTeam(skillID)) {
				if (attVal < 4)
					attVal++;
			} else {
				if (attVal > 0)
					attVal--;
			}

		float tval = 0.0f;

		switch(attVal) {
			case 0:tval = att.a_rate/100.0f;break;
			case 1:tval = att.b_rate/100.0f;break;
			case 2:tval = att.c_rate/100.0f;break;
			case 3:tval = att.d_rate/100.0f;break;
			case 4:tval = att.e_rate/100.0f;break;
		}

		if (att.type == AttrType_Physical) {
			//Effect to Weapon
			if (!attWeaponF || attWeapon < tval) {
				attWeapon = tval;
				attWeaponF = true;
			}
		} else {
			//Effect to Magic
			if (!attMagicF || attMagic < tval) {
				attMagic = tval;
				attMagicF = true;
			}
		}
	}

	val *= attWeapon * attMagic;

	return val;
}

float ScrBattleEffectValues::getSkillEffChance( int actID, int tgtID, int skillID )
{
	LdbSkill *skill = &(pGM->ldb.dSkill[skillID-1]);

	if (skill->fail_msg == 3) {//Only RPG2000, it is assume normal attack
		return getAttackEffChance(actID, tgtID);
	} else {
		float hitper = 1.0f;
		int actState = pBattler->getTopState(actID);
		
		if (actState > 0)
			hitper = pGM->ldb.dCond[actState-1].reduce_hit_ratio/100.0f;

		return skill->hit/100.0f * hitper;
	}
}

bool ScrBattleEffectValues::isSkillUseable(int actID, int skillID) {
	
	MgrSkill mskill(pGM);
	LdbSkill &skill = pGM->ldb.dSkill[skillID-1];

	if (pBattler->isHero(actID)) {
		ArrayList<LdbCondition> &conds = pGM->ldb.dCond;

		for(int i=0; i<skill.attribute_effects.GetLength() &&
					 i<conds.GetLength(); i++)
		{
			if (skill.attribute_effects[i] && 
				conds[i].type == 1 &&
				!pBattler->isStateEff(actID, i, false)) {
					//If Have Weapon Condition, should have special Weapon
					return false;
			}
		}
	}

	if (pBattler->get(actID, MP) < getSkillSpendMp(actID, skillID))
		return false;

	return true;
}

int ScrBattleEffectValues::getSkillSpendMp(int actID, int skillID) {
	LdbSkill sk = pGM->ldb.dSkill[skillID-1];

	int SpMP=0;
	int hMPMax = pBattler->get(actID, MP);

	if (sk.mp_type == 0)
		SpMP = sk.mp_cost;
	else
		SpMP = hMPMax * sk.mp_per / 100;

	return SpMP;
}

float ScrBattleEffectValues::getEscapePer(bool isHero) {
	int AgiHAvg = 0, AgiEAvg = 0;
	int HLen = 0, ELen = 0;
	int i=0;

	float val;

	for(int i = 0; i<SCR_BATTLE_MAX_HEROS; i++) {
		if (pBattler->getID(i)>0) {
			AgiHAvg += pBattler->get(i, AGI);
			HLen++;
		}
	}
	for(; i<SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES; i++) {
		if (pBattler->getID(i)>0) {
			AgiEAvg += pBattler->get(i, AGI);
			ELen++;
		}
	}

	AgiHAvg /= HLen;
	AgiEAvg /= ELen;

	if (isHero)
		val = (1.5f - ((float)AgiEAvg / AgiHAvg));
	else
		val =  (1.5f - ((float)AgiHAvg / AgiEAvg));

	if (val < 0.0f)
		val = 0.0f;

	return val;
}

bool ScrBattleEffectValues::isAttackAll( int actID )
{
	bool isUseTwoHands = false;
	bool result = false; 

	GlbEquipment equip = pBattler->getEquip(actID);

	int actLdbID = pBattler->getID(actID);

	LdbCharacter &ldbCh = pGM->ldb.dChar[actLdbID-1];

	if (ldbCh.use_two_hand && equip.shield_id > 0)
		isUseTwoHands = true;

	LdbItem *item;

	if (isUseTwoHands && equip.shield_id > 0) {
		item = &(pGM->ldb.dItem[equip.shield_id-1]);
		if (item->attack_all)result = true;
	}

	if (equip.weapon_id > 0) {
		item = &(pGM->ldb.dItem[equip.weapon_id-1]);
		if (item->attack_all)result = true;
	}

	return result;
}

bool ScrBattleEffectValues::isSkillAll( int actID, int skillID )
{
	return true;
}

bool ScrBattleEffectValues::isLastAttackCritical() {
	return isLastAttackIsCritical;
}
