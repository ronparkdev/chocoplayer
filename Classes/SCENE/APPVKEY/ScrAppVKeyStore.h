#ifndef __R2K_SCREEN_APP_VIRTUAL_KEY_STORE__
#define __R2K_SCREEN_APP_VIRTUAL_KEY_STORE__

#include "cocos2d.h"
USING_NS_CC;

#define VKEY_CNT 22

enum ScrAppVKeyStoreState{
	VKEY_STORE_STATE_PLUS,
	VKEY_STORE_STATE_CANCEL,
	VKEY_STORE_STATE_REMOVE,
};

class ScrAppVKeyStore
{
private:
	CCNode *pLayer;

	CCSprite *pSpr;
	CCSprite *pSprPlus;
	CCSprite *pSprRemove;

	bool isShaking;
	float width;
	float radius;
	ScrAppVKeyStoreState state;
	CCAction* aniShake;
public:
	ScrAppVKeyStore();
	~ScrAppVKeyStore();

	void init(CCNode *layer);

	float getRadius();
	CCPoint getPosition();
	bool isCycleIn(float x, float y);
	bool isCycleDup( float x, float y, float r );

	void doChange(ScrAppVKeyStoreState state, float time = 0.3f);
	ScrAppVKeyStoreState getState();
	void setShake(bool val);
};

#endif