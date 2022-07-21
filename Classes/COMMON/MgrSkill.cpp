#include "MgrSkill.h"

#include "MgrCharacter.h"
#include "MgrState.h"
#include "MgrSetting.h"
#include "MgrTeleport.h"

MgrSkill::MgrSkill( GameMain *main )
{
	pGM = main;
}

bool MgrSkill::isUseable( int hID, int sID, bool isBattle /*= false*/ )
{
	if (	hID< 0 || pGM->ldb.dChar.GetLength()  < hID ||
		sID<=0 || pGM->ldb.dSkill.GetLength() < sID)
			return false;

	MgrCharacter mchar(pGM);
	MgrTeleport mtele(pGM);

	if (hID != 0 && mchar.get(hID, MP) < getSpendMP(hID, sID))
		return false;

	LdbSkill sk = pGM->ldb.dSkill[sID-1];

	GlbEquipment equip = mchar.getEquip(hID);

	ArrayList<LdbItem> &items = pGM->ldb.dItem;

	for(int i=0; i<sk.attribute_effects.GetLength(); i++) {
		if (!sk.attribute_effects[i])
			continue;

		LdbAttribute &att = pGM->ldb.dAttr[i];

		if (att.type == 0) {
			bool chk = false;

			if (equip.weapon_id > 0 &&
				i < items[equip.weapon_id-1].attribute_set.GetLength() &&
				items[equip.weapon_id-1].attribute_set[i])
					chk = true;

			if (equip.shield_id > 0 &&
				i < items[equip.shield_id-1].attribute_set.GetLength() &&
				items[equip.shield_id].attribute_set[i])
				chk = true;

			if (equip.helmet_id > 0 &&
				i < items[equip.helmet_id-1].attribute_set.GetLength() &&
				items[equip.helmet_id-1].attribute_set[i])
				chk = true;

			if (equip.armor_id > 0 &&
				i < items[equip.armor_id-1].attribute_set.GetLength() &&
				items[equip.armor_id-1].attribute_set[i])
				chk = true;

			if (equip.accessory_id > 0 &&
				i < items[equip.accessory_id-1].attribute_set.GetLength() &&
				items[equip.accessory_id-1].attribute_set[i])
				chk = true;


			if (!chk)
				return false;
		}
	}

	if (!isBattle) {
		//is field..
		switch(sk.type) {
			case 1:return mtele.isTeleportable();
			case 2:return mtele.isEscapeable();
			case 3:
				if (sk.occasion_field)
					return true;
				else
					return false;

			default:
				if (sk.scope>=2)
					return true;
				else
					return false;
		}
	} else {
		switch(sk.type) {
			case 1:
			case 2:return false;
			case 3:
				if (sk.occasion_battle)
					return true;
				else
					return false;

			default:return true;
		}
	}
}

bool MgrSkill::isAllEffect( int sID )
{
	if ( sID<=0 || pGM->ldb.dSkill.GetLength() < sID)
		return false;

	LdbSkill sk = pGM->ldb.dSkill[sID-1];

	return (sk.scope == 1 || sk.scope == 4);
}

bool MgrSkill::isEffMyTeam( int sID )
{
	if ( sID<=0 || pGM->ldb.dSkill.GetLength() < sID)
		return false;

	LdbSkill sk = pGM->ldb.dSkill[sID-1];

	switch(sk.scope) {
		case 0:
		case 1:
			return false;
		case 2:
		case 3:
		case 4:
			return true;
				
		default:{
			//WTF
			return false;
		}
	}
}

bool MgrSkill::isNotSelect( int sID )
{
	if ( sID<=0 || pGM->ldb.dSkill.GetLength() < sID)
		return false;

	LdbSkill sk = pGM->ldb.dSkill[sID-1];

	switch(sk.type) {
		case 1:
		case 2:
		case 3:
			return true;
		default:
			return false;
	}
}

int MgrSkill::getEffVal( int hID, int sID )
{
	if (	hID<=0 || pGM->ldb.dChar.GetLength()  < hID ||
		sID<=0 || pGM->ldb.dSkill.GetLength() < sID)
		return 0;

	MgrCharacter mchar(pGM);
	int hAttack,hSpilit;
	hAttack = mchar.get(hID, ATK);
	hSpilit = mchar.get(hID, SPT);

	LdbSkill sk = pGM->ldb.dSkill[sID-1];

	int val = sk.power + hAttack*sk.pdef_f/20 + hSpilit*sk.mdef_f/40;

	val += val*(rand()*2-1)*(0.05*sk.variance);

	return val;
}

int MgrSkill::getSpendMP( int hID, int sID )
{
	if (	hID<=0 || pGM->ldb.dChar.GetLength()  < hID ||
		sID<=0 || pGM->ldb.dSkill.GetLength() < sID)
		return 0;

	MgrCharacter mchar(pGM);
	LdbSkill sk = pGM->ldb.dSkill[sID-1];

	int SpMP=0;
	int hMPMax = mchar.get(hID, MAXHP);

	if (sk.mp_type == 0)
		SpMP = sk.mp_cost;
	else
		SpMP = hMPMax * sk.mp_per / 100;

	return SpMP;
}

bool MgrSkill::useSkill( int userID, int targetID, int skillID, bool useMP /*= true*/ ) {
	if (	pGM->ldb.dChar.GetLength()  < userID ||
		pGM->ldb.dChar.GetLength()  < targetID ||
		pGM->ldb.dSkill.GetLength() < skillID)
		return false;

	MgrTeleport mtele(pGM);
	LdbSkill i = pGM->ldb.dSkill[skillID-1];

	bool Used = false;

	int SpMP = getSpendMP(userID, skillID);

	switch(i.type) {
		case 1: 
		{
			ScreenLayerManager *slm = ScreenLayerManager::getInstance();
			slm->setSettingScrMenu(scr_menu_tel, skillID, userID);
			slm->setScreen(scr_menu);
			Used = true;
			break;
		}
		case 2:
		{
			mtele.moveEscape();
			Used = true;
			break;
		}
		case 3:
		{
			MgrState mstate(pGM);
			mstate.setSw(i.switch_id, true);

			pGM->slm->setSettingScrMap();
			pGM->slm->setScreen(scr_map);
		
			Used = true;
			break;
		}
		case 0:
		case 4:
		{
			switch(i.scope) {
				case 2:Used |= useSkillOne(userID, userID, skillID);break;
				case 3:Used |= useSkillOne(userID, targetID, skillID);break;
				case 4:
				{
					ArrayList<short> &party = pGM->lsd.State.party;
					for(int n=0; n<party.GetLength(); n++)
						Used |= useSkillOne(userID, party[n], skillID);
				
					break;
				}
			}
		}
	}

	if (useMP) {
		MgrCharacter mchar(pGM);
		mchar.add(userID, MP, -SpMP);
	}
	return Used;
}

bool MgrSkill::useSkillOne( int userID, int targetID, int skillID )
{
	bool itemUsed = false;

	int val = getEffVal(userID, skillID);

	MgrCharacter mchar(pGM);
	LdbSkill i = pGM->ldb.dSkill[skillID-1];

	int maxHP = mchar.get(targetID, MAXHP);
	int maxMP = mchar.get(targetID, MAXMP);
	int curHP = mchar.get(targetID, HP);
	int curMP = mchar.get(targetID, MP);

	if (i.affect_hp && curHP < maxHP) {
		mchar.add(targetID, HP, val);
		itemUsed = true;
	}
	if (i.affect_mp && curMP < maxMP) {
		mchar.add(targetID, MP, val);
		itemUsed = true;
	}
	if (i.affect_attack)
		if (mchar.add(targetID, ATK, val)>0)
			itemUsed = true;

	if (i.affect_defence)
		if (mchar.add(targetID, DEF, val)>0)
			itemUsed = true;

	if (i.affect_spirit)
		if (mchar.add(targetID, SPT, val)>0)
			itemUsed = true;

	if (i.affect_agility)
		if (mchar.add(targetID, AGI, val)>0)
			itemUsed = true;

	for(int n=0; n<i.state_effects.GetLength(); n++) {
		if (i.state_effects[n] && mchar.getState(targetID, n+1)) {
			mchar.setState(targetID, n+1, false);
			itemUsed = true;
		}
	}

	return itemUsed;
}

TString MgrSkill::getName( int skillID )
{
	TString str;
	if (skillID <= 0 || skillID > pGM->ldb.dSkill.GetLength())
		return "";
	else if (MgrSetting::getInstance()->getBool(RST_TRANSLATE_LDB) && pGM->trans.getSkillName(skillID, str))
		return str;
	else
		return pGM->ldb.dSkill[skillID-1].name;
}


TString MgrSkill::getDesc( int skillID )
{
	TString str;
	if (skillID <= 0 || skillID > pGM->ldb.dSkill.GetLength())
		return "";
	else if (MgrSetting::getInstance()->getBool(RST_TRANSLATE_LDB) && pGM->trans.getSkillDesc(skillID, str))
		return str;
	else
		return pGM->ldb.dSkill[skillID-1].desc;
}
