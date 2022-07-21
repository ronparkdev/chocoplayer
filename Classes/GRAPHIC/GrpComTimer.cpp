#include "GrpComTimer.h"

#include "../COMMON/ScreenCommon.h"

GrpComTimer::GrpComTimer(GameMain *main)
{
	pLsd = &(main->lsd);

	for(int i=0; i<2; i++) {
		pTimers[i] = new GrpTimer(main);
		pTimers[i]->setPosition(ccp(i * (320-44), 0));
		this->addChild(pTimers[i]);
	}

	CCNode::autorelease();
}

GrpComTimer::~GrpComTimer()
{
	for(int i=0; i<2; i++)
		this->removeChild(pTimers[i]);
}

void GrpComTimer::draw( int framedelta )
{
	LsdState &st = pLsd->State;
	draw(0,	framedelta, st.timer1_visible, st.timer1_using, st.timer1_leftframe);
	draw(1,	framedelta, st.timer2_visible, st.timer2_using, st.timer2_leftframe);
}

void GrpComTimer::draw( int index, int framedelta, bool &isvisible, bool &isusing, int &leftframe )
{
	if (isusing && leftframe > 0) {
		leftframe -= framedelta;
		if (leftframe <= 0) {
			isusing = false;
		}
	}

	if (isusing & isvisible) {
		pTimers[index]->refresh(leftframe);
	} else {
		pTimers[index]->refresh(GRP_TIMER_NOT_VISIBLE);
	}

}