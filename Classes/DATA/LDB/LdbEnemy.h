#ifndef __R2K_DATA__LDB_ENEMY__
#define __R2K_DATA__LDB_ENEMY__

#include <iostream>
#include "../BaseStruct.h"
#include "LdbEnemyAction.h"

class LdbEnemy : public BaseStruct
{
public: 
	/*01*/TString name;
	/*02*/TString BattlerName;
	/*03*/int	battler_hue;
	/*04*/int	max_hp;
	/*05*/int	max_mp;
	/*06*/int	attack;
	/*07*/int	defense;
	/*08*/int	spirit;
	/*09*/int	agility;
	/*0A*/bool	transparent;
	/*0B*/int	take_exp;
	/*0C*/int	take_money;
	/*0D*/int	drop_id;
	/*0E*/int	drop_chance;
	/*15*/bool	critical_hit;
	/*16*/int	critical_hit_chance;
	/*1A*/bool	miss;
	/*1C*/bool	flying;
	/*1F*/int	state_ranks_count;
	/*20*/ArrayList<byte> state_ranks;
	/*21*/int	attribute_ranks_count;
	/*22*/ArrayList<byte> attribute_ranks;
	/*2A*/ArrayList<LdbEnemyAction> actions;

public:
	void init() {
		/*01*/name			= "";
		/*02*/BattlerName	= "";
		/*03*/battler_hue= 0;
		/*04*/max_hp		= 10;
		/*05*/max_mp		= 10;
		/*06*/attack	= 10;
		/*07*/defense	= 10;
		/*08*/spirit	= 10;
		/*09*/agility	= 10;
		/*0A*/transparent = false;
		/*0B*/take_exp		= 0;
		/*0C*/take_money		= 0;
		/*0D*/drop_id	= 0;
		/*0E*/drop_chance	= 100;
		/*15*/critical_hit = false ;
		/*16*/critical_hit_chance = 30;
		/*1A*/miss = false;
		/*1C*/flying = false;
		/*1F*/state_ranks_count	= 0;
		/*20*/state_ranks.RemoveAll();
		/*21*/attribute_ranks_count= 0;
		/*22*/attribute_ranks.RemoveAll();
		/*2A*/actions.RemoveAll();

	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, BattlerName);
		BaseStruct::serialize (sl, 0x03, battler_hue);
		BaseStruct::serialize (sl, 0x04, max_hp);
		BaseStruct::serialize (sl, 0x05, max_mp);
		BaseStruct::serialize (sl, 0x06, attack);
		BaseStruct::serialize (sl, 0x07, defense);
		BaseStruct::serialize (sl, 0x08, spirit);
		BaseStruct::serialize (sl, 0x09, agility);
		BaseStruct::serialize (sl, 0x0A, transparent);
		BaseStruct::serialize (sl, 0x0B, take_exp);
		BaseStruct::serialize (sl, 0x0C, take_money);
		BaseStruct::serialize (sl, 0x0D, drop_id);
		BaseStruct::serialize (sl, 0x0E, drop_chance);
		BaseStruct::serialize (sl, 0x15, critical_hit);
		BaseStruct::serialize (sl, 0x16, critical_hit_chance);
		BaseStruct::serialize (sl, 0x1A, miss);
		BaseStruct::serialize (sl, 0x1C, flying);
		BaseStruct::serialize (sl, 0x1F, state_ranks_count);
		BaseStruct::serialize (sl, 0x20, state_ranks);
		BaseStruct::serialize (sl, 0x21, attribute_ranks_count);
		BaseStruct::serialize (sl, 0x22, attribute_ranks);
		BaseStruct::serialize(sl, 0x2A, actions);
		sl.finishStruct();
	}
};

#endif
