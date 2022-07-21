#include "LsdChk.h"

#define LSD_COMPARE(S1, S2, PARAMETER) if (S1.PARAMETER != S2.PARAMETER) CCLOG((_ST("") + "NotMatching : Evt " + index + ":" + #PARAMETER + " (" + S1.PARAMETER + "->" + S2.PARAMETER + ")").getTextUTF8());

void LsdChk::chkHeroObject( LsdLocationHero &s1, LsdLocationHero &s2 )
{
	int index = -1;

	chkObject(index, s1, s2);

	LSD_COMPARE(s1, s2, encounter_steps);
	LSD_COMPARE(s1, s2, map_save_count);
	LSD_COMPARE(s1, s2, pan_curX);
	LSD_COMPARE(s1, s2, pan_curY);
	LSD_COMPARE(s1, s2, pan_finX);
	LSD_COMPARE(s1, s2, pan_finY);
}

void LsdChk::chkRideObject( int index, LsdLocationRide &s1, LsdLocationRide &s2 )
{
	index = -1 - index;

	chkObject(index, s1, s2);

	LSD_COMPARE(s1, s2, sprite2_id);
	LSD_COMPARE(s1, s2, sprite2_name);
}

void LsdChk::chkMapObject( int index, LsdMapEvent &s1, LsdMapEvent &s2 )
{
	chkObject(index, s1, s2);

	LSD_COMPARE(s1, s2, direction_event);
	LSD_COMPARE(s1, s2, evtpage);
}

void LsdChk::chkObject( int index, LsdObjectCommon &s1, LsdObjectCommon &s2 )
{
	LSD_COMPARE(s1, s2, ani_next_left_frame);
	LSD_COMPARE(s1, s2, ani_paused);
	LSD_COMPARE(s1, s2, ani_type);
	LSD_COMPARE(s1, s2, ani_walk);
	LSD_COMPARE(s1, s2, direction);
	LSD_COMPARE(s1, s2, direction_lock);
	LSD_COMPARE(s1, s2, direction_moving);
	LSD_COMPARE(s1, s2, draw_dontover);
	LSD_COMPARE(s1, s2, draw_priority);
	LSD_COMPARE(s1, s2, flash_continuous);
	LSD_COMPARE(s1, s2, flash_cur_lev);
	LSD_COMPARE(s1, s2, flash_fin_r);
	LSD_COMPARE(s1, s2, flash_fin_g);
	LSD_COMPARE(s1, s2, flash_fin_b);
	LSD_COMPARE(s1, s2, flash_frame_left);
	LSD_COMPARE(s1, s2, flying);
	LSD_COMPARE(s1, s2, jumping_from_x);
	LSD_COMPARE(s1, s2, jumping_from_y);
	LSD_COMPARE(s1, s2, map_id);
	LSD_COMPARE(s1, s2, move_frame_left);
	LSD_COMPARE(s1, s2, move_freq);
	LSD_COMPARE(s1, s2, move_route_custom);
	LSD_COMPARE(s1, s2, move_route_index);
	LSD_COMPARE(s1, s2, move_route_original_index);
	LSD_COMPARE(s1, s2, move_speed);
	LSD_COMPARE(s1, s2, pic_name);
	LSD_COMPARE(s1, s2, pic_index);
	LSD_COMPARE(s1, s2, posX);
	LSD_COMPARE(s1, s2, posY);
	LSD_COMPARE(s1, s2, ride_type);
	LSD_COMPARE(s1, s2, transparent);
	LSD_COMPARE(s1, s2, posX);

	if (s1.getIndex() != s2.getIndex())
		CCLOG("NotMatching : Evt %d :index %d => %d", index, s1.getIndex(), s2.getIndex());
}
