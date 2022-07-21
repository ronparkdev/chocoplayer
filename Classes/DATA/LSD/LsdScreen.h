#ifndef __R2K_DATA__LSD_SCREEN__
#define __R2K_DATA__LSD_SCREEN__

#include <iostream>
#include "../BaseStruct.h"

class LsdScreen : public BaseStruct
{
public: 
	/*01*/int scr_fin_r;
	/*02*/int scr_fin_g;
	/*03*/int scr_fin_b;
	/*04*/int scr_fin_gr;
	/*0B*/double scr_cur_r;
	/*0C*/double scr_cur_g;
	/*0D*/double scr_cur_b;
	/*0E*/double scr_cur_gr;
	/*0F*/int scr_timeleft;

	/*14*/int flash_continuous;
	/*15*/int flash_fin_r;
	/*16*/int flash_fin_g;
	/*17*/int flash_fin_b;
	/*18*/double flash_cur_lev;
	/*19*/int flash_frame_left;
	
	/*1E*/int shake_continuous;
	/*1F*/int shake_power;
	/*20*/int shake_speed;
	/*21*/int shake_pos;
	/*22*/int shake_what22;
	/*23*/int shake_timeleft;

	/*29*/int pan_x;
	/*2A*/int pan_y;

	/*2B*/int ani_id;
	/*2C*/int ani_target;
	/*2D*/int ani_curframe;
	/*2E*/int ani_frameleft;//TODO need check
	/*2F*/int ani_allscreen;
	
	/*30*/int weather_type;
	/*31*/int weather_power;
	
	/*40*/int screen_adjust_x;
	/*41*/int screen_adjust_y;

	float pan_move_x;
	float pan_move_y;
	float pan_moved_x;
	float pan_moved_y;
	int pan_move_speed;
public:
	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeInit) {
			pan_move_x = pan_move_y = pan_moved_x = pan_moved_y = 0.0f;
			pan_move_speed = 1;
		}

		BaseStruct::serialize (sl, 0x01, scr_fin_r	,100);
		BaseStruct::serialize (sl, 0x02, scr_fin_g	,100);
		BaseStruct::serialize (sl, 0x03, scr_fin_b	,100);
		BaseStruct::serialize (sl, 0x04, scr_fin_gr	,100);

		BaseStruct::serialize (sl, 0x0B, scr_cur_r	,(double)100.0f);
		BaseStruct::serialize (sl, 0x0C, scr_cur_g	,(double)100.0f);
		BaseStruct::serialize (sl, 0x0D, scr_cur_b	,(double)100.0f);
		BaseStruct::serialize (sl, 0x0E, scr_cur_gr	,(double)100.0f);

		BaseStruct::serialize (sl, 0x0F, scr_timeleft	,0);


		BaseStruct::serialize (sl, 0x14, flash_continuous	,0);
		BaseStruct::serialize (sl, 0x15, flash_fin_r		,31);
		BaseStruct::serialize (sl, 0x16, flash_fin_g		,31);
		BaseStruct::serialize (sl, 0x17, flash_fin_b		,31);
		BaseStruct::serialize (sl, 0x18, flash_cur_lev	,(double)0.0f);
		BaseStruct::serialize (sl, 0x19, flash_frame_left	,0);


		BaseStruct::serialize (sl, 0x1E, shake_continuous	,0);
		BaseStruct::serialize (sl, 0x1F, shake_power		,0);
		BaseStruct::serialize (sl, 0x20, shake_speed		,0);
		BaseStruct::serialize (sl, 0x21, shake_pos		,0);
		BaseStruct::serialize (sl, 0x22, shake_what22		,0);
		BaseStruct::serialize (sl, 0x23, shake_timeleft	,0);


		BaseStruct::serialize (sl, 0x29, pan_x		,0);
		BaseStruct::serialize (sl, 0x2A, pan_y		,0);


		BaseStruct::serialize (sl, 0x2B, ani_id		,0);
		BaseStruct::serialize (sl, 0x2C, ani_target	,0);
		BaseStruct::serialize (sl, 0x2D, ani_curframe	,0);
		BaseStruct::serialize (sl, 0x2E, ani_frameleft	,0);
		BaseStruct::serialize (sl, 0x2F, ani_allscreen,0);


		BaseStruct::serialize (sl, 0x30, weather_type	,0);
		BaseStruct::serialize (sl, 0x31, weather_power,0);
	
		BaseStruct::serialize (sl, 0x40, screen_adjust_x	,0);
		BaseStruct::serialize (sl, 0x41, screen_adjust_y	,0);

		sl.finishStruct();
	}

};

#endif
