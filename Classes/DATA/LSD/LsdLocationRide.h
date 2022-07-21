#ifndef __R2K_DATA__LSD_LOCATION_RIDE__
#define __R2K_DATA__LSD_LOCATION_RIDE__

#include <iostream>
#include "../BaseStruct.h"
#include "LsdObjectCommon.h"

enum Ride_Type{
	ride_boat	= 0,
	ride_ship	= 1,
	ride_airship	= 2
};

class LsdLocationRide : public LsdObjectCommon
{
public: 
	/*6A*/int what6A;
	/*6F*/TString sprite2_name;
	/*70*/int sprite2_id;

public:
	void newgame( int i, LdbMain &ldb, LmtMain &lmt ) 
	{
		direction_moving = direction = direct_down;

		switch(i) {
		case ride_boat:
			{
				pic_name	= ldb.dSys.boat_name;
				pic_index	= ldb.dSys.boat_index;
				map_id		= lmt.Info.boat_map_id;
				posX		= lmt.Info.boat_x;
				posY		= lmt.Info.boat_y;
				break;
			}
		case ride_ship:
			{
				pic_name	= ldb.dSys.ship_name;
				pic_index	= ldb.dSys.ship_index;
				map_id		= lmt.Info.ship_map_id;
				posX		= lmt.Info.ship_x;
				posY		= lmt.Info.ship_y;
				break;
			}
		case ride_airship:
			{
				pic_name	= ldb.dSys.airship_name;
				pic_index		= ldb.dSys.airship_index;
				map_id		= lmt.Info.airship_map_id;
				posX		= lmt.Info.airship_x;
				posY		= lmt.Info.airship_y;
				break;
			}
		}

		ani_type = oanitype_none;
		draw_priority = drawpri_mid;
		move_speed = 5;
		move_freq = 8;
	}

	void serialize(StructLoader &sl) {
		LsdObjectCommon::serialize(sl);

		BaseStruct::serialize (sl, 0x6A, what6A		,0);
		BaseStruct::serialize (sl, 0x6F, sprite2_name	,"");
		BaseStruct::serialize (sl, 0x70, sprite2_id	,0);
		sl.finishStruct();
	}
};

#endif
