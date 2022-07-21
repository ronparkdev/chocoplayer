#include "GlobalLangFont.h"

#include "../AppMulti.h"
#include <cocos2d.h>
USING_NS_CC;

char GlobalLangFont::defFontName[30];

char GlobalLangFont::lastFontName[2][30];

#define FONT_FOR_JP "japanese_hannari"
#define FONT_FOR_CH "chinese_wts11"
#define FONT_FOR_KR "korean_malgun"

void GlobalLangFont::Init() {
	strcpy(defFontName, getFontPath(FONT_FOR_KR).getTextUTF8());
}

TString GlobalLangFont::getFontPath( const TString &fontname )
{
#if (SR_WIN == 1)
    return "fontname";
#endif
#if (SR_ANDROID == 1)
	return "fonts/" + fontname + ".ttf";
#endif
#if (SR_IPHONE == 1)
    return fontname + ".ttf";
#endif
}

void GlobalLangFont::setLang( Encoding encoding )
{
	TString fontname;

	if (encoding == LJAPANESE) {
		fontname = FONT_FOR_JP;
	} else if (encoding == LCHINESE) {
		fontname = FONT_FOR_CH;
	} else {//For KOREAN and etc
		fontname = FONT_FOR_KR;
	}

	fontname = getFontPath(fontname);

	strcpy(lastFontName[0], fontname.getTextUTF8());
	strcpy(lastFontName[1], fontname.getTextUTF8());
}

const char * GlobalLangFont::getDefFontName()
{
	return defFontName;
}

const char * GlobalLangFont::getFontName(int index)
{
	if (index < 0 || index >= 2)
		index = 0;

	return lastFontName[index];
}

TString GlobalLangFont::getTransFontName()
{
	switch(CCApplication::sharedApplication()->getCurrentLanguage()) 
	{
		case kLanguageChinese:		return getFontPath(FONT_FOR_CH);
		case kLanguageJapanese:		return getFontPath(FONT_FOR_JP);
		case kLanguageKorean:		return getFontPath(FONT_FOR_KR);
		default:					return getFontPath(FONT_FOR_KR);
	};
}
