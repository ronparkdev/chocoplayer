#include "MgrEquip.h"

#include "MgrCharacter.h"
#include "MgrItem.h"

MgrEquip::MgrEquip( GameMain *main )
{
	pGM = main;
}

bool MgrEquip::isEquipable( int hID, int iID )
{
	if (	iID<=0 || iID>pGM->ldb.dItem.GetLength() || 
		hID<=0 || hID>pGM->ldb.dChar.GetLength())
		return false;

	LdbItem &item = pGM->ldb.dItem[iID-1];
	LsdCharacter &lsdCh = pGM->lsd.Character[hID-1];

	switch(item.Type) {
		case 1:if (item.two_handed)
				   return lsdCh.is_two_hand_sword;
		case 2:
		case 3:
		case 4:
		case 5:
			return true;

		default:
			return false;
	}
}

bool MgrEquip::isEquiped( int hID, int iID )
{
	return (getEquipedCnt(hID, iID) > 0);
}

int MgrEquip::getEquipedCnt( int hID, int iID )
{
	if (	iID<=0 || iID>pGM->ldb.dItem.GetLength() || 
		hID<=0 || hID>pGM->ldb.dChar.GetLength())
		return 0;

	MgrCharacter mchar(pGM);
	GlbEquipment equip = mchar.getEquip(hID);

	int c = 0;
	if (equip.weapon_id		== iID)c++;
	if (equip.shield_id		== iID)c++;
	if (equip.armor_id		== iID)c++;
	if (equip.helmet_id		== iID)c++;
	if (equip.accessory_id	== iID)c++;

	return c;
}

bool MgrEquip::equip( int hID, int item_id, int type)
{
	if (item_id == 0)
		return unequip(hID, type);

	if (	item_id<=0 || item_id>pGM->ldb.dItem.GetLength())
		return false;

	MgrItem mitem(pGM);
	MgrCharacter mchar(pGM);
	LdbItem &item = pGM->ldb.dItem[item_id-1];

	unequip(hID, type);

	GlbEquipment equip = mchar.getEquip(hID);

	if (item_id != 0) {
		if (type == -1)
			type = item.Type;

		switch(type) {
			case 1:
			{					
				equip.weapon_id = item_id;
				if (item.two_handed && equip.shield_id != 0) {
					mitem.addItemCount(equip.shield_id, 1);
					equip.shield_id = 0;
				}
				break;
			}
			case 2:equip.shield_id		= item_id;break;
			case 3:equip.armor_id		= item_id;break;
			case 4:equip.helmet_id		= item_id;break;
			case 5:equip.accessory_id	= item_id;break;	
			default:return false;
		}
		
		mitem.addItemCount(item_id, -1);
	}

	mchar.setEquip(hID, equip);

	return true;
}

bool MgrEquip::unequip( int hID, int type )
{
	MgrItem mitem(pGM);
	MgrCharacter mchar(pGM);

	GlbEquipment equip = mchar.getEquip(hID);

	int last_item_id = 0;
	switch(type) {
	case 1:
		last_item_id = equip.weapon_id;
		equip.weapon_id = 0;
		break;
	case 2:
		last_item_id = equip.shield_id;
		equip.shield_id = 0;
		break;
	case 3:
		last_item_id = equip.armor_id;
		equip.armor_id = 0;
		break;
	case 4:
		last_item_id = equip.helmet_id;
		equip.helmet_id = 0;
		break;
	case 5:
		last_item_id = equip.accessory_id;
		equip.accessory_id = 0;
		break;	
	}

	if (last_item_id != 0)
		mitem.addItemCount(last_item_id, 1);

	mchar.setEquip(hID, equip);

	return (last_item_id > 0);
}

bool MgrEquip::set( int hID, int iID )
{
	if (	iID<=0 || iID>pGM->ldb.dItem.GetLength())
		return false;

	MgrCharacter mchar(pGM);
	GlbEquipment equip = mchar.getEquip(hID);
	LdbItem &item = pGM->ldb.dItem[iID-1];

	switch(item.Type) {
		case 1:{
			equip.weapon_id = iID;
			if (item.two_handed)
				equip.shield_id = 0;
			break;
		}
		case 2:equip.shield_id	= iID;break;
		case 3:equip.armor_id	= iID;break;
		case 4:equip.helmet_id	= iID;break;
		case 5:equip.accessory_id	= iID;break;	
		default:return false;
	}

	mchar.setEquip(hID, equip);

	return true;
}

void MgrEquip::remove( int hID, int type )
{
	MgrCharacter mchar(pGM);
	GlbEquipment equip = mchar.getEquip(hID);

	switch(type) {
		case 0:equip.weapon_id	= 0;break;
		case 1:equip.shield_id	= 0;break;
		case 2:equip.armor_id	= 0;break;
		case 3:equip.helmet_id	= 0;break;
		case 4:equip.accessory_id = 0;break;
		case 5:
		{
			equip.weapon_id		= 0;
			equip.shield_id		= 0;
			equip.armor_id		= 0;
			equip.helmet_id		= 0;
			equip.accessory_id	= 0;
			break;
		}
	}

	mchar.setEquip(hID, equip);
}

TString MgrEquip::getName( int hID, int type )
{
	if (	hID<=0 || hID>pGM->ldb.dChar.GetLength())
		return false;

	MgrCharacter mchar(pGM);
	GlbEquipment equip = mchar.getEquip(hID);

	int id;

	switch(type) {
		case 1:id = equip.weapon_id;	break;
		case 2:id = equip.shield_id;	break;
		case 3:id = equip.armor_id;		break;
		case 4:id = equip.helmet_id;	break;
		case 5:id = equip.accessory_id;	break;
		default:id = 0;break;
	}
	
	TString str;
	if (id <= 0 || id > pGM->ldb.dItem.GetLength())
		return "";
	else if (pGM->trans.getItemDesc(id, str))
		return str;
	else
		return pGM->ldb.dItem[id-1].name;
}
