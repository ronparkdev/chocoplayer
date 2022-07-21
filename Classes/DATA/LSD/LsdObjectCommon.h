#ifndef __R2K_DATA__LSD_LOCATION_COMMON__
#define __R2K_DATA__LSD_LOCATION_COMMON__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbMoveRoute.h"
#include "../LMU/LmuMain.h"

const int ObjectDirectX[] = {0, 0, 1, 0,-1, 1, 1,-1,-1};
const int ObjectDirectY[] = {0,-1, 0, 1, 0,-1, 1, 1,-1};

enum ObjectAniType{
	oanitype_none			= 0,
	oanitype_walking		= 1,
	oanitype_dlock_none		= 2,
	oanitype_dlock_walking	= 3,
	oanitype_lock			= 4,
	oanitype_rotate_clock	= 5
};

class LsdObjectCommon : public BaseStruct
{
public: 
	bool need_picname_check; //for game

	/*xx*/int screen_pos_x;
	/*xx*/int screen_pos_y;

	/*01*/bool isUsed;
	/*0B*/int map_id;
	
	/*0C*/int posX;
	/*0D*/int posY;
	
	/*15*/int direction_moving;	//실제 움직이는 방향
	/*16*/int direction;		//화면상으로 보이는 방향
	
	/*17*/int ani_walk;	//1,3 Mid 2Left 0Right
	/*18*/int transparent;//0~7 0은 투명하지않음
	
	/*1F*/int move_frame_left;

	/*20*/int move_freq;
	/*21*/int draw_priority;
	/*22*/bool draw_dontover;
	/*23*/int ani_type;
	/*24*/bool direction_lock;
	/*25*/int move_speed;
	
	/*29*/GlbMoveRoute move_route;
	/*2A*/bool move_route_custom;
	/*2B*/int move_route_index;
	/*2C*/bool move_route_repeated;
	
	/*2E*/bool invisible;
	/*2F*/bool nowall;
	/*30*/bool ani_paused;
	/*33*/bool no_match_condition;
	
	/*34*/int wait_cur_frame;
	/*35*/int ani_next_left_frame;
	/*36*/int wait_total_frame;
	
	/*3D*/bool jumping;
	/*3E*/int jumping_from_x;
	/*3F*/int jumping_from_y;

	/*46*/int serial_event_page;
	/*47*/bool serial_event_queue;

	/*48*/bool flying;
	
	/*49*/TString pic_name;
	/*4A*/int pic_index;
	
	/*4B*/int flash_continuous;
	/*51*/int flash_fin_r;
	/*52*/int flash_fin_g;
	/*53*/int flash_fin_b;
	/*54*/double flash_cur_lev;
	/*55*/int flash_frame_left;
	
	/*65*/int ride_type;
	/*66*/int move_route_original_index;
public:
	void serialize(StructLoader &sl) {
		BaseStruct::serialize (sl, 0x01, isUsed				,true);
		BaseStruct::serialize (sl, 0x0B, map_id);
		BaseStruct::serialize (sl, 0x0C, posX);
		BaseStruct::serialize (sl, 0x0D, posY);
		BaseStruct::serialize (sl, 0x15, direction_moving		,0);
		BaseStruct::serialize (sl, 0x16, direction			,0);
		BaseStruct::serialize (sl, 0x17, ani_walk				,1);
		BaseStruct::serialize (sl, 0x18, transparent			,0);
		BaseStruct::serialize (sl, 0x1F, move_frame_left		,0);
		BaseStruct::serialize (sl, 0x20, move_freq			,1);
		BaseStruct::serialize (sl, 0x21, draw_priority		,(int)drawpri_low);
		BaseStruct::serialize (sl, 0x22, draw_dontover		,false);
		BaseStruct::serialize (sl, 0x23, ani_type				,(int)oanitype_walking);//0?
		BaseStruct::serialize (sl, 0x24, direction_lock		,false);
		BaseStruct::serialize (sl, 0x25, move_speed			,0);//init is 3
		BaseStruct::serialize (sl, 0x29, &move_route);
		BaseStruct::serialize (sl, 0x2A, move_route_custom	,false);
		BaseStruct::serialize (sl, 0x2B, move_route_index		,0);
		BaseStruct::serialize (sl, 0x2C, move_route_repeated	,false);
		BaseStruct::serialize (sl, 0x2E, invisible			,false);
		BaseStruct::serialize (sl, 0x2F, nowall				,false);
		BaseStruct::serialize (sl, 0x30, ani_paused			,false);
		BaseStruct::serialize (sl, 0x33, no_match_condition	,false);

		BaseStruct::serialize (sl, 0x34, wait_cur_frame		,0);
		BaseStruct::serialize (sl, 0x35, ani_next_left_frame	,0);
		BaseStruct::serialize (sl, 0x36, wait_total_frame		,0);

		BaseStruct::serialize (sl, 0x3D, jumping				,false);
		BaseStruct::serialize (sl, 0x3E, jumping_from_x		,0);
		BaseStruct::serialize (sl, 0x3F, jumping_from_y		,0);

		BaseStruct::serialize	(sl, 0x46, serial_event_page	,0);
		BaseStruct::serialize (sl, 0x47, serial_event_queue	,false);

		BaseStruct::serialize (sl, 0x48, flying				,false);
		BaseStruct::serialize (sl, 0x49, pic_name				,"");
		BaseStruct::serialize (sl, 0x4A, pic_index			,0);
		BaseStruct::serialize (sl, 0x4B, flash_continuous		,0);
		BaseStruct::serialize (sl, 0x51, flash_fin_r			,31);
		BaseStruct::serialize (sl, 0x52, flash_fin_g			,31);
		BaseStruct::serialize (sl, 0x53, flash_fin_b			,31);
		BaseStruct::serialize (sl, 0x54, flash_cur_lev		,(double)0.0f);
		BaseStruct::serialize (sl, 0x55, flash_frame_left		,0);
		BaseStruct::serialize (sl, 0x65, ride_type			,0);//1:boat 2:ship 3:fly
		BaseStruct::serialize (sl, 0x66, move_route_original_index	,0);
	}
};

#endif 
