#ifndef __R2K_DATA__LMT_MAPS_INFO__
#define __R2K_DATA__LMT_MAPS_INFO__

#include <iostream>
#include "../BaseStruct.h"

class LmtMapsInfo : public BaseStruct
{
public: 
	/*01*/int party_map_id;
	/*02*/int party_x;
	/*03*/int party_y;
	/*0B*/int boat_map_id;
	/*0C*/int boat_x;
	/*0D*/int boat_y;
	/*15*/int ship_map_id;
	/*16*/int ship_x;
	/*17*/int ship_y;
	/*1F*/int airship_map_id;
	/*20*/int airship_x;
	/*21*/int airship_y; 


public:
	void init() {
		party_map_id = 0;
		party_x = 0;
		party_y = 0;
		boat_map_id = 0;
		boat_x = 0;
		boat_y = 0;
		ship_map_id = 0;
		ship_x = 0;
		ship_y = 0;
		airship_map_id = 0;
		airship_x = 0;
		airship_y = 0; 
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, party_map_id);
		BaseStruct::serialize (sl, 0x02, party_x);
		BaseStruct::serialize (sl, 0x03, party_y);
		BaseStruct::serialize (sl, 0x0B, boat_map_id);
		BaseStruct::serialize (sl, 0x0C, boat_x);
		BaseStruct::serialize (sl, 0x0D, boat_y);
		BaseStruct::serialize (sl, 0x15, ship_map_id);
		BaseStruct::serialize (sl, 0x16, ship_x);
		BaseStruct::serialize (sl, 0x17, ship_y);
		BaseStruct::serialize (sl, 0x1F, airship_map_id);
		BaseStruct::serialize (sl, 0x20, airship_x);
		BaseStruct::serialize (sl, 0x21, airship_y); 
		sl.finishStruct();
	}

};

#endif
