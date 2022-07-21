#ifndef __R2K_DATA__LDB_MAIN_SIMPLE__
#define __R2K_DATA__LDB_MAIN_SIMPLE__

#include <iostream>
#include "../BaseStruct.h"

#include "LdbSystemSimple.h"

#define LDB_HEADER "LcfDataBase"

class LdbMainSimple : public BaseStruct
{
public: 
	TString header;

public:
	LdbSystemSimple		dSys;

public:
	void init() {
		header = LDB_HEADER;
	}

	void serialize(StructLoader &sl) {
		init();
		BaseStruct::serialize(sl, header);
		if (header == LDB_HEADER) {
			BaseStruct::serialize (sl, 0x16, &dSys	);
		}
	}

};

#endif
