#ifndef __R2K_DATA__LDB_ANIMATION__
#define __R2K_DATA__LDB_ANIMATION__

#include <iostream>
#include "../BaseStruct.h"
#include "LdbAniTiming.h"
#include "LdbAniFrame.h"

enum AniScope {
	AniScope_target = 0,
	AniScope_screen = 1
};

enum AniPos {
	AniPos_Up = 0,
	AniPos_Mid = 1,
	AniPos_Down = 2
};

class LdbAni : public BaseStruct
{
public: 
	/*01*/TString	name;
	/*02*/TString	animation_name;
	/*05*/int		timings_length;
	/*06*/ArrayList<LdbAniTiming> timings;
	/*09*/int		scope;
	/*0A*/int		position;
	/*0B*/int		frames_length;
	/*0C*/ArrayList<LdbAniFrame> frames;


public:
	void init() {
		name			= "";
		animation_name	= "";

		timings_length	= 0;
		timings.RemoveAll();

		scope			= AniScope_target;
		position		= AniPos_Down;

		frames_length	= 0;
		frames.RemoveAll();
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize (sl, 0x02, animation_name);
		BaseStruct::serialize (sl, 0x05, timings_length);
		BaseStruct::serialize(sl, 0x06, timings);
		BaseStruct::serialize (sl, 0x09, scope);
		BaseStruct::serialize (sl, 0x0A, position);
		BaseStruct::serialize (sl, 0x0B, frames_length);
		BaseStruct::serialize(sl, 0x0C, frames);
		sl.finishStruct();
	}
};

#endif
