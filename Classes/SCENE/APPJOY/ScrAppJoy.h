#ifndef __R2K_SCREEN_APP_JOY_STICK__
#define __R2K_SCREEN_APP_JOY_STICK__

#include "cocos2d.h"
USING_NS_CC;

#include "../../DATA/ArrayList.h"

class ScrAppJoy : public CCLayer
{
private:
	CCSize sz;

	int curKey;

	bool isMenuClosing;

	CCSprite* pBg;
	
	CCLabelTTF* pText;
	CCLabelTTF* pText2;

	CCMenuItemImage* pBtnPrevious;
	CCMenuItemImage* pBtnNext;

public:
	CREATE_FUNC(ScrAppJoy);

	virtual bool init();
	void initBtns(float menu_scale);

	void draw();
	void refreshKey();

	void doMenuClicked(CCObject* pSender);
};

#endif