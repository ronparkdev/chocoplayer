#ifndef __R2K_GRAPHIC__GRAPHIC_IO__
#define __R2K_GRAPHIC__GRAPHIC_IO__
#include "../COMMON/DebugingCode.h"

typedef unsigned long ULONG;

#include <iostream>
#include <fstream>
#include <algorithm>

#include "cocos2d.h"
USING_NS_CC;

#include "zlib_1.2.3/zlib.h"
#include "../DATA/TString.h"

using namespace std;

class GraphicIO
{
public:
	bool loadPicture( char *data, int datasize , CCImage &image , bool useAlpha);
private: 
	bool loadPictureXYZ( char *data, int datasize , CCImage &image , bool useAlpha = true);

	bool loadPicturePNG( char *data, int datasize , CCImage &image , bool useAlpha = true);

	bool loadPicturePNG_Header( char *data, ULONG datalen, ULONG &pos);

	bool loadPicturePNG_IHDR( char *data, ULONG datalen, ULONG &pos, 
								int &simgW, int &simgH, int &sBitDepth, int &sColorType, int &sCompression, int &sFilter);

	bool loadPicturePNG_PLTE( char *data, ULONG datalen, ULONG &pos, 
								bool useAlpha, int palette[]);

	bool loadPicturePNG_IDAT( char *data, ULONG datalen, ULONG &pos, 
								int &dat_startpos, int &dat_length);

	bool loadPicturePNG_Proc( Bytef *data, unsigned long datalen, ULONG &pos , CCImage &image, 
								int simgW, int simgH, int dat_startpos, int dat_length, int palette[], int sFilter);

	bool loadPictureBMP( char *data, int datasize , CCImage &image , bool useAlpha);
};

#endif