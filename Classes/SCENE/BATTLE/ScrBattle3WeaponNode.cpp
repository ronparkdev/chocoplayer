#include "ScrBattle3WeaponNode.h"

ScrBattle3WeaponNode::ScrBattle3WeaponNode( GameMain *main )
{
	m_main = main;

	m_sprite_weapon = new CCSprite();
	this->addChild(m_sprite_weapon);

	CCNode::autorelease();
}

ScrBattle3WeaponNode::~ScrBattle3WeaponNode()
{

}

void ScrBattle3WeaponNode::init( ScrBattleHueMgr *hue )
{
	hue->init(SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES, m_sprite_weapon);
}

void ScrBattle3WeaponNode::set( TString sprite_name, int sprite_id )
{
	m_sprite_id = sprite_id;
	
	if (m_last_sprite_name != sprite_name) {
		int tex_id = IBuffer::getInstance()->get(imgBattleWeapon, sprite_name);
		CCTexture2D *tex = IBuffer::getInstance()->get(imgBattleWeapon, tex_id);

		m_sprite_weapon->initWithTexture(tex);
		m_last_sprite_name = sprite_name;
	}
	m_sprite_weapon->setTextureRect(CCRect(
		0,
		m_sprite_id*WEAPON_SIZE,
		WEAPON_SIZE, WEAPON_SIZE));

	m_ani_cur = 0;
	m_ani_frame_left = WEAPON_ANI_FRAME;
}

void ScrBattle3WeaponNode::Draw(int framedelta) {

	if (m_last_sprite_name.isEmpty())
		return;

	m_ani_frame_left -= framedelta;
	if (m_ani_frame_left <= 0) {
		m_ani_cur = (m_ani_cur + 1) % 3;
		m_ani_frame_left = WEAPON_ANI_FRAME;

		m_sprite_weapon->setTextureRect(CCRect(
			m_ani_cur*WEAPON_SIZE,
			m_sprite_id*WEAPON_SIZE,
			WEAPON_SIZE, WEAPON_SIZE));
	}
}