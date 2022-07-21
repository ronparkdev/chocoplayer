#ifndef __R2K_DATA__LSD_PICTURE__
#define __R2K_DATA__LSD_PICTURE__

#include <iostream>
#include "../BaseStruct.h"

#define PICTURE_MAX 50

class LsdPicture : public BaseStruct
{
public: 
	bool need_picname_check; //for game

	/*01*/TString picName;

	/*02*/double staX;
	/*03*/double staY;
	/*04*/double curX;
	/*05*/double curY;
	/*1F*/double finX;
	/*20*/double finY;

	/*06*/bool is_fixed_to_map;
	/*09*/bool is_use_backcolor;

	/*07*/double curS;
	/*21*/int finS;

	/*08*/double curTA;
	/*22*/int finTA;

	/*12*/double curBA;
	/*23*/int finBA;

	/*0B*/double curR;
	/*0C*/double curG;
	/*0D*/double curB;
	/*0E*/double curGr;

	/*29*/int finR;
	/*2A*/int finG;
	/*2B*/int finB;
	/*2C*/int finGr;

	/*0F*/int effect_type;
	/*10*/int effect_rotate_speed;
	/*2E*/int effect_wave_speed;

	/*33*/int frame_left;
	/*34*/double effect_cur_rotate_speed;
	/*35*/double effect_cur_wave_speed;

public:
	void serialize(StructLoader &sl) {
		BaseStruct::serialize (sl, 0x01, picName			,"");
		BaseStruct::serialize (sl, 0x02, staX				,(double)160.0f);
		BaseStruct::serialize (sl, 0x03, staY				,(double)160.0f);
		BaseStruct::serialize (sl, 0x04, curX				,(double)160.0f);
		BaseStruct::serialize (sl, 0x05, curY				,(double)160.0f);
		BaseStruct::serialize (sl, 0x06, is_fixed_to_map	,false);
		BaseStruct::serialize (sl, 0x07, curS				,(double)100.0f);
		BaseStruct::serialize (sl, 0x08, curTA			,(double)0.0f);
		BaseStruct::serialize (sl, 0x09, is_use_backcolor	,false);
		BaseStruct::serialize (sl, 0x0B, curR				,(double)100.0f);
		BaseStruct::serialize (sl, 0x0C, curG				,(double)100.0f);
		BaseStruct::serialize (sl, 0x0D, curB				,(double)100.0f);
		BaseStruct::serialize (sl, 0x0E, curGr			,(double)100.0f);
		BaseStruct::serialize (sl, 0x0F, effect_type		,0);
		BaseStruct::serialize (sl, 0x10, effect_rotate_speed,5);//TODO CHK
		BaseStruct::serialize (sl, 0x12, curBA			,(double)0.0f);
		BaseStruct::serialize (sl, 0x1F, finX				,(double)160.0f);
		BaseStruct::serialize (sl, 0x20, finY				,(double)160.0f);
		BaseStruct::serialize (sl, 0x21, finS				,100);
		BaseStruct::serialize (sl, 0x22, finTA			,0);
		BaseStruct::serialize (sl, 0x23, finBA			,0);
		BaseStruct::serialize (sl, 0x29, finR				,100);
		BaseStruct::serialize (sl, 0x2A, finG				,100);
		BaseStruct::serialize (sl, 0x2B, finB				,100);
		BaseStruct::serialize (sl, 0x2C, finGr			,100);
		BaseStruct::serialize (sl, 0x2E, effect_wave_speed,5);//TODO CHK
		BaseStruct::serialize (sl, 0x33, frame_left		,0);
		BaseStruct::serialize (sl, 0x34, effect_cur_rotate_speed	,(double)0.0f);
		BaseStruct::serialize (sl, 0x35, effect_cur_wave_speed	,(double)0.0f);

		sl.finishStruct();
	}

};

#endif
