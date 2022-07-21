#ifndef __R2K_SCENE_SHOP__
#define __R2K_SCENE_SHOP__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GameMain.h"
#include "../../DATA/ArrayList.h"
#include "../../GRAPHIC/GrpFontL.h"
#include "../../GRAPHIC/OBJECT/GrpSelection.h"
#include "../../GRAPHIC/OBJECT/GrpFace.h"
#include "../../GRAPHIC/GrpMenu.h"
#include "../../GRAPHIC/OBJECT/GrpMsgbox.h"

USING_NS_CC;

enum ScrShopWorkType{
	SCR_SHOP_WORK_BOTH	= 0,
	SCR_SHOP_WORK_BUY	= 1,
	SCR_SHOP_WORK_SELL	= 2
};

class ScrShop : public ScreenLayer
{
	enum ScrShopPos{
		MENU_NONE,
		MENU_MAIN,
		MENU_BUY,
		MENU_BUY_COUNT,
		MENU_SELL,
		MENU_SELL_COUNT,
	};

	GameMain *pGM;
	ArrayList<int> boughtItemList;
	ScrShopPos curScreen;

	TString msg_buy;
	TString msg_buynum;
	TString msg_buysel;
	TString msg_greet;
	TString msg_leave;
	TString msg_purchased;
	TString msg_regreet;
	TString msg_sell;
	TString msg_sellnum;
	TString msg_sellsel;
	TString msg_sold;

	GrpMenu *pMenuMain;
	GrpMenu *pMenuCaption;

	GrpMsgbox	*pSubNothing;
	GrpMenu		*pSubMenuItemBuy;
	GrpMenu		*pSubMenuItemSell;
	GrpMsgbox	*pSubMenuHero;
	GrpMenu		*pSubMenuState;
	GrpMenu		*pSubMenuMoney;

	int lastIndex;
	int selItemId;
	int selItemCnt;
	int selItemMaxCnt;
	bool isSelEnabled;
	int lastKeyArrow;
	int lastKeyEnter;
	int lastKeyEsc;

public:
	static bool g_bought_something;

	ScrShop( GameMain *main, ScrShopWorkType worktype, int langtype, const ArrayList<int> &item_list );
	~ScrShop();

	void initLang(int type);
	void setPos(ScrShopPos pos);

	void reloadShaders();
	void Draw(int framedelta);

	void refreshCaption(const TString &str);
	void refreshItemCount();
private:
};

#endif 
