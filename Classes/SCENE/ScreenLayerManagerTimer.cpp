#include "ScreenLayerManager.h"

#include "../GameMain.h"
#include "../COMMON/GlobalLangFont.h"
#include "../COMMON/GlobalLangFile.h"
#include "../COMMON/MultiOutter.h"
#include "../AppMulti.h"

void ScreenLayerManager::initTimeLimit() {
	limitFrameLeft = 100;

	int ltw = ScreenCommon::game_w * ScreenCommon::game_s;
	int lth = ScreenCommon::game_h * ScreenCommon::game_s;
	int ltx = ScreenCommon::game_x + ltw * 0.5f;
	int lty = ScreenCommon::game_y + lth * 0.5f; 

	pLimitTimeLeft = CCLabelTTF::create("", GlobalLangFont::getDefFontName(), ScreenCommon::game_s * 9.0f );
	pLimitTimeLeft->setPosition(ccp(ltx, lty));
	pLimitTimeLeft->setVerticalAlignment(kCCVerticalTextAlignmentTop);
	pLimitTimeLeft->setHorizontalAlignment(kCCTextAlignmentLeft);
	pLimitTimeLeft->setDimensions(CCSize(ltw, lth));
	pLimitTimeLeft->setVisible(false);
	this->addChild(pLimitTimeLeft, sz_controler-1);
}

void ScreenLayerManager::startTimeLimit() {
	limitFrameLeft = 1 * 60 * 60;
}

void ScreenLayerManager::checkTimeLimit(int scrType) {
#if (SR_IPHONE == 1)
	switch(scrType) {
	case scr_app_main:
	case scr_title:
	case scr_save:
		pLimitTimeLeft->setVisible(false);
		break;

	default:
		pLimitTimeLeft->setVisible(true);
		break;
	}
#endif
}
void ScreenLayerManager::drawTimeLimit(int framedelta) {

#if (SR_IPHONE == 1)
    switch(scrType) {
        case scr_app_main:
        case scr_title:
        case scr_save:
            return;
    }
    
    if (MultiOutter::isProI()) {
        limitFrameLeft = 100;
        return;
    }
    
	int limitSecLeft, limitSecLeftLast;
	limitSecLeft = limitFrameLeft / 60;
	if (limitFrameLeft > 0) {
		limitFrameLeft -= framedelta;
		limitSecLeftLast = limitFrameLeft / 60;
        
        GlobalLangFile *l = GlobalLangFile::getInstance();
        
		if (limitFrameLeft <= 0) {
			limitFrameLeft = 0;
			setScrPaused(true);

			pLimitTimeLeft->setString(l->getStringForKey("TIMELIMIT_FINISH"));
		} else {
			if (limitSecLeft != limitSecLeftLast) {
				int m = limitSecLeft / 60;
				int s = limitSecLeft % 60;

				char buf[128];

				sprintf(buf, l->getStringForKey("TIMELIMIT_NORMAL"), m, s);

				pLimitTimeLeft->setString(buf);
			}
		}
	}
    
#endif

}