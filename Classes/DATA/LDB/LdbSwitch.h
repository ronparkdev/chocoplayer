#ifndef __R2K_DATA__LDB_SWITCH__
#define __R2K_DATA__LDB_SWITCH__

#include <iostream>
#include "../BaseStruct.h"

class LdbSwitch : public BaseStruct
{
public: 
	/*01*/TString name;

public:
	void init() {
		/*01*/name = "";
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		sl.finishStruct();
	}
};

#endif
