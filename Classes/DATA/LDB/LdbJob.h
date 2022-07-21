#ifndef __R2K_DATA__LDB_JOB__
#define __R2K_DATA__LDB_JOB__

#include <iostream>
#include "../BaseStruct.h"

class LdbJob : public BaseStruct
{
public: 
	/*01*/TString	name;
	/*15*/bool		use_two_hand;
	/*16*/bool		fix_equip;
	/*17*/bool		auto_battle;
	/*18*/bool		super_guard;
	/*1F*/LdbCharacterParameter parameters;
	/*29*/int		exp_base;
	/*2A*/int		exp_inflation;
	/*2B*/int		exp_correction;
	/*3E*/int		battler_ani;
	/*3F*/ArrayList<GlbSkillLearning> skills;
	/*47*/byte		state_ranks_count;
	/*48*/ArrayList<byte> state_ranks;
	/*49*/byte		attribute_ranks_count;
	/*4A*/ArrayList<byte>	attribute_ranks;
	/*50*/ArrayList<int>	battle_commends;

public:
	void init() {
		/*01*/name			= "";

		/*15*/use_two_hand= false;
		/*16*/fix_equip	= false;
		/*17*/auto_battle	= false;
		/*18*/super_guard	= false;

//		/*1F*/parameters;

		/*29*/exp_base		= 300;
		/*2A*/exp_inflation	= 300;
		/*2B*/exp_correction = 0;

		/*3E*/battler_ani = 1;
		
		/*3F*/skills.RemoveAll();
		
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
		BaseStruct::serialize (sl, 0x15, use_two_hand);
		BaseStruct::serialize (sl, 0x16, fix_equip);
		BaseStruct::serialize (sl, 0x07, auto_battle);
		BaseStruct::serialize (sl, 0x08, super_guard);
		BaseStruct::serialize (sl, 0x1F, &parameters);
		BaseStruct::serialize (sl, 0x29, exp_base);
		BaseStruct::serialize (sl, 0x2A, exp_inflation);
		BaseStruct::serialize (sl, 0x2B, exp_correction);
		BaseStruct::serialize (sl, 0x3E, battler_ani);
		BaseStruct::serialize(sl, 0x3F, skills);
		BaseStruct::serialize (sl, 0x47, state_ranks_count);
		BaseStruct::serialize (sl, 0x48, state_ranks);
		BaseStruct::serialize (sl, 0x49, attribute_ranks_count);
		BaseStruct::serialize (sl, 0x4A, attribute_ranks);
		BaseStruct::serialize (sl, 0x50, battle_commends);
		sl.finishStruct();
	}
};

#endif
