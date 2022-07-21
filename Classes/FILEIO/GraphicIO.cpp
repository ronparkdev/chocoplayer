#include "GraphicIO.h"

#include "TFolder.h"

template <typename T>
inline void readint( char *dataf, int &datapos, T &val) {
	char data[sizeof(T)];
	int i, j;
	memcpy(data, dataf + datapos, sizeof(T));

	datapos += sizeof(T);

	val = 0;
	j = sizeof(T);
	for(i=0; i<sizeof(T); ++i)
		val = (val << 8) | (data[--j] & 0xFF);
}

bool GraphicIO::loadPicture( char *data, int datasize , CCImage &image , bool useAlpha)
{
	bool result = false;

	int t = 0, header;
	readint(data, t, header);

	if (		header == 828004696)
		result = loadPictureXYZ( data , datasize , image , useAlpha);
	else if (header == 1196314761)
		result = loadPicturePNG( data , datasize, image , useAlpha);
	else if ((header&0xFFFF) == 19778)//TODO CHK
		result = loadPictureBMP( data , datasize, image , useAlpha);
	
	return result;
}
