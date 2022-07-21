#ifndef __R2K_COMMON_ITEM_MANAGER__
#define __R2K_COMMON_ITEM_MANAGER__
#include "DebugingCode.h"

#include "ComMgrCMD.h"
#include "../GameMain.h"

enum IMGR_CMD{
	NORMAL	= 0,
	WEAPON	= 1,
	SHIELD	= 2,
	ARMOR	= 3,
	HELMET	= 4,
	ACCESSOR= 5,
	MEDICINE= 6,
	BOOK	= 7,
	SEED	= 8,
	SPECIAL	= 9,
	SWITCH	= 10
};

class MgrItem{
	GameMain *pGM;

public:
	MgrItem(GameMain *main);

	bool isHeroUseable(int hID, int iID);
	bool isUseable	(int iID, bool isBattle);
	bool isEquipable(int hID, int iID);
	bool isNotSelect(int iID);

	bool isAllEffect(int iID);
	
	int	 getItemCount( int itemID );
	int  getEquipedCount( int itemID );
	void addItemCount	(int iID, int val);

	bool useItem	(int hID, int iID, bool mustuse = false);

	int getEffectVal( GlbEquipment eq, CMGR_CMD cmd );

	void useItem	(int iID);//For ScrBattle3Do::doItem

	TString getName( int itemID );
	TString getDesc( int itemID );
private:
	bool useItemOne	(int hID, int iID);
	int getEffectVal( int itemID, CMGR_CMD cmd );
};
#endif




