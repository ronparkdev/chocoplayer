#ifndef __R2K_CONTROL_MAIN__
#define __R2K_CONTROL_MAIN__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
#include "ctlButton.h"
#include "../CONTROL/MultiJoystick.h"
#include "../DATA/ArrayList.h"

USING_NS_CC;

#define R2K_KEYCODE_COUNT 22

#define R2K_KEY_NUM_COUNT 10
#define R2K_KEY_SPE_COUNT 5

#define R2K_KEY_ARROW_WIDTH 250
#define R2K_KEY_PUBLIC_WIDTH 200
#define R2K_KEY_RECYCLE_WIDTH 500


class ctlMain : public CCNode {
private:
	bool isVisible;

	ctlButton *virtualKeys[R2K_KEYCODE_COUNT];
	int joystickKeys[JOYSTICK_KEY_COUNT];
	ArrayList<int> joystickArrowStack;

public:
	ctlMain();
	void Init();

	void draw();

	bool onTouch(Touch_Type type, int touch_id, float x, float y);

	int getKey(R2K_KEYCODE keytype);
	void setKey(R2K_KEYCODE keytype, int val);

	void updateSetting();

	void setVisible(bool isVisible);
};

#endif