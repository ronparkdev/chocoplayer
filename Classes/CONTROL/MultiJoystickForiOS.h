﻿#ifndef __R2K_COMMON_JOYSTICK_FOR_IOS__
#define __R2K_COMMON_JOYSTICK_FOR_IOS__
#include "../AppMulti.h"

#include "MultiJoystick.h"

#if (SR_IPHONE == 1)

enum JOYSTICK_KEY_REAL_STRING{
    IOS_KEYCODE_UNKNOWN,
    IOS_KEYCODE_SOFT_LEFT,
    IOS_KEYCODE_SOFT_RIGHT,
    IOS_KEYCODE_HOME,
    IOS_KEYCODE_BACK,
    IOS_KEYCODE_CALL,
    IOS_KEYCODE_ENDCALL,
    IOS_KEYCODE_0,
    IOS_KEYCODE_1,
    IOS_KEYCODE_2,
    IOS_KEYCODE_3,
    IOS_KEYCODE_4,
    IOS_KEYCODE_5,
    IOS_KEYCODE_6,
    IOS_KEYCODE_7,
    IOS_KEYCODE_8,
    IOS_KEYCODE_9,
    IOS_KEYCODE_STAR,
    IOS_KEYCODE_POUND,
    IOS_KEYCODE_DPAD_UP,
    IOS_KEYCODE_DPAD_DOWN,
    IOS_KEYCODE_DPAD_LEFT,
    IOS_KEYCODE_DPAD_RIGHT,
    IOS_KEYCODE_DPAD_CENTER,
    IOS_KEYCODE_VOLUME_UP,
    IOS_KEYCODE_VOLUME_DOWN,
    IOS_KEYCODE_POWER,
    IOS_KEYCODE_CAMERA,
    IOS_KEYCODE_CLEAR,
    IOS_KEYCODE_A,
    IOS_KEYCODE_B,
    IOS_KEYCODE_C,
    IOS_KEYCODE_D,
    IOS_KEYCODE_E,
    IOS_KEYCODE_F,
    IOS_KEYCODE_G,
    IOS_KEYCODE_H,
    IOS_KEYCODE_I,
    IOS_KEYCODE_J,
    IOS_KEYCODE_K,
    IOS_KEYCODE_L,
    IOS_KEYCODE_M,
    IOS_KEYCODE_N,
    IOS_KEYCODE_O,
    IOS_KEYCODE_P,
    IOS_KEYCODE_Q,
    IOS_KEYCODE_R,
    IOS_KEYCODE_S,
    IOS_KEYCODE_T,
    IOS_KEYCODE_U,
    IOS_KEYCODE_V,
    IOS_KEYCODE_W,
    IOS_KEYCODE_X,
    IOS_KEYCODE_Y,
    IOS_KEYCODE_Z,
    IOS_KEYCODE_COMMA,
    IOS_KEYCODE_PERIOD,
    IOS_KEYCODE_ALT_LEFT,
    IOS_KEYCODE_ALT_RIGHT,
    IOS_KEYCODE_SHIFT_LEFT,
    IOS_KEYCODE_SHIFT_RIGHT,
    IOS_KEYCODE_TAB,
    IOS_KEYCODE_SPACE,
    IOS_KEYCODE_SYM,
    IOS_KEYCODE_EXPLORER,
    IOS_KEYCODE_ENVELOPE,
    IOS_KEYCODE_ENTER,
    IOS_KEYCODE_DEL,
    IOS_KEYCODE_GRAVE,
    IOS_KEYCODE_MINUS,
    IOS_KEYCODE_EQUALS,
    IOS_KEYCODE_LEFT_BRACKET,
    IOS_KEYCODE_RIGHT_BRACKET,
    IOS_KEYCODE_BACKSLASH,
    IOS_KEYCODE_SEMICOLON,
    IOS_KEYCODE_APOSTROPHE,
    IOS_KEYCODE_SLASH,
    IOS_KEYCODE_AT,
    IOS_KEYCODE_NUM,
    IOS_KEYCODE_HEADSETHOOK,
    IOS_KEYCODE_FOCUS,
    IOS_KEYCODE_PLUS,
    IOS_KEYCODE_MENU,
    IOS_KEYCODE_NOTIFICATION,
    IOS_KEYCODE_SEARCH,
    IOS_KEYCODE_MEDIA_PLAY_PAUSE,
    IOS_KEYCODE_MEDIA_STOP,
    IOS_KEYCODE_MEDIA_NEXT,
    IOS_KEYCODE_MEDIA_PREVIOUS,
    IOS_KEYCODE_MEDIA_REWIND,
    IOS_KEYCODE_MEDIA_FAST_FORWARD,
    IOS_KEYCODE_MUTE,
    IOS_KEYCODE_PAGE_UP,
    IOS_KEYCODE_PAGE_DOWN,
    IOS_KEYCODE_PICTSYMBOLS,
    IOS_KEYCODE_SWITCH_CHARSET,
    IOS_KEYCODE_BUTTON_A,
    IOS_KEYCODE_BUTTON_B,
    IOS_KEYCODE_BUTTON_C,
    IOS_KEYCODE_BUTTON_X,
    IOS_KEYCODE_BUTTON_Y,
    IOS_KEYCODE_BUTTON_Z,
    IOS_KEYCODE_BUTTON_L1,
    IOS_KEYCODE_BUTTON_R1,
    IOS_KEYCODE_BUTTON_L2,
    IOS_KEYCODE_BUTTON_R2,
    IOS_KEYCODE_BUTTON_THUMBL,
    IOS_KEYCODE_BUTTON_THUMBR,
    IOS_KEYCODE_BUTTON_START,
    IOS_KEYCODE_BUTTON_SELECT,
    IOS_KEYCODE_BUTTON_MODE,
    IOS_KEYCODE_ESCAPE,
    IOS_KEYCODE_FORWARD_DEL,
    IOS_KEYCODE_CTRL_LEFT,
    IOS_KEYCODE_CTRL_RIGHT,
    IOS_KEYCODE_CAPS_LOCK,
    IOS_KEYCODE_SCROLL_LOCK,
    IOS_KEYCODE_META_LEFT,
    IOS_KEYCODE_META_RIGHT,
    IOS_KEYCODE_FUNCTION,
    IOS_KEYCODE_SYSRQ,
    IOS_KEYCODE_BREAK,
    IOS_KEYCODE_MOVE_HOME,
    IOS_KEYCODE_MOVE_END,
    IOS_KEYCODE_INSERT,
    IOS_KEYCODE_FORWARD,
    IOS_KEYCODE_MEDIA_PLAY,
    IOS_KEYCODE_MEDIA_PAUSE,
    IOS_KEYCODE_MEDIA_CLOSE,
    IOS_KEYCODE_MEDIA_EJECT,
    IOS_KEYCODE_MEDIA_RECORD,
    IOS_KEYCODE_F1,
    IOS_KEYCODE_F2,
    IOS_KEYCODE_F3,
    IOS_KEYCODE_F4,
    IOS_KEYCODE_F5,
    IOS_KEYCODE_F6,
    IOS_KEYCODE_F7,
    IOS_KEYCODE_F8,
    IOS_KEYCODE_F9,
    IOS_KEYCODE_F10,
    IOS_KEYCODE_F11,
    IOS_KEYCODE_F12,
    IOS_KEYCODE_NUM_LOCK,
    IOS_KEYCODE_NUMPAD_0,
    IOS_KEYCODE_NUMPAD_1,
    IOS_KEYCODE_NUMPAD_2,
    IOS_KEYCODE_NUMPAD_3,
    IOS_KEYCODE_NUMPAD_4,
    IOS_KEYCODE_NUMPAD_5,
    IOS_KEYCODE_NUMPAD_6,
    IOS_KEYCODE_NUMPAD_7,
    IOS_KEYCODE_NUMPAD_8,
    IOS_KEYCODE_NUMPAD_9,
    IOS_KEYCODE_NUMPAD_DIVIDE,
    IOS_KEYCODE_NUMPAD_MULTIPLY,
    IOS_KEYCODE_NUMPAD_SUBTRACT,
    IOS_KEYCODE_NUMPAD_ADD,
    IOS_KEYCODE_NUMPAD_DOT,
    IOS_KEYCODE_NUMPAD_COMMA,
    IOS_KEYCODE_NUMPAD_ENTER,
    IOS_KEYCODE_NUMPAD_EQUALS,
    IOS_KEYCODE_NUMPAD_LEFT_PAREN,
    IOS_KEYCODE_NUMPAD_RIGHT_PAREN,
    IOS_KEYCODE_VOLUME_MUTE,
    IOS_KEYCODE_INFO,
    IOS_KEYCODE_CHANNEL_UP,
    IOS_KEYCODE_CHANNEL_DOWN,
    IOS_KEYCODE_ZOOM_IN,
    IOS_KEYCODE_ZOOM_OUT,
    IOS_KEYCODE_TV,
    IOS_KEYCODE_WINDOW,
    IOS_KEYCODE_GUIDE,
    IOS_KEYCODE_DVR,
    IOS_KEYCODE_BOOKMARK,
    IOS_KEYCODE_CAPTIONS,
    IOS_KEYCODE_SETTINGS,
    IOS_KEYCODE_TV_POWER,
    IOS_KEYCODE_TV_INPUT,
    IOS_KEYCODE_STB_POWER,
    IOS_KEYCODE_STB_INPUT,
    IOS_KEYCODE_AVR_POWER,
    IOS_KEYCODE_AVR_INPUT,
    IOS_KEYCODE_PROG_RED,
    IOS_KEYCODE_PROG_GREEN,
    IOS_KEYCODE_PROG_YELLOW,
    IOS_KEYCODE_PROG_BLUE,
    IOS_KEYCODE_APP_SWITCH,
    IOS_KEYCODE_BUTTON_1,
    IOS_KEYCODE_BUTTON_2,
    IOS_KEYCODE_BUTTON_3,
    IOS_KEYCODE_BUTTON_4,
    IOS_KEYCODE_BUTTON_5,
    IOS_KEYCODE_BUTTON_6,
    IOS_KEYCODE_BUTTON_7,
    IOS_KEYCODE_BUTTON_8,
    IOS_KEYCODE_BUTTON_9,
    IOS_KEYCODE_BUTTON_10,
    IOS_KEYCODE_BUTTON_11,
    IOS_KEYCODE_BUTTON_12,
    IOS_KEYCODE_BUTTON_13,
    IOS_KEYCODE_BUTTON_14,
    IOS_KEYCODE_BUTTON_15,
    IOS_KEYCODE_BUTTON_16,
    IOS_KEYCODE_LANGUAGE_SWITCH,
    IOS_KEYCODE_MANNER_MODE,
    IOS_KEYCODE_3D_MODE,
    IOS_KEYCODE_CONTACTS,
    IOS_KEYCODE_CALENDAR,
    IOS_KEYCODE_MUSIC,
    IOS_KEYCODE_CALCULATOR,
    IOS_KEYCODE_ZENKAKU_HANKAKU,
    IOS_KEYCODE_EISU,
    IOS_KEYCODE_MUHENKAN,
    IOS_KEYCODE_HENKAN,
    IOS_KEYCODE_KATAKANA_HIRAGANA,
    IOS_KEYCODE_YEN,
    IOS_KEYCODE_RO,
    IOS_KEYCODE_KANA,
    IOS_KEYCODE_ASSIST,
    IOS_KEYCODE_BRIGHTNESS_DOWN,
    IOS_KEYCODE_BRIGHTNESS_UP,
    IOS_KEYCODE_MEDIA_AUDIO_TRACK,
    IOS_KEYCODE_SLEEP,
    IOS_KEYCODE_WAKEUP,
    IOS_KEYCODE_PAIRING,
    IOS_KEYCODE_MEDIA_TOP_MENU,
    IOS_KEYCODE_11,
    IOS_KEYCODE_12,
    IOS_KEYCODE_LAST_CHANNEL,
    IOS_KEYCODE_TV_DATA_SERVICE,
    IOS_KEYCODE_VOICE_ASSIST,
    IOS_KEYCODE_TV_RADIO_SERVICE,
    IOS_KEYCODE_TV_TELETEXT,
    IOS_KEYCODE_TV_NUMBER_ENTRY,
    IOS_KEYCODE_TV_TERRESTRIAL_ANALOG,
    IOS_KEYCODE_TV_TERRESTRIAL_DIGITAL,
    IOS_KEYCODE_TV_SATELLITE,
    IOS_KEYCODE_TV_SATELLITE_BS,
    IOS_KEYCODE_TV_SATELLITE_CS,
    IOS_KEYCODE_TV_SATELLITE_SERVICE,
    IOS_KEYCODE_TV_NETWORK,
    IOS_KEYCODE_TV_ANTENNA_CABLE,
    IOS_KEYCODE_TV_INPUT_HDMI_1,
    IOS_KEYCODE_TV_INPUT_HDMI_2,
    IOS_KEYCODE_TV_INPUT_HDMI_3,
    IOS_KEYCODE_TV_INPUT_HDMI_4,
    IOS_KEYCODE_TV_INPUT_COMPOSITE_1,
    IOS_KEYCODE_TV_INPUT_COMPOSITE_2,
    IOS_KEYCODE_TV_INPUT_COMPONENT_1,
    IOS_KEYCODE_TV_INPUT_COMPONENT_2,
    IOS_KEYCODE_TV_INPUT_VGA_1,
    IOS_KEYCODE_TV_AUDIO_DESCRIPTION,
    IOS_KEYCODE_TV_AUDIO_DESCRIPTION_MIX_UP,
    IOS_KEYCODE_TV_AUDIO_DESCRIPTION_MIX_DOWN,
    
};

extern const char* JOYSTICK_KEY_REAL_STRING[256];

const int JOYSTICK_DEF_KEY_CODE[JOYSTICK_KEY_COUNT] = {
    IOS_KEYCODE_DPAD_UP, IOS_KEYCODE_DPAD_DOWN, IOS_KEYCODE_DPAD_LEFT, IOS_KEYCODE_DPAD_RIGHT,
    IOS_KEYCODE_BUTTON_A, IOS_KEYCODE_BUTTON_B, IOS_KEYCODE_BUTTON_C,
    IOS_KEYCODE_0, IOS_KEYCODE_1, IOS_KEYCODE_2, IOS_KEYCODE_3, IOS_KEYCODE_4,
    IOS_KEYCODE_5, IOS_KEYCODE_6, IOS_KEYCODE_7, IOS_KEYCODE_8, IOS_KEYCODE_9,
    IOS_KEYCODE_PLUS, IOS_KEYCODE_MINUS, IOS_KEYCODE_STAR, IOS_KEYCODE_SLASH, IOS_KEYCODE_COMMA,
    0, IOS_KEYCODE_D, IOS_KEYCODE_E
};

#endif

#endif