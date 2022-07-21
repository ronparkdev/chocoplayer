#ifndef __R2K_COMMON_JOYSTICK__
#define __R2K_COMMON_JOYSTICK__
#include "../AppMulti.h"

#define JOYSTICK_KEY_COUNT 25

#if (SR_WIN == 1 || SR_ANDROID == 1)
#include "MultiJoystickForAndroid.h"
#elif (SR_IPHONE == 1)
#include "MultiJoystickForiOS.h"
#endif

enum JOYSTICK_KEY_NAME{
	JOYSTICK_UP, JOYSTICK_DOWN, JOYSTICK_LEFT, JOYSTICK_RIGHT,
	JOYSTICK_A, JOYSTICK_B, JOYSTICK_S, 
	JOYSTICK_0, JOYSTICK_1, JOYSTICK_2, JOYSTICK_3, JOYSTICK_4, 
	JOYSTICK_5, JOYSTICK_6, JOYSTICK_7, JOYSTICK_8, JOYSTICK_9, 
	JOYSTICK_PLUS, JOYSTICK_MINUS, JOYSTICK_STAR, JOYSTICK_SLUSH, JOYSTICK_COMMA,
	JOYSTICK_MENU, JOYSTICK_TURBO, JOYSTICK_QUICKSAVE
};

extern const int JOYSTICK_KEY_CODE[JOYSTICK_KEY_COUNT];
extern const int JOYSTICK_KEY_CODEVAL[JOYSTICK_KEY_COUNT];
extern const char* JOYSTICK_KEY_STRING[JOYSTICK_KEY_COUNT];

extern bool m_pushed[256];
extern bool m_pushed_checked[256];
extern int m_last_pushed;

void DoJoystickDown(int keycode);
void DoJoystickUp(int keycode);

#endif