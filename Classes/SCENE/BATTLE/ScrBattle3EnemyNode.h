#ifndef __R2K_SCENE_BATTLE_FOR_2003_ENEMY_NODE__
#define __R2K_SCENE_BATTLE_FOR_2003_ENEMY_NODE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "ScrBattleBattlerManager.h"
#include "ScrBattleHueManager.h"
#include "../../GameMain.h"
#include "../../GRAPHIC/OBJECT/GrpHandLeft.h"

USING_NS_CC;

#define SCR_BATTLE_ENEMY_FLY_MOVE_Y 10

#define SCR_BATTLE_ENEMY_HIDE_FRAME 60

class ScrBattle3EnemyNode: public CCNode
{
	GameMain *m_main;
	ScrBattleBattlerMgr * m_manager;
	ScrBattleHueMgr * m_hue;

	int m_enemy_id;
	int m_in_battle_id;

	bool m_is_flying;
	bool m_is_right;
	
	CCSprite *m_sprite_enemy;
	GrpHandLeft *m_sprite_hand;
	float m_sprite_opacity;

	int m_sprite_width;

	CCPoint default_pos;
	bool m_last_is_alive;

	int m_ani_left_frame;

	bool m_is_showed;

public:
	ScrBattle3EnemyNode( GameMain *main, ScrBattleBattlerMgr *manager, int in_battle_id, CCTexture2D *pTexSys2);
	~ScrBattle3EnemyNode();

	void init(ScrBattleHueMgr *hue);
	void change(int mID);

	void setDirection(bool is_right);
	bool getDirection();

	int getWidth();
	void setHand(bool visible);
	bool isShowed();

	void Draw(int framedelta);
private:
};

#endif 
