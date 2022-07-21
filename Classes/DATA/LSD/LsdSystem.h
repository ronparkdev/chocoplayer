#ifndef __R2K_DATA__LSD_SYSTEM__
#define __R2K_DATA__LSD_SYSTEM__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbMusic.h"
#include "../GLB/GlbSound.h"
#include "../LDB/LdbMain.h"

class LsdSystem : public BaseStruct
{
public: 
	LdbSystem * m_link;

	/*01*/int		screen;
	/*0B*/long		frame_count;

	/*15*/TString	system_name;
	
	/*1F*/int		switch_length;
	/*20*/ArrayList<bool> switches;

	/*21*/int		variable_length;
	/*22*/ArrayList<int> variables;

	/*29*/bool		msg_visiable;
	/*2A*/int		msg_pos;
	/*2B*/bool		msg_automove;
	/*2C*/bool		msg_moveevent;

	/*33*/TString	face_name;
	/*34*/int		face_index;
	/*35*/bool		face_right;
	/*36*/bool		face_flip;

	/*37*/int		what_37;
	/*3D*/int		what_3D;

	/*47*/GlbMusic music_title;
	/*48*/GlbMusic music_battle;
	/*49*/GlbMusic music_battle_end;
	/*4A*/GlbMusic music_inn;
	
	/*4B*/GlbMusic music_current;
	/*4C*/GlbMusic music_before_vehicle;
	/*4D*/GlbMusic music_before_battle;
	/*4E*/GlbMusic music_saved;

	/*4F*/GlbMusic music_boat;
	/*50*/GlbMusic music_ship;
	/*51*/GlbMusic music_airship;
	/*52*/GlbMusic music_gameover;

	/*5B*/GlbSound se_cursor;
	/*5C*/GlbSound se_decision;
	/*5D*/GlbSound se_cancel;
	/*5E*/GlbSound se_buzzer;
	/*5F*/GlbSound se_battle;
	/*60*/GlbSound se_escape;
	/*61*/GlbSound se_enemy_attack;
	/*62*/GlbSound se_enemy_damaged;
	/*63*/GlbSound se_actor_damaged;
	/*64*/GlbSound se_dodge;
	/*65*/GlbSound se_enemy_death;
	/*66*/GlbSound se_item;

	/*6F*/byte transition_out;
	/*70*/byte transition_in;
	/*71*/byte battle_start_fadeout;
	/*72*/byte battle_start_fadein;
	/*73*/byte battle_end_fadeout;
	/*74*/byte battle_end_fadein;

	/*79*/bool able_teleport;
	/*7A*/bool able_escape;
	/*7B*/bool able_save;
	/*7C*/bool able_menu;

	/*82*/TString frame_name;//????
	/*83*/int save_count;
	/*84*/int save_slot;

	/*8C*/bool battle_action_wait;
public:
	void newgame( LdbMain &ldb ) 
	{
		LdbSystem &s = ldb.dSys;

		screen		= 5;
		frame_count   = 0;

		system_name		= s.system_name;

		switch_length	= ldb.dSw.GetLength();
		switches.RemoveAll();
		switches.SetLength(switch_length);
		for(int i=0; i<switch_length; i++)
			switches[i] = false;

		variable_length = ldb.dVar.GetLength();
		variables.RemoveAll();
		variables.SetLength(variable_length);
		for(int i=0; i<variable_length; i++)
			variables[i] = 0;

		music_title		= s.title_music;
		music_battle	= s.battle_music;
		music_battle_end= s.battle_end_music;
		music_inn		= s.inn_music;

		music_boat		= s.boat_music;
		music_ship		= s.ship_music;
		music_airship	= s.airship_music;
		music_gameover	= s.gameover_music;

		se_cursor		= s.cursor_se;
		se_decision		= s.decision_se;
		se_cancel		= s.cancel_se;
		se_buzzer		= s.buzzer_se;
		se_battle		= s.battle_se;
		se_escape		= s.escape_se;
		se_enemy_attack	= s.enemy_attack_se;
		se_enemy_damaged= s.enemy_damaged_se;
		se_actor_damaged= s.actor_damaged_se;
		se_dodge		= s.dodge_se;
		se_enemy_death	= s.enemy_death_se;
		se_item			= s.item_se;

		transition_out = s.transition_out;
		transition_in = s.transition_in;
		battle_start_fadeout = s.battle_start_fadeout;
		battle_start_fadein = s.battle_start_fadein;
		battle_end_fadeout = s.battle_end_fadeout;
		battle_end_fadein = s.battle_end_fadein;
	}

	void serialize(StructLoader &sl) {
		BaseStruct::serialize (sl, 0x01, screen			,0);
		BaseStruct::serialize (sl, 0x0B, frame_count		,0L);

		BaseStruct::serialize (sl, 0x15, system_name		,m_link->system_name);

		BaseStruct::serialize (sl, 0x1F, switch_length	,0);
		BaseStruct::serialize (sl, 0x20, switches);

		BaseStruct::serialize (sl, 0x21, variable_length	,0);
		BaseStruct::serialize (sl, 0x22, variables);
		
		BaseStruct::serialize (sl, 0x29, msg_visiable		,false);
		BaseStruct::serialize (sl, 0x2A, msg_pos			,2);
		BaseStruct::serialize (sl, 0x2B, msg_automove		,true);
		BaseStruct::serialize (sl, 0x2C, msg_moveevent	,false);

		BaseStruct::serialize (sl, 0x33, face_name		,"");
		BaseStruct::serialize (sl, 0x34, face_index		,0);
		BaseStruct::serialize (sl, 0x35, face_right		,false);
		BaseStruct::serialize (sl, 0x36, face_flip		,false);

		BaseStruct::serialize (sl, 0x37, what_37			,0);
		BaseStruct::serialize (sl, 0x3D, what_3D			,0);

		BaseStruct::serialize (sl, 0x47, &music_title);
		BaseStruct::serialize (sl, 0x48, &music_battle);
		BaseStruct::serialize (sl, 0x49, &music_battle_end);
		BaseStruct::serialize (sl, 0x4A, &music_inn);
 
		BaseStruct::serialize (sl, 0x4B, &music_current);
		BaseStruct::serialize (sl, 0x4C, &music_before_vehicle);
		BaseStruct::serialize (sl, 0x4D, &music_before_battle);
		BaseStruct::serialize (sl, 0x4E, &music_saved);

		BaseStruct::serialize (sl, 0x4F, &music_boat);
		BaseStruct::serialize (sl, 0x50, &music_ship);
		BaseStruct::serialize (sl, 0x51, &music_airship);
		BaseStruct::serialize (sl, 0x52, &music_gameover);

		BaseStruct::serialize (sl, 0x5B, &se_cursor);
		BaseStruct::serialize (sl, 0x5C, &se_decision);
		BaseStruct::serialize (sl, 0x5D, &se_cancel);
		BaseStruct::serialize (sl, 0x5E, &se_buzzer);
		BaseStruct::serialize (sl, 0x5F, &se_battle);
		BaseStruct::serialize (sl, 0x60, &se_escape);
		BaseStruct::serialize (sl, 0x61, &se_enemy_attack);
		BaseStruct::serialize (sl, 0x62, &se_enemy_damaged);
		BaseStruct::serialize (sl, 0x63, &se_actor_damaged);
		BaseStruct::serialize (sl, 0x64, &se_dodge);
		BaseStruct::serialize (sl, 0x65, &se_enemy_death);
		BaseStruct::serialize (sl, 0x66, &se_item);

		BaseStruct::serialize (sl, 0x6F, transition_out		,m_link->transition_out);
		BaseStruct::serialize (sl, 0x70, transition_in		,m_link->transition_in);
		BaseStruct::serialize (sl, 0x71, battle_start_fadeout	,m_link->battle_start_fadeout);
		BaseStruct::serialize (sl, 0x72, battle_start_fadein	,m_link->battle_start_fadein);
		BaseStruct::serialize (sl, 0x73, battle_end_fadeout	,m_link->battle_end_fadeout);
		BaseStruct::serialize (sl, 0x74, battle_end_fadein	,m_link->battle_end_fadein);

		BaseStruct::serialize (sl, 0x79, able_teleport		,true);
		BaseStruct::serialize (sl, 0x7A, able_escape			,true);
		BaseStruct::serialize (sl, 0x7B, able_save			,true);
		BaseStruct::serialize (sl, 0x7C, able_menu			,true);

		BaseStruct::serialize (sl, 0x82, frame_name, m_link->frame_name);//TODO
		BaseStruct::serialize (sl, 0x83, save_count			,0);
		BaseStruct::serialize (sl, 0x84, save_slot			,1);
		BaseStruct::serialize (sl, 0x8C, battle_action_wait	,false);
		
		sl.finishStruct();
	}

	void link( LdbSystem *dSys ) 
	{
		m_link = dSys;

		music_title.link		(&(dSys->title_music));
		music_battle.link		(&(dSys->battle_music));
		music_battle_end.link	(&(dSys->battle_end_music));
		music_inn.link			(&(dSys->inn_music));
		
		music_boat.link			(&(dSys->boat_music));
		music_ship.link			(&(dSys->ship_music));
		music_airship.link		(&(dSys->airship_music));
		music_gameover.link		(&(dSys->gameover_music));
		
		se_cursor.link			(&(dSys->cursor_se));
		se_decision.link		(&(dSys->decision_se));
		se_cancel.link			(&(dSys->cancel_se));
		se_buzzer.link			(&(dSys->buzzer_se));
		
		se_battle.link			(&(dSys->battle_se));
		se_escape.link			(&(dSys->escape_se));
		se_enemy_attack.link	(&(dSys->enemy_attack_se));
		se_enemy_damaged.link	(&(dSys->enemy_damaged_se));
		se_actor_damaged.link	(&(dSys->actor_damaged_se));
		se_dodge.link			(&(dSys->dodge_se));
		se_enemy_death.link		(&(dSys->enemy_death_se));
		
		se_item.link			(&(dSys->item_se));
	}
};

#endif
