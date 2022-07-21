#include "StructLoader.h"

#include "BaseStruct.h"
#include "TString.h"

inline void readDouble( StructLoader &sl, double &val ) {
	double dOut = 1.0f;
	bool b[64];	
	for(int i=0; i<8; i++) {
		int c = sl.getRawData()[sl.offset + 7-i];
		for(int j=7; j>=0; j--) {
			b[i*8+j] = ((c&0x1)?true:false);
			c >>= 1;
		}
	}

	int exp = 0;
	for(int i=1; i<12; i++) {
		exp <<= 1;
		if (b[i])exp |= 0x1;
	}
	exp -= 1023;

	double dT = 1.0f;
	for(int i=12; i<64; i++) {
		dT /= 2.0f;
		if (b[i])dOut += dT;
	}

	dOut *= pow(2.0f, exp);

	if (b[0])dOut *= -1.0f;

	val = dOut;
}

inline void readFloat( StructLoader &sl, float &val ) {
	double dOut = 1.0f;
	bool b[32];	
	for(int i=0; i<4; i++) {
		int c = sl.getRawData()[sl.offset + 7-i];
		for(int j=7; j>=0; j--) {
			b[i*8+j] = ((c&0x1)?true:false);
			c >>= 1;
		}
	}

	int exp = 0;
	for(int i=1; i<9; i--) {//TODO NeedCheck
		exp <<= 1;
		if (b[i])exp += 0x1;
	}
	exp -= 127;

	double dT = 1.0f;
	for(int i=9; i<32; i++) {
		dT /= 2.0f;
		if (b[i])dOut += dT;
	}

	dOut *= pow(2.0f, exp);

	if (b[0])dOut *= -1.0f;

	val = (float)dOut;
}
inline void writeDouble( StructLoader &sl, const double &val )
{
	bool b[64];

	memset(b, 0, sizeof(bool)*64);

	double fval = val;

	if (val < 0.0f) {
		b[0] = true;
		fval *= -1.0f;
	}

	int exp = 0;

	//if (isnan(fval) || !finite(fval)) {
	//	return;
	//}

	while(fval > 2.0f && exp <= 1024) {
		fval /= 2;
		exp++;
	}

	while(fval != 0.0f && fval < 1.0f && exp > -1024) {
		fval *= 2;
		exp--;
	}

	exp += 1023;

	for(int i=1; i<=11; i++) {
		if ((exp & 0x1) == 0x1)b[12-i] = true;
		exp >>= 1;
	}

	double dT = 1.0f;
	fval -= 1.0f;
	for(int i=12; i<64; i++) {
		dT /= 2.0f;
		if (fval >= dT) {
			b[i] = true;
			fval -=dT;
		}
	}

	sl.allocForWrite(8);

	//little-endian.. -_-;;
	for(int i=7; i>=0; i--) {
		byte tb = 0;
		for(int n=0; n<8; n++) {
			tb <<= 1;
			if (b[i*8+n])
				tb += 1;
		}
		sl.pData[sl.offset++] = tb;
	}
}

inline void writeFloat( StructLoader &sl, const float &val )
{
	bool b[32];

	memset(b, 0, sizeof(bool)*32);

	double fval = val;

	if (val < 0.0f) {
		b[0] = true;
		fval *= -1.0f;
	}

	int exp = 0;

	//if (_isnan(fval) || !_finite(fval)) {
	//	return;
	//}

	while(fval > 2.0f && exp <= 128) {
		fval /= 2;
		exp++;
	}

	while(fval != 0.0f && fval < 1.0f && exp > 128) {
		fval *= 2;
		exp--;
	}

	exp += 127;

	for(int i=1; i<=9; i++) {
		if ((exp & 0x1) == 0x1)b[10-i] = true;
		exp >>= 1;
	}

	double dT = 1.0f;
	fval -= 1.0f;
	for(int i=9; i<32; i++) {
		dT /= 2.0f;
		if (fval >= dT) {
			b[i] = true;
			fval -=dT;
		}
	}

	sl.allocForWrite(8);

	//little-endian.. -_-;;
	for(int i=3; i>=0; i--) {
		byte tb = 0;
		for(int n=0; n<8; n++) {
			tb <<= 1;
			if (b[i*8+n])
				tb += 1;
		}
		sl.pData[sl.offset++] = tb;
	}
}

StructLoader& operator>>( StructLoader &sl, double &val );
StructLoader& operator<<( StructLoader &sl, const double &val );
StructLoader& operator>>( StructLoader &sl, float &val );
StructLoader& operator<<( StructLoader &sl, const float &val );
StructLoader& operator>>( StructLoader &sl, int &val );
StructLoader& operator<<( StructLoader &sl, const int &val );

StructLoader& operator>>( StructLoader &sl, double &val )
{
	int size = BaseStruct::size;

	int pos = sl.offset;

	if (!sl.isSizeReadable(size)) {
		throw StructLoaderException_WrongStruct();
	} else if (size == 8) {
		readDouble(sl, val);
		sl.offset += size;
	} else if (size == 4) {
		float tmpVal = 0.0f;
		readFloat(sl, tmpVal);
		val = (double)tmpVal;
		sl.offset += size;
	} else {
		val = 0.0f;//Not IEEE Type!
		sl.offset += size;
	}
	return sl;
}

StructLoader& operator<<( StructLoader &sl, const double &val )
{
	writeDouble(sl, val);
	return sl;
}

StructLoader& operator>>( StructLoader &sl, float &val )
{
	float dOut = 1.0f;

	int size;
	sl >> size;

	int pos = sl.offset;

	if (!sl.isSizeReadable(size)) {
		throw StructLoaderException_WrongStruct();
	} else if (size == 4) {
		readFloat(sl, val);
		sl.offset += size;
	} else {
		val = 0.0f;//Not IEEE Type!
		sl.offset += size;
	}
	return sl;
}

StructLoader& operator<<( StructLoader &sl, const float &val )
{
	writeFloat(sl, val);
	return sl;
}

StructLoader& operator>>( StructLoader &sl, int &val )
{
	int i, tmpVal = 0;
	do {
		tmpVal = (tmpVal << 7) | (sl.pData[sl.offset++] & 0x7F);
	} while ((sl.pData[sl.offset-1] & 0x80) && (sl.offset < sl.dataLength));
	val = tmpVal;

	return sl;
}

StructLoader& operator<<( StructLoader &sl, const int &val )
{
	int tmpVal = val;
	if (tmpVal<=0) {
		sl.allocForWrite(1);
		sl.pData[sl.offset++] = 0;
	} else {
		int bitLeft = 32;
		int bufferOffset = 0;
		byte buffer[8];

		while (bitLeft > 0 && tmpVal > 0) {
			int shiftCount = 8 - min(bitLeft, 7);
			buffer[bufferOffset++] = (((tmpVal << shiftCount) & 0xFF) >> shiftCount);

			tmpVal >>= 7;
			bitLeft -= 7;
		}
		sl.allocForWrite(bufferOffset);

		while (bufferOffset > 0) {
			sl.pData[sl.offset++] = buffer[--bufferOffset];
		}
	}

	return sl;
}

StructLoader& operator>>( StructLoader &sl, unsigned int &val )
{
	if (sl.isSizeReadable(4)) {
		val = ( ((sl.pData[sl.offset+1] & 0xFF) << 24) | ((sl.pData[sl.offset+1] & 0xFF) << 16) | ((sl.pData[sl.offset+1] & 0xFF) << 8) | (sl.pData[sl.offset+0] & 0xFF));
		sl.offset += 2;
	}

	return sl;
}

StructLoader& operator<<( StructLoader &sl, const unsigned int &val )
{
	sl.allocForWrite(2);
	sl.pData[sl.offset+1] = (byte) ((val>>8) & 0xFF);
	sl.pData[sl.offset+0] = (byte) (val & 0xFF);
	sl.offset += 2;

	return sl;
}

StructLoader& operator>>( StructLoader &sl, unsigned short &val )
{
	if (sl.isSizeReadable(2)) {
		val = ( ((sl.pData[sl.offset+1] & 0xFF) << 8) | (sl.pData[sl.offset+0] & 0xFF));
		sl.offset += 2;
	}

	return sl;
}

StructLoader& operator<<( StructLoader &sl, const unsigned short &val )
{
	sl.allocForWrite(2);
	sl.pData[sl.offset+1] = (byte) ((val>>8) & 0xFF);
	sl.pData[sl.offset+0] = (byte) (val & 0xFF);
	sl.offset += 2;

	return sl;
}

StructLoader& operator>>( StructLoader &sl, unsigned char &val )
{
	if (sl.isSizeReadable(1)) {
		val = (unsigned char)sl.pData[sl.offset++];
	}

	return sl;
}

StructLoader& operator<<( StructLoader &sl, const unsigned char &val )
{
	sl.allocForWrite(1);
	sl.pData[sl.offset++] = val;
	return sl;
}

StructLoader& operator>>( StructLoader &sl, bool &val )
{
	if (sl.isSizeReadable(1)) {
		val = (sl.pData[sl.offset++]!=0x00)?true:false;
	}
		
	return sl;
}

StructLoader& operator<<( StructLoader &sl, const bool &val )
{
	sl.allocForWrite(1);
	sl.pData[sl.offset++] = (val!=0)?0x01:0x00;
	return sl;
}

StructLoader& operator>>(StructLoader &sl, TString &str) {
	int strsize = BaseStruct::size;

	if (strsize==0) {
		str.setText("");
	} else if (strsize>0 && sl.isSizeReadable(strsize)) {
		char *strbuf = new char[strsize+1];
		memcpy(strbuf, sl.getRawData() + sl.getOffset(), sizeof(char) * (strsize));
		strbuf[strsize] = '\0';

		str.setText(strbuf);
		delete []strbuf;

		sl.setOffset(sl.getOffset() + strsize);
	} else {
		sl.setOffset(sl.getOffset() + strsize);
		//throw BaseStructException_WrongStruct();
	}

	return sl;
}


StructLoader& operator<<(StructLoader &sl, const TString &str) {
	char buf[STRUCT_DEFINE_STRING_BUFFER];

	str.getText_s(buf,STRUCT_DEFINE_STRING_BUFFER);
	int strlength = strlen(buf);

	sl.allocForWrite(strlength);
	memcpy(sl.getRawData() + sl.getOffset(), buf, strlength);
	sl.setOffset(sl.getOffset() + strlength);

	return sl;
}
