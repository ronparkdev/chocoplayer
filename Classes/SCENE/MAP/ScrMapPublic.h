#ifndef __R2K_SCENE_MAP_PUBLIC__
#define __R2K_SCENE_MAP_PUBLIC__
#include "../../COMMON/DebugingCode.h"

#include "../../GameMain.h"
#include "../../COMMON/ScreenCommon.h"
#include "../../COMMON/MgrItem.h"
#include "../../COMMON/MgrState.h"
#include "../../DATA/LMU/LmuEvent.h"
#include "../../DATA/LSD/LsdMain.h"
#include "../../EVENT/EvtMain.h"

class GameMain;

class ScrMapPublic{
public:
	static int getEventPage(GameMain *main, LmuEvent &ev);
	static int getChipsetID(int chipID);
	static int getTerrainID(LdbMain *ldb, LsdMain *lsd, LmuMain *lmu, int x, int y);
	static void getEventScreenPos( LsdMain * m_pSave, int eventid, int &posx, int &posy);
	static inline bool chkEventPage(GameMain *main, LmuEventPage &page);
};
#endif