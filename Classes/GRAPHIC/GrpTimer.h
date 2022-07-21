#ifndef __R2K_GRP_TIMER__
#define __R2K_GRP_TIMER__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../GameMain.h"

#define GRP_TIMER_NOT_VISIBLE -1

class GrpTimer: public CCNode{
private:
	GameMain *pGM;

	CCSprite *pSpr[5];//12:34
	int lastSec;
	bool isLastColonVisible;
public:
	GrpTimer(GameMain *main);
	~GrpTimer();

	void refresh(int time);
	bool isTimerVisible();
};

#endif