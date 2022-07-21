#ifndef __R2K_COMMON_SKILL_MANAGER__
#define __R2K_COMMON_SKILL_MANAGER__
#include "DebugingCode.h"

#include "../GameMain.h"

class MgrSkill{
	GameMain *pGM;

public:
	MgrSkill(GameMain *main);

	bool isUseable(int hID, int sID, bool isBattle = false);

	bool isAllEffect(int sID);
	bool isEffMyTeam(int sID);
	bool isNotSelect(int sID);

	int getEffVal(int hID, int sID);
	int getSpendMP(int hID, int sID);

	bool useSkill(int userID, int targetID, int skillID, bool useMP = true);

	TString getName( int skillID );
	TString getDesc( int skillID );
private:
	bool useSkillOne(int userID, int targetID, int skillID);
};
#endif




