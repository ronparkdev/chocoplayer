#ifndef __R2K_DATA__LSD_COMMON_EVENT__
#define __R2K_DATA__LSD_COMMON_EVENT__

#include <iostream>
#include "../BaseStruct.h"

class LsdCommonEvent : public BaseStruct
{
public: 
	/*01*/LsdMapEventStack eventstack;
public:
	void serialize(StructLoader &sl) {
		BaseStruct::serialize (sl, 0x01, &eventstack);
		sl.finishStruct();
	}

};

#endif
