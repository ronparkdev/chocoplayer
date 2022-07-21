#ifndef __R2K_ADVERTISE_MAIN__
#define __R2K_ADVERTISE_MAIN__

#include "../COMMON/ScreenCommon.h"

class AdMain{
private:
	static bool isShowingBigAd;
	static bool isShowingLineAd;

public:
	static void chkAd();
	static void chkAd( ScreenType type );

	static void showBigAd();
	static void hideBigAd();

	static void showLineAd();
	static void hideLineAd();
};


#endif