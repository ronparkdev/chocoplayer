#include "ScrBattle3HeroNode.h"

ScrBattle3HeroNode::ScrBattle3HeroNode( GameMain *main, ScrBattleBattlerMgr *manager, ScrBattleHueMgr *hue, int in_battle_id, CCTexture2D *pTexSys2)
{
	m_main = main;
	m_manager = manager;

	m_hero_id = manager->getID(in_battle_id);
	m_in_battle_id = in_battle_id;

	m_sprite_hero = new CCSprite();
	m_sprite_hero->setVisible(true);
	this->addChild(m_sprite_hero, 1);

	m_sprite_weapon = new CCSprite();
	m_sprite_weapon->setVisible(false);
	this->addChild(m_sprite_weapon, 0);
	
	m_animation_hero = new GrpAniBat(main, hue, in_battle_id);
	this->addChild(m_animation_hero, 1);

	m_sprite_hand = new GrpHandDown(pTexSys2);
	m_sprite_hand->setPosition(ccp(0, 30));
	m_sprite_hand->setVisible(false);
	this->addChild(m_sprite_hand, 2);

	m_total_frame = m_left_frame = 0;
	m_last_pose = 0;
	m_last_ani_type = 0;
	m_last_weapon_ani_id = 0;
	m_is_right = false;
	m_custom_ani = false;

	m_sprite_hero_inited = false;

	CCNode::autorelease();
}

ScrBattle3HeroNode::~ScrBattle3HeroNode()
{

}

void ScrBattle3HeroNode::init( ScrBattleHueMgr *hue )
{
	m_hue = hue;
	m_hue->init(m_in_battle_id, m_sprite_hero);
	m_hue->init(m_in_battle_id, m_sprite_weapon);
}

void ScrBattle3HeroNode::Draw(int framedelta)
{
	int cur_pose = 0, cur_ani = 0;

	if (m_custom_ani && m_left_frame > 0) {
		m_left_frame -= framedelta;

		if (m_left_frame <= 0)
			m_custom_ani = false;
	}

	if (!m_custom_ani) {
		m_left_frame += framedelta;
		
		//None Animation

		int state = m_manager->getTopState(m_in_battle_id);

		if (state == 0) {
			//Stand Animation
			if (m_manager->get(m_in_battle_id, BAT_DEFENCE_TYPE) == 0)
				cur_pose = 0;
			else
				cur_pose = 7;
		}else
		if (state == 1) {
			//Dead
			cur_pose = m_main->ldb.dCond[state-1].battler_animation_id;
			m_sprite_hero->setPositionY(-HERO_SIZE/4);
		} else {
			//State Oriented Animation
			cur_pose = m_main->ldb.dCond[state-1].battler_animation_id;
		}

		cur_ani = m_left_frame/HERO_ANI_FRAME % 4;
		if (cur_ani == 3)cur_ani = 1;

	} else {
		//Called Animation

		cur_pose = m_custom_pose;

		cur_ani = (m_total_frame - m_left_frame)/HERO_ANI_FRAME;

		switch(m_last_ani_type) {
		case 0://Once Animation (Stop At Last Animation)
			{
				if (cur_ani > 2)cur_ani = 2;
				break;
			}
		case 1://Infinity Animation
			{
				cur_ani %= 3;
				break;
			}
		case 2://Walking Animation
			{
				cur_ani %= 4;
				if (cur_ani == 3)cur_ani = 1;
				break;
			}
		}
	}

	//If Direction is right, flip animation order
	if (m_is_right)
		cur_ani = 2 - cur_ani;

	int bat_ani_id = m_main->ldb.dChar[m_hero_id-1].battler_ani;
	LdbBattlerAni &bat_ani = m_main->ldb.dBatrAnim[bat_ani_id-1];

	LdbBattlerAniExt &ext = bat_ani.base_data[cur_pose];

	//Display Hero
	if (m_last_pose != cur_pose || !m_sprite_hero_inited) {

		if (ext.animation_type == 0) {
			if (m_last_battler_name != ext.battler_name) {
				int texID = IBuffer::getInstance()->get(imgBattleCharSet, ext.battler_name);
				CCTexture2D *tex = IBuffer::getInstance()->get(imgBattleCharSet, texID);

				m_sprite_hero->initWithTexture(tex);
				m_hue->init(m_in_battle_id, m_sprite_hero);

				m_last_battler_name = ext.battler_name;
			}
			m_sprite_hero->setVisible(true);
			m_animation_hero->setVisible(false);
		} else {
			m_animation_hero->set(ext.animation_id, 0, 0, 0, true);

			m_sprite_hero->setVisible(false);
			m_animation_hero->setVisible(true);
		}

		m_last_pose = cur_pose;
		m_sprite_hero_inited = true;
	}

	if (ext.animation_type == 0) {
		m_sprite_hero->setTextureRect(CCRect(
			cur_ani*HERO_SIZE, 
			ext.battler_index*HERO_SIZE,
			HERO_SIZE, HERO_SIZE));
	} else {
		m_animation_hero->Draw(framedelta);
	}

	//Display Weapon
	bool is_use_weapon_ani = false;

	if (cur_pose == 1 || cur_pose == 2) {
		int weapon_id;
		GlbEquipment equip = m_manager->getEquip(m_in_battle_id);
		if (cur_pose == 1)
			weapon_id = equip.weapon_id;
		else
			weapon_id = equip.shield_id;

		if (weapon_id > 0) {
			int weapon_ani_id = 1;
			LdbItem item = m_main->ldb.dItem[weapon_ani_id-1];
			if (m_hero_id-1 < item.animation_data.GetLength())
				weapon_ani_id = item.animation_data[m_hero_id-1].weapon_anim;

			int weapon_num = bat_ani.weapon_data[weapon_ani_id-1].battler_index;
			//TODO Check

			if (m_last_weapon_ani_id != weapon_ani_id) {
				int tex_id = IBuffer::getInstance()->get(imgBattleWeapon, bat_ani.weapon_data[weapon_ani_id-1].battler_name);
				CCTexture2D *tex = IBuffer::getInstance()->get(imgBattleWeapon, tex_id);
				m_sprite_weapon->initWithTexture(tex);
				m_hue->init(m_in_battle_id, m_sprite_weapon);

				m_last_weapon_ani_id = weapon_ani_id;
			}

			m_sprite_weapon->setTextureRect(CCRect(
				cur_ani*WEAPON_SIZE,
				weapon_num*WEAPON_SIZE,
				WEAPON_SIZE, WEAPON_SIZE));

			is_use_weapon_ani = true;
		}
	}

	m_sprite_weapon->setVisible(is_use_weapon_ani);
}

void ScrBattle3HeroNode::setAnimation( int pose, int type, int run_frame )
{
	m_custom_pose = pose;
	m_last_ani_type = type;
	m_total_frame = m_left_frame = run_frame;
	m_custom_ani = true;
}

void ScrBattle3HeroNode::setDirection( bool is_right )
{
	m_is_right = is_right;

	m_sprite_hero->setScaleX(is_right?-1.0f:1.0f);
	m_sprite_weapon->setScaleX(is_right?-1.0f:1.0f);
}

bool ScrBattle3HeroNode::getDirection()
{
	return m_is_right;
}

void ScrBattle3HeroNode::setHand(bool visible) {
	m_sprite_hand->setVisible(visible);
}