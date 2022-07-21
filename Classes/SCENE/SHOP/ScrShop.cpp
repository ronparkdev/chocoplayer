#include "ScrShop.h"

#include "../../COMMON/MgrItem.h"
#include "../../COMMON/MgrState.h"

bool ScrShop::g_bought_something = false;

ScrShop::ScrShop( GameMain *main, ScrShopWorkType worktype, int langtype, const ArrayList<int> &item_list )
{
	pGM = main;
	boughtItemList = item_list;
	curScreen = MENU_NONE;

	pMenuCaption = new GrpMenu(pGM, 1, 1, 320);
	pMenuCaption->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(16)));
	this->addChild(pMenuCaption,0);

	pSubNothing = new GrpMsgbox(pGM, 320, 128);
	pSubNothing->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(96)));
	this->addChild(pSubNothing,0);

	pMenuMain = new GrpMenu(pGM, 4, 1, 320);
	pMenuMain->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(200)));
	this->addChild(pMenuMain,0);

	pSubMenuItemBuy = new GrpMenu(pGM, 7, 1, 184);
	pSubMenuItemBuy->setPosition(ccp(SCR_POS_X(92), SCR_POS_Y(96)));
	this->addChild(pSubMenuItemBuy,1);

	pSubMenuItemSell = new GrpMenu(pGM, 7, 2, 320);
	pSubMenuItemSell->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(96)));
	this->addChild(pSubMenuItemSell,1);

	pSubMenuHero = new GrpMsgbox(pGM, 136, 48);
	pSubMenuHero->setPosition(ccp(SCR_POS_X(252), SCR_POS_Y(56)));
	this->addChild(pSubMenuHero,1);

	pSubMenuState = new GrpMenu(pGM, 2, 1, 136);
	pSubMenuState->setPosition(ccp(SCR_POS_X(252), SCR_POS_Y(104)));
	this->addChild(pSubMenuState,1);

	pSubMenuMoney = new GrpMenu(pGM, 1, 1, 136);
	pSubMenuMoney->setPosition(ccp(SCR_POS_X(252), SCR_POS_Y(144)));
	this->addChild(pSubMenuMoney,1);

	initLang(langtype);
	setPos(MENU_MAIN);

	g_bought_something = false;
}

void ScrShop::initLang(int type) {
	LdbVoca &voca = pGM->ldb.dVoca;
	msg_buy		= voca.shop_buy[type];
	msg_buynum	= voca.shop_buy_number[type];
	msg_buysel	= voca.shop_buy_select[type];
	msg_greet	= voca.shop_greeting[type];
	msg_leave	= voca.shop_leave[type];
	msg_purchased = voca.shop_purchased[type];
	msg_regreet = voca.shop_regreeting[type];
	msg_sell	= voca.shop_sell[type];
	msg_sellnum = voca.shop_sell_number[type];
	msg_sellsel = voca.shop_sell_select[type];
	msg_sold	= voca.shop_sold[type];
}

ScrShop::~ScrShop()
{

}

void ScrShop::setPos( ScrShopPos pos )
{
	LdbVoca &voca = pGM->ldb.dVoca;

	MgrItem mitem(pGM);

	ArrayList<TString> list;
	ArrayList<TString> list2;

	switch(pos) {
	case MENU_MAIN:
		{
			refreshCaption("");

			if (curScreen == MENU_NONE)
				list.Add(msg_greet);
			else
				list.Add(msg_regreet);

			list.Add(msg_buy);
			list.Add(msg_sell);
			list.Add(msg_leave);

			pMenuMain->refreshNodes(list);

			break;
		}

	case MENU_BUY:
		{
			if (curScreen == MENU_BUY_COUNT && isSelEnabled)
				list.Add(msg_purchased);
			else
				list.Add(msg_buysel);

			pMenuMain->refreshNodes(list);

			ArrayList<LdbItem> &its = pGM->ldb.dItem;

			for(int i=0; i<boughtItemList.GetLength(); i++) {
				LdbItem &it = its[boughtItemList[i]-1];
				if (pGM->lsd.State.money >= it.price)
					list2.Add(mitem.getName(boughtItemList[i]) + "\\L[168]" + it.price);
				else
					list2.Add("\\C[3]" + mitem.getName(boughtItemList[i])  + "\\L[168]" + it.price);
			}

			pSubMenuItemBuy->refreshNodes(list2);

			list.RemoveAll();
			list.Add(_ST("\\L[120]") + pGM->lsd.State.money + _ST(" ") + pGM->ldb.dVoca.gold);
			pSubMenuMoney->refreshNodes(list);

			break;
		}

	case MENU_BUY_COUNT:
		{
			selItemId = boughtItemList[pSubMenuItemBuy->getIndex()];
			selItemCnt = 1;
			selItemMaxCnt = pGM->lsd.State.money / pGM->ldb.dItem[selItemId-1].price;
			refreshItemCount();

			break;
		}

	case MENU_SELL:
		{
			if (curScreen == MENU_SELL_COUNT && isSelEnabled)
				list.Add(msg_sold);
			else
				list.Add(msg_sellsel);

			pMenuMain->refreshNodes(list);

			ArrayList<LdbItem> &its = pGM->ldb.dItem;
			LsdState &state = pGM->lsd.State;

			for(int i=0; i<state.item_index.GetLength(); i++) {
				int itID = state.item_index[i];
				LdbItem &it = its[itID-1];

				if (it.price > 0)
					list2.Add(mitem.getName(itID) + _ST("\\L[150]: ") + state.item_count[i]);
				else
					list2.Add("\\C[3]" + mitem.getName(itID) + _ST("\\L[150]: ") + state.item_count[i]);
			}

			pSubMenuItemSell->refreshNodes(list2);

			break;
		}

	case MENU_SELL_COUNT:
		{
			MgrItem mitem(pGM);

			selItemId = pGM->lsd.State.item_index[pSubMenuItemSell->getIndex()];
			selItemCnt = 1;
			selItemMaxCnt = mitem.getItemCount(selItemId);
			refreshItemCount();

			break;
		}
	}

	//bool fromTitle = (m_pos == MENU_MAIN) ^ (pos == MENU_MAIN);
	switch(pos) {
	case MENU_MAIN:
		{
			pSubMenuItemBuy	->setVisible(false);
			pSubMenuItemSell	->setVisible(false);
			pSubMenuHero		->setVisible(false);
			pSubMenuState		->setVisible(false);
			pSubMenuMoney		->setVisible(false);

			pMenuMain		->setInControl(true);
			pMenuMain		->setIndex(1);
			pSubMenuItemBuy	->setInControl(false);
			pSubMenuItemSell	->setInControl(false);
			break;

		}

	case MENU_BUY:
		{
			pSubMenuItemBuy	->setVisible(true);
			pSubMenuHero		->setVisible(true);
			pSubMenuState		->setVisible(true);
			pSubMenuMoney		->setVisible(true);

			pMenuMain		->setInControl(false);
			pSubMenuItemBuy	->setInControl(true);

			if (curScreen == MENU_MAIN)
				pSubMenuItemBuy	->setIndex(0);
			break;
		}

	case MENU_SELL:
		{
			pSubMenuItemSell	->setVisible(true);

			pMenuMain		->setInControl(false);
			pSubMenuItemSell	->setInControl(true);

			if (curScreen == MENU_MAIN)
				pSubMenuItemSell	->setIndex(0);
			break;
		}

	case MENU_BUY_COUNT:
	case MENU_SELL_COUNT:
		{
			pSubMenuItemBuy	->setInControl(false);
			pSubMenuItemSell	->setInControl(false);
			curScreen = pos;
			refreshItemCount();

			lastKeyArrow = pGM->ctl->getKey(rkc_arrow);
			lastKeyEnter = pGM->ctl->getKey(rkc_enter);
			lastKeyEsc	 = pGM->ctl->getKey(rkc_esc);
		}
	}

	lastIndex = -1;

	curScreen = pos;
}

void ScrShop::reloadShaders()
{
	pMenuMain->reloadShaders();
	pMenuCaption->reloadShaders();

	pSubMenuItemBuy->reloadShaders();
	pSubMenuItemSell->reloadShaders();
	pSubMenuState->reloadShaders();
	pSubMenuMoney->reloadShaders();
}

void ScrShop::Draw( int framedelta )
{
	int result, last_index;
	MgrItem mitem(pGM);
	LdbVoca &voca = pGM->ldb.dVoca;
	ArrayList<TString> list;
	switch(curScreen) {
	case MENU_MAIN:
		{
			last_index = pMenuMain->getIndex();
			result = pMenuMain->getInput();
			if (result == GRP_MENU_CANCEL || result == 3) {
				pGM->slm->setSettingScrMap();
				pGM->slm->setScreen(scr_map, MENU_CHANGE_TIME);
			} else if (result != GRP_MENU_NOTWORK) {
				switch(result) {
				case 1:setPos(MENU_BUY);break;
				case 2:setPos(MENU_SELL);break;
				}
			}
			if (pMenuMain->isInControl() && pMenuMain->getIndex() == 0)
				pMenuMain->setIndex((last_index > 1)?1:3);
			break;
		}
	case MENU_BUY:
		{
			result = pSubMenuItemBuy->getInput();
			if (result == GRP_MENU_CANCEL) {
				setPos(MENU_MAIN);
			} else if (result != GRP_MENU_NOTWORK) {
				int itemID = boughtItemList[result];
				LdbItem &item = pGM->ldb.dItem[itemID-1];
				if (pGM->lsd.State.money > item.price)
					setPos(MENU_BUY_COUNT);
			}
			if (lastIndex != pSubMenuItemBuy->getIndex()) {
				lastIndex = pSubMenuItemBuy->getIndex();
				int itemID = boughtItemList[lastIndex];
				MgrItem mitem(pGM);
				
				refreshCaption(mitem.getDesc(itemID));

				list.Add("\\C[1]" + voca.possessed_items + "\\L[120]\\C[0]" + mitem.getItemCount(itemID));
				list.Add("\\C[1]" + voca.equipped_items + "\\L[120]\\C[0]" + mitem.getEquipedCount(itemID));
				pSubMenuState->refreshNodes(list);
			}

			break;
		}
	case MENU_SELL:
		{
			result = pSubMenuItemSell->getInput();
			if (result == GRP_MENU_CANCEL) {
				setPos(MENU_MAIN);
			} else if (result != GRP_MENU_NOTWORK) {
				int itemID = pGM->lsd.State.item_index[result];
				LdbItem &item = pGM->ldb.dItem[itemID-1];
				if (item.price > 0)
					setPos(MENU_SELL_COUNT);
			}
			if (lastIndex != pSubMenuItemSell->getIndex()) {
				lastIndex = pSubMenuItemSell->getIndex();
				int itemID = pGM->lsd.State.item_index[lastIndex];
				LdbItem &item = pGM->ldb.dItem[itemID-1];

				refreshCaption(mitem.getDesc(itemID));
			}
			break;
		}
	case MENU_BUY_COUNT:
	case MENU_SELL_COUNT:
		{
			int key_arrow	= pGM->ctl->getKey(rkc_arrow);
			int key_enter	= pGM->ctl->getKey(rkc_enter);
			int key_esc		= pGM->ctl->getKey(rkc_esc);

			if (key_arrow > 0 && lastKeyArrow <= 0) {
				int cnt = selItemCnt;
				switch(key_arrow) {
					case rkca_up:	cnt += 1;break;
					case rkca_down:	cnt -= 1;break;
					case rkca_right:cnt += 10;break;
					case rkca_left:	cnt -= 10;break;
				}
				if (cnt < 1)
					cnt = 1;
				if (cnt > selItemMaxCnt)
					cnt = selItemMaxCnt;
				
				selItemCnt = cnt;
				refreshItemCount();
			} else if (key_enter > 0 && lastKeyEnter <= 0) {
				LdbItem &item = pGM->ldb.dItem[selItemId-1];
				MgrItem mitem(pGM);
				MgrState mstate(pGM);
				isSelEnabled = true;
				int price_one, price;
				if (curScreen == MENU_BUY_COUNT) {
					price_one = item.price;
					price = price_one * selItemCnt;
					mstate.addMoney(-price);
					mitem.addItemCount(selItemId, selItemCnt);
					g_bought_something = true;
					setPos(MENU_BUY);
				} else {
					price_one = item.price / 2;
					price = price_one * selItemCnt;
					mstate.addMoney(price);
					mitem.addItemCount(selItemId, -selItemCnt);
					g_bought_something = true;
					setPos(MENU_SELL);
				}
			} else if (key_esc > 0 && lastKeyEsc <= 0) {
				isSelEnabled = false;
				if (curScreen == MENU_BUY_COUNT)
					setPos(MENU_BUY);
				else
					setPos(MENU_SELL);
			}

			lastKeyArrow= key_arrow;
			lastKeyEnter= key_enter;
			lastKeyEsc	= key_esc;
		}
	}
	
	

	//BuyCount
	
}

void ScrShop::refreshCaption( const TString &str )
{
	ArrayList<TString> list;
	list.Add(str);

	pMenuCaption->refreshNodes(list);
}

void ScrShop::refreshItemCount()
{
	ArrayList<TString> list;
	MgrItem mitem(pGM);
	LdbItem &item = pGM->ldb.dItem[selItemId-1];
	int price_one, price = item.price * selItemCnt;

	if (curScreen == MENU_BUY_COUNT) {
		list.Add(msg_buynum);
		price_one = item.price;
	} else {
		list.Add(msg_sellnum);
		price_one = item.price / 2;
	}
	price = price_one * selItemCnt;
	
	list.Add(mitem.getName(selItemId) + " " + 
			 price_one + " " + 
			 pGM->ldb.dVoca.gold + " x " + 
			 selItemCnt + " = " + 
			 price + pGM->ldb.dVoca.gold);

	pMenuMain->refreshNodes(list);
}
