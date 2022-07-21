
#include "ScreenLayerManager.h"

#include "APPMAIN/ScrAppMain.h"
#include "TITLE/ScrTitle.h"
#include "MAP/ScrMap.h"
#include "MENU/ScrMenu.h"
#include "SAVE/ScrSave.h"
#include "GAMEOVER/ScrGameover.h"
#include "BATTLE/ScrBattle0.h"
#include "BATTLE/ScrBattle3.h"
#include "MENU/ScrMenuEquip.h"
#include "MENU/ScrMenuItem.h"
#include "MENU/ScrMenuOrder.h"
#include "MENU/ScrMenuQuit.h"
#include "MENU/ScrMenuSkill.h"
#include "MENU/ScrMenuState.h"
#include "MENU/ScrMenuUse.h"
#include "MENU/ScrMenuTeleport.h"
#include "SHOP/ScrShop.h"
#include "NAMEINPUT/ScrNameInput.h"

void ScreenLayerManager::setSettingScrMap()
{
	LsdObjectCommon *obj;
	LsdObjectCommon *objhero = &(pGM->lsd.LocHero);
	switch(pGM->lsd.LocHero.ride_type) {
		case 0:obj = objhero;break;
		case 1:obj = &(pGM->lsd.LocRide[0]);break;
		case 2:obj = &(pGM->lsd.LocRide[1]);break;
		case 3:obj = &(pGM->lsd.LocRide[2]);break;
	}

	sgMap_mapId	= obj->map_id;
	sgMap_posX	= obj->posX;
	sgMap_posY	= obj->posY;
	sgMap_direct	= obj->direction;

	sgMap_isInit = false;
}

void ScreenLayerManager::setSettingScrMap( int mapid, int posx, int posy, int chardirect, bool init) {
	sgMap_mapId = mapid;
	sgMap_posX = posx;
	sgMap_posY = posy;
	sgMap_direct = chardirect;
	sgMap_isInit = init;
}

void ScreenLayerManager::setSettingScrMenu(ScrMenuType type, int arg1, int arg2, bool arg3) {
	sgMenu_type	= type;
	sgMenu_arg1	= arg1;
	sgMenu_arg2	= arg2;
	sgMenu_arg3	= arg3;
}

void ScreenLayerManager::setSettingScrSave( bool isSave, bool isfromTitle ) {
	sgSave_isSave = isSave;
	sgSave_isFromTitle = isfromTitle;
}

void ScreenLayerManager::setSettingScrBattle( 
		int battlepos, int battleid, const ScrBattleBgInfo &bg_info, 
		bool escapemap, bool losetomap, bool firstattack)
{
	sgBat_batPos = battlepos;
	sgBat_batId = battleid;
	sgBat_bgInfo = bg_info;
	sgBat_isEscapeToMap = escapemap;
	sgBat_isLoseToMap = losetomap;
	sgBat_isFirstAttack = firstattack;
}

void ScreenLayerManager::setSettingScrShop( int worktype, int langtype, ArrayList<int> items )
{
	sgShop_workType = worktype;
	sgShop_langType = langtype;
	sgShop_itemList = items;
}

void ScreenLayerManager::setSettingScrNameInput( TString *defstr )
{
	sgName_defStr = defstr;
}

void ScreenLayerManager::setAniType( ScreenType type, ScreenType lastType ) {
	LsdSystem &sys = pGM->lsd.System;

	if (lastType == scr_app_main) {
		aniTypeShow = 20;
		aniTypeHide = 20;

		GameMain::getInstance()->snd.StopMusic();
		GameMain::getInstance()->snd.StopSound();
		return;
	}

	switch(type) {
	case scr_app_main:
		{
			aniTypeShow = 20;
			aniTypeHide = 0;
			break;
		}
	case scr_battle:
		{
			aniTypeShow = sys.battle_start_fadein;
			aniTypeHide = sys.battle_start_fadeout;
			break;
		}

	case scr_map:
		{
			if (scrType == scr_battle)
			{
				aniTypeShow = sys.battle_end_fadein;
				aniTypeHide = sys.battle_end_fadeout;
			} else {
				aniTypeShow = sys.transition_in;
				aniTypeHide = sys.transition_out;
			}
			break;
		}

	default:
		{
			aniTypeShow = 0;
			aniTypeHide = 0;
			break;
		}
	}
}

void ScreenLayerManager::addScreen( ScreenType type )
{
	if (pCurLayer != NULL) {
		this->removeChild(pCurLayer);
		pCurLayer->autorelease();
		pCurLayer = NULL;
	}

	IBuffer::getInstance()->mapChanging();

	switch(type) {
	case scr_app_main:	pCurLayer = new ScrAppMain(pGM);break;
	case scr_title:		pCurLayer = new ScrTitle(pGM); break;
	case scr_gameover:	pCurLayer = new ScrGameover(pGM);break;

	case scr_map:
		{
			CCLOG((_ST("Main::MapChange ") + sgMap_mapId).getTextUTF8());

			LsdObjectCommon *obj;
			LsdObjectCommon *objhero = &(pGM->lsd.LocHero);
			switch(pGM->lsd.LocHero.ride_type) {
			case 0:obj = objhero;break;
			case 1:obj = &(pGM->lsd.LocRide[0]);break;
			case 2:obj = &(pGM->lsd.LocRide[1]);break;
			case 3:obj = &(pGM->lsd.LocRide[2]);break;
			}

			pGM->curMapId = objhero->map_id = obj->map_id = sgMap_mapId;
			objhero->posX = obj->posX = sgMap_posX;
			objhero->posY = obj->posY = sgMap_posY;
			if (sgMap_direct != 0) {
				objhero->direction = obj->direction = sgMap_direct;
			}

			pCurLayer 
				= new ScrMap(
					pGM, 
					sgMap_mapId, 
					sgMap_isInit
					);

			break;
		}

	case scr_menu:	
		{
			switch(sgMenu_type) {
				case scr_menu_main:	pCurLayer = new ScrMenu(pGM);break;	
				case scr_menu_equip:pCurLayer = new ScrMenuEquip(pGM, sgMenu_arg1);break;	
				case scr_menu_item:	pCurLayer = new ScrMenuItem(pGM);break;	
				case scr_menu_order:pCurLayer = new ScrMenuOrder(pGM);break;	
				case scr_menu_quit:	pCurLayer = new ScrMenuQuit(pGM);break;	
				case scr_menu_skill:pCurLayer = new ScrMenuSkill(pGM, sgMenu_arg1);break;	
				case scr_menu_state:pCurLayer = new ScrMenuState(pGM, sgMenu_arg1);break;	
				case scr_menu_use:	pCurLayer = new ScrMenuUse(pGM, sgMenu_arg1, sgMenu_arg2, sgMenu_arg3);break;
				case scr_menu_tel:	pCurLayer = new ScrMenuTeleport(pGM, sgMenu_arg2, sgMenu_arg3);break;
			}
			
			break;
		}

	case scr_save:
		{
			pCurLayer 
				= new ScrSave(	
					pGM, 
					sgSave_isSave, 
					sgSave_isFromTitle
					); 
			
			break;
		}
            
	case scr_battle:
		{
			if (pGM->getVersion() == RPG2000) {
				pCurLayer 
					= new ScrBattle0(
						pGM,
						sgBat_batPos,
						sgBat_batId,
						sgBat_bgInfo,
						sgBat_isEscapeToMap,
						sgBat_isLoseToMap,
						sgBat_isFirstAttack
						); 
			} else {
				pCurLayer 
					= new ScrBattle3(
						pGM,
						sgBat_batPos,
						sgBat_batId,
						sgBat_bgInfo,
						sgBat_isEscapeToMap,
						sgBat_isLoseToMap,
						sgBat_isFirstAttack
						); 
			}
			break;
		}
            
	case scr_shop:
		{
			pCurLayer
				= new ScrShop(
						pGM,
						(ScrShopWorkType)sgShop_workType,
						sgShop_langType,
						sgShop_itemList
					);
			break;
		}
            
	case scr_nameinput:
		{
			pCurLayer
				= new ScrNameInput(
						pGM,
						sgName_defStr
					);
			break;
		}

    default:
            break;
	}

	pCurLayer->Init();
	pCurLayer->setVisible(true);
	setDefaultPosition();
	this->addChild(pCurLayer, 0);
	
	isFrameNeedRefresh = true;
	gcFrameLeft = 1;
}
