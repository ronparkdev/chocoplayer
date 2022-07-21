#ifndef __R2K_COMMON_TELEPORT_MANAGER__
#define __R2K_COMMON_TELEPORT_MANAGER__
#include "DebugingCode.h"

#include "../GameMain.h"

class MgrTeleport{
	GameMain *pGM;

public:
	MgrTeleport(GameMain *main);
	
	void setEscape(int mapid, int posx, int posy, int swid = 0);
	void moveEscape();
	bool isEscapeable();

	void addTeleport(int mapid, int posx, int posy, int swid = 0);
	void removeTeleport(int mapid);
	void getTeleportList(ArrayList<TString> &list);
	void moveTeleport(int id);
	bool isTeleportable();
};
#endif




