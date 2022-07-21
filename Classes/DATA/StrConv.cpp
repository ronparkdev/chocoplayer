#include "StrConv.h"

#include <cocos2d.h>
USING_NS_CC;

#include "../AppMulti.h"
#include "../COMMON/ComMath.h"
#include "TString.h"

#include "iostream"
#include "string"
using std::string ;

#if (SR_WIN == 1)
#pragma comment(lib, "libiconv.lib")

int charset_convert( const char *from_charset, const char *to_charset,
                    const char *inbuf, size_t inlen,
                    char *outbuf, size_t outlen )
{
    iconv_t cd;
    memset(outbuf,0,outlen);
    if ((cd = iconv_open(to_charset,from_charset)) == (iconv_t)-1)
        return -1;

	const char *cinbuf = inbuf;
	char *coutbuf = outbuf;

	if (iconv(cd,&cinbuf,&inlen,&coutbuf,&outlen) == -1)
		return -1;

    iconv_close(cd);
    return 0;
}

#else

#include "errno.h"

#if (SR_IPHONE == 1)
static char CONV_BUFFER[2049];
#endif

int charset_convert( const char *from_charset, const char *to_charset,
                        const char *inbuf, size_t inlen,
                        char *outbuf, size_t outlen )
{
    memset(outbuf,0,outlen);
    
    iconv_t cd;
    if ((cd = iconv_open(to_charset,from_charset)) == (iconv_t) -1)
    {
        fprintf(stderr, "iconv err1\n");
        return -1;
    }
    
    //fprintf(stderr,"in: %s\n",inbuf);
    
    char *coutbuf = outbuf;
    
#if (SR_IPHONE == 1)
    char *pCONV_BUFFER = CONV_BUFFER;
    
    strncpy(CONV_BUFFER, inbuf, 2048);
    CONV_BUFFER[2048] = '\0';
    
    inlen = _MIN(inlen, 2048);
    
    if (iconv(cd,&pCONV_BUFFER,&inlen,&coutbuf,&outlen) == -1) {
        //fprintf(stderr, "iconv err2 %s >%s<\n", strerror(errno), pCONV_BUFFER);
        return -1;
    }
#else
	const char *cinbuf = inbuf;
    if (iconv(cd,&cinbuf,&inlen,&coutbuf,&outlen) == -1) {
        fprintf(stderr, "iconv err2 %s >%s<\n", strerror(errno), cinbuf);
        return -1;
    }
#endif
    
    //fprintf(stderr,"out: %s\n",outbuf);
    
    iconv_close(cd);
    return 0;
}

#endif

bool isUTF8(const char *str) {
	int i = 0 , chk = 0;

	bool isHaveUTF8Chars = false;

	for(;str[i] != '\0'; i++) {
		if (chk == 0) {
			if ((str[i] & 0x80) == 0x00)
				continue;
			else if ((str[i] & 0xE0) == 0xC0) {
				chk = 1;
				isHaveUTF8Chars = true;
			} else if ((str[i] & 0xF0) == 0xE0) {
				chk = 2;
				isHaveUTF8Chars = true;
			} else if ((str[i] & 0xF8) == 0xF0) {
				chk = 3;
				isHaveUTF8Chars = true;
			}else 
				return false;
		} else if (chk > 0) {
			if ((str[i] & 0xC0) != 0x80)
				return false;
			chk--;
		}
	}

	if (chk == 0)
		return isHaveUTF8Chars;
	else
		return false;
}

const char* getUTF8Lang( const TString &str )
{
	int i, c;

	int cntKor = 0;
	int cntChi = 0;
	int cntJpn = 0;

	for(i=0; i<str.getLength(); i++) {
		c = str.charAt(i);

		if ((0x3130<= c && c < 0x318F) || (0xAC00 <= c && c < 0xD7A4))
			cntKor++;
		else if ((0x3040 <= c && c < 0x30FF) || (0x31F0 <= c && c < 0x31FF))
			cntJpn++; 
		else if ((0x4E00 <= c && c < 0x9Fa5))
			cntChi++;
	}
	
	if (cntKor == 0 && cntJpn == 0 && cntChi == 0)
		return "i don't know";
	else if (cntKor > 0 || cntJpn > 0) {
		if (cntKor > cntJpn)
			return "Korean";
		else
			return "Japanese";
	}else
		return "Chinese";
}
