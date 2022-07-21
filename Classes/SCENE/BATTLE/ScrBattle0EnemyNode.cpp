#include "ScrBattle0EnemyNode.h"

#include "../../COMMON/ComMath.h"
#include "../../AppMultiTime.h"

ScrBattle0EnemyNode::ScrBattle0EnemyNode( GameMain *main, ScrBattleBattlerMgr *manager, int in_battle_id )
{
	m_main = main;
	m_manager = manager;

	m_enemy_id = manager->getID(in_battle_id);
	m_in_battle_id = in_battle_id;

	LdbEnemy &enemy = m_main->ldb.dEnemy[m_enemy_id-1];

	m_is_flying = enemy.flying;

	int texID = IBuffer::getInstance()->get(imgMonster, enemy.BattlerName);
	CCTexture2D *tex = IBuffer::getInstance()->get(imgMonster, texID);

	m_sprite_enemy = CCSprite::createWithTexture(tex);

	m_sprite_height = m_sprite_enemy->getTexture()->getContentSize().height;

	m_last_is_alive = m_manager->isAlive(in_battle_id);
	m_ani_left_frame = 1;

	m_sprite_opacity = (enemy.transparent)?0.5f:1.0f;

	m_is_showed = (manager->isAlive(in_battle_id) != 0);

	this->addChild(m_sprite_enemy);

	CCNode::autorelease();

}

ScrBattle0EnemyNode::~ScrBattle0EnemyNode()
{
	this->removeChild(m_sprite_enemy);
}

void ScrBattle0EnemyNode::init( ScrBattleHueMgr *hue )
{
	hue->init(m_in_battle_id, m_sprite_enemy);

	m_hue = hue;
}

void ScrBattle0EnemyNode::change(int mID) {
	m_enemy_id = mID;

	m_manager->setID(m_in_battle_id, mID);

	LdbEnemy &enemy = m_main->ldb.dEnemy[m_enemy_id-1];

	m_is_flying = enemy.flying;

	int texID = IBuffer::getInstance()->get(imgMonster, enemy.BattlerName);
	CCTexture2D *tex = IBuffer::getInstance()->get(imgMonster, texID);

	m_sprite_enemy->initWithTexture(tex);
}

void ScrBattle0EnemyNode::Draw( int framedelta )
{
	if (m_is_flying) {
		float pos = (sin(getTime()+m_in_battle_id*0.2f) * SCR_BATTLE_ENEMY_FLY_MOVE_Y);
		m_sprite_enemy->setPositionY(pos);
	}
	bool is_alive = m_manager->isAlive(m_in_battle_id);

	if (m_last_is_alive != is_alive) {
		m_ani_left_frame = SCR_BATTLE_ENEMY_HIDE_FRAME;
		m_last_is_alive = is_alive;

		if (m_last_is_alive)
			m_is_showed = true;
	}

	if (m_ani_left_frame > 0) {
		m_ani_left_frame -= framedelta;


		if (m_ani_left_frame <= 0) {
			m_ani_left_frame = 0;

			if (!m_last_is_alive)
				m_hue->setAlpha(m_in_battle_id, 0.0f);
			else
				m_hue->setAlpha(m_in_battle_id, m_sprite_opacity);
		} else {
			float per = (float)m_ani_left_frame / SCR_BATTLE_ENEMY_HIDE_FRAME;

			if (!m_last_is_alive)
				m_hue->setAlpha(m_in_battle_id, m_sprite_opacity*per);
			else
				m_hue->setAlpha(m_in_battle_id, m_sprite_opacity*(1.0f-per));
		}
	}
}

bool ScrBattle0EnemyNode::isShowed()
{
	return m_is_showed;
}

int ScrBattle0EnemyNode::getHeight()
{
	return m_sprite_height;
}

void ScrBattle0EnemyNode::setAlpha( float per )
{
	m_hue->setAlpha(m_in_battle_id, per);
}
