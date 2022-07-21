#include "GrpSelection.h"
#include "../../AppMultiTime.h"

GrpSelection::GrpSelection( CCTexture2D *pTexSystem, float w, float h )
{
	m_width = w;
	m_height = h;

	m_SpriteLen = 0;
	for(int i=0; i<2; i++) {
		m_pSpriteBatch[i] = CCSpriteBatchNode::createWithTexture(pTexSystem);

		int n = 32 * i;

		addSprite(i,	64+n,	0,	8,	8,	0,		0);
		addSprite(i,	88+n,	0,	8,	8,	w-8,	0);
		addSprite(i,	64+n,	24,	8,	8,	0,		h-8);
		addSprite(i,	88+n,	24,	8,	8,	w-8,	h-8);

		addSprite(i,	72+n,	0,	16,	8,	8,		0,		w-16,	8);
		addSprite(i,	72+n,	24,	16,	8,	8,		h-8,	w-16,	8);
		addSprite(i,	64+n,	8,	8,	16,	0,		8,		8,		h-16);
		addSprite(i,	88+n,	8,	8,	16,	w-8,	8,		8,		h-16);

		this->addChild(m_pSpriteBatch[i], i);
	}

	CCNode::autorelease();
}

GrpSelection::~GrpSelection()
{
	int id = 0;
	for(int i=0; i<8; i++)
		m_pSpriteBatch[0]->removeChild(m_pSprite[id++], false);

	for(int i=8; i<16; i++)
		m_pSpriteBatch[1]->removeChild(m_pSprite[id++], false);

	for(int i=0; i<2; i++)
		this->removeChild(m_pSpriteBatch[i]);
}

void GrpSelection::addSprite(int i, int srcx, int srcy, int srcw, int srch, int dstx, int dsty) {
	addSprite(i, srcx, srcy, srcw, srch, dstx, dsty, srcw, srch);
}
void GrpSelection::addSprite(int i, int srcx, int srcy, int srcw, int srch, int dstx, int dsty, int dstw, int dsth) {
	CCTexture2D *pTexture = m_pSpriteBatch[i]->getTexture();
	CCSprite *pSprite;
	pSprite = CCSprite::createWithTexture(pTexture);
	pSprite->setTextureRect(CCRect(srcx, srcy, srcw, srch));
	pSprite->setPosition(ccp(dstx + (dstw - m_width)/2, -(dsty + (dsth - m_height)/2) ));
	pSprite->setScaleX((float)dstw/srcw);
	pSprite->setScaleY((float)dsth/srch);
	m_pSprite[m_SpriteLen++] = pSprite;
	m_pSpriteBatch[i]->addChild(pSprite);
}

void GrpSelection::draw()
{
	float per = 255.0f * (sin(getTime() * 2.0f)+1.0f)/2.0f;
	for(int i=8; i<16; i++)
		m_pSprite[i]->setOpacity(per);
}
