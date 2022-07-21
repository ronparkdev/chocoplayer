#include "GraphicIO.h"

bool GraphicIO::loadPictureXYZ( char *data, int datasize , CCImage &image , bool useAlpha)
{
	bool isSuccess = false;

	ifstream file;

	long datalen = datasize;
	Bytef *decdata;
	uLongf decdatalen;
	int *bitarr;

	int imgw, imgh, palette[256], pos;

	imgw = ((data[5]&0xFF)<<8) | (data[4]&0xFF);		
	imgh = ((data[7]&0xFF)<<8) | (data[6]&0xFF);

	decdatalen = imgw*imgh+256*3;
	decdata = new Bytef[decdatalen];

	int result = uncompress(decdata, &decdatalen, (Bytef *)data + 8, datalen - 8);

	if (result == Z_OK) {

		pos=0;

		for(int i=0 ; i<256 ; i++ ) {
			palette[i] = 0xFF000000 |(0xFF & decdata[pos++]);
			palette[i] |= ((0xFF & decdata[pos++])<<8);
			palette[i] |= ((0xFF & decdata[pos++])<<16);
		}

		if (useAlpha)
			palette[0] ^= 0xFF000000;

		bitarr = new int[imgw*imgh];

		for(int i=0 ; i<imgw*imgh ; i++)
			bitarr[i] = palette[0xFF & decdata[pos++]];


		if (image.initWithImageData((void*)bitarr, imgw*imgh, CCImage::kFmtRawData, imgw, imgh, 32))
			isSuccess = true;

		delete []bitarr;
	}

	delete []decdata;

	return isSuccess;
}