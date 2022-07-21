#include "GrpHandDown.h"
#include "../../AppMultiTime.h"

GrpHandDown::GrpHandDown( CCTexture2D *pTexSystem2 )
{
	m_pSpriteBatch = CCSpriteBatchNode::createWithTexture(pTexSystem2);
	m_pSpriteBatch->retain();

	for(int i=0; i<3; i++)
		m_pSprites[i] = getSprite(i*16, 16, 16, 16, 0, 0);

	CCNode::autorelease();
}

GrpHandDown::~GrpHandDown() {
	for(int i=0; i<3; i++)
		this->removeChild(m_pSprites[i]);

	m_pSpriteBatch->release();
}

void GrpHandDown::draw() {
	int ani = (int)(getTime() * 3.3f) % 3;

	for(int i=0; i<3; i++)
		m_pSprites[i]->setVisible(i == ani);
}

CCSprite* GrpHandDown::getSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty) {
	return getSprite(srcx, srcy, srcw, srch, dstx, dsty, srcw, srch);
}

CCSprite* GrpHandDown::getSprite(int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth) {
	CCTexture2D *pTexture = m_pSpriteBatch->getTexture();
	CCSprite *pSprite;
	pSprite = CCSprite::createWithTexture(pTexture);
	pSprite->setTextureRect(CCRect(srcx, srcy, srcw, srch));
	pSprite->setPosition(ccp(dstx, -dsty));
	pSprite->setScaleX((float)dstw/srcw);
	pSprite->setScaleY((float)dsth/srch);
	this->addChild(pSprite);
	return pSprite;
}