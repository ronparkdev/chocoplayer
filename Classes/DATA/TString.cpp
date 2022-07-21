#include "TString.h"

#include "StrConv.h"
#include "BaseStruct.h"
#include "../COMMON/GlobalLang.h"

int TString::defEncoding = 0;

TString::TString()
	:pData(NULL),
	 length(0)
{
}

TString::TString( const char *cstr )
	:pData(NULL),
	 length(0)
{
	setText(cstr);
}

TString::TString( const std::string &str )
	:pData(NULL),
	length(0)
{
	setText(str);
}

TString::TString( const TString &tstr )
	:pData(NULL),
	 length(0)
{
	setText(tstr);
}

TString::TString( int num )
	:pData(NULL),
	length(0)
{
	setText(num);
}

TString::~TString()
{
	if (pData != NULL)
		delete[] pData;
}

char* TString::getTextUTF8() const
{
	if (length == 0)
		return "";
	else
		return pData;
}

void TString::getTextUTF8_s(char *cstr, int bufsize) const
{
    strncpy(cstr, getTextUTF8(), bufsize);
}

void TString::getText_s(char *cstr, int bufsize) const
{
	if (length == 0 || pData == NULL) {
		if (bufsize > 0)
			cstr[0] = '\0';
	} else {
		charset_convert("utf-8", EncodingCode[defEncoding], pData, length, cstr, bufsize);
	}
}

void TString::setText( const char *cstr )
{
	int cLen = strlen(cstr);
	int tLen, result;
	
	tLen = (cLen<<1)+2;
	if (pData != NULL)
		delete[] pData;
	pData = NULL;

	if (cLen == 0) {
		length = 0;
		return;
	} else if (cLen == 1) {
		pData = new char[2];
		pData[0] = cstr[0];
		pData[1] = '\0';
		length = 1;
	} else {
		pData = new char[tLen];
		result = charset_convert(EncodingCode[defEncoding], "utf-8", cstr, cLen, pData, tLen-1);

		if (result == -1) {
			tLen = (cLen<<2)+2;
			if (pData != NULL)
				delete[] pData;
			pData = new char[tLen];

			result = charset_convert(EncodingCode[defEncoding], "utf-8", cstr, cLen, pData, tLen-1);
		}

		length = strlen((char *)pData);
		pData[length] = '\0';
	}
}

void TString::setText( const std::string &str )
{
	setText((char*)str.c_str());
}

void TString::setText( const TString &tstr )
 {
	if (&tstr == this)
		return;

	if (tstr.length == 0) {
		if (pData != NULL)
			delete[] pData;

		pData = NULL;
		length = 0;
	} else {
		if (pData != NULL)
			delete[] pData;

		int tlen = tstr.length;
		pData = new char[tlen+1];
		length = tlen;

		memcpy(pData, tstr.pData, tlen+1);
	}
}

void TString::setText( int num )
{
	bool isminus = false;
	int numlen;
	if (num < 0) {
		isminus = true;
		numlen = (int)(log((float)(-num))/log(10.0f)) + 2;
	} else if (num == 0) {
		numlen = 1;	
	} else {
		numlen = (int)(log((float)num)/log(10.0f)) + 1;
	}
	setText(num, numlen);
}

void TString::setText( int num, int numlen )
{
	bool isminus = (num<0);
	char *numstr = new char[numlen + 1];
	if (num < 0)
		num = -num;
	for(int i=numlen-1; i>=0; i--) {
		if (isminus && i == 0) {
			numstr[i] = '-';
		} else {
			numstr[i] = num%10 + '0';
			num /= 10;
		}
	}
	numstr[numlen] = '\0';

	setText(numstr);

	delete []numstr;
}

void TString::setTextUTF8( const char *cstr )
{
	int cLen = strlen(cstr);
	if (pData != NULL)
		delete[] pData;

	pData = new char[cLen+1];

	length = cLen;

	memcpy(pData, cstr, cLen+1);
}

void TString::setTextHEX( int value )
{
	char cstr[11];

	cstr[0] = '0';
	cstr[1] = 'x';

	for(int i=0; i<8; i++) {
		int n = value & 0xF;

		if (n < 10)
			cstr[9-i] = (char)(n + '0');
		else
			cstr[9-i] = (char)(n-10 + 'a');

		value >>= 4;
	}

	cstr[10] = '\0';

	setText(cstr);
}

void TString::setChar( int c )
{
	if (pData != NULL)
		delete[] pData;

	if (c < 0x7F) {
		pData = new char[2];
		length = 1;
		pData[0] = c;
	} else if (c < 0x7FF) {
		pData = new char[3];
		length = 2;
		pData[0] = 0xC0 | ((c>>6)	& 0x1F);
		pData[1] = 0x80 | (c		& 0x3F);
	} else if (c < 0xFFFF) {
		pData = new char[4];
		length = 3;
		pData[0] = 0xE0 | ((c>>12)	& 0x0F);
		pData[1] = 0x80 | ((c>>6)	& 0x3F);
		pData[2] = 0x80 | (c		& 0x3F);
	} else if (c < 0x1FFFFF) {
		pData = new char[5];
		length = 4;
		pData[0] = 0xF0 | ((c>>18)	& 0x07);
		pData[1] = 0x80 | ((c>>12)	& 0x3F);
		pData[2] = 0x80 | ((c>>6)	& 0x3F);
		pData[3] = 0x80 | (c		& 0x3F);
	} else {
		pData = NULL;
		length = 0;
		return;
	}

	if (length != 0) {
		pData[length] = '\0';
	}
}

int TString::getInt()
{
	int s = 0;
	for(int i=0; i<length; i++) {
		char c = pData[i];
		if ('0' <= c && c <= '9')
			s = s * 10 + (c - '0');
	}
	return s;
}

int TString::getLength() const
{
	int len = 0;
	int p = 0;
	while(p < length) {
		if ((pData[p] & 0x80) == 0x00) {
			p += 1;
		} else if ((pData[p] & 0xE0) == 0xC0) {
			p += 2;
		} else if ((pData[p] & 0xF0) == 0xE0) {
			p += 3;
		} else if ((pData[p] & 0xF8) == 0xF0) {
			p += 4;
		} else {
			break;
		}
		len++;
	}
	return len;
}

int TString::charAt( int fp ) const
{
	int c = 0;
	int i = 0;
	int p = 0;
	while(i < length && p <= fp) {
		if ((pData[i] & 0x80) == 0x00) {
			c = pData[i] & 0x7F;
			i += 1;
		} else if ((pData[i] & 0xE0) == 0xC0) {
			if (i+1 >= length)break;
			c = ((pData[i] & 0x1F) << 6) | (pData[i+1] & 0x3F);
			i += 2;
		} else if ((pData[i] & 0xF0) == 0xE0) {
			if (i+2 >= length)break;
			c = ((pData[i] & 0x0F) << 12) | ((pData[i+1] & 0x3F) << 6) | (pData[i+2] & 0x3F);
			i += 3;
		} else if ((pData[i] & 0xF8) == 0xF0) {
			if (i+3 >= length)break;
			c = ((pData[i] & 0x07) << 18) | ((pData[i+1] & 0x3F) << 12) | ((pData[i+2] & 0x3F) << 6) | (pData[i+3] & 0x3F);
			i += 4;
		} else {
			break;
		}
		p++;
	}
	return c;
}

int TString::indexOf( int fc ) const
{
	return indexOf(fc, 0);
}

int TString::indexOf( int fc, int startpos ) const
{
	int c = 0;
	int p = 0;
	int len = 0;
	while(p < length) {
		if ((pData[p] & 0x80) == 0x00) {
			c = pData[p] & 0x7F;
			p += 1;
		} else if ((pData[p] & 0xE0) == 0xC0) {
			if (p+1 >= length)break;
			c = ((pData[p] & 0x1F) << 6) | (pData[p+1] & 0x3F);
			p += 2;
		} else if ((pData[p] & 0xF0) == 0xE0) {
			if (p+2 >= length)break;
			c = ((pData[p] & 0x0F) << 12) | ((pData[p+1] & 0x3F) << 6) | (pData[p+2] & 0x3F);
			p += 3;
		} else if ((pData[p] & 0xF8) == 0xF0) {
			if (p+3 >= length)break;
			c = ((pData[p] & 0x07) << 18) | ((pData[p+1] & 0x3F) << 12) | ((pData[p+2] & 0x3F) << 6) | (pData[p+3] & 0x3F);
			p += 4;
		} else {
			break;
		}
		if (startpos <= len && c == fc)
			return len;
		len++;
	}
	return -1;
}

int TString::indexOf(const TString &findstr) const
{
	if (length < findstr.length)
		return -1;

	int checklength = length - findstr.length;
	int p = 0;
	int len = 0;
	while(p <= checklength) {
		if (memcmp(pData + p , findstr.pData, sizeof(char) * findstr.length) == 0) {
			return len;
		}

		if ((pData[p] & 0x80) == 0x00) {
			p += 1;
		} else if ((pData[p] & 0xE0) == 0xC0) {
			if (p+1 >= length)break;
			p += 2;
		} else if ((pData[p] & 0xF0) == 0xE0) {
			if (p+2 >= length)break;
			p += 3;
		} else if ((pData[p] & 0xF8) == 0xF0) {
			if (p+3 >= length)break;
			p += 4;
		} else {
			break;
		}
		len++;
	}
	return -1;
}

int TString::lastIndexOf( int fc ) const
{
	return lastIndexOf(fc, 0);
}

int TString::lastIndexOf( int fc, int startpos ) const
{
	int c = 0;
	int p = 0;
	int len = 0;
	int find = -1;
	while(p < length) {
		if ((pData[p] & 0x80) == 0x00) {
			c = pData[p] & 0x7F;
			p += 1;
		} else if ((pData[p] & 0xE0) == 0xC0) {
			if (p+1 >= length)break;
			c = ((pData[p] & 0x1F) << 6) | (pData[p+1] & 0x3F);
			p += 2;
		} else if ((pData[p] & 0xF0) == 0xE0) {
			if (p+2 >= length)break;
			c = ((pData[p] & 0x0F) << 12) | ((pData[p+1] & 0x3F) << 6) | (pData[p+2] & 0x3F);
			p += 3;
		} else if ((pData[p] & 0xF8) == 0xF0) {
			if (p+3 >= length)break;
			c = ((pData[p] & 0x07) << 18) | ((pData[p+1] & 0x3F) << 12) | ((pData[p+2] & 0x3F) << 6) | (pData[p+3] & 0x3F);
			p += 4;
		} else {
			break;
		}
		if (startpos <= len && c == fc)
			find = len;
		len++;
	}
	return find;
}

TString TString::substring( int frompos ) const
{
	return substring(frompos, this->getLength());
}

TString TString::substring( int frompos, int topos ) const
{
	if (frompos >= topos)
		return "";

	int p = 0;
	int len = 0;
	int fp = 0;
	int tp = 0;
	while(p < length) {
		if (frompos == len)
			fp = p;

		if ((pData[p] & 0x80) == 0x00) {
			p += 1;
		} else if ((pData[p] & 0xE0) == 0xC0) {
			p += 2;
		} else if ((pData[p] & 0xF0) == 0xE0) {
			p += 3;
		} else if ((pData[p] & 0xF8) == 0xF0) {
			p += 4;
		} else {
			break;
		}
		
		len++;

		if (topos == len) {
			tp = p;
			break;
		}
	}
	if (fp < tp) {
		TString str;
		char *data = new char[tp - fp + 1];
		memcpy(data, pData+fp, sizeof(char)*(tp-fp));
		data[tp - fp] = '\0';
		str.setTextUTF8(data);
		delete []data;
		return str;
	} else {
		return "";
	}
}

void TString::setDefEncoding( int encid )
{
	if (encid < 0 || encid >= ENCODING_COUNT)
		return;

	defEncoding = encid;
}

int TString::getEncoding() const
{
	return defEncoding;
}

void TString::operator=( const char *cstr )
{
	setText(cstr);
}

void TString::operator=( const TString &tstr )
{
	setText(tstr);
}

bool TString::isEmpty() const
{
	return (length == 0);
}

TString TString::toUpper() const
{
	if (length == 0)
		return TString();
	else{
		int tlen = length;
		TString newstr;
		newstr.pData = new char[tlen+1];
		newstr.length = tlen;

		for(int i=0;i <=tlen; i++) {
			if ('a' <= pData[i] && pData[i] <= 'z')
				newstr.pData[i] = pData[i] - 'a' + 'A';
			else
				newstr.pData[i] = pData[i];
		}

		return newstr;
	}
}

TString TString::toLower() const
{
	if (length == 0)
		return TString();
	else{
		int tlen = length;
		TString newstr;
		newstr.pData = new char[tlen+1];
		newstr.length = tlen;

		for(int i=0;i <=tlen; i++) {
			if ('A' <= pData[i] && pData[i] <= 'Z')
				newstr.pData[i] = pData[i] - 'A' + 'a';
			else
				newstr.pData[i] = pData[i];
		}

		return newstr;
	}
}

int TString::getSize() const
{
	return length;
}

// BaseStruct

void TString::serialize(StructLoader &sl)
{
	int strsize, startpos;
	if (sl.getMode() == StructLoaderModeInit) {
		setText("");
	} else if (sl.isOpened()) {
		if (sl.getMode() == StructLoaderModeRead) {
			sl >> strsize;
			if (strsize == 0) {
				setText("");
			} else if (strsize>0 && sl.isSizeReadable(strsize)) {
				char *strbuf = new char[strsize+1];
				memcpy(strbuf, sl.getRawData() + sl.getOffset(), sizeof(char) * (strsize));
				strbuf[strsize] = '\0';

				setText(strbuf);
				delete []strbuf;

				sl.setOffset(sl.getOffset() + strsize);
			} else {
				return;
				//throw BaseStructException_WrongStruct();
			}
		} else if (sl.getMode() == StructLoaderModeWrite) {
			char buf[STRUCT_DEFINE_STRING_BUFFER];

			getText_s(buf,STRUCT_DEFINE_STRING_BUFFER);
			strsize = getSize();

			startpos = sl.getOffset();

			sl.allocForWrite(strsize);
			memcpy(sl.getRawData() + sl.getOffset(), buf, strsize);
			sl.setOffset(startpos + strsize);
			sl.writeStructSize(startpos);
		}
	}
}

// Operators

bool operator==(const TString &tstr, const char *cstr)
{
	TString nstr(cstr);
	return (tstr == nstr);
}

bool operator==(const char *cstr, const TString &tstr)
{
	TString nstr(cstr);
	return (nstr == tstr);
}

bool operator==(const TString &tstr1, const TString &tstr2)
{
	if (tstr1.length != tstr2.length)
		return false;
	else{
		return (memcmp(tstr1.pData, tstr2.pData, tstr1.length) == 0);
	}
}

bool operator!=(const TString &tstr, const char *cstr)
{
	TString nstr(cstr);
	return (tstr != nstr);
}

bool operator!=(const char *cstr, const TString &tstr)
{
	TString nstr(cstr);
	return (nstr != tstr);
}

bool operator!=(const TString &tstr1, const TString &tstr2)
{
	return !(tstr1 == tstr2);
}

TString operator+( const TString &tstr, const char *cstr )
{
	TString nstr(cstr);
	return (tstr + nstr);
}

TString operator+( const char *cstr, const TString &tstr )
{
	TString nstr(cstr);
	return (nstr + tstr);
}

TString operator+( const TString &tstr1, const TString &tstr2 )
{
	if (tstr1.length != 0) {
		if (tstr2.length == 0) {
			return tstr1;
		} else {
			TString nstr;
			nstr.length = tstr1.length + tstr2.length;
			char *data = new char[nstr.length+1];

			memcpy(data				
				 , tstr1.pData		
				 , sizeof(char) * tstr1.length);

			memcpy(data			+ tstr1.length	
				 , tstr2.pData 
				 , sizeof(char) * tstr2.length);

			data[nstr.length] = '\0';
			nstr.pData = data;
			return nstr;
		}
	} else {
		return tstr2;
	}
}

TString operator+( const TString &tstr, int num )
{
	TString numstr(num);
	return tstr + numstr;
}

TString operator+( int num, const TString &tstr )
{
	bool isminus = false;
	int numlen;
	if (num < 0) {
		isminus = true;
		numlen = (int)(log((float)(-num))/log(10.0f)) + 2;
	} else if (num == 0) {
		numlen = 1;	
	} else {
		numlen = (int)(log((float)num)/log(10.0f)) + 1;
	}
	char *numstr = new char[numlen + 1];
	for(int i=numlen-1; i>=0; i--) {
		if (isminus && i == 0) {
			numstr[i] = '-';
		} else {
			numstr[i] = num%10 + '0';
			num /= 10;
		}
	}
	numstr[numlen] = '\0';

	return numstr + tstr;
}

std::ostream& operator<<(std::ostream &os, TString &tstr) {
	char BUF[100];
	tstr.getText_s(BUF,100);
	os<<BUF;
	return os;
}

