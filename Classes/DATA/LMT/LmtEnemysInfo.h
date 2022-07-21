#ifndef __R2K_DATA__LMT_ENEMYS_INFO__
#define __R2K_DATA__LMT_ENEMYS_INFO__

#include <iostream>
#include "../BaseStruct.h"

class LmtEnemiesInfo : public BaseStruct
{
public: 
	/*01*/int Monsters_ID;


public:
	void init() {
		Monsters_ID = 1;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, Monsters_ID);

		sl.finishStruct();
	}

};

#endif
