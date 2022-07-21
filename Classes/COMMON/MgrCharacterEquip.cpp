#include "MgrCharacter.h"

GlbEquipment MgrCharacter::getEquip( int id )
{
	if (id <= 0 || id > m_main->ldb.dChar.GetLength())
		return GlbEquipment();

	return m_main->lsd.Character[id-1].cequip;
}

void MgrCharacter::setEquip( int id, GlbEquipment &equip)
{
	if (id <= 0 || id > m_main->ldb.dChar.GetLength())
		return;

	m_main->lsd.Character[id-1].cequip = equip;
}

void MgrCharacter::setEquip( int hID, int itemID )
{
	LdbItem &item = m_main->ldb.dItem[itemID-1];

	GlbEquipment equip = getEquip(hID);
	switch(item.Type-1) {
		case 0:equip.weapon_id	= itemID;break;
		case 1:equip.shield_id	= itemID;break;
		case 2:equip.armor_id	= itemID;break;
		case 3:equip.helmet_id	= itemID;break;
		case 4:equip.accessory_id=itemID;break;
	}
	setEquip(hID, equip);
}

void MgrCharacter::unsetEquip( int hID, int itemID )
{
	GlbEquipment equip = getEquip(hID);
	switch(itemID) {
		case 0:equip.weapon_id	= 0;break;
		case 1:equip.shield_id	= 0;break;
		case 2:equip.armor_id	= 0;break;
		case 3:equip.helmet_id	= 0;break;
		case 4:equip.accessory_id=0;break;
	}
	setEquip(hID, equip);
}

bool MgrCharacter::isEquiped( int hID, int inVal )
{
	GlbEquipment equip = getEquip(hID);
	if (equip.weapon_id == inVal)
		return true;

	if (equip.shield_id == inVal)
		return true;

	if (equip.helmet_id == inVal)
		return true;

	if (equip.armor_id == inVal)
		return true;

	if (equip.accessory_id == inVal)
		return true;

	return false;
}
