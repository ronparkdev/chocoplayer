#ifndef __R2K_DATA__LDB_TERRAIN__
#define __R2K_DATA__LDB_TERRAIN__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbSound.h"

class LdbTerrain : public BaseStruct
{
public: 
	/*01*/TString name;
	/*02*/int	damage;
	/*03*/int	encounter_rate;
	/*04*/TString background_name;
	/*05*/bool	boat_pass;
	/*06*/bool	ship_pass;
	/*07*/bool	airship_pass;
	/*09*/bool	airship_land;
	/*0B*/int	bush_depth;
	/*0F*/GlbSound footstep;
	/*10*/bool	on_damage_se;
	/*11*/int	background_type;
	/*15*/TString background_a_name;
	/*16*/bool	background_a_scrollh;
	/*17*/bool	background_a_scrollv;
	/*18*/int	background_a_scrollh_speed;
	/*19*/int	background_a_scrollv_speed;
	/*1E*/bool	background_b;
	/*1F*/TString background_b_name;
	/*20*/bool	background_b_scrollh;
	/*21*/bool	background_b_scrollv;
	/*22*/int	background_b_scrollh_speed;
	/*23*/int	background_b_scrollv_speed;
	/*28*/int	special_flags;
	/*29*/int	special_back_party;
	/*2A*/int	special_back_enemies;
	/*2B*/int	special_lateral_party;
	/*2C*/int	special_lateral_enemies;
	/*2D*/int	grid_location;
	/*2E*/int	grid_a;
	/*2F*/int	grid_b;
	/*30*/int	grid_c;

public:
	void init() {
		/*01*/name				= "";
		/*02*/damage			= 0;
		/*03*/encounter_rate	= 100;
		/*04*/background_name	= "";
		/*05*/boat_pass			= false;
		/*06*/ship_pass			= false;
		/*07*/airship_pass		= true;
		/*09*/airship_land		= true;
		/*0B*/bush_depth		= 0;//TODO CHK
		/*10*/background_type	= 0;//TODO CHK
		/*28*/special_flags		= 0;//TODO CHK
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, damage);
		BaseStruct::serialize (sl, 0x03, encounter_rate);
		BaseStruct::serialize (sl, 0x04, background_name);
		BaseStruct::serialize (sl, 0x05, boat_pass);
		BaseStruct::serialize (sl, 0x06, ship_pass);
		BaseStruct::serialize (sl, 0x07, airship_pass);
		BaseStruct::serialize (sl, 0x09, airship_land);
		BaseStruct::serialize (sl, 0x0B, bush_depth);
		BaseStruct::serialize (sl, 0x0F, footstep);
		BaseStruct::serialize (sl, 0x10, on_damage_se);
		BaseStruct::serialize (sl, 0x11, background_type);
		BaseStruct::serialize (sl, 0x15, background_a_name);
		BaseStruct::serialize (sl, 0x16, background_a_scrollh);
		BaseStruct::serialize (sl, 0x17, background_b_scrollv);
		BaseStruct::serialize (sl, 0x18, background_a_scrollh_speed);
		BaseStruct::serialize (sl, 0x19, background_b_scrollv_speed);
		BaseStruct::serialize (sl, 0x1E, background_b);
		BaseStruct::serialize (sl, 0x1F, background_b_name);
		BaseStruct::serialize (sl, 0x20, background_b_scrollh);
		BaseStruct::serialize (sl, 0x21, background_b_scrollv);
		BaseStruct::serialize (sl, 0x22, background_b_scrollh_speed);
		BaseStruct::serialize (sl, 0x23, background_b_scrollv_speed);
		BaseStruct::serialize (sl, 0x28, special_flags);
		BaseStruct::serialize (sl, 0x29, special_back_party);
		BaseStruct::serialize (sl, 0x2A, special_back_enemies);
		BaseStruct::serialize (sl, 0x2B, special_lateral_party);
		BaseStruct::serialize (sl, 0x2C, special_lateral_enemies);
		BaseStruct::serialize (sl, 0x2D, grid_location);
		BaseStruct::serialize (sl, 0x2E, grid_a);
		BaseStruct::serialize (sl, 0x2F, grid_b);
		BaseStruct::serialize (sl, 0x30, grid_c);
		sl.finishStruct();
	}
};

#endif
