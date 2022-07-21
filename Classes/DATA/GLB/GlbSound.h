#ifndef __R2K_DATA__GLB_SOUND__
#define __R2K_DATA__GLB_SOUND__

#include <iostream>
#include "../BaseStruct.h"

class GlbSound : public BaseStruct
{
public: 
	GlbSound *m_link;

	TString name;
	int		volume;
	int		tempo;
	int		balance;

public:
	GlbSound()
		:m_link(NULL)
	{}

	void init() {
		name	= "";
		volume	= 100;
		tempo   = 100;
		balance = 50;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

        //TODO
//		if (m_link != NULL)
//			BaseStruct::serialize (sl, 0x01, name, m_link->name, true, false);
//		else
			BaseStruct::serializeD(sl, 0x01, name);

		BaseStruct::serialize (sl, 0x02, volume,	100);
		BaseStruct::serialize (sl, 0x03, tempo,	100);
		BaseStruct::serialize (sl, 0x04, balance,	50);

		sl.finishStruct();
	}

	void link( GlbSound* link ) 
	{
		m_link = link;
	}


};

#endif
