#include "GrpGaze.h"

GrpGaze::GrpGaze( CCTexture2D *pTexSystem2, int type )
	:m_SpriteLen(0)
{
	m_pSpriteBatch = CCSpriteBatchNode::createWithTexture(pTexSystem2);
	m_pSpriteBatch->retain();

	m_type = type;

	int sy = 32 + 16*m_type;

	addSprite(0, sy, 16, 16, -29, -8);
	addSprite(16, sy, 16, 16, -13, -8, 25, 16);
	addSprite(32, sy, 16, 16, 12, -8);

	for(int i=0; i<2; i++)
		addSprite(48 + i*16,	sy,16,16,-8,-8);

	refresh(1.0f);

	CCNode::autorelease();
}

GrpGaze::~GrpGaze()
{
	m_pSpriteBatch->release();
}

void GrpGaze::refresh( float per )
{
	double scale = 25.0f / 16.0f * per;
	double width = 25.0f * per;
	if (per < 1.0f) {
		m_pSprite[3]->setPositionX(-13+width/2);
		m_pSprite[3]->setScaleX(scale);
		m_pSprite[3]->setVisible(true);
		m_pSprite[4]->setVisible(false);
	} else {
		m_pSprite[4]->setPositionX(-13+width/2	);
		m_pSprite[4]->setScaleX(scale);
		m_pSprite[3]->setVisible(false);
		m_pSprite[4]->setVisible(true);
	}
}

void GrpGaze::addSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty) {
	addSprite(srcx, srcy, srcw, srch, dstx, dsty, srcw, srch);
}

void GrpGaze::addSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth) {
	CCTexture2D *pTexture = m_pSpriteBatch->getTexture();
	CCSprite *pSprite;
	pSprite = CCSprite::createWithTexture(pTexture);
	pSprite->setTextureRect(CCRect(srcx, srcy, srcw, srch));
	pSprite->setPosition(ccp(dstx + dstw/2, -(dsty + dsth/2) ));
	pSprite->setScaleX((float)dstw/srcw);
	pSprite->setScaleY((float)dsth/srch);
	m_pSprite[m_SpriteLen++] = pSprite;
	this->addChild(pSprite, 0);
}