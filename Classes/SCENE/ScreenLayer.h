#ifndef __R2K_SCREEN_LAYER__
#define __R2K_SCREEN_LAYER__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
#include "../CONTROL/ctlButton.h"
USING_NS_CC;

class ScreenLayer : public CCLayer{
public:
	virtual void Init();
	virtual void onTouch(Touch_Type type, int id, float x, float y);
	virtual void setEnable(bool isEnable);
	virtual void Draw(int framedelta);

	virtual void reloadSetting();
	virtual void reloadShaders();

	virtual bool isFullScreen();
};

#endif