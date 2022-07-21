#ifndef __R2K_DATA__LSD_PREVIEW__
#define __R2K_DATA__LSD_PREVIEW__

#include <iostream>
#include "../BaseStruct.h"

#define MAX_HERO 4

class LsdPreview : public BaseStruct
{
public: 
	/*01*/double Time;
	/*0B*/TString Hero1Name;
	/*0C*/int Hero1Level;
	/*0D*/int Hero1HP;

	/*15*/TString HeroFaceName[MAX_HERO];
	/*16*/int	  HeroFaceID[MAX_HERO];

public:
	void serialize(StructLoader &sl) {
		BaseStruct::serialize (sl, 0x01, Time			,(double)0.0f);
		BaseStruct::serialize (sl, 0x0B, Hero1Name	,""	,true);
		BaseStruct::serialize (sl, 0x0C, Hero1Level	,0);
		BaseStruct::serialize (sl, 0x0D, Hero1HP		,0);
 		for(int i=0; i<MAX_HERO; i++) {
			BaseStruct::serialize (sl, 0x15 + 2 * i, HeroFaceName[i]	,"");
			BaseStruct::serialize (sl, 0x16 + 2 * i, HeroFaceID[i]	,0);
		}
		sl.finishStruct();
	}

};

#endif
