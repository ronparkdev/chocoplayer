#ifndef __R2K_DATA__GLB_MUSIC__
#define __R2K_DATA__GLB_MUSIC__

#include <iostream>
#include "../BaseStruct.h"

class GlbMusic : public BaseStruct
{
public:
	GlbMusic *m_link;

	/*01*/TString name;
	/*02*/int	fadein;
	/*03*/int	volume;
	/*04*/int	tempo;
	/*05*/int	balance;

public:
	GlbMusic()
		:m_link(NULL)
	{}

	void init() {
		/*01*/name	= "";
		/*02*/fadein = 0;
		/*03*/volume = 100;
		/*04*/tempo = 100;
		/*05*/balance = 50;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		if (m_link != NULL)
			BaseStruct::serialize (sl, 0x01, name, m_link->name, true, false);
		else
			BaseStruct::serialize (sl, 0x01, name);

		BaseStruct::serialize (sl, 0x02, fadein	,0);
		BaseStruct::serialize (sl, 0x03, volume	,100);
		BaseStruct::serialize (sl, 0x04, tempo	,100);
		BaseStruct::serialize (sl, 0x05, balance	,50);

		sl.finishStruct();
	}

	void link( GlbMusic *link ) 
	{
		m_link = link;
	}


};

#endif
