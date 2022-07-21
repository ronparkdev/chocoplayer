#include "ScrBattle0.h"

#include "../../COMMON/MgrSkill.h"
#include "../../COMMON/MgrItem.h"

#define SCR_BATTLE0_MENU_MOVING_FRAME 10

void ScrBattle0::initMenus()
{
	pMenuAuto = new GrpMenu(pGM, 4,1,76);
	pMenuAuto->setPosition(ccp(SCR_POS_X(38), SCR_POS_Y(160+40)));
	pMenuAuto->setInControl(true);
	this->addChild(pMenuAuto, 1000);

	pMenuAction = new GrpMenu(pGM, 4,1,76);
	pMenuAction->setPosition(ccp(SCR_POS_X(320+38), SCR_POS_Y(160+40)));
	this->addChild(pMenuAction, 1000);

	pMenuEnemy = new GrpMenu(pGM, 4, 1,136);
	pMenuEnemy->setPosition(ccp(SCR_POS_X(68), SCR_POS_Y(160+40)));
	pMenuEnemy->setVisible(false);
	this->addChild(pMenuEnemy, 1003);

	pMenuHero = new GrpMenu(pGM, 4, 1, 244);
	pMenuHero->setPosition(ccp(SCR_POS_X(76+122), SCR_POS_Y(160+40)));
	this->addChild(pMenuHero, 1000);

	pMenuSub = new GrpMenu(pGM, 4, 2, 320);
	pMenuSub->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(200)));
	pMenuSub->setVisible(false);
	this->addChild(pMenuSub, 1002);

	menuSubCaption = new GrpMenu(pGM, 1, 1, 320);
	menuSubCaption->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(16)));
	menuSubCaption->setVisible(false);
	this->addChild(menuSubCaption, 1001);

	int texID = IBuffer::getInstance()->get(imgSystem, IMAGE_BUFFER_NOALPHA_PRESTR + pGM->lsd.System.system_name);
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	pMenuProcBg = new GrpMsgbox(pGM, 320, 80);
	pMenuProcBg->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(200)));
	pMenuProcBg->setVisible(false);
	this->addChild(pMenuProcBg, 1020);

	pMenuProcRes = new GrpFont(pGM);
	pMenuProcRes->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(168)));
	this->addChild(pMenuProcRes, 1021);

	for(int i=0; i<4; i++) {
		pMenuProcRun[i] = new GrpFontL(pGM);
		pMenuProcRun[i]->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(168+16*i)));
		this->addChild(pMenuProcRun[i], 1021);
	}

	heroCmdType.SetLength(4);
	heroCmdTgt.SetLength(4);
}

void ScrBattle0::updateMenuPos( int framedelta )
{
	if (menuMoveLeftFrame > 0) {
		menuMoveLeftFrame -= framedelta;

		float per = (float)(SCR_BATTLE0_MENU_MOVING_FRAME - menuMoveLeftFrame)/SCR_BATTLE0_MENU_MOVING_FRAME;
		if (menuMoveLeftFrame <= 0)
			per = 1.0f;

		if (isMenuModeRight)
			per = 1.0f - per;

		float pos = per * 76.0f;
		pMenuAuto->setPositionX(SCR_POS_X(0+38-pos));
		pMenuAction->setPositionX(SCR_POS_X(320+38-pos));
		pMenuHero->setPositionX(SCR_POS_X(76+122-pos));
	}
}

void ScrBattle0::updateAutoMenu()
{
	LdbVoca &voca = pGM->ldb.dVoca;

	ArrayList<TString> list;
	list.Add(voca.battle_fight);
	list.Add(voca.battle_auto);
	list.Add(voca.battle_escape);

	pMenuAuto->refreshNodes(list);
}

void ScrBattle0::updateHeroMenu()
{
	ArrayList<TString> list;
	
	LdbVoca &voca = pGM->ldb.dVoca;
	for(int i=0; i<nodeHeroLen; i++) {
		list.Add(battler.getS(i, NAME) +
			" \\R[76]" + battler.getS(i, STATE) + 

			" \\L[140]\\C[1]" + voca.hp_short + 
			" \\L[174]" + battler.getS(i, HP) + "/"
			" \\L[200]\\C[0]" + battler.get(i, MAXHP) + 

			" \\L[216]\\C[1]" + voca.mp_short + 
			" \\L[230]" + battler.getS(i, MP)
			);
	}

	pMenuHero->refreshNodes(list);
}

void ScrBattle0::updateActionMenu()
{
	LdbVoca &voca = pGM->ldb.dVoca;

	ArrayList<TString> list;
	list.Add(voca.command_attack);
	list.Add(voca.command_skill);
	list.Add(voca.command_defend);
	list.Add(voca.command_item);

	pMenuAction->refreshNodes(list);
}

void ScrBattle0::updateEnemyMenu()
{
	ArrayList<TString> list;
	ArrayList<int> listIndex;
	for(int i = 0; i<nodeEnemyLen; i++) {
		int id = i + SCR_BATTLE_MAX_HEROS;

		int eLdbID = battler.getID(id);
		//not dead and visible
		if (battler.isAlive(id)) {
			list.Add(pGM->ldb.dEnemy[eLdbID-1].name);
			listIndex.Add(id);
		}
	}

	pMenuEnemy->refreshNodes(list);
	menuEnemyId = listIndex;
}

void ScrBattle0::updateSkillMenu()
{
	ArrayList<TString> list;
	ArrayList<int> index;

	int ldbID = battler.getID(curSelHero);
	LsdCharacter &lsdCh = pGM->lsd.Character[ldbID-1];

	MgrSkill mskill(pGM);

	for(int i=0; i<pGM->ldb.dSkill.GetLength(); i++) {
		int skillID = i+1;
		if (pGM->lsdbuf.GetSkill(ldbID, skillID)) {
			LdbSkill &skill = pGM->ldb.dSkill[i];

			index.Add(skillID);

			int mpcost = mskill.getSpendMP(ldbID, skillID);

			if (mskill.isUseable(ldbID, skillID, true))
				list.Add(mskill.getName(skillID) + "\\L[148]- " + mpcost);
			else
				list.Add("\\C[3]" + mskill.getName(skillID) + "\\L[148]- " + mpcost);
		}
	}

	pMenuSub->refreshNodes(list);
	menuSubId = index;
}

void ScrBattle0::updateItemMenu()
{
	ArrayList<TString> list;
	ArrayList<int> index;
	ArrayList<unsigned short> &items = pGM->lsd.State.item_index;

	MgrItem mitem(pGM);

	for(int i=0; i<items.GetLength(); i++) {
		int itemID = items[i];
		int itemcount = mitem.getItemCount(itemID);

		index.Add(itemID);

		if (mitem.isUseable(itemID, true))
			list.Add(mitem.getName(itemID) + "\\L[148]- " + itemcount);
		else
			list.Add("\\C[3]" + mitem.getName(itemID) + "\\L[148]- " + itemcount);
	}

	pMenuSub->refreshNodes(list);
	menuSubId = index;
}

bool ScrBattle0::setNextActiveHero()
{
	for(int i=curSelHero+1; i<nodeHeroLen; i++) {
		int stateID = battler.getTopState(i);

		if (!battler.isAlive(i))
			continue;

		if (stateID > 0 && pGM->ldb.dCond[stateID-1].restriction != 0)
			continue;

		curSelHero = i;
		pMenuHero->setIndex(i);
		return true;
	}
	pMenuHero->setIndex(-1);
	return false;
}

bool ScrBattle0::setPrevActiveHero()
{
	for(int i=curSelHero-1; i>=0; i--) {
		int stateID = battler.getTopState(i);

		if (!battler.isAlive(i))
			continue;

		if (stateID > 0 && pGM->ldb.dCond[stateID-1].restriction != 0)
			continue;

		curSelHero = i;
		pMenuHero->setIndex(i);
		return true;
	}
	pMenuHero->setIndex(-1);
	return false;
}


void ScrBattle0::chkMenuKeys()
{
	int result;
	int actID = curSelHero;
	int actLdbID = battler.getID(actID);

	switch(curMenu) {
	case BMS_AUTO:
		{
			result = pMenuAuto->getInput();
			if (result != GRP_MENU_NOTWORK) {
				switch(result) {
				case 0://Manual
					curSelHero = -1;

					isActHeroAuto = false;
					isActHeroEscape = false;
					if (!setNextActiveHero()) {
						curSelHero = 4;
						doMenuFinish();
					} else {
						doChangeMenu(BMS_ACTION);

						menuMoveLeftFrame = SCR_BATTLE0_MENU_MOVING_FRAME;
						isMenuModeRight = false;
					}

					break;

				case 1://Auto
					isActHeroAuto = true;
					isActHeroEscape = false;
					doChangeMenu(BMS_PROCESS);
					break;


				case 2://Escape
					isActHeroAuto = false;
					isActHeroEscape = true;
					doChangeMenu(BMS_PROCESS);
					break;
				}
			}
			break;
		}

	case BMS_ACTION:
		{
			result = pMenuAction->getInput();
			
			if (result == GRP_MENU_CANCEL) {
				if (!setPrevActiveHero()) {
					doChangeMenu(BMS_AUTO);

					menuMoveLeftFrame = SCR_BATTLE0_MENU_MOVING_FRAME;
					isMenuModeRight = true;
				}

			} else if (result != GRP_MENU_NOTWORK) {
				heroCmdType[actID] = result;
				switch(result) {
					case 0://Attack
						{
							if (!effVals.isAttackAll(actID)) {
								lastMenu = curMenu;
								doChangeMenu(BMS_TARGET_ENEMY);
							} else {
								doMenuFinish();
							}
							break;
						}
					case 1://Skill
						{
							doChangeMenu(BMS_SKILL);
							updateSkillMenu();
							updateSubCaption(0);
							break;
						}
					case 2://Defence
						{
							doMenuFinish();
							break;
						}
					case 3://Item
						{
							doChangeMenu(BMS_ITEM);
							updateItemMenu();
							updateSubCaption(0);
							break;
						}
				}
			}
			break;
		}

	case BMS_ITEM:
		{
			result = pMenuSub->getInput();
			if (result == GRP_MENU_CANCEL) {
				doChangeMenu(BMS_ACTION);
			} else if (result != GRP_MENU_NOTWORK) {
				int itemID = menuSubId[result];

				LdbItem &item = pGM->ldb.dItem[itemID-1];

				MgrItem mitem(pGM);
				
				if (mitem.isUseable(itemID, true)) {
					heroCmdUsing[actID] = itemID;

					if (item.Type != 9) {
						if (!mitem.isAllEffect(itemID)) {
							lastMenu = curMenu;
							doChangeMenu(BMS_TARGET_HERO);
						} else {
							doMenuFinish();
						}
					} else {
						int skillID = item.skill_id;

						MgrSkill mskill(pGM);

						heroCmdType[actID] = 1;//Skill
						heroCmdUsing[actID] = skillID;

						if (!mskill.isAllEffect(skillID)) {
							lastMenu = curMenu;
							if (mskill.isEffMyTeam(skillID))
								doChangeMenu(BMS_TARGET_HERO);
							else
								doChangeMenu(BMS_TARGET_ENEMY);
					
						} else {
							doMenuFinish();
						}
					}
				}
			}

			result = pMenuSub->getIndex();
			if (menuSubLastRes != result)
				updateSubCaption(result);

			break;
		}
	case BMS_SKILL:
		{
			result = pMenuSub->getInput();
			if (result == GRP_MENU_CANCEL) {
				doChangeMenu(BMS_ACTION);
			} else if (result != GRP_MENU_NOTWORK) {
				int skillID = menuSubId[result];

				MgrSkill mskill(pGM);

				if (mskill.isUseable(actLdbID, skillID, true)) {
					heroCmdUsing[actID] = skillID;

					if (!mskill.isAllEffect(skillID)) {
						lastMenu = curMenu;
						if (mskill.isEffMyTeam(skillID))
							doChangeMenu(BMS_TARGET_HERO);
						else
							doChangeMenu(BMS_TARGET_ENEMY);
					} else {
						doMenuFinish();
					}
				}
			}

			result = pMenuSub->getIndex();
			if (menuSubLastRes != result)
				updateSubCaption(result);

			break;
		}

	case BMS_TARGET_ENEMY:
		{
			result = pMenuEnemy->getInput();
			if (result == GRP_MENU_CANCEL) {
				doChangeMenu(lastMenu);
			} else if (result != GRP_MENU_NOTWORK) {
				heroCmdTgt[actID] = menuEnemyId[result];
				doMenuFinish();
			}

			break;
		}

	case BMS_TARGET_HERO:
		{
			result = pMenuHero->getInput();
			if (result == GRP_MENU_CANCEL) {
				doChangeMenu(lastMenu);
			} else if (result != GRP_MENU_NOTWORK) {
				heroCmdTgt[actID] = result;
				doMenuFinish();
			}

			break;
		}
	}
}

void ScrBattle0::doChangeMenu(BatMenuPos menu) {
	switch(menu) {
	case BMS_AUTO:
		{
			pMenuAuto->setInControl(true);
			pMenuHero->setInControl(false);
			pMenuAction->setInControl(false);
			pMenuEnemy->setInControl(false);
			pMenuEnemy->setVisible(false);
			pMenuSub->setInControl(false);
			pMenuSub->setVisible(false);
			menuSubCaption->setVisible(false);
			this->reorderChild(pMenuHero, 1000);
			break;
		}
	case BMS_ACTION:
		{
			pMenuHero->setIndex(curSelHero);

			pMenuAuto->setInControl(false);
			pMenuHero->setInControl(true);
			pMenuAction->setInControl(true);
			pMenuEnemy->setInControl(false);
			pMenuEnemy->setVisible(false);
			pMenuSub->setInControl(false);
			pMenuSub->setVisible(false);
			menuSubCaption->setVisible(false);
			this->reorderChild(pMenuHero, 1000);
			break;
		}
	case BMS_ITEM:
	case BMS_SKILL:
		{
			pMenuAuto->setInControl(false);
			pMenuHero->setInControl(true);
			pMenuAction->setInControl(true);
			pMenuEnemy->setInControl(false);
			pMenuEnemy->setVisible(false);
			pMenuSub->setInControl(true);
			pMenuSub->setVisible(true);
			menuSubCaption->setVisible(true);
			this->reorderChild(pMenuHero, 1000);
			break;
		}

	case BMS_TARGET_ENEMY:
		{
			pMenuEnemy->setInControl(true);
			pMenuEnemy->setVisible(true);
			this->reorderChild(pMenuHero, 1000);
			break;
		}

	case BMS_TARGET_HERO:
		{
			pMenuHero->setIndex(0);
			pMenuHero->setInControl(true);
			this->reorderChild(pMenuHero, 1010);
			break;
		}

	case BMS_PROCESS:
		{
			pMenuAuto->setInControl(false);
			pMenuHero->setInControl(true);
			pMenuAction->setInControl(true);
			pMenuEnemy->setInControl(false);
			pMenuSub->setInControl(false);
			pMenuSub->setVisible(false);
			menuSubCaption->setVisible(false);

			updateAction();
			chkTurnPriority();

			pMenuProcBg->setVisible(true);
			break;
		}
	}

	
	curMenu = menu;
}

void ScrBattle0::doMenuStart() {
	menuMoveLeftFrame = 1;
	isMenuModeRight = true;
	pMenuProcBg->setVisible(false);
	doChangeMenu(BMS_AUTO);

	for(int i=0; i<4; i++)
		pMenuProcRun[i]->removeText();

	curMsgRow = 0;

	pMenuProcRes->removeText();

	updateHeroMenu();
	updateEnemyMenu();

	turnCnt++;
}

void ScrBattle0::doMenuFinish() {
	if (!setNextActiveHero()) {
		doChangeMenu(BMS_PROCESS);

		isEventNeedCheck = true;
		evtPage = 0;
	} else {
		doChangeMenu(BMS_ACTION);
	}
}

void ScrBattle0::updateSubCaption( int result )
{
	if (result < 0)
		return;

	if (result >= menuSubId.GetLength()) {
		ArrayList<TString> list;
		pMenuSub->refreshNodes(list);
		return;
	}

	int index = menuSubId[result];
	pMenuSub->setIndex(result);

	TString str;

	switch(curMenu) {
	case BMS_ITEM:
		{
			MgrItem mitem(pGM);
			str = mitem.getDesc(index);
			break;
		}

	case BMS_SKILL:
		{
			MgrSkill mskill(pGM);
			str = mskill.getDesc(index);
		}
	}

	ArrayList<TString> list;
	list.Add(str);

	menuSubCaption->refreshNodes(list);

	menuSubLastRes = result;
}