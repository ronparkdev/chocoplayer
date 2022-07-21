#ifndef __R2K__TSECURE__
#define __R2K__TSECURE__
#include "../COMMON/DebugingCode.h"

#include "../DATA/TString.h"

class TSequre{
public:
	static TString getMD5( const char *data, int datalen );
	static TString getMD5( const TString &data );
};

#endif