#ifndef __R2K_DATA__LDB_CHIPSET__
#define __R2K_DATA__LDB_CHIPSET__

#include <iostream>
#include "../BaseStruct.h"

#define CHIPSET_LEN 162;

enum ChipAniType{
	ChipAniType_1232 = 0,
	ChipAniType_123	 = 1
};

enum ChipAniSpeed{
	ChipAniSpeed_Slow = 0,
	ChipAniSpeed_Fast = 1
};

class LdbChipset : public BaseStruct
{
public: 

	/*01*/TString	Name;
	/*02*/TString	ChipsetName; 
	/*03*/ArrayList<unsigned short> TerrainData;
	/*04*/ArrayList<byte> passableDataLower;
	/*05*/ArrayList<byte> passableDataUpper;
	/*0B*/int		Animation_Type;
	/*0C*/int		Animation_Speed;

public:
	void init() {
		/*01*/Name				= "";
		/*02*/ChipsetName		= "";
		/*03*/TerrainData.RemoveAll();
		/*04*/passableDataLower.RemoveAll();
		/*05*/passableDataUpper.RemoveAll();
		/*0B*/Animation_Type	= ChipAniType_1232;
		/*0C*/Animation_Speed	= ChipAniSpeed_Slow;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, Name);
		BaseStruct::serialize (sl, 0x02, ChipsetName);
		BaseStruct::serialize (sl, 0x03, TerrainData);
		BaseStruct::serialize (sl, 0x04, passableDataLower);
		BaseStruct::serialize (sl, 0x05, passableDataUpper);
		BaseStruct::serialize (sl, 0x0B, Animation_Type);
		BaseStruct::serialize (sl, 0x0C, Animation_Speed);
		sl.finishStruct();
	}
};

#endif
