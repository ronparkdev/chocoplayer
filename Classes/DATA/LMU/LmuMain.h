#ifndef __R2K_DATA__LMU_MAIN__
#define __R2K_DATA__LMU_MAIN__

#include <iostream>
#include "../BaseStruct.h"
#include "LmuEvent.h"

#define LMU_HEADER "LcfMapUnit"

enum ScrollType{
	scroll_none			= 0,
	scroll_horizonal	= 1,
	scroll_vertical		= 2,
	scroll_both			= 3
};

class LmuMain : public BaseStruct
{
public: 
	TString header;

	/*01*/int	chipSetID;
	/*02*/int	width;
	/*03*/int	height;
	/*0B*/int	scrollType;
	/*1F*/bool	usePanorama;
	/*20*/TString bg_name;
	/*21*/bool	bg_hscroll;
	/*22*/bool	bg_vscroll;
	/*23*/bool	bg_hscrollauto;
	/*24*/int	bg_hscrollauto_speed;
	/*25*/bool	bg_vscrollauto;
	/*26*/int	bg_vscrollauto_speed;
	/*47*/ArrayList<unsigned short> lowerMap;
	/*48*/ArrayList<unsigned short> upperMap;
	/*51*/ArrayList<LmuEvent> Events;
public:
	void init() {
		header = LMU_HEADER;

		/*01*/chipSetID = 1;
		/*02*/width     = 20;
		/*03*/height    = 15;
		/*0B*/scrollType = 0;
		/*1F*/usePanorama = false;
		/*20*/bg_name = "";
		/*21*/bg_hscroll = false;
		/*22*/bg_vscroll = false;
		/*23*/bg_hscrollauto = false;
		/*24*/bg_hscrollauto_speed = 0;
		/*25*/bg_vscrollauto = false;
		/*26*/bg_vscrollauto_speed = 0;
		/*47*/lowerMap.RemoveAll();
		/*48*/upperMap.RemoveAll();
		/*51*/Events.RemoveAll();
	}

	void serialize(StructLoader &sl) {
		init();
		BaseStruct::serialize(sl, header);
		if (header == LMU_HEADER) {
			BaseStruct::serialize (sl, 0x01, chipSetID);
			BaseStruct::serialize (sl, 0x02, width);
			BaseStruct::serialize (sl, 0x03, height);
			BaseStruct::serialize (sl, 0x0B, scrollType);
			BaseStruct::serialize (sl, 0x1F, usePanorama);
			BaseStruct::serialize (sl, 0x20, bg_name);
			BaseStruct::serialize (sl, 0x21, bg_hscroll);
			BaseStruct::serialize (sl, 0x22, bg_vscroll);
			BaseStruct::serialize (sl, 0x23, bg_hscrollauto);
			BaseStruct::serialize (sl, 0x24, bg_hscrollauto_speed);
			BaseStruct::serialize (sl, 0x25, bg_vscrollauto);
			BaseStruct::serialize (sl, 0x26, bg_vscrollauto_speed);
			BaseStruct::serializeEmpty(sl, 0x2A);
			BaseStruct::serializeEmpty(sl, 0x32);
			BaseStruct::serializeEmpty(sl, 0x3C);
			BaseStruct::serializeEmpty(sl, 0x3D);
			BaseStruct::serializeEmpty(sl, 0x3E);
			BaseStruct::serialize (sl, 0x47, lowerMap);
			BaseStruct::serialize (sl, 0x48, upperMap);
			BaseStruct::serialize(sl, 0x51, Events);
		}
	}
};

#endif
