#ifndef __R2K_DATA__LSD_TELEPORT__
#define __R2K_DATA__LSD_TELEPORT__

#include <iostream>
#include "../BaseStruct.h"

class LsdTeleport : public BaseStruct
{
public:
	int mapid;
	int pos_x;
	int pos_y;
	bool use_switch;
	int switchid;
public:
	void serialize(StructLoader &sl) {
		BaseStruct::serialize (sl, 0x01, mapid, 0);//mapid is cannot duplicated
		BaseStruct::serialize (sl, 0x02, pos_x, 0);
		BaseStruct::serialize (sl, 0x03, pos_y, 0);
		BaseStruct::serialize (sl, 0x03, use_switch, false);
		BaseStruct::serialize (sl, 0x05, switchid, 0);
		sl.finishStruct();
	}
};

#endif
