#ifndef __R2K_DATA__LDB_SYSTEM_SIMPLE__
#define __R2K_DATA__LDB_SYSTEM_SIMPLE__

#include <iostream>
#include "../BaseStruct.h"

class LdbSystemSimple : public BaseStruct
{
public: 
	/*0A*/int	ldb_id;
	/*11*/TString title_name;

public:
	void init() {
		/*0A*/ldb_id		= 0;
		/*11*/title_name	= "";
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x0A, ldb_id);
		BaseStruct::serialize (sl, 0x11, title_name);
		sl.finishStruct();
	}
};

#endif
