#ifndef __R2K_DATA__LSD_MAP_EVENT__
#define __R2K_DATA__LSD_MAP_EVENT__

#include <iostream>
#include "LsdObjectCommon.h"
#include "LsdMapEventStack.h"

class LsdMapEvent : public LsdObjectCommon
{
public: 
	int evtpage;
	/*67*/LsdMapEventStack eventstack;
	/*68*/int direction_event;
	/*80*/int parallel_event_page;
public:
	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeInit)
			evtpage = -1;
		else
			evtpage = -2;
		
		LsdObjectCommon::serialize(sl);

		BaseStruct::serialize (sl, 0x67, &eventstack);
		BaseStruct::serialize (sl, 0x68, direction_event, 0);
		BaseStruct::serialize (sl, 0x80, parallel_event_page, 0);

		sl.finishStruct();
	}



};

#endif
