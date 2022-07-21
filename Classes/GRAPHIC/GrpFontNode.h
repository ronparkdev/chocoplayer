#ifndef __R2K_GRP_FONT_NODE__
#define __R2K_GRP_FONT_NODE__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../DATA/TString.h"
#include "GrpShaderFont.h"

class GrpFontNode : public CCNode{
	TString txt;//UTF-8 4byte + null 1byte
	int x;
	int y;
	int width;
	int delay;
	int color;
	float fontSize;
	bool isAlignRight;
	bool isWaitKey;
	char customchar;

	CCLabelTTF *pFont;
	CCLabelTTF *pFontShadow;
	CCSprite *pCustom;
	CCSprite *pCustomShadow;

public:
	GrpFontNode();
	~GrpFontNode();
	void operator=(const GrpFontNode& val);

	int Init(GrpShaderFont *shader);
	int getWidth();
	void setOpacity( float val );
	friend class GrpFont;
	friend class GrpFontL;
};

#endif