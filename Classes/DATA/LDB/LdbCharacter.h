#ifndef __R2K_DATA__LDB_CHARACTER__
#define __R2K_DATA__LDB_CHARACTER__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbEqipment.h"
#include "../GLB/GlbSkillLearning.h"
#include "LdbCharacterParameter.h"

class LdbCharacter : public BaseStruct
{
public: 

	/*01*/TString	name;
	/*02*/TString	title;
	/*03*/TString	pic_name;
	/*04*/int		pic_id;
	/*05*/bool		transparent;
	/*07*/int		init_level;
	/*08*/int		final_level;
	/*09*/bool		critical_hit;
	/*0A*/int		critical_hit_chance;
	/*0F*/TString	face_name;
	/*10*/int		face_index;
	/*15*/bool		use_two_hand;
	/*16*/bool		fix_equip;
	/*17*/bool		auto_battle;
	/*18*/bool		super_guard;
	/*1F*/LdbCharacterParameter parameters;
	/*29*/int		exp_base;
	/*2A*/int		exp_inflation;
	/*2B*/int		exp_correction;
	/*33*/GlbEquipment			init_equip;
	/*38*/int		unarmed_ani;
	/*39*/int		job_id;
	/*3B*/int		battle_x;
	/*3C*/int		battle_y;
	/*3E*/int		battler_ani;
	/*3F*/ArrayList<GlbSkillLearning> skills;
	/*42*/bool		rename_skill;
	/*43*/TString	skill_name;		//WHAT?
	/*47*/byte		state_ranks_count;
	/*48*/ArrayList<byte>		state_ranks;
	/*49*/byte		attribute_ranks_count;
	/*4A*/ArrayList<byte>		attribute_ranks;
	/*50*/ArrayList<int>battle_commends;

public:
	void init() {
		/*01*/name = "";
		/*02*/title = "";
		/*03*/pic_name = "";
		/*04*/pic_id = 0;
		/*05*/transparent = false;
		/*07*/init_level = 1;
		/*08*/final_level = 99;
		/*09*/critical_hit = true;
		/*0A*/critical_hit_chance = 30;
		/*0F*/face_name = "";
		/*10*/face_index = 0;
		/*15*/use_two_hand = false;
		/*16*/fix_equip = false;
		/*17*/auto_battle = false;
		/*18*/super_guard = false;
//		/*1F*/parameters;
		/*29*/exp_base = 0;
		/*2A*/exp_inflation = 0;
		/*2B*/exp_correction = 0;
		/*33*///InitialEquipment.init();
		/*38*/unarmed_ani = 1;
		/*39*/job_id = 0;
		/*3B*/battle_x = 0;
		/*3C*/battle_y = 120;
		/*3E*/battler_ani = 1;
		/*3F*/skills.RemoveAll();
		/*42*/rename_skill = false;//WHAT?
		/*43*/skill_name = "";
		/*47*/state_ranks_count = 0;
		/*48*/state_ranks.RemoveAll();
		/*49*/attribute_ranks_count = 0;
		/*4A*/attribute_ranks.RemoveAll();
		/*50*/battle_commends.RemoveAll();
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, title);
		BaseStruct::serialize (sl, 0x03, pic_name);
		BaseStruct::serialize (sl, 0x04, pic_id);
		BaseStruct::serialize (sl, 0x05, transparent);
		BaseStruct::serialize (sl, 0x07, init_level);
		BaseStruct::serialize (sl, 0x08, final_level);
		BaseStruct::serialize (sl, 0x09, critical_hit);
		BaseStruct::serialize (sl, 0x0A, critical_hit_chance);
		BaseStruct::serialize (sl, 0x0F, face_name);
		BaseStruct::serialize (sl, 0x10, face_index);
		BaseStruct::serialize (sl, 0x15, use_two_hand);
		BaseStruct::serialize (sl, 0x16, fix_equip);
		BaseStruct::serialize (sl, 0x17, auto_battle);
		BaseStruct::serialize (sl, 0x18, super_guard);
		BaseStruct::serialize (sl, 0x1F, &parameters);
		BaseStruct::serialize (sl, 0x29, exp_base);
		BaseStruct::serialize (sl, 0x2A, exp_inflation);
		BaseStruct::serialize (sl, 0x2B, exp_correction);
		BaseStruct::serialize (sl, 0x33, &init_equip);
		BaseStruct::serialize (sl, 0x38, unarmed_ani);
		BaseStruct::serialize (sl, 0x39, job_id);
		BaseStruct::serialize (sl, 0x3B, battle_x);
		BaseStruct::serialize (sl, 0x3C, battle_y);
		BaseStruct::serialize (sl, 0x3E, battler_ani);
		BaseStruct::serialize(sl, 0x3F, skills);
		BaseStruct::serialize (sl, 0x42, rename_skill);
		BaseStruct::serialize (sl, 0x43, skill_name);
		BaseStruct::serialize (sl, 0x47, state_ranks_count);
		BaseStruct::serialize (sl, 0x48, state_ranks);
		BaseStruct::serialize (sl, 0x49, attribute_ranks_count);
		BaseStruct::serialize (sl, 0x4A, attribute_ranks);
		BaseStruct::serialize (sl, 0x50, battle_commends);
		sl.finishStruct();
	}
};

#endif
