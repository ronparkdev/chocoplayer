#ifndef __R2K_CONTROL_CCLAYER_CTL__
#define __R2K_CONTROL_CCLAYER_CTL__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "ctlMain.h"

class CCLayerCTL : public cocos2d::CCLayer{

private:
	ctlMain *pControl;

protected:
	virtual bool init();

	virtual void onTouch(Touch_Type type, int id, float x, float y);

	void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
	void ccTouchesMoved(CCSet* pTouches, CCEvent* event);
	void ccTouchesEnded(CCSet* pTouches, CCEvent* event);
	void ccTouchesCancelled(CCSet* pTouches, CCEvent* event);

public:
	void setCtl(ctlMain *pCtl);
	ctlMain *getCtl() const;
};

#endif




