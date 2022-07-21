#ifndef __R2K_DATA__LDB_ENEMY_GROUP_EVENT_CONDITION__
#define __R2K_DATA__LDB_ENEMY_GROUP_EVENT_CONDITION__

#include <iostream>
#include "../BaseStruct.h"

enum BattleEventCondition {
	batcond_switch_a	= 1,
	batcond_switch_b	= 2,
	batcond_variable	= 4,
	batcond_turn		= 8,
	batcond_fatigue		= 16,
	batcond_enemy_hp	= 32,
	batcond_actor_hp	= 64,
	batcond_turn_enemy	= 128,
	batcond_turn_actor	= 256,
	batcond_command_actor = 512
};

class LdbEnemyGroupEventCondition : public BaseStruct
{
public: 
	/*01*/unsigned short flags;
	/*02*/int switch_a_id;
	/*03*/int switch_b_id;
	/*04*/int variable_id;
	/*05*/int variable_value;
	/*06*/int turn_a;
	/*07*/int turn_b;
	/*08*/int fatigue_min;
	/*09*/int fatigue_max;
	/*0A*/int enemy_id;
	/*0B*/int enemy_hp_min;
	/*0C*/int enemy_hp_max;
	/*0D*/int actor_id;
	/*0E*/int actor_hp_min;
	/*0F*/int actor_hp_max;
	/*10*/int turn_enemy_id;
	/*11*/int turn_enemy_a;
	/*12*/int turn_enemy_b;
	/*13*/int turn_actor_id;
	/*14*/int turn_actor_a;
	/*15*/int turn_actor_b;
	/*16*/int command_actor_id;
	/*17*/int command_id;

public:
	void init() {
		/*01*/flags = 0;
		/*02*/switch_a_id = 1;
		/*03*/switch_b_id = 1;
		/*04*/variable_id = 1;
		/*05*/variable_value = 0;
		/*06*/turn_a = 0;
		/*07*/turn_b = 0;
		/*08*/fatigue_min = 0;
		/*09*/fatigue_max = 100;
		/*0A*/enemy_id = 1;
		/*0B*/enemy_hp_min = 0;
		/*0C*/enemy_hp_max = 100;
		/*0D*/actor_id = 1;
		/*0E*/actor_hp_min = 0;
		/*0F*/actor_hp_max = 100;
		/*10*/turn_enemy_id = 1;
		/*11*/turn_enemy_a = 0;
		/*12*/turn_enemy_b = 0;
		/*13*/turn_actor_id = 1;
		/*14*/turn_actor_a = 0;
		/*15*/turn_actor_b = 0;
		/*16*/command_actor_id = 1;
		/*17*/command_id = 1;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, flags);
		BaseStruct::serialize (sl, 0x02, switch_a_id);
		BaseStruct::serialize (sl, 0x03, switch_b_id);
		BaseStruct::serialize (sl, 0x04, variable_id);
		BaseStruct::serialize (sl, 0x05, variable_value);
		BaseStruct::serialize (sl, 0x06, turn_a);
		BaseStruct::serialize (sl, 0x07, turn_b);
		BaseStruct::serialize (sl, 0x08, fatigue_min);
		BaseStruct::serialize (sl, 0x09, fatigue_max);
		BaseStruct::serialize (sl, 0x0A, enemy_id);
		BaseStruct::serialize (sl, 0x0B, enemy_hp_min);
		BaseStruct::serialize (sl, 0x0C, enemy_hp_max);
		BaseStruct::serialize (sl, 0x0D, actor_id);
		BaseStruct::serialize (sl, 0x0E, actor_hp_min);
		BaseStruct::serialize (sl, 0x0F, actor_hp_max);
		BaseStruct::serialize (sl, 0x10, turn_enemy_id);
		BaseStruct::serialize (sl, 0x11, turn_enemy_a);
		BaseStruct::serialize (sl, 0x12, turn_enemy_b);
		BaseStruct::serialize (sl, 0x13, turn_actor_id);
		BaseStruct::serialize (sl, 0x14, turn_actor_a);
		BaseStruct::serialize (sl, 0x15, turn_actor_b);
		BaseStruct::serialize (sl, 0x16, command_actor_id);
		BaseStruct::serialize (sl, 0x17, command_id);
		sl.finishStruct();
	}
};

#endif
