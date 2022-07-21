#ifndef __R2K_DATA__GLB_SKILL_LEARNING__
#define __R2K_DATA__GLB_SKILL_LEARNING__

#include <iostream>
#include "../BaseStruct.h"

class GlbSkillLearning : public BaseStruct
{
public: 
	int level;
	int skill_id;

public:
	void init() {
		level = 1;
		skill_id = 1;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, level);
		BaseStruct::serialize (sl, 0x02, skill_id);
		sl.finishStruct();
	}
};

#endif
