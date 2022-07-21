#include "GrpBatDam.h"

#include "../../COMMON/ComMath.h"

GrpBatDam::GrpBatDam( GameMain *m_main )
{
	m_pFont = new GrpFontL(m_main);
	this->addChild(m_pFont);

	m_is_set = false;

	CCNode::autorelease();
}

GrpBatDam::~GrpBatDam()
{
	this->removeChild(m_pFont);
}

void GrpBatDam::init( int num )
{
	bool isMinus = false;
	int cnt = 0;

	if (num < 0) {
		isMinus = true;
		num *= -1;
	}

	if (!isMinus)
		m_pFont->setText(_ST("") + num);
	else
		m_pFont->setText(_ST("\\C[4]") + num);
}

void GrpBatDam::reloadShaders()
{
	m_pFont->reloadShaders();
}

void GrpBatDam::set( float posX, float posY, int num )
{
	m_is_set = true;
	m_def_pos = ccp(posX, posY);
	m_cur_frame = 0;
	m_move_x = RANDF() * 120.0f - 60.0f;

	init(num);
	this->setPosition(posX, posY);
	this->setVisible(true);
}

void GrpBatDam::Draw( int framedelta )
{
	if (!m_is_set)
		return;

	m_cur_frame += framedelta;

	float total_per = (float)m_cur_frame / GRP_BAT_NUM_MAX_FRAME;

	float hide_per = (float)(m_cur_frame - GRP_BAT_NUM_HIDE_FRAME) / (GRP_BAT_NUM_MAX_FRAME - GRP_BAT_NUM_HIDE_FRAME);

	if (hide_per >= 0.0f)
		m_pFont->setOpacity(255.0f * (1.0f - hide_per));

	if (total_per <= 1.0f) {
		float x = total_per * m_move_x;
		float y = getCalPosY(x);

		this->setPosition(x + m_def_pos.x, y + m_def_pos.y);
	} else {
		m_is_set = false;
		this->setVisible(false);
	}
}

bool GrpBatDam::isSet()
{
	return m_is_set;
}

float GrpBatDam::getCalPosY(float x) {
	float k, l;

	l = 30;
	k = m_move_x/2;

	return -4 * l / (k*k) * x * (x - k);
}
