#ifndef __R2K_DATA__LDB_SYSTEM__
#define __R2K_DATA__LDB_SYSTEM__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbSound.h"
#include "../GLB/GlbMusic.h"
#include "../GLB/GlbTestBattler.h"

class LdbSystem : public BaseStruct
{
public: 
	/*0A*/int	ldb_id;
	/*0B*/TString boat_name;
	/*0C*/TString ship_name;
	/*0D*/TString airship_name;
	/*0E*/int	boat_index;
	/*0F*/int	ship_index;
	/*10*/int	airship_index;
	/*11*/TString title_name;
	/*12*/TString gameover_name;
	/*13*/TString system_name;
	/*14*/TString system2_name;
	/*15*/int	party_count;
	/*16*/ArrayList<short> party;
	/*1A*/int	menu_commands_count;
	/*1B*/ArrayList<short> menu_commands;

	/*1F*/GlbMusic title_music;
	/*20*/GlbMusic battle_music;
	/*21*/GlbMusic battle_end_music;
	/*22*/GlbMusic inn_music;
	/*23*/GlbMusic boat_music;
	/*24*/GlbMusic ship_music;
	/*25*/GlbMusic airship_music;
	/*26*/GlbMusic gameover_music;

	/*29*/GlbSound cursor_se;
	/*2A*/GlbSound decision_se;
	/*2B*/GlbSound cancel_se;
	/*2C*/GlbSound buzzer_se;
	/*2D*/GlbSound battle_se;
	/*2E*/GlbSound escape_se;
	/*2F*/GlbSound enemy_attack_se;
	/*30*/GlbSound enemy_damaged_se;
	/*31*/GlbSound actor_damaged_se;
	/*32*/GlbSound dodge_se;
	/*33*/GlbSound enemy_death_se;
	/*34*/GlbSound item_se;

	/*3D*/byte	transition_out;
	/*3E*/byte	transition_in;
	/*3F*/byte	battle_start_fadeout;
	/*40*/byte	battle_start_fadein;
	/*41*/byte	battle_end_fadeout;
	/*42*/byte	battle_end_fadein;
	/*47*/int	message_stretch;
	/*48*/int	font_id;
	/*51*/int	selected_condition;
	/*52*/int	selected_hero;
	/*54*/TString battletest_background;
	/*55*/ArrayList<GlbTestBattler> battletest_data;
	/*5B*/int	saved_times;
	/*5E*/int	battletest_terrain;
	/*5F*/int	battletest_formation;
	/*60*/int	battletest_condition;
	/*61*/bool	item_limit_by_job;
	/*63*/bool	show_frame;
	/*64*/TString frame_name;
	/*65*/bool	invert_animations;

public:
	void init() {
	/*0A*/ldb_id		= 0;
	/*0B*/boat_name		= "";
	/*0C*/ship_name		= "";
	/*0D*/airship_name	= "";
	/*0E*/boat_index	= 0;
	/*0F*/ship_index	= 0;
	/*10*/airship_index	= 0;
	/*11*/title_name	= "";
	/*12*/gameover_name	= "";
	/*13*/system_name	= "";
	/*14*/system2_name	= "";
	/*15*/party_count	= 0;
	/*16*/party.RemoveAll();
	/*1A*/menu_commands_count = 0;
	/*1B*/menu_commands.RemoveAll();

	/*1F*/title_music.init();
	/*20*/battle_music.init();
	/*21*/battle_end_music.init();
	/*22*/inn_music.init();
	/*23*/boat_music.init();
	/*24*/ship_music.init();
	/*25*/airship_music.init();
	/*26*/gameover_music.init();

	/*29*/cursor_se.init();
	/*2A*/decision_se.init();
	/*2B*/cancel_se.init();
	/*2C*/buzzer_se.init();
	/*2D*/battle_se.init();
	/*2E*/escape_se.init();
	/*2F*/enemy_attack_se.init();
	/*30*/enemy_damaged_se.init();
	/*31*/actor_damaged_se.init();
	/*32*/dodge_se.init();
	/*33*/enemy_death_se.init();
	/*34*/item_se.init();

	/*3D*/transition_out		= 0;
	/*3E*/transition_in			= 0;
	/*3F*/battle_start_fadeout	= 0;
	/*40*/battle_start_fadein	= 0;
	/*41*/battle_end_fadeout	= 0;
	/*42*/battle_end_fadein		= 0;
	
	/*47*/message_stretch		= 0;
	
	/*48*/font_id				= 0;
	/*51*/selected_condition	= 0;
	/*52*/selected_hero			= 0;
	/*54*/battletest_background	= "";
	/*55*/battletest_data.RemoveAll();
	/*5B*/saved_times			= 0;
	/*5E*/battletest_terrain	= 0;
	/*5F*/battletest_formation	= 0;
	/*60*/battletest_condition	= 0;
	/*61*/item_limit_by_job		= 0;
	/*63*/show_frame			= 0;
	/*64*/frame_name			= "";
	/*65*/invert_animations		= 0;

	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x0A, ldb_id);
		BaseStruct::serialize (sl, 0x0B, boat_name);
		BaseStruct::serialize (sl, 0x0C, ship_name);
		BaseStruct::serialize (sl, 0x0D, airship_name);
		BaseStruct::serialize (sl, 0x0E, boat_index);
		BaseStruct::serialize (sl, 0x0F, ship_index);
		BaseStruct::serialize (sl, 0x10, airship_index);
		BaseStruct::serialize (sl, 0x11, title_name);
		BaseStruct::serialize (sl, 0x12, gameover_name);
		BaseStruct::serialize (sl, 0x13, system_name);
		BaseStruct::serialize (sl, 0x14, system2_name);
		BaseStruct::serialize (sl, 0x15, party_count);
		BaseStruct::serialize (sl, 0x16, party);
		BaseStruct::serialize (sl, 0x1A, menu_commands_count);
		BaseStruct::serialize (sl, 0x1B, menu_commands);
		BaseStruct::serialize (sl, 0x1F, &title_music);
		BaseStruct::serialize (sl, 0x20, &battle_music);
		BaseStruct::serialize (sl, 0x21, &battle_end_music);
		BaseStruct::serialize (sl, 0x22, &inn_music);
		BaseStruct::serialize (sl, 0x23, &boat_music);
		BaseStruct::serialize (sl, 0x24, &ship_music);
		BaseStruct::serialize (sl, 0x25, &airship_music);
		BaseStruct::serialize (sl, 0x26, &gameover_music);
		BaseStruct::serialize (sl, 0x29, &cursor_se);
		BaseStruct::serialize (sl, 0x2A, &decision_se);
		BaseStruct::serialize (sl, 0x2B, &cancel_se);
		BaseStruct::serialize (sl, 0x2C, &buzzer_se);
		BaseStruct::serialize (sl, 0x2D, &battle_se);
		BaseStruct::serialize (sl, 0x2E, &escape_se);
		BaseStruct::serialize (sl, 0x2F, &enemy_attack_se);
		BaseStruct::serialize (sl, 0x30, &enemy_damaged_se);
		BaseStruct::serialize (sl, 0x31, &actor_damaged_se);
		BaseStruct::serialize (sl, 0x32, &dodge_se);
		BaseStruct::serialize (sl, 0x33, &enemy_death_se);
		BaseStruct::serialize (sl, 0x34, &item_se);
		BaseStruct::serialize (sl, 0x3D, transition_out);
		BaseStruct::serialize (sl, 0x3E, transition_in);
		BaseStruct::serialize (sl, 0x3F, battle_start_fadeout);
		BaseStruct::serialize (sl, 0x40, battle_start_fadein);
		BaseStruct::serialize (sl, 0x41, battle_end_fadeout);
		BaseStruct::serialize (sl, 0x42, battle_start_fadein);
		BaseStruct::serialize (sl, 0x47, message_stretch);
		BaseStruct::serialize (sl, 0x48, font_id);
		BaseStruct::serialize (sl, 0x51, selected_condition);
		BaseStruct::serialize (sl, 0x52, selected_hero);
		BaseStruct::serialize (sl, 0x54, battletest_background);
		BaseStruct::serialize(sl, 0x55, battletest_data);
		BaseStruct::serialize (sl, 0x5B, saved_times);
		BaseStruct::serialize (sl, 0x5E, battletest_terrain);
		BaseStruct::serialize (sl, 0x5F, battletest_formation);
		BaseStruct::serialize (sl, 0x60, battletest_condition);
		BaseStruct::serialize (sl, 0x61, item_limit_by_job);
		BaseStruct::serialize (sl, 0x63, show_frame);
		BaseStruct::serialize (sl, 0x64, frame_name);
		BaseStruct::serialize (sl, 0x65, invert_animations);
		sl.finishStruct();
	}
};

#endif
