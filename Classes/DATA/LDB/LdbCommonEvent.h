#ifndef __R2K_DATA__LDB_COMMON_EVENT__
#define __R2K_DATA__LDB_COMMON_EVENT__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbEvent.h"

enum CommonEvtTrigger {
	CommonEvtTrigger_Automatic = 3,
	CommonEvtTrigger_Parallel = 4,
	CommonEvtTrigger_Call = 5
};

class LdbCommonEvent : public BaseStruct
{
public: 
	/*01*/TString	name;
	/*0B*/int		trigger;
	/*0C*/bool		switch_flag;
	/*0D*/int		switch_id;
	/*15*/int		event_length;
	/*16*/GlbEvent	event_data;

public:
	void init() {
		/*01*/name			= "";
		/*0B*/trigger		= 0;//NEEDCHECK
		/*0C*/switch_flag	= 0;
		/*0D*/switch_id		= 1;
		/*15*/event_length	= 0;
//		/*16*/event_data;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x0B, trigger);
		BaseStruct::serialize (sl, 0x0C, switch_flag);
		BaseStruct::serialize (sl, 0x0D, switch_id);
		BaseStruct::serialize (sl, 0x15, event_length);
		BaseStruct::serialize (sl, 0x16, &event_data);
		sl.finishStruct();
	}
};

#endif
