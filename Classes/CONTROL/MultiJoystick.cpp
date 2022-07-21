#include "MultiJoystick.h"
#include "../CONTROL/ctlButton.h"

const int JOYSTICK_KEY_CODE[JOYSTICK_KEY_COUNT] = {
	rkc_arrow, rkc_arrow, rkc_arrow, rkc_arrow, 
	rkc_enter, rkc_esc, rkc_shift, 
	rkc_num, rkc_num, rkc_num, rkc_num, rkc_num, 
	rkc_num, rkc_num, rkc_num, rkc_num, rkc_num, 
	rkc_special, rkc_special, rkc_special, rkc_special, rkc_special, 
	rkc_menu, rkc_turbo, rkc_qsave
};

const int JOYSTICK_KEY_CODEVAL[JOYSTICK_KEY_COUNT] = {
	rkca_up, rkca_down, rkca_left, rkca_right, 
	KEYCODE_ENTER, KEYCODE_ESC, kEYCODE_SHIFT, 
	KEYCODE_NUMBER[0], KEYCODE_NUMBER[1], KEYCODE_NUMBER[2], KEYCODE_NUMBER[3], KEYCODE_NUMBER[4], 
	KEYCODE_NUMBER[5], KEYCODE_NUMBER[6], KEYCODE_NUMBER[7], KEYCODE_NUMBER[8], KEYCODE_NUMBER[9], 
	KEYCODE_SPECIAL[0],KEYCODE_SPECIAL[1],KEYCODE_SPECIAL[2],KEYCODE_SPECIAL[3],KEYCODE_SPECIAL[4],
	1, 1, 1
};

const char* JOYSTICK_KEY_STRING[JOYSTICK_KEY_COUNT] = {
	"Up", "Down", "Left", "Right", 
	"A", "B", "Shift(For Rpg2000)", 
	"Num 0(For Rpg2003)", "Num 1(For Rpg2003)", "Num 2(For Rpg2003)", "Num 3(For Rpg2003)", "Num 4(For Rpg2003)",  
	"Num 5(For Rpg2003)", "Num 6(For Rpg2003)", "Num 7(For Rpg2003)", "Num 8(For Rpg2003)", "Num 9(For Rpg2003)",
	"Special +(For Rpg2003)", "Special -(For Rpg2003)", "Special *(For Rpg2003)", "Special /(For Rpg2003)", "Special ◎(For Rpg2003)",
	"Menu", "Turbo", "QuickSave"
};

bool m_pushed[256] = {false, };
bool m_pushed_checked[256] = {false, };

int m_last_pushed = -1;

void DoJoystickDown(int keycode) {
	//CCLOG((_ST("KeyDown : ") + keycode).getTextUTF8());
	if (0 <= keycode && keycode < 256) {
		m_pushed[keycode] = true;
		m_last_pushed = keycode;
	}
}

void DoJoystickUp(int keycode) {
	//CCLOG((_ST("KeyUp : ") + keycode).getTextUTF8());
	if (0 <= keycode && keycode < 256)
		m_pushed[keycode] = false;
}
