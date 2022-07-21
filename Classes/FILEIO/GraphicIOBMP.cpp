#include "GraphicIO.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

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

#define bmpColorByte 4

struct bmpColorTable{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char reserved;
};

bool GraphicIO::loadPictureBMP( char *data, int datasize , CCImage &image , bool useAlpha)
{
	bool isSuccess = false;

	ifstream fin;

	short	dHeader_Sign;		//'BM'
	int		dHeader_FileSize;
	int		dHeader_reserved;
	int		dHeader_DataOffeset;

	int		dInfo_Size;
	int		dInfo_Width;
	int		dInfo_Height;
	short	dInfo_Planes;
	short	dInfo_BitCount;
	int		dInfo_NumColors;
	int		dInfo_Compression;
	int		dInfo_ImageSize;
	int		dInfo_XpixelsPerMeter;
	int		dInfo_YpixelsPerMeter;
	int		dInfo_ColorsUsed;
	int		dInfo_ColorsImportant;

	bmpColorTable	*dColorTable;
	char		*dRasterData;
	char		*dRGBData;

	int pos = 0;

	//Check it is bitmap
	readint(data, pos, dHeader_Sign);

	if (dHeader_Sign != 19778)
		return false;

	//Read file header
	readint(data, pos, dHeader_FileSize);
	readint(data, pos, dHeader_reserved);
	readint(data, pos, dHeader_DataOffeset);

	//Read image information
	readint(data, pos, dInfo_Size);
	readint(data, pos, dInfo_Width);
	readint(data, pos, dInfo_Height);
	readint(data, pos, dInfo_Planes);
	readint(data, pos, dInfo_BitCount);

	dInfo_NumColors = 1 << dInfo_BitCount;

	readint(data, pos, dInfo_Compression);
	readint(data, pos, dInfo_ImageSize);
	readint(data, pos, dInfo_XpixelsPerMeter);
	readint(data, pos, dInfo_YpixelsPerMeter);
	readint(data, pos, dInfo_ColorsUsed);
	readint(data, pos, dInfo_ColorsImportant);

	//Read Color Table
	dColorTable = new bmpColorTable[dInfo_NumColors];
	memcpy(dColorTable, data + pos, sizeof(bmpColorTable) * dInfo_NumColors);
	pos += sizeof(bmpColorTable) * dInfo_NumColors;

	if (dInfo_ImageSize == 0) {
		dInfo_ImageSize = dInfo_Width * dInfo_Height * dInfo_BitCount*dInfo_Planes/8;
	}

	//Read Raster Data
	dRasterData = new char[dInfo_ImageSize];
	memcpy(dRasterData, data + pos, dInfo_ImageSize);
	pos += dInfo_ImageSize;

	int x, y, i, c;

	char *rgbpos, *datapos;

	//Init color table
	if (useAlpha) {
		dColorTable[0].reserved = 0x00;
		for(i=1; i<dInfo_NumColors; i++)
			dColorTable[i].reserved = 0xFF;
	} else {
		for(i=0; i<dInfo_NumColors; i++)
			dColorTable[i].reserved = 0xFF;
	}

	//Prepare reading
	int data_per_byte = 8/dInfo_BitCount;
	int byte_per_data = dInfo_BitCount/8;
	int byte_per_line, bit_shift_count, bit_mask;
	if (byte_per_data == 0) {
		bit_shift_count = 8 - dInfo_BitCount;
		bit_mask = 0xFF>>bit_shift_count;
		byte_per_line = (int)ceil(dInfo_Width/8.0f*dInfo_BitCount);
	} else {
		bit_shift_count = dInfo_BitCount;
		bit_mask = 0;
		for(i=0; i<byte_per_data; i++)
			bit_mask = (bit_mask << 8) | 0xFF;
		byte_per_line = dInfo_Width;
	}
	int padding_per_y = 0;
	while(padding_per_y < byte_per_line)
		padding_per_y += 4;

	//dInfo_ImageSize = dInfo_Width * dInfo_Height;

	//Read from Raster Data to RGB Data
	rgbpos = dRGBData = new char[dInfo_Width * dInfo_Height * bmpColorByte];
	datapos = dRasterData + dInfo_ImageSize - padding_per_y;

	padding_per_y += byte_per_line;

	if (byte_per_data == 0) {
		for(y=0; y<dInfo_Height; y++) {
			for(x=0; x<dInfo_Width; x++) {
				if ((x % data_per_byte) == 0)
					c = *(datapos++);

				memcpy(rgbpos, dColorTable + ((c>>bit_shift_count) & bit_mask), bmpColorByte);
				rgbpos += bmpColorByte;
				c<<=dInfo_BitCount;
			}

			datapos -= padding_per_y;
		}
	} else {
		for(y=0; y<dInfo_Height; y++) {
			for(x=0; x<dInfo_Width; x++) {
				c = 0;
				for(i=byte_per_data-1; i>=0; i--)
					c = (c << 8) | (0xFF & *(datapos + i));
				datapos += byte_per_data;
				memcpy(rgbpos, dColorTable + c, bmpColorByte);
				rgbpos += bmpColorByte;
			}
			datapos -= padding_per_y;
		}
	}

	//Coonvert from RGB Data to CCImage
	isSuccess = image.initWithImageData((void*)dRGBData, dInfo_Width*dInfo_Height, CCImage::kFmtRawData, dInfo_Width, dInfo_Height, 32);

	//Release Mem
	delete []dColorTable;
	delete []dRasterData;
	delete []dRGBData;

	return isSuccess;
}