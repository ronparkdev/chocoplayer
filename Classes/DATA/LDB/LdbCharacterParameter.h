#ifndef __R2K_DATA__LDB_CHARACTER_PARAMETER__
#define __R2K_DATA__LDB_CHARACTER_PARAMETER__

#include <iostream>
#include "../BaseStruct.h"

#define MAXLEVEL 99

class LdbCharacterParameter : public BaseStruct
{
public: 
	int level;
	unsigned short mhp[MAXLEVEL];
	unsigned short mmp[MAXLEVEL];
	unsigned short atk[MAXLEVEL];
	unsigned short def[MAXLEVEL];
	unsigned short spt[MAXLEVEL];
	unsigned short agi[MAXLEVEL];
public:
	void init() {
		for(int i=0; i<MAXLEVEL; i++) {
			mhp[i] = 0;
			mmp[i] = 0;
			atk[i] = 0;
			def[i] = 0;
			spt[i] = 0;
			agi[i] = 0;
		}
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeInit) {
			return;
		} else if (sl.getMode() == StructLoaderModeRead) {
			level = BaseStruct::size/(sizeof(short)*6);
			init();
		}
		for(int i=0; i<level; i++)
			BaseStruct::serialize (sl,mhp[i]);
		for(int i=0; i<level; i++)
			BaseStruct::serialize (sl,mmp[i]);
		for(int i=0; i<level; i++)
			BaseStruct::serialize (sl,atk[i]);
		for(int i=0; i<level; i++)
			BaseStruct::serialize (sl,def[i]);
		for(int i=0; i<level; i++)
			BaseStruct::serialize (sl,spt[i]);
		for(int i=0; i<level; i++)
			BaseStruct::serialize (sl,agi[i]);
	}
};

#endif
