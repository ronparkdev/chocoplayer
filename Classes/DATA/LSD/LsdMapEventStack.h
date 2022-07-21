#ifndef __R2K_DATA__LSD_Map_EVENT_STACK__
#define __R2K_DATA__LSD_Map_EVENT_STACK__

#include <iostream>
#include "../BaseStruct.h"

#include "LsdMapEventData.h"
class LsdMapEventStack : public BaseStruct
{
public: 
	/*01*/ArrayList<LsdMapEventData> data;
	/*1F*/int wait_left;//frame단위, 60=1s, 30=0.5s
	int m_wait_key;

	int m_key_arrow_last;
	int m_key_enter_last;
	int m_key_esc_last;
	int m_key_num_last;
	int m_key_special_last;

public:
	void serialize(StructLoader &sl) {
		m_wait_key = 0;
		m_key_arrow_last	= 0;
		m_key_enter_last	= 0;
		m_key_esc_last		= 0;
		m_key_num_last		= 0;
		m_key_special_last	= 0;

		BaseStruct::serialize(sl, 0x01, data);
		data.ChangeMode(ListMode);

		BaseStruct::serialize (sl, 0x1F, wait_left	,0);

		sl.finishStruct();
	}

};

#endif
