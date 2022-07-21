#ifndef __R2K_GRP_MENU__
#define __R2K_GRP_MENU__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "GrpFontL.h"
#include "OBJECT/GrpMsgbox.h"
#include "OBJECT/GrpSelection.h"
#include "OBJECT/GrpSysUp.h"
#include "OBJECT/GrpSysDown.h"
#include "../GameMain.h"
#include "../COMMON/ScreenCommon.h"
#include "../DATA/ArrayList.h"
#include "../DATA/TString.h"

#define GRP_MENU_NOTWORK -1
#define GRP_MENU_CANCEL -2


class GrpMenu : public CCNode{
private:
	GameMain * pGM;
	ctlMain *pCtl;

	ArrayList<TString> nodes;
	bool isAutoWidth;
	int row, col, width, height;
	int curId, curPage, nodesPerPage;
	int widthPerOne;

	GrpFontL	**pFonts;
	GrpMsgbox	*pMsgbox;
	GrpSelection *pSelection;
	GrpSysUp	*pSysUp;
	GrpSysDown	*pSysDown;

	bool isInControl_;
	CCTexture2D * pTexSystem;

	int keyLastEnter;
	int keyLastEsc;
	int keyLastArrow;

public:
	GrpMenu( GameMain *main, int row, int col, int width, bool autowidth = false );
	~GrpMenu();

	void reloadShaders();

	void setInControl(bool val);
	bool isInControl();

	void refreshNodes(const ArrayList<TString> &nodes);
	int getInput();

	int getIndex();
	void setIndex(int val);

private:
	void initInput();
	void refreshPos();
	void refreshPage();
};

#endif