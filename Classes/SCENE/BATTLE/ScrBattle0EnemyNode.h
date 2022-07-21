#ifndef __R2K_SCENE_BATTLE_FOR_2000_ENEMY_NODE__
#define __R2K_SCENE_BATTLE_FOR_2000_ENEMY_NODE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "ScrBattleBattlerManager.h"
#include "ScrBattleHueManager.h"
#include "../../GameMain.h"

#define SCR_BATTLE_ENEMY_FLY_MOVE_Y 10

#define SCR_BATTLE_ENEMY_HIDE_FRAME 60

class ScrBattle0EnemyNode: public CCNode
{
	GameMain *m_main;
	ScrBattleBattlerMgr * m_manager;
	ScrBattleHueMgr * m_hue;

	int m_enemy_id;
	int m_in_battle_id;

	bool m_is_flying;

	CCSprite *m_sprite_enemy;
	float m_sprite_opacity;

	bool m_last_is_alive;
	int m_ani_left_frame;

	int m_sprite_height;

	bool m_is_showed;

public:
	ScrBattle0EnemyNode( GameMain *main, ScrBattleBattlerMgr *manager, int in_battle_id);
	~ScrBattle0EnemyNode();

	void init(ScrBattleHueMgr *hue);
	void change(int mID);

	bool isShowed();

	void setAlpha(float per);

	void Draw(int framedelta);
	int getHeight();
private:
};

#endif 
