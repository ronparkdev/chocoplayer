#include "../../COMMON/DebugingCode.h"

#include "GrpBatNum.h"
#include "../../COMMON/ComMath.h"

GrpBatNum::GrpBatNum( CCTexture2D *pTexSystem2 )
{
	m_pSpriteBatch =CCSpriteBatchNode::createWithTexture(pTexSystem2);
	m_pSpriteBatch->retain();

	CCTexture2D *pTex = m_pSpriteBatch->getTexture();

	for(int i=0; i<GRP_BAT_NUM_MAX; i++) {
		m_pSptite[i] = CCSprite::createWithTexture(pTex);
		this->addChild(m_pSptite[i]);
	}

	CCNode::autorelease();
}

GrpBatNum::~GrpBatNum()
{
	for(int i=0; i<GRP_BAT_NUM_MAX; i++)
		this->removeChild(m_pSptite[i]);
	
	m_pSpriteBatch->release();
}

void GrpBatNum::set( int num )
{
	int i, cnt = 0;

	while(num > 0) {
		init(cnt, num % 10);

		cnt++;
		num /= 10;
	}

	m_using_sprite_cnt = cnt;

	for(i=0; i<cnt; i++) {
		m_pSptite[i]->setVisible(true);
		m_pSptite[i]->setPositionX(-4-8*(i));
	}

	for(;i<GRP_BAT_NUM_MAX; i++)
		m_pSptite[i]->setVisible(false);
}

void GrpBatNum::init( int id, int num )
{
	int sx = 8 * num;
	int sy = 80;

	m_pSptite[id]->setTextureRect(CCRect(sx, sy, 8, 16));
}