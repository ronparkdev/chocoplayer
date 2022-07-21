#include "GraphicIO.h"

bool getPallete(CCImage &image, int palette[]) {
	const int pwidth = 30;
	int *bitmapArr = new int[256*pwidth*pwidth];
	for(int i=0; i<256; i++) {
		int x = i % 16;
		int y = i / 16;
		for(int ix = 0; ix<pwidth; ix++)
			for(int iy=0; iy<pwidth; iy++)
				bitmapArr[(x*pwidth+ix) + 16*pwidth*(y*pwidth+iy)] = palette[i];
	}
	bool result = image.initWithImageData((void*)bitmapArr, pwidth*pwidth*256, CCImage::kFmtRawData, pwidth*16, pwidth*16, 32);
	delete []bitmapArr;

	return result;
}

bool GraphicIO::loadPicturePNG( char *data, int datasize , CCImage &image , bool useAlpha) {
	bool isSuccess = false;

	ifstream file;

	ULONG datalen, pos;

	datalen = datasize;

	int simgW, simgH, sBitDepth, sColorType, sCompression, sFilter;
	int palette[256];

	int dat_startpos, dat_length;

	pos = 0;

	if (!loadPicturePNG_Header(data, datalen, pos))
		return false;

	if (!loadPicturePNG_IHDR(data, datalen, pos, 
		simgW, simgH, sBitDepth, sColorType, sCompression, sFilter))
		return false;

	if (!loadPicturePNG_PLTE(data, datalen, pos, useAlpha, palette))
		return false;

	dat_startpos = -1;
	dat_length = 0;

	while(loadPicturePNG_IDAT(data, datalen, pos, dat_startpos, dat_length));

	if (dat_startpos == -1) {
		pos = 0;
		while(loadPicturePNG_IDAT(data, datalen, pos, dat_startpos, dat_length));
	}

	if (dat_startpos != -1) {
		isSuccess = loadPicturePNG_Proc((Bytef *)data, datalen, pos, image, simgW, simgH, dat_startpos, dat_length, palette, sFilter);
	}

	//getPallete(image, palette);

	return isSuccess;
}

bool GraphicIO::loadPicturePNG_Header( char *data, unsigned long datalen, ULONG &pos) {
	const char PNG_HEADER[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A}; 

	int comp = memcmp(data+pos, PNG_HEADER, sizeof(PNG_HEADER));

	pos += sizeof(PNG_HEADER);

	if (pos >= datalen)
		return false;

	return (comp == 0);
}

bool GraphicIO::loadPicturePNG_IHDR( char *data, unsigned long datalen, ULONG &pos, 
											int &simgW, int &simgH, int &sBitDepth, int &sColorType, int &sCompression, int &sFilter) {
	const char IHDR_HEADER[] = {'I','H','D','R'}; 

	pos += sizeof(IHDR_HEADER);

	while(pos + sizeof(IHDR_HEADER) < datalen) {
		if (memcmp(data+pos, IHDR_HEADER, sizeof(IHDR_HEADER)) == 0)
			break;
		pos++;
	}
	
	if (pos + sizeof(IHDR_HEADER) >= datalen)
		return false;

	pos += sizeof(IHDR_HEADER);

	for(int i=0; i<4; i++)
		simgW = simgW << 8 | (data[pos++] & 0xFF);

	for(int i=0; i<4; i++)
		simgH = simgH << 8 | (data[pos++] & 0xFF);

	sBitDepth		= data[pos++];
	if (sBitDepth != 8)
		return false;

	sColorType		= data[pos++];
	sCompression	= data[pos++];
	sFilter			= data[pos++];

	return true;
}

bool GraphicIO::loadPicturePNG_PLTE( char *data, ULONG datalen, ULONG &pos, bool useAlpha, int palette[]) {
	const char PLTE_HEADER[] = {'P','L','T','E'};


	while(pos + sizeof(PLTE_HEADER) < datalen) {
		if (memcmp(data+pos, PLTE_HEADER, sizeof(PLTE_HEADER)) == 0)
			break;
		pos++;
	}

	if (pos + sizeof(PLTE_HEADER) >= datalen)
		return false;

	pos += sizeof(PLTE_HEADER);

	for(int i=0 ; pos<datalen && i<256 ; i++ ) {
		palette[i] = 0xFF000000 |(0xFF & data[pos++]);
		palette[i] |= ((0xFF & data[pos++])<<8);
		palette[i] |= ((0xFF & data[pos++])<<16);
	}

	if (useAlpha)
		palette[0] &= 0x00FFFFFF;

	return true;
}

bool GraphicIO::loadPicturePNG_IDAT( char *data, ULONG datalen, ULONG &pos, 
											int &dat_startpos, int &dat_length) {
	const char IDAT_HEADER[] = {'I','D','A','T'};


	while(pos + sizeof(IDAT_HEADER) < datalen) {
		if (memcmp(data+pos, IDAT_HEADER, sizeof(IDAT_HEADER)) == 0)
			break;
		pos++;
	}

	if (pos + sizeof(IDAT_HEADER) >= datalen)
		return false;

	pos += sizeof(IDAT_HEADER);


	int zipedLen = 0;
	for(int i=-8; i<-4; i++)
		zipedLen = zipedLen << 8 | (data[pos+i] & 0xFF);

	if (dat_startpos == -1) {
		dat_startpos = pos;
		dat_length = zipedLen;
	} else {
		memmove(data + dat_startpos + dat_length, 
				data + pos, sizeof(char) * zipedLen);
		pos += zipedLen;
		dat_length += zipedLen;
	}

	return true;
}

bool GraphicIO::loadPicturePNG_Proc( Bytef *data, unsigned long datalen, ULONG &pos , 
				CCImage &image, int simgW, int simgH, int dat_startpos, int dat_length, int palette[], int sFilter) {

	uLongf buflen = simgW*simgH*2;
	char *buf = new char[buflen];
	
	int result_unzip = uncompress((Bytef *)buf, &buflen, data + dat_startpos, dat_length);

	if (result_unzip != Z_OK)
		return false;

	bool result;
	int *bArr;
	int p = -1, n = -1;
	int lastpal = -1;
	int rowFilter = 0;

	bArr = new int[simgW * simgH];

	{
		rowFilter = buf[++p] & 0xFF;
		switch(rowFilter) {
		case 0:
			{//none
				for(int x=0 ; x<simgW ; ++x) {
					bArr[++n] = buf[++p] & 0xFF;
				}
				break;
			}
		case 1:
			{//Left
				bArr[++n] = lastpal = buf[++p] & 0xFF;
				for(int x=1 ; x<simgW ; ++x) {
					bArr[++n] = lastpal = (lastpal + buf[++p]) & 0xFF;
				}
				break;
			}
		case 2:
			{//Top1
				for(int x=0 ; x<simgW ; ++x) {
					bArr[++n] = buf[++p]&0xFF;
				}
				break;
			}
		case 3:
			{//Average
				lastpal = 0;
				for(int x=0 ; x<simgW ; ++x) {
					bArr[++n] = lastpal = (lastpal/2 + buf[++p])&0xFF;
				}
				break;
			}
		case 4:
			{//Paeth
				//; a = left, b = above, c = upper left
				int v, va,vb,vc, r, ra = 0,rb = 0,rc;
				for(int x=0 ; x<simgW ; ++x) {
					r = (buf[++p] & 0xFF);
					rc = rb;
					rb = 0;

					v = ra + rb - rc;
					va = abs(v - ra);
					vb = abs(v - rb);
					vc = abs(v - rc);

					if ( va <= vb && va <= vc)
						bArr[++n] = ra = (r+ra)&0xFF;
					else if ( vb <= vc )
						bArr[++n] = ra = (r+rb)&0xFF;
					else
						bArr[++n] = ra = (r+rc)&0xFF;
				}
				break;
			}
		}
	}
	for(int y=1; y<simgH; y++) {
		rowFilter = buf[++p] & 0xFF;
		switch(rowFilter) {
		case 0:
			{//none
				for(int x=0 ; x<simgW ; ++x) {
					bArr[++n] = buf[++p] & 0xFF;
				}
				break;
			}
		case 1:
			{//Left
				bArr[++n] = lastpal = buf[++p] & 0xFF;
				for(int x=1 ; x<simgW ; ++x) {
					bArr[++n] = lastpal = (lastpal + buf[++p]) & 0xFF;
				}
				break;
			}
		case 2:
			{//Top1
				int tp = (y-1)*simgW - 1;
				for(int x=0 ; x<simgW ; ++x) {
					bArr[++n] = (bArr[++tp] + buf[++p])&0xFF;
				}
				break;
			}
		case 3:
			{//Average
				int tp = (y-1)*simgW - 1;
				lastpal = 0;
				for(int x=0 ; x<simgW ; ++x) {
					bArr[++n] = lastpal = ((lastpal + bArr[++tp])/2 + buf[++p])&0xFF;
				}
				break;
			}
		case 4:
			{//Paeth
				//; a = left, b = above, c = upper left
				int pb = (y-1)*simgW - 1;
				int v, va,vb,vc, r, ra = 0,rb = 0,rc;
				for(int x=0 ; x<simgW ; ++x) {
					r = (buf[++p] & 0xFF);
					rc = rb;
					rb = bArr[++pb];

					v = ra + rb - rc;
					va = abs(v - ra);
					vb = abs(v - rb);
					vc = abs(v - rc);

					if ( va <= vb && va <= vc)
						bArr[++n] = ra = (r+ra)&0xFF;
					else if ( vb <= vc )
						bArr[++n] = ra = (r+rb)&0xFF;
					else
						bArr[++n] = ra = (r+rc)&0xFF;
				}
				break;
			}
		}
	}

	for(int i=0; i<simgW * simgH; ++i)
		bArr[i] = palette[bArr[i]&0xFF];

	result = image.initWithImageData((void*)bArr, simgW*simgH, CCImage::kFmtRawData, simgW, simgH, 32);

	delete []bArr;
	delete []buf;

	return result;
}