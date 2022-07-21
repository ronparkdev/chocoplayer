#ifndef __R2K_DATA__LSD_STATE__
#define __R2K_DATA__LSD_STATE__

#include <iostream>
#include "../BaseStruct.h"
#include "../LDB/LdbMain.h"

class LsdState : public BaseStruct
{
public: 
	/*01*/int party_length;
	/*02*/ArrayList<short> party;

	/*0B*/int item_length;
	/*0C*/ArrayList<unsigned short> item_index;
	/*0D*/ArrayList<byte> item_count;
	/*0E*/ArrayList<byte> item_used;

	/*15*/int money;

	/*17*/int timer1_leftframe;
	/*18*/bool timer1_using;
	/*19*/bool timer1_visible;
	/*1A*/bool timer1_runinbattle;

	/*1B*/int timer2_leftframe;
	/*1C*/bool timer2_using;
	/*1D*/bool timer2_visible;
	/*1E*/bool timer2_runinbattle;

	/*20*/int count_battle;
	/*21*/int count_lose;
	/*22*/int count_escape;
	/*23*/int count_victory;
	/*29*/int count_save;
	/*2A*/int save_slot;
public:
	void serialize(StructLoader &sl) {

		if (sl.getMode() == StructLoaderModeInit) {
			item_length = 0;
			item_index.RemoveAll();
			item_count.RemoveAll();
			item_used.RemoveAll();
		}

		if (sl.getMode() == StructLoaderModeWrite) {
			party_length = party.GetLength();
			item_length = item_index.GetLength();
		}


		BaseStruct::serialize (sl, 0x01, party_length);
		BaseStruct::serialize (sl, 0x02, party);

		BaseStruct::serialize (sl, 0x0B, item_length);
		BaseStruct::serialize (sl, 0x0C, item_index);
		BaseStruct::serialize (sl, 0x0D, item_count);
		BaseStruct::serialize (sl, 0x0E, item_used);

		BaseStruct::serialize (sl, 0x15, money		,0);

		BaseStruct::serialize (sl, 0x17, timer1_leftframe		,0);
		BaseStruct::serialize (sl, 0x18, timer1_using			,false);
		BaseStruct::serialize (sl, 0x19, timer1_visible		,false);
		BaseStruct::serialize (sl, 0x1A, timer1_runinbattle	,false);

		BaseStruct::serialize (sl, 0x1B, timer2_leftframe		,0);
		BaseStruct::serialize (sl, 0x1C, timer2_using			,false);
		BaseStruct::serialize (sl, 0x1D, timer2_visible		,false);
		BaseStruct::serialize (sl, 0x1E, timer2_runinbattle	,false);

		BaseStruct::serialize (sl, 0x20, count_battle		,0);
		BaseStruct::serialize (sl, 0x21, count_lose		,0);
		BaseStruct::serialize (sl, 0x22, count_escape		,0);
		BaseStruct::serialize (sl, 0x23, count_victory	,0);
		BaseStruct::serialize (sl, 0x29, count_save		,0);
		BaseStruct::serialize (sl, 0x2A, save_slot		,0);
		sl.finishStruct();
	}

	void newgame( LdbMain & ldb ) 
	{
		party = ldb.dSys.party;
	}


};

#endif
