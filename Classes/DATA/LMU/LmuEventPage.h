#ifndef __R2K_DATA__LMU_EVENT_PAGE__
#define __R2K_DATA__LMU_EVENT_PAGE__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbEvent.h"
#include "../GLB/GlbMoveRoute.h"
#include "LmuEventTrigger.h"

enum DrawPriority{
	drawpri_low		= 0,
	drawpri_mid		= 1,
	drawpri_high	= 2
};

enum ObjectDirect{
	direct_up = 1,
	direct_right = 2,
	direct_down = 3,
	direct_left = 4,
	direct_upright = 5,
	direct_downright = 6,
	direct_downleft = 7,
	direct_upleft = 8
};

enum ObjectMoveType{
	movetype_none = 0,
	movetype_random = 1,
	movetype_vertial = 2,
	movetype_horizonal = 3,
	movetype_to_hero = 4,
	movetype_away_from_hero = 5,
	movetype_custom = 6
};

enum MapTriggerType{
	MapEvtTrigger_key = 0,
	MapEvtTrigger_touchhero = 1,
	MapEvtTrigger_touchevent = 2,
	MapEvtTrigger_Autometic = 3,
	MapEvtTrigger_Parallel = 4
};

class LmuEventPage : public BaseStruct
{
public: 
	/*02*/LmuEventTrigger trigger;
	/*15*/TString pic_name;
	/*16*/int pic_id;
	/*17*/int direction;
	/*18*/int ani_walk;
	/*19*/bool semi_trans;
	/*1F*/int move_type;
	/*20*/int move_freq;
	/*21*/int trigger_type;
	/*22*/int draw_priority;
	/*23*/bool draw_dont_over;
	/*24*/int ani_type;
	/*25*/int move_speed;
	/*29*/GlbMoveRoute move_route;
	/*33*/int event_length;
	/*34*/GlbEvent event_data;

public:
	void init() {

		/*15*/pic_name = "";
		/*16*/pic_id = 0;
		/*17*/direction = 2;
		/*18*/ani_walk = 1;
		/*19*/semi_trans = false;
		/*1F*/move_type = movetype_none;
		/*20*/move_freq = 3;
		/*21*/trigger_type = 0;
		/*22*/draw_priority = drawpri_mid;
		/*23*/draw_dont_over = false;
		/*24*/ani_type = 0;
		/*25*/move_speed = 3;

		/*33*/event_length = 0;

	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x02, &trigger);
		BaseStruct::serialize (sl, 0x15, pic_name);
		BaseStruct::serialize (sl, 0x16, pic_id);
		BaseStruct::serialize (sl, 0x17, direction);
		BaseStruct::serialize (sl, 0x18, ani_walk);
		BaseStruct::serialize (sl, 0x19, semi_trans);
		BaseStruct::serialize (sl, 0x1F, move_type);
		BaseStruct::serialize (sl, 0x20, move_freq);
		BaseStruct::serialize (sl, 0x21, trigger_type);
		BaseStruct::serialize (sl, 0x22, draw_priority);
		BaseStruct::serialize (sl, 0x23, draw_dont_over);
		BaseStruct::serialize (sl, 0x24, ani_type);
		BaseStruct::serialize (sl, 0x25, move_speed);
		BaseStruct::serialize (sl, 0x29, &move_route);
		BaseStruct::serialize (sl, 0x33, event_length);
		BaseStruct::serialize (sl, 0x34, &event_data);
		sl.finishStruct();
	}
};

#endif
