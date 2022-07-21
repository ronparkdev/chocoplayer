#ifndef __R2K_COMMON_EQUIPMENT_MANAGER__
#define __R2K_COMMON_EQUIPMENT_MANAGER__
#include "DebugingCode.h"

#include "../GameMain.h"

class MgrEquip{
	GameMain *pGM;
public:
	MgrEquip(GameMain *main);

	bool isEquipable(int hID, int iID);
	bool isEquiped(int hID, int iID);
	int  getEquipedCnt(int hID, int iID);

	bool equip(int hID, int iID, int type);
	bool unequip( int hID, int type );

	bool set(int hID, int iID);
	void remove(int hID, int type);

	TString getName( int hID, int type );
};
#endif




