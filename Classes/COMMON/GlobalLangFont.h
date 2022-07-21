#ifndef __R2K_COMMON_GLOBAL_LANGUAGE_FONT__
#define __R2K_COMMON_GLOBAL_LANGUAGE_FONT__
#include "DebugingCode.h"

#include "GlobalLang.h"

class GlobalLangFont{
	static char defFontName[30];
	static char lastFontName[2][30];

public:
	static void Init();

	static void setLang(Encoding encoding);

	static const char * getDefFontName();
	static const char * getFontName(int index);
	static TString getTransFontName();
private:
	static TString getFontPath( const TString &fontname );
};

#endif