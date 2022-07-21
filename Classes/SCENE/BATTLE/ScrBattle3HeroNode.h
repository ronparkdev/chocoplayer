#ifndef __R2K_SCENE_BATTLE_FOR_2003_HERO_NODE__
#define __R2K_SCENE_BATTLE_FOR_2003_HERO_NODE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "ScrBattleBattlerManager.h"
#include "ScrBattleHueManager.h"
#include "../../GameMain.h"
#include "../../GRAPHIC/OBJECT/GrpHandDown.h"
#include "../../GRAPHIC/GrpAniBat.h"

#define HERO_ANI_FRAME 10
#define WEAPON_SIZE 64
#define HERO_SIZE 48

class ScrBattle3HeroNode: public CCNode
{
	GameMain *m_main;
	ScrBattleBattlerMgr * m_manager;

	int m_hero_id;
	int m_in_battle_id;
	
	int m_total_frame;
	int m_left_frame;

	bool m_custom_ani;
	int m_custom_pose;

	int m_last_pose;
	int m_last_ani_type;
	TString m_last_battler_name;

	bool m_is_right;

	CCSprite *m_sprite_hero;
	CCSprite *m_sprite_weapon;
	GrpAniBat *m_animation_hero;
	GrpHandDown *m_sprite_hand;

	bool m_sprite_hero_inited;
	int m_last_weapon_ani_id;
	ScrBattleHueMgr * m_hue;
public:
	ScrBattle3HeroNode( GameMain *main, ScrBattleBattlerMgr *manager, ScrBattleHueMgr *hue, int in_battle_id, CCTexture2D *pTexSys2);
	~ScrBattle3HeroNode();

	void init(ScrBattleHueMgr *hue);

	void setAnimation(int pose, int type, int run_frame);
	void setDirection(bool is_right);
	bool getDirection();
	void setHand(bool visible);

	void Draw(int framedelta);
private:
};

#endif 
