#ifndef __R2K_DATA__LDB_ITEM__
#define __R2K_DATA__LDB_ITEM__

#include <iostream>
#include "../BaseStruct.h"
#include "LdbBattleAniData.h"

enum ItemType {
	ItemType_normal = 0,
	ItemType_weapon = 1,
	ItemType_shield = 2,
	ItemType_armor = 3,
	ItemType_helmet = 4,
	ItemType_accessory = 5,
	ItemType_medicine = 6,
	ItemType_book = 7,
	ItemType_material = 8,
	ItemType_special = 9,
	ItemType_switch = 10
};
enum ItemTrajectory {
	ItemTrajectory_straight = 0,
	ItemTrajectory_return = 1
};
enum ItemTarget {
	ItemTarget_single = 0,
	ItemTarget_center = 1,
	ItemTarget_simultaneous = 2,
	ItemTarget_sequential = 3
};


class LdbItem : public BaseStruct
{
public: 
	/*01*/TString	name;
	/*02*/TString	desc;
	/*03*/int		Type;
	/*05*/int		price;
	/*06*/int		uses;
	/*0B*/int		atk_points1;
	/*0C*/int		def_points1;
	/*0D*/int		spi_points1;
	/*0E*/int		agi_points1;
	/*0F*/bool		two_handed;
	/*10*/int		sp_cost;
	/*11*/int		hit;
	/*12*/int		critical_hit;
	/*14*/int		animation_id;
	/*15*/bool		preemptive;
	/*16*/bool		dual_attack;
	/*17*/bool		attack_all;
	/*18*/bool		ignore_evasion;
	/*19*/bool 		prevent_critical;
	/*1A*/bool 		raise_evasion;
	/*1B*/bool 		half_sp_cost;
	/*1C*/bool 		no_terrain_damage;
	/*1D*/bool 		fix_equip;
	/*1F*/bool 		entire_party;
	/*20*/int 		recover_hp;
	/*21*/int 		recover_hp_rate;
	/*22*/int 		recover_mp;
	/*23*/int 		recover_mp_rate;
	/*25*/bool 		occasion_field1;
	/*26*/bool 		ko_only;
	/*29*/int 		max_hp_points;
	/*2A*/int 		max_sp_points;
	/*2B*/int		atk_points2;
	/*2C*/int		def_points2;
	/*2D*/int		spi_points2;
	/*2E*/int		agi_points2;
	/*33*/int		using_message;
	/*35*/int		skill_id;
	/*37*/int		switch_id;
	/*39*/bool		occasion_field2;
	/*3A*/bool		occasion_battle;
	/*3D*/int		actor_set_count;
	/*3E*/ArrayList<bool> actor_set;
	/*3F*/int		state_effects_count;
	/*40*/ArrayList<bool> state_effects;
	/*41*/int		attribute_set_count;
	/*42*/ArrayList<bool> attribute_set;
	/*43*/int		state_chance;
	/*44*/bool		state_effect;
	/*45*/int		weapon_animation;
	/*46*/ArrayList<LdbBattleAniData> animation_data;
	/*47*/bool		use_skill;
	/*48*/int		class_set_count;
	/*49*/ArrayList<bool> class_set;
	/*4B*/int		ranged_trajectory;
	/*4C*/int		ranged_target;

public:
	void init() {

		/*01*/name			= "";
		/*02*/desc	= "";
		/*03*/Type			= 0;
		/*05*/price			= 0;
		/*06*/uses			= 1;
		/*0B*/atk_points1	= 0;
		/*0C*/def_points1	= 0;
		/*0D*/spi_points1	= 0;
		/*0E*/agi_points1	= 0;
		/*0F*/two_handed	= false;
		/*10*/sp_cost		= 0;
		/*11*/hit			= 90;
		/*12*/critical_hit	= 0;
		/*14*/animation_id	= 1;
		/*15*/preemptive		= false;
		/*16*/dual_attack		= false;
		/*17*/attack_all		= false;
		/*18*/ignore_evasion	= false;
		/*19*/prevent_critical	= false;
		/*1A*/raise_evasion		= false;
		/*1B*/half_sp_cost		= false;
		/*1C*/no_terrain_damage	= false;
		/*1D*/fix_equip			= false;
		/*1F*/entire_party		= false;
		/*20*/recover_hp		= false;
		/*21*/recover_hp_rate	= 0;
		/*22*/recover_mp		= false;
		/*23*/recover_mp_rate	= 0;
		/*25*/occasion_field1	= false;
		/*26*/ko_only			= false;
		/*29*/max_hp_points		= 0;
		/*2A*/max_sp_points		= 0;
		/*2B*/atk_points2	= 0;
		/*2C*/def_points2	= 0;
		/*2D*/spi_points2	= 0;
		/*2E*/agi_points2	= 0;
		/*33*/using_message = 0;
		/*35*/skill_id		= 1;
		/*37*/switch_id		= 1;
		/*39*/occasion_field2	= true;
		/*3A*/occasion_battle	= false;
		/*3D*/actor_set_count	= 0;
		/*3E*/actor_set.RemoveAll();
		/*3F*/state_effects_count	= 0;
		/*40*/state_effects.RemoveAll();
		/*41*/attribute_set_count=0;
		/*42*/attribute_set.RemoveAll();
		/*43*/state_chance		= 0;
		/*44*/state_effect		= false;
		/*45*/weapon_animation	= 1;
		/*46*/animation_data.RemoveAll();
		/*47*/use_skill			= false;
		/*48*/class_set_count	= 0;
		/*49*/class_set.RemoveAll();
		/*4B*/ranged_trajectory	= ItemTrajectory_straight;
		/*4C*/ranged_target		= ItemTarget_single;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, desc);
		BaseStruct::serialize (sl, 0x03, Type);
		BaseStruct::serialize (sl, 0x05, price);
		BaseStruct::serialize (sl, 0x06, uses);
		BaseStruct::serialize (sl, 0x0B, atk_points1);
		BaseStruct::serialize (sl, 0x0C, def_points1);
		BaseStruct::serialize (sl, 0x0D, spi_points1);
		BaseStruct::serialize (sl, 0x0E, agi_points1);
		BaseStruct::serialize (sl, 0x0F, two_handed);
		BaseStruct::serialize (sl, 0x10, sp_cost);
		BaseStruct::serialize (sl, 0x11, hit);
		BaseStruct::serialize (sl, 0x12, critical_hit);
		BaseStruct::serialize (sl, 0x14, animation_id);
		BaseStruct::serialize (sl, 0x15, preemptive);
		BaseStruct::serialize (sl, 0x16, dual_attack);
		BaseStruct::serialize (sl, 0x17, attack_all);
		BaseStruct::serialize (sl, 0x18, ignore_evasion);
		BaseStruct::serialize (sl, 0x19, prevent_critical);
		BaseStruct::serialize (sl, 0x1A, raise_evasion);
		BaseStruct::serialize (sl, 0x1B, half_sp_cost);
		BaseStruct::serialize (sl, 0x1C, no_terrain_damage);
		BaseStruct::serialize (sl, 0x1D, fix_equip);
		BaseStruct::serialize (sl, 0x1F, entire_party);
		BaseStruct::serialize (sl, 0x20, recover_hp);
		BaseStruct::serialize (sl, 0x21, recover_hp_rate);
		BaseStruct::serialize (sl, 0x22, recover_mp);
		BaseStruct::serialize (sl, 0x23, recover_mp_rate);
		BaseStruct::serialize (sl, 0x25, occasion_field1);
		BaseStruct::serialize (sl, 0x26, ko_only);
		BaseStruct::serialize (sl, 0x29, max_hp_points);
		BaseStruct::serialize (sl, 0x2A, max_sp_points);
		BaseStruct::serialize (sl, 0x2B, atk_points2);
		BaseStruct::serialize (sl, 0x2C, def_points2);
		BaseStruct::serialize (sl, 0x2D, spi_points2);
		BaseStruct::serialize (sl, 0x2E, agi_points2);
		BaseStruct::serialize (sl, 0x33, using_message);
		BaseStruct::serialize (sl, 0x35, skill_id);
		BaseStruct::serialize (sl, 0x37, switch_id);
		BaseStruct::serialize (sl, 0x39, occasion_field2);
		BaseStruct::serialize (sl, 0x3A, occasion_battle);
		BaseStruct::serialize (sl, 0x3D, actor_set_count);
		BaseStruct::serialize (sl, 0x3E, actor_set);
		BaseStruct::serialize (sl, 0x3F, state_effects_count);
		BaseStruct::serialize (sl, 0x40, state_effects);
		BaseStruct::serialize (sl, 0x41, attribute_set_count);
		BaseStruct::serialize (sl, 0x42, attribute_set);
		BaseStruct::serialize (sl, 0x43, state_chance);
		BaseStruct::serialize (sl, 0x44, state_effect);
		BaseStruct::serialize (sl, 0x45, weapon_animation);
		BaseStruct::serialize(sl, 0x46, animation_data);
		BaseStruct::serialize (sl, 0x47, use_skill);
		BaseStruct::serialize (sl, 0x48, class_set_count);
		BaseStruct::serialize (sl, 0x49, class_set);
		BaseStruct::serialize (sl, 0x4B, ranged_trajectory);
		BaseStruct::serialize (sl, 0x4C, ranged_target);
		sl.finishStruct();
	}
};

#endif
