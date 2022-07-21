#ifndef __R2K_BATTLE_BACKGROUND_FLAGS__
#define __R2K_BATTLE_BACKGROUND_FLAGS__

#include "../../DATA/LDB/LdbTerrain.h"

class ScrBattleBgInfo{
public:
	bool use_ab;

	TString bg;

	TString	bg_a;
	bool	bg_a_h;
	int		bg_a_h_speed;
	bool	bg_a_v;
	int		bg_a_v_speed;

	TString	bg_b;
	bool	bg_b_h;
	int		bg_b_h_speed;
	bool	bg_b_v;
	int		bg_b_v_speed;


	void set(const LdbTerrain &terr) {
		use_ab = (terr.background_type == 1);

		bg = terr.background_name;

		bg_a			= terr.background_a_name;
		bg_a_h			= terr.background_a_scrollh;
		bg_a_h_speed	= terr.background_a_scrollh_speed;
		bg_a_v			= terr.background_a_scrollv;
		bg_a_v_speed	= terr.background_a_scrollv_speed;

		bg_b			= terr.background_b_name;
		bg_b_h			= terr.background_b_scrollh;
		bg_b_h_speed	= terr.background_b_scrollh_speed;
		bg_b_v			= terr.background_b_scrollv;
		bg_b_v_speed	= terr.background_b_scrollv_speed;
	}

	void set(const TString &background_name) {
		use_ab = false;
		bg = background_name;
	}
};

#endif