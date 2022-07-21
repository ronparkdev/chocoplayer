#ifndef __R2K_DATA__LMU_EVENT__
#define __R2K_DATA__LMU_EVENT__

#include <iostream>
#include "../BaseStruct.h"
#include "LmuEventPage.h"

class LmuEvent : public BaseStruct
{
public: 
	/*01*/TString Name;
	/*02*/int posX;
	/*03*/int posY;
	/*05*/ArrayList<LmuEventPage> Page;

public:
	void init() {
		posX = 0;
		posY = 0;
		Page.RemoveAll();
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead) {
			init();
		}
		BaseStruct::serialize (sl, 0x01, Name);
		BaseStruct::serialize (sl, 0x02, posX);
		BaseStruct::serialize (sl, 0x03, posY);
		BaseStruct::serialize(sl, 0x05, Page);
		sl.finishStruct();
	}
};

#endif
