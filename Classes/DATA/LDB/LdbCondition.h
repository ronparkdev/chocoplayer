#ifndef __R2K_DATA__LDB_CONDITION__
#define __R2K_DATA__LDB_CONDITION__

#include <iostream>
#include "../BaseStruct.h"

enum ConditionPersistence {
	CondPsistence_ends = 0,
	CondPersistence_persists = 1
};
enum CondRestriction {
	CondRestriction_normal = 0,
	CondRestriction_do_nothing = 1,
	CondRestriction_attack_enemy = 2,
	CondRestriction_attack_ally = 3
};
enum CondAffectType {
	CondAffectType_half = 0,
	CondAffectType_double = 1,
	CondAffectType_nothing = 2
};
enum ChangeType {
	CondChangeType_lose = 0,
	CondChangeType_gain = 1,
	CondChangeType_nothing = 2
};

class LdbCondition : public BaseStruct
{
public: 
	/*01*/TString name;
	/*02*/int	type;
	/*03*/int	color;
	/*04*/int	priority;
	/*05*/int	restriction;
	/*0B*/int	a_rate;
	/*0C*/int	b_rate;
	/*0D*/int	c_rate;
	/*0E*/int	d_rate;
	/*0F*/int	e_rate;
	/*15*/int	hold_turn;
	/*16*/int	auto_release_prob;
	/*17*/int	release_by_damage;
	/*1E*/int	affect_type;
	/*1F*/bool	affect_attack;
	/*20*/bool	affect_defense;
	/*21*/bool	affect_spirit;
	/*22*/bool	affect_agility;
	/*23*/int	reduce_hit_ratio;
	/*24*/bool	avoid_attacks;
	/*25*/bool	reflect_magic;
	/*26*/bool	fixed_equip;
	/*27*/int	battler_animation_id;
	/*29*/bool	restrict_skill;
	/*2A*/int	restrict_skill_level;
	/*2B*/bool	restrict_magic;
	/*2C*/int	restrict_magic_level;
	/*2D*/int	hp_change_type;
	/*2E*/int	mp_change_type;
	/*33*/TString message_actor;
	/*34*/TString message_enemy;
	/*35*/TString message_already;
	/*36*/TString message_affected;
	/*37*/TString message_recovery;
	/*3D*/int	hp_change_max;
	/*3E*/int	hp_change_val;
	/*3F*/int	hp_change_map_steps;
	/*40*/int	hp_change_map_val;
	/*41*/int	mp_change_max;
	/*42*/int	mp_change_val;
	/*43*/int	mp_change_map_steps;
	/*44*/int	mp_change_map_val;

public:
	void init() {
		/*01*/name			= "";
		/*02*/type			= 0;
		/*03*/color			= 6;
		/*04*/priority		= 50;
		/*05*/restriction	= 0;
		/*0B*/a_rate		= 100;
		/*0C*/b_rate 		= 80;
		/*0D*/c_rate 		= 60;
		/*0E*/d_rate 		= 30;
		/*0F*/e_rate 		= 0;
		/*15*/hold_turn		= 0;
		/*16*/auto_release_prob = 0;
		/*17*/release_by_damage = 0;
		/*1E*/affect_type		= CondAffectType_half;
		/*1F*/affect_attack		= false;
		/*20*/affect_defense	= false;
		/*21*/affect_spirit		= false;
		/*22*/affect_agility	= false;
		/*23*/reduce_hit_ratio	= 100;
		/*24*/avoid_attacks		= false;
		/*25*/reflect_magic		= false;
		/*26*/fixed_equip		= false;
		/*27*/battler_animation_id = 6;
		/*29*/restrict_skill		= false;
		/*2A*/restrict_skill_level	= 0;
		/*2B*/restrict_magic		= false;
		/*2C*/restrict_magic_level	= 0;
		/*2D*/hp_change_type = CondChangeType_lose;
		/*2E*/mp_change_type = CondChangeType_lose;
		/*33*/message_actor		= "";
		/*34*/message_enemy		= "";
		/*35*/message_already	= "";
		/*36*/message_affected	= "";
		/*37*/message_recovery	= "";
		/*3D*/hp_change_max		= 0;
		/*3E*/hp_change_val		= 0;
		/*3F*/hp_change_map_steps = 0;
		/*40*/hp_change_map_val=0;
		/*41*/mp_change_max		= 0;
		/*42*/mp_change_val		= 0;
		/*43*/mp_change_map_steps = 0;
		/*44*/mp_change_map_val=0;

	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, type);
		BaseStruct::serialize (sl, 0x03, color);
		BaseStruct::serialize (sl, 0x04, priority);
		BaseStruct::serialize (sl, 0x05, restriction);
		BaseStruct::serialize (sl, 0x0B, a_rate);
		BaseStruct::serialize (sl, 0x0C, b_rate);
		BaseStruct::serialize (sl, 0x0D, c_rate);
		BaseStruct::serialize (sl, 0x0E, d_rate);
		BaseStruct::serialize (sl, 0x0F, e_rate);
		BaseStruct::serialize (sl, 0x15, hold_turn);
		BaseStruct::serialize (sl, 0x16, auto_release_prob);
		BaseStruct::serialize (sl, 0x17, release_by_damage);
		BaseStruct::serialize (sl, 0x1E, affect_type);
		BaseStruct::serialize (sl, 0x1F, affect_attack);
		BaseStruct::serialize (sl, 0x20, affect_defense);
		BaseStruct::serialize (sl, 0x21, affect_spirit);
		BaseStruct::serialize (sl, 0x22, affect_agility);
		BaseStruct::serialize (sl, 0x23, reduce_hit_ratio);
		BaseStruct::serialize (sl, 0x24, avoid_attacks);
		BaseStruct::serialize (sl, 0x25, reflect_magic);
		BaseStruct::serialize (sl, 0x26, fixed_equip);
		BaseStruct::serialize (sl, 0x27, battler_animation_id);
		BaseStruct::serialize (sl, 0x29, restrict_skill);
		BaseStruct::serialize (sl, 0x2A, restrict_skill_level);
		BaseStruct::serialize (sl, 0x2B, restrict_magic);
		BaseStruct::serialize (sl, 0x2C, restrict_magic_level);
		BaseStruct::serialize (sl, 0x2D, hp_change_type);
		BaseStruct::serialize (sl, 0x2E, mp_change_type);
		BaseStruct::serialize (sl, 0x33, message_actor);
		BaseStruct::serialize (sl, 0x34, message_enemy);
		BaseStruct::serialize (sl, 0x35, message_already);
		BaseStruct::serialize (sl, 0x36, message_affected);
		BaseStruct::serialize (sl, 0x37, message_recovery);
		BaseStruct::serialize (sl, 0x3D, hp_change_max);
		BaseStruct::serialize (sl, 0x3E, hp_change_val);
		BaseStruct::serialize (sl, 0x3F, hp_change_map_steps);
		BaseStruct::serialize (sl, 0x40, hp_change_map_val);
		BaseStruct::serialize (sl, 0x41, mp_change_max);
		BaseStruct::serialize (sl, 0x42, mp_change_val);
		BaseStruct::serialize (sl, 0x43, mp_change_map_steps);
		BaseStruct::serialize (sl, 0x44, mp_change_map_val);
		sl.finishStruct();
	}
};

#endif
