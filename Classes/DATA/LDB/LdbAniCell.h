#ifndef __R2K_DATA__LDB_ANIMATION_CELL__
#define __R2K_DATA__LDB_ANIMATION_CELL__

#include <iostream>
#include "../BaseStruct.h"

class LdbAniCell : public BaseStruct
{
public: 
	/*01*/int priority;
	/*02*/int cell_id;
	/*03*/int x;
	/*04*/int y;
	/*05*/int zoom;
	/*06*/int tone_red;
	/*07*/int tone_green;
	/*08*/int tone_blue;
	/*09*/int tone_gray;
	/*0A*/int transparency;

public:
	void init() {
		/*01*/priority	= 0;//CHECKTHIS
		/*02*/cell_id	= 0;
		/*03*/x			= 0;
		/*04*/y			= 0;
		/*05*/zoom		= 100;
		/*06*/tone_red	= 100;
		/*07*/tone_green= 100;
		/*08*/tone_blue = 100;
		/*09*/tone_gray = 100;
		/*0A*/transparency = 0;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, priority);
		BaseStruct::serialize (sl, 0x02, cell_id);
		BaseStruct::serialize (sl, 0x03, x);
		BaseStruct::serialize (sl, 0x04, y);
		BaseStruct::serialize (sl, 0x05, zoom);
		BaseStruct::serialize (sl, 0x06, tone_red);
		BaseStruct::serialize (sl, 0x07, tone_green);
		BaseStruct::serialize (sl, 0x08, tone_blue);
		BaseStruct::serialize (sl, 0x09, tone_gray);
		BaseStruct::serialize (sl, 0x0A, transparency);
		sl.finishStruct();
	}
};

#endif
