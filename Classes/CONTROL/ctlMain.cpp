#include "ctlMain.h"

#include "../GameMain.h"
#include "../COMMON/ScreenCommon.h"
#include "../COMMON/MgrSetting.h"

ctlMain::ctlMain() {
	this->autorelease();
}

void ctlMain::Init() {
	int i=0, j;
	virtualKeys[i++] = new ctlButton(rkc_arrow, 0);
	virtualKeys[i++] = new ctlButton(rkc_enter, 0);
	virtualKeys[i++] = new ctlButton(rkc_esc, 0);
	virtualKeys[i++] = new ctlButton(rkc_shift, 0);

	for(j=0; j<10; j++)
		virtualKeys[i++] = new ctlButton(rkc_num, j);

	for(j=0; j<5; j++)
		virtualKeys[i++] = new ctlButton(rkc_special, j);

	virtualKeys[i++] = new ctlButton(rkc_menu, 0);
	virtualKeys[i++] = new ctlButton(rkc_turbo, 0);
	virtualKeys[i++] = new ctlButton(rkc_qsave, 0);

	for(j=0; j<R2K_KEYCODE_COUNT; j++)
		this->addChild(virtualKeys[j], 1);

// 	m_pSprBlack1->setOpacity(125.0f);
// 	m_pSprBlack2->setOpacity(125.0f);
	isVisible = true;
}

void ctlMain::draw()
{
	int i, j, keycode;
	for(i=0; i<JOYSTICK_KEY_COUNT; i++) {
		keycode = joystickKeys[i];

		if (keycode <= 0)
			continue;

		if (m_pushed[keycode] == m_pushed_checked[keycode])
			continue;

		m_pushed_checked[keycode] = m_pushed[keycode];

		if (m_pushed[keycode]) {
	
			setKey((R2K_KEYCODE)JOYSTICK_KEY_CODE[i], JOYSTICK_KEY_CODEVAL[i]);
			if (JOYSTICK_KEY_CODE[i] == rkc_arrow)
				joystickArrowStack.Add(i);
		} else {
			if (JOYSTICK_KEY_CODE[i] == rkc_arrow) {
				for(j=0; j<joystickArrowStack.GetLength(); j++) {
					if (joystickArrowStack[j] == i) {
						joystickArrowStack.Remove(j);
						j--;
					}
				}
				if (joystickArrowStack.GetLength() > 0) {

					setKey((R2K_KEYCODE)JOYSTICK_KEY_CODE[i], 
							JOYSTICK_KEY_CODEVAL[*(joystickArrowStack.GetTail())]);
					continue;
				}
			}

			setKey((R2K_KEYCODE)JOYSTICK_KEY_CODE[i], 0);
		}
	}
}

bool ctlMain::onTouch( Touch_Type type, int touch_id, float x, float y )
{
	if (!isVisible) {
		return false;
	} else {
		bool isTouched = false;
		for(int i=0; i<R2K_KEYCODE_COUNT; i++)
			isTouched |= virtualKeys[i]->onTouch(type, touch_id, x, y);

		return isTouched;
	}
}

int ctlMain::getKey(R2K_KEYCODE keytype)
{
	switch(keytype) {
	case rkc_arrow:
		return virtualKeys[0]->getID();

	case rkc_enter:
		return virtualKeys[1]->getID();

	case rkc_esc:
		return virtualKeys[2]->getID();

	case rkc_shift:
		return virtualKeys[3]->getID();

	case rkc_num:	
		for(int i=4; i<14; i++)
			if (virtualKeys[i]->getID() != 0)
				return virtualKeys[i]->getID();
		return 0;
		break;

	case rkc_special:	
		for(int i=14; i<19; i++)
			if (virtualKeys[i]->getID() != 0)
				return virtualKeys[i]->getID();
		return 0;
		break;

	case rkc_menu:	
		return virtualKeys[19]->getID();

	case rkc_turbo:	
		return virtualKeys[20]->getID();

	case rkc_qsave:	
		return virtualKeys[21]->getID();

	default:
		return 0;
	}
}

void ctlMain::setKey(R2K_KEYCODE keytype, int val)
{
	switch(keytype) {
	case rkc_arrow:
		virtualKeys[0]->setID(val);
		break;

	case rkc_enter:
		virtualKeys[1]->setID(val!=0);
		break;

	case rkc_esc:
		virtualKeys[2]->setID(val!=0);
		break;

	case rkc_shift:
		virtualKeys[3]->setID(val!=0);
		break;

	case rkc_num:	
		for(int i=0; i<10; i++)
			virtualKeys[i+4]->setID(KEYCODE_NUMBER[i]==val);
		break;

	case rkc_special:	
		for(int i=0; i<5; i++)
			virtualKeys[i+14]->setID(KEYCODE_SPECIAL[i]==val);
		break;

	case rkc_menu:
		virtualKeys[19]->setID(val!=0);
		break;

	case rkc_turbo:
		virtualKeys[20]->setID(val!=0);
		break;

	case rkc_qsave:
		virtualKeys[21]->setID(val!=0);
	}
}

void ctlMain::updateSetting()
{
	for(int i=0; i<R2K_KEYCODE_COUNT; i++)
		virtualKeys[i]->updateSetting();

	if (!isVisible) {
		for(int i=0; i<R2K_KEYCODE_COUNT; i++)
			virtualKeys[i]->setVisible(false);
	}

	MgrSetting *s = MgrSetting::getInstance();
	for(int i=0; i<JOYSTICK_KEY_COUNT; i++)
		joystickKeys[i] = s->getInt(RST_JOYSTICK_KEYCODE + i);
}

void ctlMain::setVisible(bool isVisible)
{
	this->isVisible = isVisible;
	if (!isVisible) {
		for(int i=0; i<R2K_KEYCODE_COUNT; i++)
			virtualKeys[i]->setVisible(false);
	} else {
		for(int i=0; i<R2K_KEYCODE_COUNT; i++)
			virtualKeys[i]->updateSetting();

	}
}