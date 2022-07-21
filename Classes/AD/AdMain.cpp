#include "AdMain.h"

#include "../GameMain.h"

void AdMain::chkAd() {
	GameMain *main = GameMain::getInstance();

	if (main != NULL) {
		chkAd((ScreenType)main->lsd.System.screen);
	}
}

void AdMain::chkAd( ScreenType type )
{
	CCLOG("checking Advertise : %d", (int)type);
	switch(type) {
		case scr_title:
		case scr_app_main:
			showLineAd();
			break;
			/*
		default:
			hideLineAd();
			break;
			*/
	}
}
