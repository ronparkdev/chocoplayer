#ifndef __R2K_DATA__LDB_ANIMATION_FRAME__
#define __R2K_DATA__LDB_ANIMATION_FRAME__

#include <iostream>
#include "../BaseStruct.h"
#include "LdbAniCell.h"


class LdbAniFrame : public BaseStruct
{
public: 
	/*01*/ArrayList<LdbAniCell> cells;

public:
	void init() {
		cells.RemoveAll();
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize(sl, 0x01, cells);
		sl.finishStruct();
	}
};

#endif
