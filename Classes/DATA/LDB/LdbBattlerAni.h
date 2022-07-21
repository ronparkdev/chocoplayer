#ifndef __R2K_DATA__LDB_BATTLER_ANIMATION__
#define __R2K_DATA__LDB_BATTLER_ANIMATION__

#include <iostream>
#include "../BaseStruct.h"
#include "LdbBattlerAniExt.h"

class LdbBattlerAni : public BaseStruct
{
public: 
	/*01*/TString	name;
	/*02*/int		speed;
	/*0A*/ArrayList<LdbBattlerAniExt> base_data;
	/*0B*/ArrayList<LdbBattlerAniExt> weapon_data;

public:
	void init() {
		/*01*/name = "";
		/*02*/speed = 0;//????? cant find this on editor
		/*0A*/base_data.RemoveAll();
		/*0B*/weapon_data.RemoveAll();
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, speed);
		BaseStruct::serialize(sl, 0x0A, base_data);
		BaseStruct::serialize(sl, 0x0B, weapon_data);
		sl.finishStruct();
	}
};

#endif
