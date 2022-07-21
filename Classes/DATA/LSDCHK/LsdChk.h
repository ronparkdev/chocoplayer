#ifndef __R2K__LSD_CHK__
#define __R2K__LSD_CHK__


#include "../LSD/LsdMain.h"

#include <cocos2d.h>
USING_NS_CC;

class LsdChk{
public:
	static void chk(LsdMain &lsd1, LsdMain &lsd2);

private:
	static void chkSystem(LsdSystem &s1, LsdSystem &s2);

	static void chkScreen(LsdScreen &s1, LsdScreen &s2);

	static void chkHeroObject(LsdLocationHero &s1, LsdLocationHero &s2);
	static void chkRideObject(int index, LsdLocationRide &s1, LsdLocationRide &s2);
	static void chkMapObject(int index, LsdMapEvent &s1, LsdMapEvent &s2);
	static void chkObject(int index, LsdObjectCommon &s1, LsdObjectCommon &s2);

};

#endif

