#ifndef __R2K_DATA__LDB_BATTLE_ANIMATION_DATA__
#define __R2K_DATA__LDB_BATTLE_ANIMATION_DATA__

#include <iostream>
#include "../BaseStruct.h"

class LdbBattleAniData : public BaseStruct
{
public: 
	/*03*/bool	weapon_usebattle;
	/*04*/int	weapon_anim;	
	/*0D*/int	weapon_battle_anim;

	/*05*/int	movement;		
	/*06*/int	after_image;
	/*07*/int	attacks;

	/*08*/bool	ranged;
	/*09*/int	ranged_anim;
	/*0C*/int	ranged_speed;

	/*0E*/int	skill_pose;
public:
	void init() {
		/*03*/weapon_usebattle	= false;
		/*04*/weapon_anim		= 1;
		/*0D*/weapon_battle_anim= 1;		
	
		/*05*/movement			= 0;			
		/*06*/after_image		= 0;
		/*07*/attacks			= 0;		//0:1 attack 1:2 attacks 2:3 attacks 

		/*08*/ranged			= false;
		/*09*/ranged_anim		= 0;		//
		/*0C*/ranged_speed		= 0; 		//0Fast - 2Slow

		/*0E*/skill_pose = 3;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x03, weapon_usebattle);
		BaseStruct::serialize (sl, 0x04, weapon_anim);

		BaseStruct::serialize (sl, 0x05, movement);
		BaseStruct::serialize (sl, 0x06, after_image);
		BaseStruct::serialize (sl, 0x07, attacks);

		BaseStruct::serialize (sl, 0x08, ranged);
		BaseStruct::serialize (sl, 0x09, ranged_anim);
		BaseStruct::serialize (sl, 0x0C, ranged_speed);
		BaseStruct::serialize (sl, 0x0D, weapon_battle_anim);

		BaseStruct::serialize (sl, 0x0E, skill_pose);

		sl.finishStruct();
	}
};

#endif
