#ifndef __R2K_DATA_GLOBAL_LANGUAGE__
#define __R2K_DATA_GLOBAL_LANGUAGE__
#include "DebugingCode.h"

#include <string>
#include "../DATA/TString.h"

#define GLOBAL_LANG_FILENAME "/language.ini"

#define ENCODING_COUNT 5

enum Encoding{
	LKOREAN			= 0,
	LJAPANESE		,
	LCHINESE		,
	LMSEURO			,
	LRUSSIAN		,
	LENGLISH		,
};

static TString EncodingCaption[]= {	"Korean",	"Japanese",	"Chinese",	"Europe",		"Russian",	"English"};
static char* EncodingCode[]		= {	"CP949",	"SHIFT-JIS","BIG5",		"ISO_8859-16",	"CP1251",	"English"};
static float EncodingWeight[]	= {	1.0f,		1.0f,		0.1f,		1.0f,			1.0f,		1.0f};

class GlobalLang{

	int countEncoding[ENCODING_COUNT];

public:
	int getEncoding( const TString &path, bool isarchive );
private:
	void getEncFromFolder(const TString &path);
	void getEncFromString(const TString &str);
};

#endif