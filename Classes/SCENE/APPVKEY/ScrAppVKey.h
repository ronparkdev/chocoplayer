#ifndef __R2K_SCREEN_APP_VIRTUAL_KEY__
#define __R2K_SCREEN_APP_VIRTUAL_KEY__

#include "cocos2d.h"
USING_NS_CC;

#include "../../DATA/ArrayList.h"
#include "ScrAppVKeyNode.h"
#include "ScrAppVKeyStore.h"
#include "ScrAppVKeyOK.h"

class ScrAppVKey : public CCLayer
{
private:
	ArrayList<ScrAppVKeyNode> nodes;
	ScrAppVKeyStore btnStore;
	ScrAppVKeyOK btnOK;
	CCSprite *pSprBg;

	int m_selNodeID;
public:
	CREATE_FUNC(ScrAppVKey);
	~ScrAppVKey();

	virtual bool init();

	void draw();

	bool ccTouchBegan( CCTouch *pTouch, CCEvent* event );
	void ccTouchMoved( CCTouch *pTouch, CCEvent* event );
	void ccTouchEnded( CCTouch *pTouch, CCEvent* event );

	void doClose();
};

#endif