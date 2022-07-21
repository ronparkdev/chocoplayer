#ifndef __R2K_GRP_FONT_FOR_LABEL__
#define __R2K_GRP_FONT_FOR_LABEL__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "GrpFontNode.h"
#include "../GameMain.h"
#include "../DATA/TString.h"

#define GRP_FONT_SIZE 11
#define GRP_NEXTLINE_SIZE 17

class GrpFontL: public CCNode
{
private:
	GameMain *m_main;

	TString oriText;
	TString text;
	float fontSize;
	float fontScale;
	float lineHeight;
	ArrayList<GrpFontNode *> pFontNodes;

	GrpShaderFont *pFontShaders[20];
	GrpShaderFont *pFontExtraShaders[20];
	CCTexture2D* lastSysTex;
public:
	GrpFontL(GameMain *main = NULL);
	virtual ~GrpFontL();

	void reloadShaders();

	void setText(const TString &str);
	void setText(const TString &str, CCTexture2D *pSysTex);
	void removeText();

	int getWidth();

	void setOpacity( float val );
	friend class EvtMain;
};

#endif