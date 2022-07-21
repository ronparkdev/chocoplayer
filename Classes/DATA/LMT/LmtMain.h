#ifndef __R2K_DATA__LMT_MAIN__
#define __R2K_DATA__LMT_MAIN__

#include <iostream>
#include "../BaseStruct.h"
#include "LmtMapInfo.h"
#include "LmtMapsInfo.h"

class LmtMain : public BaseStruct
{
public: 
	TString header;

	ArrayList<LmtMapInfo>	MInfo;
	LmtMapsInfo				Info;
	ArrayList<int>			MTable;

public:
	void init();

	void serialize(StructLoader &sl);

};

#endif
