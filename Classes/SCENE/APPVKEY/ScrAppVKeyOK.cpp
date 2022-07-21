#include "ScrAppVKeyOK.h"

#include "../AppScaleTools.h"

#define VKEY_OK_Z_ACTIVE 100

void ScrAppVKeyOK::init( CCNode *layer )
{
	CCSize sz = CCDirector::sharedDirector()->getVisibleSize();

	pLayer = layer;

	pSpr = CCSprite::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("gamelist_btn_ok.png"));
	pSpr->setScale(AppScaleTools::cmToPx(1.0f) / pSpr->getContentSize().width);

	width = pSpr->getContentSize().width;
	radius = pSpr->getScale() * width * 0.5f;

	pSpr->setPosition(ccp(sz.width/2, radius * 1.3f));

	pLayer->addChild(pSpr, VKEY_OK_Z_ACTIVE);
}

void ScrAppVKeyOK::show( ArrayList<ScrAppVKeyNode> &m_nodes, ScrAppVKeyStore &m_store )
{
	CCSize sz = CCDirector::sharedDirector()->getVisibleSize();
	float x, y, r = radius;
	float xFrom = r;
	float xTo = sz.width - r;
	float yFrom = r;
	float yTo = sz.height - r;
	int i, j;

	for(i=0; i<100; i++) {
		if (i == 0) {
			x = pSpr->getPositionX();
			y = pSpr->getPositionY();

			if (x < xFrom || xTo < x || y < yFrom || yTo < y)
				continue;

		} else if (i==1) {
			x = sz.width/2;
			y = radius * 1.3f;			
		} else {
			x = (rand() % (int)(xTo - xFrom)) + xFrom;
			y = (rand() % (int)(yTo - yFrom)) + yFrom;
		}
		for(j=0; j<m_nodes.GetLength(); j++) {
			if (!m_nodes[j].isUsing())
				continue;

			if (m_nodes[j].isCycleDup(x, y, r))
				break;
		}

		if (j == m_nodes.GetLength() && !m_store.isCycleDup(x, y, r))
			break;
	}

	pSpr->stopAllActions();
	pSpr->runAction(CCEaseBackOut::create(CCMoveTo::create(0.3f, ccp(x,y))));
	pSpr->runAction(CCFadeTo::create(0.3f, 255));
}

void ScrAppVKeyOK::hide()
{
	pSpr->runAction(CCFadeTo::create(0.3f, 50));
}

bool ScrAppVKeyOK::isCycleIn( float x, float y )
{
	float dx = (x - pSpr->getPositionX());
	float dy = (y - pSpr->getPositionY());
	float d = (float)sqrt(dx*dx + dy*dy);

	return (d<radius);
}
