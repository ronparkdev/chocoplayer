#ifndef __R2K_SCREEN_APP_VIRTUAL_KEY_NODE__
#define __R2K_SCREEN_APP_VIRTUAL_KEY_NODE__

#include "cocos2d.h"
USING_NS_CC;

#define VKEY_CNT 22
#include "../../DATA/ArrayList.h"
#include "ScrAppVKeyStore.h"

class ScrAppVKeyNode
{
private:
	CCNode *pLayer;
	int id;
	
	CCSprite* pSpr;
	int width;
	float radius;

	bool isUsing_;
	float scaleUse;
	float scaleUnuse;

	float unusePosX;
	float unusePosY;

public:
	void init(CCNode *layer, int index);

	void setUsing(bool val);

	bool isUsing();
	bool isCycleIn(float x, float y);
	bool isCycleDup( float x, float y, float r );
	bool isCycleDup( ScrAppVKeyStore &n );
	void load();
	void save();

	void doOpenStore( float x, float y, float time = 0.3f );
	void doCloseStore( float x, float y, float time = 0.3f );
	void doSetForeground( float isForeground, float time = 0.3f );
	void doSetUse( bool isUse, float time = 0.3f );
	void setRandomPosition( ArrayList<ScrAppVKeyNode> &m_nodes, ScrAppVKeyStore &m_store );
	void addPosition( float x, float y );
};

#endif