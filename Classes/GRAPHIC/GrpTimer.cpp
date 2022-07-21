#include "GrpTimer.h"

#include "../COMMON/ScreenCommon.h"

GrpTimer::GrpTimer(GameMain *main)
	:pGM(main),
	 lastSec(GRP_TIMER_NOT_VISIBLE)
{
	for(int i=0; i<5; i++) {
		pSpr[i] = CCSprite::create();
		pSpr[i]->setPosition(ccp(SCR_POS_X(i*8+4), SCR_POS_Y(8)));
		pSpr[i]->setVisible(false);
		this->addChild(pSpr[i]);
	}

	CCNode::autorelease();
}

GrpTimer::~GrpTimer()
{
	for(int i=0; i<5; i++)
		this->removeChild(pSpr[i]);
}

void GrpTimer::refresh(int cur_frame)
{
	int i, c, min, sec, totalsec;
	bool viewcolon = ((cur_frame % FRAME_PER_SEC) > FRAME_PER_SEC/2);
	
	if (cur_frame != GRP_TIMER_NOT_VISIBLE)
		totalsec = cur_frame/FRAME_PER_SEC;
	else
		totalsec = cur_frame;

	if (lastSec == totalsec && isLastColonVisible == viewcolon)
		return;

	min = totalsec / 60;
	sec = totalsec % 60;

	int texID = IBuffer::getInstance()->get(imgSystem, pGM->lsd.System.system_name);
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	if (pTex == NULL)
		return;
	
	for(int i=0; i<5; i++)
		pSpr[i]->setTexture(pTex);

	if (cur_frame == GRP_TIMER_NOT_VISIBLE) {
		for(int i=0; i<5; i++)
			pSpr[i]->setVisible(false);
	} else {
		if (lastSec == GRP_TIMER_NOT_VISIBLE)
			for(int i=0; i<5; i++)
				pSpr[i]->setVisible(true);
		
		for(i=0; i<5; i++) {
			switch(i) {
				case 0:c = (min / 10)%10;	break;
				case 1:c = min % 10;		break;
				case 2:c = 10;				break;
				case 3:c = (sec / 10)%10;	break;
				case 4:c = sec % 10;		break;
			}
			pSpr[i]->setTextureRect(CCRect(32+c*8,32,8,16));
		}
		
		pSpr[2]->setVisible(viewcolon);
	}
	lastSec = sec;
	isLastColonVisible = viewcolon;
}

bool GrpTimer::isTimerVisible()
{
	return (lastSec != GRP_TIMER_NOT_VISIBLE);
}
