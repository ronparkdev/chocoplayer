#ifndef __R2K_DATA__LDB_ENEMY_GROUP_EVENT_PAGE__
#define __R2K_DATA__LDB_ENEMY_GROUP_EVENT_PAGE__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbEvent.h"
#include "LdbEnemyGroupEventCondition.h"

class LdbEnemyGroupEventPage : public BaseStruct
{
public: 
	/*02*/LdbEnemyGroupEventCondition condition;
	/*0B*/int	event_count;
	/*0C*/GlbEvent event_data;

public:
	void init() {
		/*0B*/event_count = 0;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x02, &condition);
		BaseStruct::serialize (sl, 0x0B, event_count);
		BaseStruct::serialize (sl, 0x0C, &event_data);
		sl.finishStruct();
	}
};

#endif
