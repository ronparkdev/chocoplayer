#ifndef __R2K_SCENE_BATTLE_FOR_2003_WEAPON_NODE__
#define __R2K_SCENE_BATTLE_FOR_2003_WEAPON_NODE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "ScrBattleHueManager.h"
#include "../../GameMain.h"

#define WEAPON_ANI_FRAME 10
#define WEAPON_SIZE 64

class ScrBattle3WeaponNode: public CCNode
{
	GameMain *m_main;

	CCSprite *m_sprite_weapon;
	
	int m_sprite_id;
	int m_ani_frame_left;
	int m_ani_cur;
	
	TString m_last_sprite_name;
public:
	ScrBattle3WeaponNode( GameMain *main );
	~ScrBattle3WeaponNode();

	void init(ScrBattleHueMgr *hue);

	void set(TString sprite_name, int sprite_id);

	void Draw(int framedelta);
private:
};

#endif 
