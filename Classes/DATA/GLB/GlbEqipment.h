#ifndef __R2K_DATA__GLB_EQUIPMENT__
#define __R2K_DATA__GLB_EQUIPMENT__

#include <iostream>
#include "../BaseStruct.h"

class GlbEquipment : public BaseStruct
{
public:
	unsigned short weapon_id;
	unsigned short shield_id;
	unsigned short armor_id;
	unsigned short helmet_id;
	unsigned short accessory_id;

public:
	void init() {
		weapon_id	= 0;
		shield_id	= 0;
		armor_id	= 0; 
		helmet_id	= 0;
		accessory_id= 0;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, weapon_id);
		BaseStruct::serialize (sl, shield_id);
		BaseStruct::serialize (sl, armor_id);
		BaseStruct::serialize (sl, helmet_id);
		BaseStruct::serialize (sl, accessory_id);
	}
};

#endif
