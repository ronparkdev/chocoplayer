#ifndef __R2K_DATA__LSD_LOCATION_HERO__
#define __R2K_DATA__LSD_LOCATION_HERO__

#include <iostream>
#include "../BaseStruct.h"
#include "../LMT/LmtMain.h"
#include "LsdObjectCommon.h"

class LsdLocationHero : public LsdObjectCommon
{
public: 
	/*67*/int riding_vehicle;
	/*68*/int what68;
	/*69*/int what69;
	/*6A*/int what6A;
	/*6C*/int what6C;
	/*6F*/int pan_state;
	/*70*/int pan_curX;
	/*71*/int pan_curY;
	/*72*/int pan_finX;
	/*73*/int pan_finY;
	/*79*/int what79;
	/*7C*/int encounter_steps;		//걸을때마다 계산
	/*7D*/int what7D;
	/*83*/int map_save_count;		//LsdSystem과 맞지않으면 오작동
	/*84*/int what84;

public:
	void newgame( LdbMain &ldb, LmtMain &lmt ) 
	{
		ArrayList<short> &party = ldb.dSys.party;

		if (party.GetLength() > 0) {
			int hero_id = ldb.dSys.party[0] - 1;

			LdbCharacter &hero = ldb.dChar[hero_id];

			pic_name	= hero.pic_name;
			pic_index	= hero.pic_id;
		}

		map_id	= lmt.Info.party_map_id;
		posX	= lmt.Info.party_x;
		posY	= lmt.Info.party_y;

		direction_moving = direction = direct_down;
		ani_type = oanitype_none;
		draw_priority = drawpri_mid;
		move_speed = 4;
		move_freq = 8;
	}

	void serialize(StructLoader &sl) {
		LsdObjectCommon::serialize(sl);

		BaseStruct::serialize (sl, 0x67, riding_vehicle	,0);
		BaseStruct::serialize (sl, 0x68, what68			,0);
		BaseStruct::serialize (sl, 0x69, what69			,0);
		BaseStruct::serialize (sl, 0x6C, what6C			,0);
		BaseStruct::serialize (sl, 0x6F, pan_state		,0);
		BaseStruct::serialize (sl, 0x70, pan_curX			,0);
		BaseStruct::serialize (sl, 0x71, pan_curY			,0);
		BaseStruct::serialize (sl, 0x72, pan_finX			,0);
		BaseStruct::serialize (sl, 0x73, pan_finY			,0);
		BaseStruct::serialize (sl, 0x79, what79			,0);
		BaseStruct::serialize (sl, 0x7C, encounter_steps	,0);
		BaseStruct::serialize (sl, 0x7D, what7D			,0);
		BaseStruct::serialize (sl, 0x83, map_save_count	,0);
		BaseStruct::serialize (sl, 0x84, what84			,0);

		sl.finishStruct();
	}
};

#endif
