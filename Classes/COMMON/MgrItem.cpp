#include "MgrItem.h"

#include "MgrCharacter.h"
#include "MgrState.h"
#include "MgrSkill.h"
#include "MgrSetting.h"
#include "../GameMain.h"
#include "../DATA/LDB/LdbItem.h"

MgrItem::MgrItem( GameMain *main )
{
	pGM = main;
}

bool MgrItem::isHeroUseable( int hID, int iID )
{
	if (	iID<=0 || iID>pGM->ldb.dItem.GetLength() || 
		hID<=0 || hID>pGM->ldb.dChar.GetLength())
		return false;

	LdbItem &item = pGM->ldb.dItem[iID-1];

	bool ChkAct = false, ChkJob = false;

	if (hID < item.actor_set.GetLength())
		ChkAct = false;
	else if (!item.actor_set[hID-1])
		ChkAct = false;
	else
		ChkAct = true;

	if (hID < item.class_set.GetLength())
		ChkJob = false;
	else if (!item.class_set[hID-1])
		ChkJob = false;
	else
		ChkJob = true;

	return (ChkAct && ChkJob);
}

bool MgrItem::isUseable( int iID, bool isBattle )
{
	if (	iID<=0 || iID>pGM->ldb.dItem.GetLength()) 
		return false;

	MgrSkill mskill(pGM);
	LdbItem &item = pGM->ldb.dItem[iID-1];

	if (!isBattle) {
		//is field..
		switch(item.Type) {
		case 6:
		case 7:
		case 8:return true;

		case 9:
			if (item.use_skill)
				return mskill.isUseable(0, item.skill_id, isBattle);
			else
				return true;

		case 10://TODO 필드 스위치 체크
			if (item.occasion_field1 || item.occasion_field2)
				return true;
			else
				return false;

		default:return false;
		}
	} else {
		switch(item.Type) {
		case 6:
			if (!item.occasion_field1)
				return true;
			else 
				return false;

		case 9:
			if (item.use_skill)
				return mskill.isUseable(0, item.skill_id, isBattle);
			else
				return true;

		case 10:
			if (item.occasion_battle)
				return true;
			else
				return false;

		default:return false;
		}
	}
}

bool MgrItem::isEquipable( int hID, int iID )
{
	if (	iID<=0 || iID>pGM->ldb.dItem.GetLength() || 
		hID<=0 || hID>pGM->ldb.dChar.GetLength())
		return false;

	LdbItem &item = pGM->ldb.dItem[iID-1];

	MgrCharacter mchar(pGM);

	int heroJob = mchar.get(hID, JOB);

	switch(item.Type) {
		case 2:
		{
			int weaponID = mchar.getEquip(hID).weapon_id;
			if (0<weaponID && weaponID<=pGM->ldb.dItem.GetLength())
				if (pGM->ldb.dItem[weaponID-1].two_handed)
					return false;
			return true;
		}

		case 1:
		case 3:
		case 4:
		case 5:
		{
			if (!pGM->ldb.dSys.item_limit_by_job) {
				if (hID <= item.actor_set.GetLength() ) {
					if (!item.actor_set[hID-1])
						return false;
				}else
					return false;
			} else {
				if (heroJob <= item.class_set.GetLength() ) {
					if (!item.class_set[heroJob-1])
						return false;
				}else
					return false;
			}
			return true;
		}

		default:return false;
	}

}

bool MgrItem::isAllEffect( int iID )
{
	if (	iID<=0 || iID>pGM->ldb.dItem.GetLength()) 
		return false;

	LdbItem &item = pGM->ldb.dItem[iID-1];

	MgrSkill mskill(pGM);

	if (item.Type != 9)
		return pGM->ldb.dItem[iID-1].entire_party;
	else
		return mskill.isAllEffect(item.skill_id);
}

int MgrItem::getItemCount( int itemID )
{
	return pGM->lsdbuf.GetItem(itemID);
}

void MgrItem::addItemCount( int iID, int val)
{
	pGM->lsdbuf.AddItem(iID, val);
}

void MgrItem::useItem(int iID)
{
	pGM->lsdbuf.UseItem(iID);
}

bool MgrItem::useItem( int hID, int iID, bool mustuse )
{
	if (	iID<=0 || iID>pGM->ldb.dItem.GetLength() || 
		hID<=0 || hID>pGM->ldb.dChar.GetLength())
		return false;

	ArrayList<short> &party = pGM->lsd.State.party;
	LdbItem &item = pGM->ldb.dItem[iID-1];

	bool itemUsed = false;

	switch(item.Type) {
		case 6:
		{//Poison
			if (!item.entire_party)
				itemUsed |= useItemOne(hID, iID);
			else for(int n=0; n<party.GetLength(); n++)
				itemUsed |= useItemOne(party[n], iID);
			break;
		}
		case 7:
		case 8:
		case 10:
		{
			itemUsed |= useItemOne(hID, iID);
			break;
		}

		case 9:
		{
			itemUsed = mustuse;
			break;
		}
	}

	if (itemUsed || mustuse) {
		pGM->snd.PlaySound(pGM->lsd.System.se_item);
		useItem(iID);
	}

	return itemUsed;

}

bool MgrItem::useItemOne( int hID, int iID )
{
	if (	hID<=0 || hID>pGM->ldb.dChar.GetLength())
		return false;

	bool itemUsed = false;

	LdbItem &item = pGM->ldb.dItem[iID-1];

	MgrCharacter mchar(pGM);


	switch(item.Type) {
		case 6:
		{//Poison
			int maxHP = mchar.get(hID, MAXHP);
			int maxMP = mchar.get(hID, MAXMP);
			int curHP = mchar.get(hID, HP);
			int curMP = mchar.get(hID, MP);

			int healHP = maxHP*item.recover_hp/100 + item.recover_hp_rate;
			int healMP = maxMP*item.recover_mp/100 + item.recover_mp_rate;

			if (healHP>0 && curHP < maxHP) {
				mchar.add(hID, HP, healHP);
				itemUsed = true;
			}
			if (healMP>0 && curMP < maxMP) {
				mchar.add(hID, MP, healMP);
				itemUsed = true;
			}
			for(int n=0; n<item.state_effects.GetLength(); n++) {
				if (item.state_effects[n] && mchar.getState(hID, n)) {
					mchar.setState(hID, n, false);
					itemUsed = true;
				}
			}
			break;
		}

		case 7:
		{
			int skillID = item.skill_id - 1;
			if (0<=skillID && skillID<pGM->ldb.dSkill.GetLength()) {
				if (!mchar.getSkill(hID, skillID)) {
					mchar.setSkill(hID, skillID, true);
					itemUsed = true;
				}
			}
		}

		case 8:
		{//Seed
			if (	mchar.add(hID, MAXHP, item.max_hp_points)>0 ||
				mchar.add(hID,  MAXMP, item.max_sp_points)>0 ||
				mchar.add(hID,  ATK, item.atk_points2)>0 ||
				mchar.add(hID,  DEF, item.def_points2)>0 ||
				mchar.add(hID,  AGI, item.agi_points2)>0 ||
				mchar.add(hID,  SPT, item.spi_points2)>0)
					itemUsed = true;
			break;
		}

		case 9:
		{
			break;
		}

		case 10:
		{
			MgrState mstate(pGM);
			mstate.setSw(item.switch_id, true);

			pGM->slm->setSettingScrMap();
			pGM->slm->setScreen(scr_map);

		}
	}
	return itemUsed;
}

int MgrItem::getEffectVal( GlbEquipment eq, CMGR_CMD cmd )
{
	int v = 0;

	v += getEffectVal(eq.weapon_id, cmd);
	v += getEffectVal(eq.shield_id, cmd);
	v += getEffectVal(eq.armor_id, cmd);
	v += getEffectVal(eq.helmet_id, cmd);
	v += getEffectVal(eq.accessory_id, cmd);
	
	return v;
}


int MgrItem::getEffectVal( int itemID, CMGR_CMD cmd )
{
	if (itemID <= 0 || itemID>pGM->ldb.dItem.GetLength())
		return 0;

	LdbItem i = pGM->ldb.dItem[itemID-1];
	switch(cmd) {
		case MAXHP :return i.max_hp_points;
		case MAXMP :return i.max_sp_points;
		case ATK :	return i.atk_points1;
		case DEF :	return i.def_points1;
		case AGI :	return i.agi_points1;
		case SPT :	return i.spi_points1;
		default:return 0;
	}

	return 0;
}

bool MgrItem::isNotSelect(int iID)
{
	if (	iID<=0 || iID>pGM->ldb.dItem.GetLength())
		return false;

	bool itemUsed = false;

	LdbItem &item = pGM->ldb.dItem[iID-1];

	//return item.Type == 9 || item.Type == 10;
	return item.Type == 10;
}

int MgrItem::getEquipedCount( int itemID )
{
	MgrCharacter mchar(pGM);
	ArrayList<short> &party = pGM->lsd.State.party;
	
	int cnt = 0;

	for(int i=0; i<party.GetLength(); i++) {
		int hID = party[i];

		GlbEquipment equip = mchar.getEquip(hID);

		if (equip.weapon_id == itemID)
			cnt++;

		if (equip.shield_id == itemID)
			cnt++;

		if (equip.armor_id == itemID)
			cnt++;

		if (equip.helmet_id == itemID)
			cnt++;

		if (equip.accessory_id == itemID)
			cnt++;
	}

	return cnt;
}

TString MgrItem::getName( int itemID )
{
	TString str;
	if (itemID <= 0 || itemID > pGM->ldb.dItem.GetLength())
		return "";
	else if (MgrSetting::getInstance()->getBool(RST_TRANSLATE_LDB) && 
			pGM->trans.getItemName(itemID, str))
		return str;
	else
		return pGM->ldb.dItem[itemID-1].name;
}

TString MgrItem::getDesc( int itemID )
{
	TString str;
	if (itemID <= 0 || itemID > pGM->ldb.dItem.GetLength())
		return "";
	else if (MgrSetting::getInstance()->getBool(RST_TRANSLATE_LDB) && 
			pGM->trans.getItemDesc(itemID, str))
		return str;
	else
		return pGM->ldb.dItem[itemID-1].desc;
}

