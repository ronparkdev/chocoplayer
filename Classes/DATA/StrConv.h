// code reference : http://dev.naver.com/projects/ndkboost (naver developer center)

#ifndef __STR_CONV_H__
#define __STR_CONV_H__


#include "../AppMulti.h"
#if (SR_WIN == 1)
    #include "platform/third_party/win32/iconv/iconv.h"
#else
    #include "iconv.h"
#endif

int charset_convert( const char *from_charset, const char *to_charset,
                    const char *inbuf, size_t inlen,
                    char *outbuf, size_t outlen );

bool isUTF8(const char *str);

class TString;

const char* getUTF8Lang(const TString &str);

#endif 