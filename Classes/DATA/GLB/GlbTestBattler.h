#ifndef __R2K_DATA__GLB_TEST_BATTLER__
#define __R2K_DATA__GLB_TEST_BATTLER__

#include <iostream>
#include "../BaseStruct.h"

class GlbTestBattler : public BaseStruct
{
public: 
	/*01*/int actor_id;
	/*02*/int level;
	/*0B*/int weapon_id;
	/*0C*/int shield_id;
	/*0D*/int armor_id;
	/*0E*/int helmet_id;
	/*0F*/int accessory_id;

public:
	void init() {

	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, actor_id);
		BaseStruct::serialize (sl, 0x02, level);
		BaseStruct::serialize (sl, 0x0B, weapon_id);
		BaseStruct::serialize (sl, 0x0C, shield_id);
		BaseStruct::serialize (sl, 0x0D, armor_id);
		BaseStruct::serialize (sl, 0x0E, helmet_id);
		BaseStruct::serialize (sl, 0x0F, accessory_id);
		sl.finishStruct();
	}
};

#endif
