#ifndef __R2K_DATA__LDB_SKILL__
#define __R2K_DATA__LDB_SKILL__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbSound.h"
#include "LdbBattleAniData.h"

enum SkillType {
	SkillType_normal = 0,
	SkillType_teleport = 1,
	SkillType_escape = 2,
	SkillType_switch = 3,
	SkillType_subskill = 4
};
enum SkillMpType {
	SkillMpType_cost = 0,
	SkillMpType_percent = 1
};
enum SkillScope {
	SkillScope_enemy = 0,
	SkillScope_enemies = 1,
	SkillScope_self = 2,
	SkillScope_ally = 3,
	SkillScope_party = 4
};

class LdbSkill : public BaseStruct
{
public: 
	/*01*/TString	name;
	/*02*/TString	desc;
	/*03*/TString	using_msg_1;
	/*04*/TString	using_msg_2;
	/*07*/int		fail_msg;
	/*08*/int		type;
	/*09*/int 		mp_type;
	/*0A*/int 		mp_per;
	/*0B*/int 		mp_cost;
	/*0C*/int 		scope;
	/*0D*/int 		switch_id;
	/*0E*/int 		ani_id;
	/*10*/GlbSound	sound_effect;
	/*12*/bool 		occasion_field;
	/*13*/bool 		occasion_battle;
	/*14*/bool 		state_effect;
	/*15*/int 		pdef_f;
	/*16*/int 		mdef_f;
	/*17*/int 		variance;
	/*18*/int 		power;
	/*19*/int 		hit;
	/*1F*/bool 		affect_hp;
	/*20*/bool 		affect_mp;
	/*21*/bool 		affect_attack;
	/*22*/bool 		affect_defence;
	/*23*/bool 		affect_spirit;
	/*24*/bool 		affect_agility;
	/*25*/bool 		absorb_dmg;
	/*26*/bool 		ignore_dmg;
	/*29*/byte		state_effects_count;
	/*2A*/ArrayList<bool> state_effects;
	/*2B*/byte		attribute_effects_count;
	/*2C*/ArrayList<bool> attribute_effects;
	/*2D*/bool		affect_attr_defence;
	/*31*/int		battler_ani_count;
	/*32*/ArrayList<LdbBattleAniData> battler_ani;

public:
	void init() {
		/*01*/name		= "";
		/*02*/desc		= "";
		/*03*/using_msg_1	= "";
		/*04*/using_msg_2	= "";
		/*07*/fail_msg	= 0;
		/*08*/type		= SkillType_normal;
		/*09*/mp_type	= SkillMpType_cost;
		/*0A*/mp_per	= 1;
		/*0B*/mp_cost	= 0;
		/*0C*/scope		= SkillScope_enemy;
		/*0D*/switch_id	= 1;
		/*0E*/ani_id	= 1;
//		/*10*/sound_effect;
		/*12*/occasion_field	= true;
		/*13*/occasion_battle	= false;
		/*14*/state_effect		= false;
		/*15*/pdef_f	= 0;
		/*16*/mdef_f	= 3;
		/*17*/variance	= 4;
		/*18*/power		= 0;
		/*19*/hit		= 100;
		/*1F*/affect_hp			= false;
		/*20*/affect_mp			= false;
		/*21*/affect_attack		= false;
		/*22*/affect_defence	= false;
		/*23*/affect_spirit		= false;
		/*24*/affect_agility	= false;
		/*25*/absorb_dmg		= false;
		/*26*/ignore_dmg		= false;
		/*29*/state_effects_count = 0;
		/*2A*/state_effects.RemoveAll();
		/*2B*/attribute_effects_count = 0;
		/*2C*/attribute_effects.RemoveAll();
		/*2D*/affect_attr_defence = 0;
		/*31*/battler_ani_count	= 0;
		/*32*/battler_ani.RemoveAll();

	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, desc);
		BaseStruct::serialize (sl, 0x03, using_msg_1);
		BaseStruct::serialize (sl, 0x04, using_msg_2);
		BaseStruct::serialize (sl, 0x07, fail_msg);
		BaseStruct::serialize (sl, 0x08, type);
		BaseStruct::serialize (sl, 0x09, mp_type);
		BaseStruct::serialize (sl, 0x0A, mp_per);
		BaseStruct::serialize (sl, 0x0B, mp_cost);
		BaseStruct::serialize (sl, 0x0C, scope);
		BaseStruct::serialize (sl, 0x0D, switch_id);
		BaseStruct::serialize (sl, 0x0E, ani_id);
		BaseStruct::serialize (sl, 0x10, &sound_effect);
		BaseStruct::serialize (sl, 0x12, occasion_field);
		BaseStruct::serialize (sl, 0x13, occasion_battle);
		BaseStruct::serialize (sl, 0x14, state_effect);
		BaseStruct::serialize (sl, 0x15, pdef_f);
		BaseStruct::serialize (sl, 0x16, mdef_f);
		BaseStruct::serialize (sl, 0x17, variance);
		BaseStruct::serialize (sl, 0x18, power);
		BaseStruct::serialize (sl, 0x19, hit);
		BaseStruct::serialize (sl, 0x1F, affect_hp);
		BaseStruct::serialize (sl, 0x20, affect_mp);
		BaseStruct::serialize (sl, 0x21, affect_attack);
		BaseStruct::serialize (sl, 0x22, affect_defence);
		BaseStruct::serialize (sl, 0x23, affect_spirit);
		BaseStruct::serialize (sl, 0x24, affect_agility);
		BaseStruct::serialize (sl, 0x25, absorb_dmg);
		BaseStruct::serialize (sl, 0x26, ignore_dmg);
		BaseStruct::serialize (sl, 0x29, state_effects_count);
		BaseStruct::serialize (sl, 0x2A, state_effects);
		BaseStruct::serialize (sl, 0x2B, attribute_effects_count);
		BaseStruct::serialize (sl, 0x2C, attribute_effects);
		BaseStruct::serialize (sl, 0x2D, affect_attr_defence);
		BaseStruct::serialize (sl, 0x31, battler_ani_count);
		BaseStruct::serialize(sl, 0x32, battler_ani);

		sl.finishStruct();
	}
};

#endif
