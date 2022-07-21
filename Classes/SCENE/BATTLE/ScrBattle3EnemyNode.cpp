#include "ScrBattle3EnemyNode.h"

#include "../../COMMON/ComMath.h"
#include "../../AppMultiTime.h"


ScrBattle3EnemyNode::ScrBattle3EnemyNode( GameMain *main, ScrBattleBattlerMgr *manager, int in_battle_id, CCTexture2D *pTexSys2)
{
	m_main = main;
	m_manager = manager;

	m_enemy_id = manager->getID(in_battle_id);
	m_in_battle_id = in_battle_id;

	LdbEnemy &enemy = m_main->ldb.dEnemy[m_enemy_id-1];

	m_is_flying = enemy.flying;

	int texID = IBuffer::getInstance()->get(imgMonster, enemy.BattlerName);
	CCTexture2D *tex = IBuffer::getInstance()->get(imgMonster, texID);

	m_sprite_width = tex->getContentSize().width;

	m_sprite_enemy = CCSprite::createWithTexture(tex);
	this->addChild(m_sprite_enemy);

	m_sprite_hand = new GrpHandLeft(pTexSys2);
	m_sprite_hand->setPosition(ccp(m_sprite_width/2, 0));
	m_sprite_hand->setVisible(false);
	this->addChild(m_sprite_hand, 2);

	CCNode::autorelease();

	m_last_is_alive = m_manager->isAlive(in_battle_id);
	m_ani_left_frame = 1;

	m_sprite_opacity = (enemy.transparent)?0.5f:1.0f;

	m_is_showed = (manager->isAlive(in_battle_id) != 0);
}

void ScrBattle3EnemyNode::change(int mID) {
	m_enemy_id = mID;
	
	m_manager->setID(m_in_battle_id, mID);

	LdbEnemy &enemy = m_main->ldb.dEnemy[m_enemy_id-1];

	m_is_flying = enemy.flying;

	int texID = IBuffer::getInstance()->get(imgMonster, enemy.BattlerName);
	CCTexture2D *tex = IBuffer::getInstance()->get(imgMonster, texID);

	m_sprite_width = tex->getContentSize().width;

	m_sprite_enemy->initWithTexture(tex);
}

ScrBattle3EnemyNode::~ScrBattle3EnemyNode()
{
	this->removeChild(m_sprite_enemy);
}

void ScrBattle3EnemyNode::init( ScrBattleHueMgr *hue )
{
	hue->init(m_in_battle_id, m_sprite_enemy);

	m_hue = hue;
}

void ScrBattle3EnemyNode::setDirection( bool is_right )
{
	m_is_right = is_right;

	m_sprite_enemy->setScaleX(is_right?1.0f:-1.0f);
}

bool ScrBattle3EnemyNode::getDirection()
{
	return m_is_right;
}

int ScrBattle3EnemyNode::getWidth() {
	return m_sprite_width;
}

void ScrBattle3EnemyNode::setHand(bool visible) {
	m_sprite_hand->setVisible(visible);
}

void ScrBattle3EnemyNode::Draw( int framedelta )
{
	if (m_is_flying) {
		float pos = (sin((((int)(getTime()*1000+m_in_battle_id*200))%((int)(2000*PI)))/1000.0f) *
						SCR_BATTLE_ENEMY_FLY_MOVE_Y);
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

			if (per > 1.0f)
				per = 1.0f;

			if (!m_last_is_alive)
				m_hue->setAlpha(m_in_battle_id, m_sprite_opacity*per);
			else
				m_hue->setAlpha(m_in_battle_id, m_sprite_opacity*(1.0f-per));
		}
	}
}

bool ScrBattle3EnemyNode::isShowed()
{
	return m_is_showed;
}
