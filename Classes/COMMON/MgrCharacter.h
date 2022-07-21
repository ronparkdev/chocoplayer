#ifndef __R2K_COMMON_CHARACTER_MANAGER__
#define __R2K_COMMON_CHARACTER_MANAGER__
#include "DebugingCode.h"

#include "ComMgrCMD.h"
#include "../GameMain.h"
#include "../DATA/GLB/GlbEqipment.h"

class MgrCharacter{
	GameMain *m_main;

	int lastLevel;
	int lastAddedId;
	ArrayList<byte> lastAddedSkill;
public:
	MgrCharacter(GameMain *main);
	
	int  get(int id, CMGR_CMD cmd);
	TString getS(int id, CMGR_CMD cmd);
	int  add(int id, CMGR_CMD cmd, int val);
	

	GlbEquipment getEquip(int id);
	void setEquip( int id, GlbEquipment &equip);

	bool getState( int hID, int stateID );
	int getTopState(int hID);
	TString getStateStr(int hID);
	void setState( int hID, int stateID, bool val );
	void clearState(int hID);

	bool getSkill( int hID, int skillID );
	void setSkill( int hID, int skillID, bool val );

	void setEquip( int hID, int itemID );
	void unsetEquip( int hID, int itemID );
	bool isEquiped( int hID, int inVal );

	void getLastLevelUpMsg(ArrayList<TString>& msg);
private:

	void set(int id, CMGR_CMD cmd, int val);
	int chk(int id, CMGR_CMD cmd, int val);

	int getLevel(int hid, int exp);
};
#endif




