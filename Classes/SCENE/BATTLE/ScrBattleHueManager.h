#ifndef __R2K_SCENE_BATTLE_HUE_MANAGER__
#define __R2K_SCENE_BATTLE_HUE_MANAGER__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "ScrBattleBattlerManager.h"
#include "../../GameMain.h"
#include "../../GRAPHIC/GrpShaderColor.h"

USING_NS_CC;

#define SCR_BATTLE_MAX_HUE (SCR_BATTLE_MAX_ENEMIES+SCR_BATTLE_MAX_HEROS+1)

class ScrBattleHueMgr
{
	GameMain *m_main;

	GrpShaderColor m_shaders[SCR_BATTLE_MAX_HUE];

	float scr_cur_r, scr_cur_g, scr_cur_b, scr_cur_gr;
	int scr_fin_r, scr_fin_g, scr_fin_b, scr_fin_gr;
	int scr_left_frame, scr_total_frame;
	
	float fl_continuous[SCR_BATTLE_MAX_HUE];
	float fl_cur_lev[SCR_BATTLE_MAX_HUE];
	int fl_fin_r[SCR_BATTLE_MAX_HUE];
	int fl_fin_g[SCR_BATTLE_MAX_HUE];
	int fl_fin_b[SCR_BATTLE_MAX_HUE];
	int fl_left_frame[SCR_BATTLE_MAX_HUE];
	int fl_total_frame[SCR_BATTLE_MAX_HUE];
	
public:
	ScrBattleHueMgr(GameMain *main);
	~ScrBattleHueMgr();

	void init( int id, CCSprite *node );
	void init( int id, CCSpriteBatchNode *node );
	void setColor(int r, int g, int b, int gr, int frame);
	void setFlash( int id, int type, int r, int g, int b, int lev, int frame );
	void setAlpha( int id, float val );

	void Draw(int framedelta);
	
	void reloadShaders();

	void doSaveToLSD();
private:
};

#endif 
