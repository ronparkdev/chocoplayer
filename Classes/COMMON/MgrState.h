#ifndef __R2K_COMMON_STATE_MANAGER__
#define __R2K_COMMON_STATE_MANAGER__
#include "DebugingCode.h"

#include "../GameMain.h"

class MgrState{
	GameMain *pGM;

public:
	MgrState(GameMain *main);

	int  getVar(int index);
	void setVar(int index, int val);
	bool getSw(int index);
	void setSw(int index, bool val);

	bool isPartyIn(int charID );
	void setPartyIn(int charID, bool val );
	int getPartyLen();

	bool getAbleMenu();
	void setAbleMenu(bool val);

	bool getAbleSave();
	void setAbleSave(bool val);

	bool getAbleTeleport();
	void setAbleTeleport(bool val);

	bool getAbleEscape();
	void setAbleEscape(bool val);

	int getMoney();
	void addMoney(int val);
};
#endif




