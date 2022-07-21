#include "GrpSysDown.h"
#include "../../AppMultiTime.h"

GrpSysDown::GrpSysDown(CCTexture2D *pTexSystem)
{
	m_width = 16;
	m_height = 8;
	addSprite(pTexSystem,	40,	16,	16,	8,	0,	0);

	CCNode::autorelease();
}

GrpSysDown::~GrpSysDown()
{
	this->removeChild(m_pSprite);
}

void GrpSysDown::addSprite(CCTexture2D *pTexSystem, int srcx, int srcy, int srcw, int srch, int dstx, int dsty) {
	addSprite(pTexSystem, srcx, srcy, srcw, srch, dstx, dsty, srcw, srch);
}
void GrpSysDown::addSprite(CCTexture2D *pTexSystem, int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth) {
	CCTexture2D *pTexture = pTexSystem;
	CCSprite *pSprite;
	pSprite = CCSprite::createWithTexture(pTexture);
	pSprite->setTextureRect(CCRect(srcx, srcy, srcw, srch));
	pSprite->setPosition(ccp(dstx + (dstw - m_width)/2, -(dsty + (dsth - m_height)/2) ));
	pSprite->setScaleX((float)dstw/srcw);
	pSprite->setScaleY((float)dsth/srch);
	this->addChild(pSprite);
	m_pSprite = pSprite;
}

void GrpSysDown::draw()
{
	float per = 255.0f * (sin(getTime() * 2.0f)+1.0f)/2.0f;
	m_pSprite->setOpacity(per);
}
