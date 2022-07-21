#include "ScreenCommon.h"

#include "MgrSetting.h"

bool ScreenCommon::game_is_vertical = false;

int ScreenCommon::game_x		= 0;
int ScreenCommon::game_y		= 0;
float ScreenCommon::game_s		= 2.0f;


int ScreenCommon::game_x_init	= 0;
int ScreenCommon::game_y_init	= 0;

int ScreenCommon::game_w		= 0;
int ScreenCommon::game_h		= 0;

int ScreenCommon::app_w			= game_w;
int ScreenCommon::app_h			= game_h;

float ScreenCommon::game_screen_x	= 0.0f;
float ScreenCommon::game_screen_y	= 0.0f;

float ScreenCommon::game_screen_x_for_event = 0.0f;

float ScreenCommon::button_global_scale = 1.0f;

GameVersion ScreenCommon::game_ver = RPG2000;

void ScreenCommon::setAppScreen( int w, int h  )
{
	app_w = w;
	app_h = h;

	int t;
	bool isVertical = MgrSetting::getInstance()->getBool(RST_ETC_IS_VERTICAL);

	game_w = GAME_SCREEN_WIDTH;
	game_h = GAME_SCREEN_HEIGHT;

	game_is_vertical = isVertical;

	if (!isVertical) {

		game_s = (float)app_h/GAME_SCREEN_HEIGHT;
		game_x = (app_w-game_w*game_s)/2;
		game_y = (app_h-game_h*game_s)/2;

		button_global_scale = (float)app_h/600.0f;
	} else {

		game_s = (float)app_w/GAME_SCREEN_WIDTH;
		game_x = 0.0f;
		game_y = app_h - (game_h*game_s);

		button_global_scale = (float)app_w/600.0f;
	}

	game_x_init = 0;
	game_y_init = 0;
}



