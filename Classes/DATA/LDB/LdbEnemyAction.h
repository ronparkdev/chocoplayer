#ifndef __R2K_DATA__LDB_ENEMY_ACTION__
#define __R2K_DATA__LDB_ENEMY_ACTION__

#include <iostream>
#include "../BaseStruct.h"

enum EnemyActKind {
	EnemyActKind_basic = 0,
	EnemyActKind_skill = 1,
	EnemyActKind_transformation = 2
};
enum EnemyActBasic {
	EnemyActBasic_attack = 0,
	EnemyActBasic_dual_attack = 1,
	EnemyActBasic_defense = 2,
	EnemyActBasic_observe = 3,
	EnemyActBasic_charge = 4,
	EnemyActBasic_autodestruction = 5,
	EnemyActBasic_escape = 6,
	EnemyActBasic_nothing = 7
};
enum EnemyActCondType {
	EnemyActCondType_always = 0,
	EnemyActCondType_switch = 1,
	EnemyActCondType_turn = 2,
	EnemyActCondType_actors = 3,
	EnemyActCondType_hp = 4,
	EnemyActCondType_sp = 5,
	EnemyActCondType_party_lvl = 6,
	EnemyActCondType_party_fatigue = 7
};

class LdbEnemyAction : public BaseStruct
{
public: 
	/*01*/int	kind;			
	/*02*/int	basic;			
	/*03*/int	skill_id;			
	/*04*/int	enemy_id;			
	/*05*/int	condition_type;			
	/*06*/int	condition_param1;			
	/*07*/int	condition_param2;			
	/*08*/int	switch_id;			
	/*09*/bool	switch_on;			
	/*0A*/int	switch_on_id;			
	/*0B*/bool	switch_off;			
	/*0C*/int	switch_off_id;			
	/*0D*/int	rating;		

public:
	void init() {
		/*01*/kind		= EnemyActKind_basic;			
		/*02*/basic		= EnemyActBasic_attack;	
		/*03*/skill_id	= 1;			
		/*04*/enemy_id	= 1;		
		/*05*/condition_type	= EnemyActCondType_always;			
		/*06*/condition_param1	= 0;			
		/*07*/condition_param2	= 0;					
		/*08*/switch_id = 1;			
		/*09*/switch_on = false;			
		/*0A*/switch_on_id	= 1;			
		/*0B*/switch_off= false;			
		/*0C*/switch_off_id = 1;			
		/*0D*/rating	= 50;		
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, kind);
		BaseStruct::serialize (sl, 0x02, basic);
		BaseStruct::serialize (sl, 0x03, skill_id);
		BaseStruct::serialize (sl, 0x04, enemy_id);
		BaseStruct::serialize (sl, 0x05, condition_type);
		BaseStruct::serialize (sl, 0x06, condition_param1);
		BaseStruct::serialize (sl, 0x07, condition_param2);
		BaseStruct::serialize (sl, 0x08, switch_id);
		BaseStruct::serialize (sl, 0x09, switch_on);
		BaseStruct::serialize (sl, 0x0A, switch_on_id);
		BaseStruct::serialize (sl, 0x0B, switch_off);
		BaseStruct::serialize (sl, 0x0C, switch_off_id);
		BaseStruct::serialize (sl, 0x0D, rating);
		sl.finishStruct();
	}
};

#endif
