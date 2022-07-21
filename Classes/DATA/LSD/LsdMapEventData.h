#ifndef __R2K_DATA__LSD_MAP_EVENT_DATA__
#define __R2K_DATA__LSD_MAP_EVENT_DATA__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbEvent.h"

class LsdMapEventData : public BaseStruct
{
public: 
	/*01*/int event_length;
	/*02*/GlbEvent event;
	/*0B*/int eventpos;
	/*0C*/int event_id;//if 0 then it is common event
	/*16*/int what16;
	GlbEvent* pevent;
public:
	void serialize(StructLoader &sl) {
		pevent = NULL;

		BaseStruct::serialize (sl, 0x01, event_length	,0);
		BaseStruct::serialize (sl, 0x02, &event);
		BaseStruct::serialize (sl, 0x0B, eventpos		,0);
		BaseStruct::serialize (sl, 0x0C, event_id		,0);
		BaseStruct::serialize (sl, 0x16, what16		,0);//it seems to struct , not int

		sl.finishStruct();
	}

};

#endif
