#ifndef __R2K_DATA__LSD_MAP__
#define __R2K_DATA__LSD_MAP__

#include <iostream>
#include "../BaseStruct.h"

#include "LsdMapEvent.h"
class LsdMap : public BaseStruct
{
public: 
	LmuMain *lmu;

	/*xx*/int chipset_id;
	/*xx*/bool need_tile_refresh;

	/*01*/int pan_x;
	/*02*/int pan_y;
	/*03*/int encounter_rate;

	/*0B*/ArrayList<LsdMapEvent> events;
	
	/*10*/bool is_pan_lock;

	/*15*/byte tile_lower[144];
	/*16*/byte tile_upper[144];

	/*20*/TString bg_name;
	/*21*/bool bg_hscroll;
	/*22*/bool bg_vscroll;
	/*23*/bool bg_hscrollauto;
	/*24*/int bg_hscrollauto_speed;
	/*25*/bool bg_vscrollauto;
	/*26*/int bg_vscrollauto_speed;
public:
	void serialize(StructLoader &sl) {

		BaseStruct::serialize (sl, 0x01, pan_x			,0);
		BaseStruct::serialize (sl, 0x02, pan_y			,0);
		BaseStruct::serialize (sl, 0x03, encounter_rate	,0);

		BaseStruct::serialize(sl, 0x0B, events);

		BaseStruct::serialize (sl, 0x10, is_pan_lock		,false);

		int size;
		if (sl.getMode() == StructLoaderModeInit) {
			for(int i=0; i<144; i++) {
				tile_lower[i] = i;
				tile_upper[i] = i;
			}
			need_tile_refresh = false;
		} else if (sl.getMode() == StructLoaderModeRead) {
			if (sl.isNextCode(0x15)) {
				sl.intoCode();
				sl >> size;
				for(int i=0; i<(int)(size/sizeof(byte)); i++)
					sl >> tile_lower[i];
			}

			if (sl.isNextCode(0x16)) {
				sl.intoCode();
				sl >> size;
				for(int i=0; i<(int)(size/sizeof(byte)); i++)
					sl >> tile_upper[i];
			}
		} else {
			sl << 0x15;
			sl << (int)(144*sizeof(byte));
			for(int i=0; i<144; i++)
				sl << tile_lower[i];

			sl << 0x16;
			sl << (int)(144*sizeof(byte));
			for(int i=0; i<144; i++)
				sl << tile_upper[i];

		}

		BaseStruct::serialize (sl, 0x20, bg_name			,"");
		BaseStruct::serialize (sl, 0x21, bg_hscroll		,false);
		BaseStruct::serialize (sl, 0x22, bg_vscroll		,false);
		BaseStruct::serialize (sl, 0x23, bg_hscrollauto	,false);
		BaseStruct::serialize (sl, 0x24, bg_hscrollauto_speed	,0);
		BaseStruct::serialize (sl, 0x25, bg_vscrollauto	,false);
		BaseStruct::serialize (sl, 0x26, bg_vscrollauto_speed	,0);
		sl.finishStruct();
	}
};

#endif
