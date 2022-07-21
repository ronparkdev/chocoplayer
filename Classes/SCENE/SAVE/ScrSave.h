#ifndef __R2K_SCENE_SAVE__
#define __R2K_SCENE_SAVE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GameMain.h"
#include "../../DATA/LSD/LsdMain.h"
#include "../../GRAPHIC/GrpFontL.h"
#include "../../GRAPHIC/OBJECT/GrpSelection.h"
#include "../../GRAPHIC/OBJECT/GrpFace.h"

USING_NS_CC;

#define SCR_SAVE_COUNT 15
#define SCR_SAVE_COUNTPERPAGE 3

class ScrSave : public ScreenLayer
{
	GameMain *pGM;

	GrpFontL *pFontCaption;

	GrpFontL *pFontFile[3];

	GrpFontL *pFontHero[3];
	GrpFontL *pFontLevel[3];
	GrpFontL *pFontHP[3];
	GrpFontL *pFontDate[3];

	GrpFace *pFace[12];

	GrpSelection *pSel;

	bool isSave;
	bool isFromTitle;

	int curIndex;
	int curPage;

	int keyLastEnter;
	int keyLastEsc;
	int keyLastArrow;

	LsdMain lsds[SCR_SAVE_COUNT];
	bool isLsdLoaded[SCR_SAVE_COUNT];
	bool isLsdLastVer[SCR_SAVE_COUNT];

	int debugInt;
public:

	ScrSave( GameMain *main , bool isSave , bool fromTitle);
	~ScrSave();

	void reloadShaders();
	void Draw(int framedelta);

private:
	void refreshPage();
	void refreshPos();
	void chkKey();

	TString getPath(int i);
};

#endif 
