#ifndef __R2K_DATA__LDB_ATTRIBUTE__
#define __R2K_DATA__LDB_ATTRIBUTE__

#include <iostream>
#include "../BaseStruct.h"

enum AttrType{
	AttrType_Physical = 0,
	AttrType_Magical = 1
};

class LdbAttribute : public BaseStruct
{
public: 
	/*01*/TString name;
	/*02*/int type;
	/*0B*/int a_rate;
	/*0C*/int b_rate;
	/*0D*/int c_rate;
	/*0E*/int d_rate;
	/*0F*/int e_rate;

public:
	void init() {
		/*01*/name	 = "";
		/*02*/type	 = AttrType_Physical;
		/*0B*/a_rate = 300;
		/*0C*/b_rate = 200;
		/*0D*/c_rate = 100;
		/*0E*/d_rate = 50;
		/*0F*/e_rate = 0;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, type);
		BaseStruct::serialize (sl, 0x0B, a_rate);
		BaseStruct::serialize (sl, 0x0C, b_rate);
		BaseStruct::serialize (sl, 0x0D, c_rate);
		BaseStruct::serialize (sl, 0x0E, d_rate);
		BaseStruct::serialize (sl, 0x0F, e_rate);
		sl.finishStruct();
	}
};

#endif
