#ifndef __R2K_SCREEN_APP_VIRTUAL_KEY_OK__
#define __R2K_SCREEN_APP_VIRTUAL_KEY_OK__

#include "cocos2d.h"
USING_NS_CC;

#define VKEY_CNT 22
#include "../../DATA/ArrayList.h"
#include "ScrAppVKeyStore.h"
#include "ScrAppVKeyNode.h"

class ScrAppVKeyOK
{
private:
	CCNode *pLayer;

	CCSprite* pSpr;
	int width;
	float radius;

public:
	void init(CCNode *layer);

	void show(ArrayList<ScrAppVKeyNode> &m_nodes, ScrAppVKeyStore &m_store );
	void hide();

	bool isCycleIn( float x, float y );
};

#endif