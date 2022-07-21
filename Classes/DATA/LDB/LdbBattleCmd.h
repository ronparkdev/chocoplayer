#ifndef __R2K_DATA__LDB_BATTLE_COMMEND__
#define __R2K_DATA__LDB_BATTLE_COMMEND__

#include <iostream>
#include "../BaseStruct.h"

class LdbBattleCmdNode;

enum BattleType{
	BatType_A = 0,
	BatType_B = 1,
	BatType_G = 2
};

class LdbBattleCmd : public BaseStruct
{
public: 
	/*02*/bool enemy_auto_arrange;

	/*07*/int battle_type;//0:A Type, 1:B Type 2:Gaze Type

	/*14*/bool	window_small;
	/*18*/bool	window_semi_invisible;

	/*04*/bool	gameover_run_commonevent;

	/*09*/int	commands_length;
	/*0A*/ArrayList<LdbBattleCmdNode> commends;

	/*10*/int	gameover_run_commonevent_id;
	/*19*/bool	gameover_mapmove;
	/*1A*/int	gameover_mapmove_id;
	/*1B*/int	gameover_mapmove_x;
	/*1C*/int	gameover_mapmove_y;
	/*1D*/int	gameover_mapmove_direction;

public:
	void init() {
		/*02*/enemy_auto_arrange = false;

		/*07*/battle_type = BatType_A;//0:A Type, 1:B Type 2:Gaze Type

		/*14*/window_small = false;
		/*18*/window_semi_invisible = false;

		/*04*/gameover_run_commonevent = false;
		/*0A*/commends.RemoveAll();

		/*10*/gameover_run_commonevent_id = 0;
		/*19*/gameover_mapmove = false;
		/*1A*/gameover_mapmove_id = 1;
		/*1B*/gameover_mapmove_x = 0;
		/*1C*/gameover_mapmove_y = 0;
		/*1D*/gameover_mapmove_direction = 0;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x02, enemy_auto_arrange);
		BaseStruct::serialize (sl, 0x04, gameover_run_commonevent);
		BaseStruct::serialize (sl, 0x07, battle_type);
		BaseStruct::serialize (sl, 0x09, commands_length);
		BaseStruct::serialize(sl, 0x0A, commends);
		BaseStruct::serialize (sl, 0x10, gameover_run_commonevent_id);
		BaseStruct::serialize (sl, 0x14, window_small);
		BaseStruct::serialize (sl, 0x18, window_semi_invisible);
		BaseStruct::serialize (sl, 0x19, gameover_mapmove);
		BaseStruct::serialize (sl, 0x1A, gameover_mapmove_id);
		BaseStruct::serialize (sl, 0x1B, gameover_mapmove_x);
		BaseStruct::serialize (sl, 0x1C, gameover_mapmove_y);
		BaseStruct::serialize (sl, 0x1D, gameover_mapmove_direction);
		sl.finishStruct();
	}
};

class LdbBattleCmdNode : public BaseStruct
{
public: 
	/*01*/TString cname;
	/*02*/int ctype;

public:
	void init() {
		cname = "";
		ctype = 0;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x01, cname);
		BaseStruct::serialize (sl, 0x02, ctype);
		sl.finishStruct();
	}
};

#endif
