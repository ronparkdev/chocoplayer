#ifndef __R2K_GRP_FONT__
#define __R2K_GRP_FONT__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "GrpFontNode.h"
#include "GrpShaderFont.h"

#include "../GameMain.h"
#include "../DATA/TString.h"


#define GRP_FONT_SIZE 11
#define GRP_NEXTLINE_SIZE 17

class GrpFont: public CCNode
{
private:
	GameMain *pGM;

	TString text;
	float fontSize;
	float fontScale;
	float lineHeight;
	bool isAutoMsg;
	int showMoneyPos;
	int lastMsgDelay;

	int pos;
	int msgStartTime;
	ArrayList<GrpFontNode *> pFontNodes;

	GrpShaderFont *pFontShaders[20];
	GrpShaderFont *pFontExtraShaders[20];
	CCTexture2D* lastSysTex;

	int keyLastEnter;
	bool isWaittingKey;
public:
	GrpFont(GameMain *main = NULL);
	~GrpFont();

	void reloadShaders();

	void setText(const TString &str);
	void setText(const TString &str, CCTexture2D *pSysTex);
	void removeText();

	void showAll();

	int showStart(int curtime);
	bool refresh(int curtime, bool allowAutoMsg = true);
	bool isWaitKey();
	friend class EvtMain;
};

#endif