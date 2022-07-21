#ifndef __R2K_SCENE_SCREEN_COMMON__
#define __R2K_SCENE_SCREEN_COMMON__
#include "DebugingCode.h"

#define SCR_POS_X(posx) (posx)
#define SCR_POS_Y(posy) (ScreenCommon::game_h - (posy))

#define GAME_SCREEN_WIDTH 320
#define GAME_SCREEN_HEIGHT 240

#define GAME_SCREEN_TILE_WIDTH 20
#define GAME_SCREEN_TILE_HEIGHT 15

#define FRAME_PER_SEC 60

enum GameVersion{
	RPG2000 = 1,
	RPG2003 = 2
};

enum ScreenZorder{
	sz_background		= 100,
	sz_chipset_lower_bot= 1000,
	sz_chipset_upper_bot= 1500,
	sz_charset_bot		= 2000,
	sz_charset_mid		= 2500,
	sz_chipset_lower_top= 3000,
	sz_chipset_upper_top= 3500,
	sz_charset_top		= 4000,
	sz_animation		= 4500,
	sz_weather			= 5000,
	sz_picture			= 5500,
	sz_timer			= 5700,
	sz_msgbox			= 6000,
	sz_controler		= 10000
};

enum ScrMenuType{
	scr_menu_main,
	scr_menu_equip,
	scr_menu_item,
	scr_menu_order,
	scr_menu_quit,
	scr_menu_skill,
	scr_menu_state,
	scr_menu_use,
	scr_menu_tel
};

enum ScreenType{
	scr_app_main	= 0,
	scr_battle		= 1,
	scr_gameover	= 2,
	scr_menu		= 3,
	scr_save		= 4,
	scr_map			= 5,
	scr_shop		= 6,
	scr_teleport	= 7,
	scr_title		= 8,
	scr_nameinput	= 9
};

class ScreenCommon{
public:
	static bool game_is_vertical;
	static int game_x;
	static int game_y;
	static float game_s;

	static int game_x_init;
	static int game_y_init;

	static int game_w;
	static int game_h;

	static int app_w;
	static int app_h;

	static float game_screen_x;
	static float game_screen_y;
	
	static float game_screen_x_for_event;

	static float button_global_scale;

	static GameVersion game_ver;
	static void setAppScreen( int w, int h );
};

#endif




