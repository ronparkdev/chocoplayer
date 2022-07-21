#ifndef __R2K_DATA__LDB_BATTLER_ANIMATION_EXTENSION__
#define __R2K_DATA__LDB_BATTLER_ANIMATION_EXTENSION__

#include <iostream>
#include "../BaseStruct.h"

class LdbBattlerAniExt : public BaseStruct
{
public: 
	/*01*/TString	name;
	/*02*/TString	battler_name;
	/*03*/int		battler_index;
	/*04*/int		animation_type;
	/*05*/int		animation_id;

public:
	void init() {
		name			= "";
		battler_name	= "";
		battler_index	= 0;
		animation_type	= 0;
		animation_id	= 1;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, battler_name);
		BaseStruct::serialize (sl, 0x03, battler_index);
		BaseStruct::serialize (sl, 0x04, animation_type);
		BaseStruct::serialize (sl, 0x05, animation_id);
		sl.finishStruct();
	}
};

#endif
