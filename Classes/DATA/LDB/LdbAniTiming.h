#ifndef __R2K_DATA__LDB_ANIMATION_TIMING__
#define __R2K_DATA__LDB_ANIMATION_TIMING__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbSound.h"

enum AniTimingScope {
	AniTimingScope_none	  = 0,
	AniTimingScope_target = 1,
	AniTimingScope_screen = 2
};

class LdbAniTiming : public BaseStruct
{
public: 
	/*01*/int frame;
	/*02*/GlbSound se;
	/*03*/int flash_scope;
	/*04*/int flash_red;
	/*05*/int flash_green;
	/*06*/int flash_blue;
	/*07*/int flash_power;
	/*08*/int screen_shake;

public:
	void init() {
		/*01*/frame			= 1;
		/*03*/flash_scope	= AniTimingScope_none;
		/*04*/flash_red		= 31;
		/*05*/flash_green	= 31;
		/*06*/flash_blue	= 31;
		/*07*/flash_power	= 31;
		/*08*/screen_shake	= AniTimingScope_none;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, frame);
		BaseStruct::serialize (sl, 0x02, &se);
		BaseStruct::serialize (sl, 0x03, flash_scope);
		BaseStruct::serialize (sl, 0x04, flash_red);
		BaseStruct::serialize (sl, 0x05, flash_green);
		BaseStruct::serialize (sl, 0x06, flash_blue);
		BaseStruct::serialize (sl, 0x07, flash_power);
		BaseStruct::serialize (sl, 0x08, screen_shake);
		sl.finishStruct();
	}
};

#endif
