#ifndef __R2K_DATA__LDB_ENEMY_GROUP_MEMBER__
#define __R2K_DATA__LDB_ENEMY_GROUP_MEMBER__

#include <iostream>
#include "../BaseStruct.h"

class LdbEnemyGroupMember : public BaseStruct
{
public: 
	/*01*/int	enemy_id;
	/*02*/int	x;
	/*03*/int	y;
	/*04*/bool	invisible;

public:
	void init() {
		/*01*/enemy_id = 1;
		/*02*/x = 0;
		/*03*/y = 0;
		/*04*/invisible = false;

	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, enemy_id);
		BaseStruct::serialize (sl, 0x02, x);
		BaseStruct::serialize (sl, 0x03, y);
		BaseStruct::serialize (sl, 0x04, invisible);
		sl.finishStruct();
	}
};

#endif
