#ifndef __R2K_DATA__LDB_ENEMY_GROUP__
#define __R2K_DATA__LDB_ENEMY_GROUP__

#include <iostream>
#include "../BaseStruct.h"
#include "LdbEnemyGroupMember.h"
#include "LdbEnemyGroupEventPage.h"

class LdbEnemyGroup : public BaseStruct
{
public: 
	/*01*/TString	name;
	/*02*/ArrayList<LdbEnemyGroupMember> members;
	/*04*/int		terrain_set_count;
	/*05*/ArrayList<bool> terrain_set;
	/*0B*/ArrayList<LdbEnemyGroupEventPage> pages;

public:
	void init() {
		/*01*/name		= "";
		/*02*/members.RemoveAll();
		/*04*/terrain_set_count = 0;
		/*05*/terrain_set.RemoveAll();
		/*0B*/pages.RemoveAll();
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, name);
		BaseStruct::serialize(sl, 0x02, members);
		BaseStruct::serialize (sl, 0x04, terrain_set_count);
		BaseStruct::serialize (sl, 0x05, terrain_set);
		BaseStruct::serialize(sl, 0x0B, pages);
		sl.finishStruct();
	}
};

#endif
