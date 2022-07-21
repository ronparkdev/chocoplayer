#ifndef __R2K_GRP_COM_TIMER__
#define __R2K_GRP_COM_TIMER__
#include "../COMMON/DebugingCode.h"

#include <cocos2d.h>
USING_NS_CC;

#include "GrpTimer.h"
#include "../GameMain.h"


class GrpComTimer : public CCNode{
private:
	GrpTimer *pTimers[2];
	LsdMain *pLsd;
public:
	GrpComTimer(GameMain *main);
	~GrpComTimer();

	void init(CCNode *layer, GameMain *main);
	void draw(int framedelta);
	void draw(int index, int framedelta, bool &isvisible, bool &isusing, int &leftframe);
};

#endif