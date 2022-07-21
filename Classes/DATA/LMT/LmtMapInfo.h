#ifndef __R2K_DATA__LMT_MAP_INFO__
#define __R2K_DATA__LMT_MAP_INFO__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbMusic.h"
#include "LmtEnemysInfo.h"

enum MapInfoType{
	MapInfoType_root	= 0,
	MapInfoType_map		= 1,
	MapInfoType_area	= 2
};

enum MapInfoExpand{
	MapInfoExpand_inherit	= 0,
	MapInfoExpand_byevent	= 1,
	MapInfoExpand_specified = 2
};

enum MapInfoBG{
	MapInfoBG_inherit		= 0,
	MapInfoBG_terrain		= 1,
	MapInfoBG_specified		= 2
};

enum MapInfoFlag{
	MapInfoFlag_inherit		= 0,
	MapInfoFlag_allow		= 1,
	MapInfoFlag_disallow	= 2
};

class LmtMapInfo : public BaseStruct
{
public: 
	/*01*/TString name;
	/*02*/int	parent_map;

	/*03*/int	indentation;

	/*04*/int	type;

	/*05*/int	scrollbar_x;
	/*06*/int	scrollbar_y;

	/*07*/int	expanded_node;

	/*0B*/int	music_type;
	/*0C*/GlbMusic music;

	/*15*/int	background_type;
	/*16*/TString background_name;

	/*1F*/int	teleport;
	/*20*/int	escape;
	/*21*/int	save;

	/*29*/ArrayList<LmtEnemiesInfo> encounters_EnemysID;
	/*2C*/int	encounter_steps;

	/*33*/ArrayList<int> area;

public:
	void init() {
		/*01*/name		= "";
		/*02*/parent_map	= 0;

		/*03*/indentation	= 0;

		/*04*/type		= MapInfoType_root;

		/*05*/scrollbar_x	= 0;
		/*06*/scrollbar_y	= 0;

//		/*07*/expanded_node;

		/*0B*/music_type	= MapInfoExpand_inherit;
//		/*0C*/music;

		/*15*/background_type	= MapInfoBG_inherit;
		/*16*/background_name	= "";

		/*1F*/teleport	= MapInfoFlag_inherit;
		/*20*/escape	= MapInfoFlag_inherit;
		/*21*/save		= MapInfoFlag_inherit;

		/*29*/encounters_EnemysID.RemoveAll();
		/*2C*/encounter_steps = 25;

		/*33*/area.RemoveAll();
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, parent_map);
		BaseStruct::serialize (sl, 0x03, indentation);
		BaseStruct::serialize (sl, 0x04, type);
		BaseStruct::serialize (sl, 0x05, scrollbar_x);
		BaseStruct::serialize (sl, 0x06, scrollbar_y);
		BaseStruct::serialize (sl, 0x07, expanded_node);
		BaseStruct::serialize (sl, 0x0B, music_type);
		BaseStruct::serialize (sl, 0x0C, &music);
		BaseStruct::serialize (sl, 0x15, background_type);
		BaseStruct::serialize (sl, 0x16, background_name);
		BaseStruct::serialize (sl, 0x1F, teleport);
		BaseStruct::serialize (sl, 0x20, escape);
		BaseStruct::serialize (sl, 0x21, save);
		BaseStruct::serialize(sl, 0x29, encounters_EnemysID);
		BaseStruct::serialize (sl, 0x2C, encounter_steps);
		BaseStruct::serialize (sl, 0x33, area);
		sl.finishStruct();
	}

};

#endif
