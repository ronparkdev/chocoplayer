#ifndef __R2K_DATA__LMU_EVENT_TRIGGER__
#define __R2K_DATA__LMU_EVENT_TRIGGER__

#include <iostream>
#include "../BaseStruct.h"
#include "../GLB/GlbEvent.h"
#include "LmuEventPage.h"

enum EventTriggerType{
	Switch1		=1,
	Switch2		=2,
	Variable	=4,
	Item		=8,
	Character	=16,
	Timer1		=32,
	Timer2		=64
};

enum EventTriggerVarType{
	Same,
	AndMore,
	AndLess,
	MoreThen,
	LessThen
};

class LmuEventTrigger : public BaseStruct
{
public: 
	/*01*/int flag;//switch1=1, switch2=2, variable=4, item=8, character=16, timer1=32, timer2=64
	/*02*/int switch1id;
	/*03*/int switch2id;
	/*04*/int variableid;
	/*05*/int variabledata;
	/*10*/int variableif;
	/*06*/int itemid;
	/*07*/int characterid;
	/*08*/int timer1data;
	/*09*/int timer2data;

public:
	void init() {	
		/*01*/flag = 0;
		/*02*/switch1id = 1;
		/*03*/switch2id = 1;
		/*04*/variableid = 1;
		/*05*/variabledata = 0;
		/*06*/itemid = 1;
		/*07*/characterid = 1;
		/*08*/timer1data = 0;
		/*09*/timer2data = 0;
		/*0A*/variableif = AndMore;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();
		
		BaseStruct::serialize (sl, 0x01, flag);
		BaseStruct::serialize (sl, 0x02, switch1id);
		BaseStruct::serialize (sl, 0x03, switch2id);
		BaseStruct::serialize (sl, 0x04, variableid);
		BaseStruct::serialize (sl, 0x05, variabledata);
		BaseStruct::serialize (sl, 0x06, itemid);
		BaseStruct::serialize (sl, 0x07, characterid);
		BaseStruct::serialize (sl, 0x08, timer1data);
		BaseStruct::serialize (sl, 0x09, timer2data);
		BaseStruct::serialize (sl, 0x0A, variableif);
		sl.finishStruct();
	}
};

#endif
