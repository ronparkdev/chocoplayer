#include "ScrBattle3.h"

#include "../../COMMON/MgrSkill.h"
#include "../../COMMON/MgrState.h"
#include "../../COMMON/MgrItem.h"

#define SCR_BATTLE3_MENU_MOVING_FRAME 30

void ScrBattle3::initMenus()
{
	//Init Menus
	pMenuCaption = new GrpMenu(pGM, 1, 1, 320);
	pMenuCaption->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(16)));
	pMenuCaption->setVisible(false);
	this->addChild(pMenuCaption, 1000);

	pMenuAuto = new GrpMenu(pGM, 4,1,76);
	this->addChild(pMenuAuto, 1000);

	pMenuAction = new GrpMenu(pGM, 4,1,76);
	this->addChild(pMenuAction, 1001);

	pMenuEnemy = new GrpMenu(pGM, 4, 1,104);
	this->addChild(pMenuEnemy, 1000);

	pMenuHero = new GrpMenu(pGM, 4, 1, (batType==BTT_3A)?216:244);
	this->addChild(pMenuHero, 1000);

	pMenuSub = new GrpMenu(pGM, 4, 2, 320);
	pMenuSub->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(200)));
	pMenuSub->setVisible(false);
	this->addChild(pMenuSub, 1010);

	pMenuSubCaption = new GrpMenu(pGM, 1, 1, 320);
	pMenuSubCaption->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(16)));
	pMenuSubCaption->setVisible(false);
	this->addChild(pMenuSubCaption, 1011);

	int tex2ID = IBuffer::getInstance()->get(imgSystem2, pGM->ldb.dSys.system2_name);
	CCTexture2D *tex_sys2 = IBuffer::getInstance()->get(imgSystem2, tex2ID);

	switch(batType) {
	case BTT_3A:
		{
			curMenu = BMS_HERO;

			pMenuAuto->setVisible(false);
			pMenuAction->setVisible(false);
			pMenuAction->setPosition(ccp(SCR_POS_X(28+38), SCR_POS_Y(160+40)));
			pMenuEnemy->setPosition(ccp(SCR_POS_X(0+52), SCR_POS_Y(160+40)));
			pMenuHero->setPosition(ccp(SCR_POS_X(104+108), SCR_POS_Y(160+40)));

			for(int i=0; i<nodeHeroLen; i++) {
				pGazeTn[i] = new GrpGaze(tex_sys2, 2);
				pGazeTn[i]->setPosition(ccp(SCR_POS_X(292), SCR_POS_Y(176+i*16)));
				this->addChild(pGazeTn[i], 1001);
			}
			break;
		}

	case BTT_3B:
		{
			curMenu = BMS_AUTO;
			pMenuAuto->setInControl(true);

			pMenuAuto->setPosition(ccp(SCR_POS_X(0+38), SCR_POS_Y(160+40)));
			pMenuAction->setPosition(ccp(SCR_POS_X(320+38), SCR_POS_Y(160+40)));
			pMenuEnemy->setVisible(false);
			pMenuHero->setPosition(ccp(SCR_POS_X(76+122), SCR_POS_Y(160+40)));

			for(int i=0; i<nodeHeroLen; i++) {
				pGazeTn[i] = new GrpGaze(tex_sys2, 2);
				pGazeTn[i]->setPosition(ccp(SCR_POS_X(292), SCR_POS_Y(176+i*16)));
				this->addChild(pGazeTn[i], 1001);
			}
			break;
		}

	case BTT_3C:
		{
			curMenu = BMS_AUTO;
			pMenuAuto->setInControl(true);
			pMenuAction->setVisible(false);

			pMenuAuto->setPosition(ccp(SCR_POS_X(0+38), SCR_POS_Y(160+40)));
			pMenuAction->setPosition(ccp(SCR_POS_X(0+38), SCR_POS_Y(80+40)));
			pMenuEnemy->setVisible(false);
			pMenuHero->setVisible(false);

			for(int i=0; i<nodeHeroLen; i++) {
				int heroID = battler.getID(i);

				pMenuFade[i] = new GrpFace(pGM);
				pMenuFade[i]->setPosition(ccp(SCR_POS_X(24+80*i+76), SCR_POS_Y(208)));
				pMenuFade[i]->set(heroID);
				this->addChild(pMenuFade[i], 1001);

				pGazeHp[i] = new GrpGaze(tex_sys2, 0);
				pGazeHp[i]		->setPosition(ccp(SCR_POS_X(56+80*i+76), SCR_POS_Y(184+8)));
				this->addChild(pGazeHp[i], 1001);
				
				pGazeNum[i*2+0] = new GrpBatNum(tex_sys2);
				pGazeNum[i*2+0]	->setPosition(ccp(SCR_POS_X(56+80*i+92), SCR_POS_Y(184+8)));
				this->addChild(pGazeNum[i*2+0], 1002);


				pGazeMp[i] = new GrpGaze(tex_sys2, 1);
				pGazeMp[i]		->setPosition(ccp(SCR_POS_X(56+80*i+76), SCR_POS_Y(184+8+16)));
				this->addChild(pGazeMp[i], 1001);
				
				pGazeNum[i*2+1] = new GrpBatNum(tex_sys2);
				pGazeNum[i*2+1]	->setPosition(ccp(SCR_POS_X(56+80*i+92), SCR_POS_Y(184+8+16)));
				this->addChild(pGazeNum[i*2+1], 1002);


				pGazeTn[i] = new GrpGaze(tex_sys2, 2);
				pGazeTn[i]->setPosition(ccp(SCR_POS_X(56+80*i+76), SCR_POS_Y(184+8+32)));
				this->addChild(pGazeTn[i], 1001);
			}
			break;
		}
	}

}

void ScrBattle3::updateMenuPos(int framedelta) {
	if (menuMoveLeftFrame > 0) {
		menuMoveLeftFrame -= framedelta;

		float per = (float)(SCR_BATTLE3_MENU_MOVING_FRAME - menuMoveLeftFrame)/SCR_BATTLE3_MENU_MOVING_FRAME;
		if (menuMoveLeftFrame <= 0)
			per = 1.0f;

		if (isMenuModeRight)
			per = 1.0f - per;

		switch(batType) {
		case BTT_3B:
			{
				float pos = per * 76.0f;
				pMenuAuto->setPositionX(SCR_POS_X(0+38-pos));
				pMenuAction->setPositionX(SCR_POS_X(320+38-pos));
				pMenuHero->setPositionX(SCR_POS_X(76+122-pos));

				for(int i=0; i<nodeHeroLen; i++)
					pGazeTn[i]->setPosition(ccp(SCR_POS_X(292-pos), SCR_POS_Y(176+i*16)));

				break;
			}

		case BTT_3C:
			{
				float pos = per * 76.0f;
				pMenuAuto->setPositionX(SCR_POS_X(0+38-pos));
				for(int i=0; i<nodeHeroLen; i++) {
					pMenuFade[i]		->setPosition(ccp(SCR_POS_X(24+80*i-pos+76),	SCR_POS_Y(208)));
					pGazeHp[i]		->setPosition(ccp(SCR_POS_X(56+80*i-pos+76),	SCR_POS_Y(184+8)));
					pGazeNum[i*2+0]	->setPosition(ccp(SCR_POS_X(56+80*i-pos+92),	SCR_POS_Y(184+8)));
					pGazeMp[i]		->setPosition(ccp(SCR_POS_X(56+80*i-pos+76),	SCR_POS_Y(184+8+16)));
					pGazeNum[i*2+1]	->setPosition(ccp(SCR_POS_X(56+80*i-pos+92),	SCR_POS_Y(184+8+16)));
					pGazeTn[i]		->setPosition(ccp(SCR_POS_X(56+80*i-pos+76),	SCR_POS_Y(184+8+32)));
				}
				break;
			}

		}
	}
}

void ScrBattle3::updateAutoMenu()
{
	LdbVoca &voca = pGM->ldb.dVoca;

	int heroLdbID = battler.getID(curSelHero);

	LdbCharacter &ldbCh = pGM->ldb.dChar[heroLdbID];

	ArrayList<TString> list;
	list.Add(voca.battle_fight);
	list.Add(voca.battle_auto);
	//list.Add("\\C[0]" + voca.battle_escape);
	pMenuAuto->refreshNodes(list);
}

void ScrBattle3::updateHeroMenu()
{
	ArrayList<TString> list;
	switch(batType) {
		case BTT_3A:
		{
			for(int i=0; i<nodeHeroLen; i++) {
				list.Add(battler.getS(i, NAME) +
						 "\\R[76]" + battler.getS(i, STATE) + 
						 ">\\L[149]" + battler.get(i, HP) + ">"
						 );
			}
			break;
		}
		case BTT_3B:
		{
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
			break;
		}
		case BTT_3C:
		{
			for(int i=0; i<nodeHeroLen; i++) {
				list.Add("HAHAHA...");

				pGazeHp[i]->refresh((float)battler.get(i, HP)/battler.get(i, MAXHP));
				pGazeNum[i*2+0]->set(battler.get(i,HP));
				pGazeMp[i]->refresh((float)battler.get(i, MP)/battler.get(i, MAXMP));
				pGazeNum[i*2+1]->set(battler.get(i,MP));
			}
			break;
		}
	}

	for(int i=0; i<nodeHeroLen; i++) {
		if (!battler.isAlive(i)) {
			for(int j=0; j<heroReadyQueue.GetLength(); j++)
				if (heroReadyQueue[j] == i) {
					heroReadyQueue.Remove(j);
					j--;
				}

			if (curSelHero == i)
				doMenuFinish();

			turnPer[i] = 0.0f;
		}
	}

	pMenuHero->refreshNodes(list);
}

void ScrBattle3::updateActionMenu()
{
	LdbVoca &voca = pGM->ldb.dVoca;

	int heroLdbID = battler.getID(curSelHero);

	ArrayList<LdbBattleCmdNode> &batCmds = pGM->ldb.dBatCmd.commends;
	ArrayList<int> &cmds = pGM->lsd.Character[heroLdbID-1].battle_commands;

	ArrayList<TString> list;
	ArrayList<int> listIndex;
	for(int i=0; i<cmds.GetLength(); i++) {
		int cmdID = cmds[i];

		if (0 < cmdID && cmdID < batCmds.GetLength()+1) {
			list.Add(batCmds[cmdID-1].cname);
			listIndex.Add(cmdID);
		} else if (cmdID <= 0)
			break;
	}

	list.Add(voca.row);
	listIndex.Add(0);

	pMenuAction->refreshNodes(list);
	pMenuActionId = listIndex;
}

void ScrBattle3::updateEnemyMenu()
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

void ScrBattle3::updateSkillMenu( int sType )
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

			if (sType >= 0) {
				if (skill.type != sType + 4)
					continue;
			}

			index.Add(skillID);

			int mpcost = mskill.getSpendMP(ldbID, skillID);

			if (mskill.isUseable(ldbID, skillID, true))
				list.Add(mskill.getName(i+1) + "\\L[148]- " + mpcost);
			else
				list.Add("\\C[3]" + mskill.getName(i+1) + "\\L[148]- " + mpcost);
		}
	}

	pMenuSub->refreshNodes(list);
	menuSubId = index;
}

void ScrBattle3::updateItemMenu()
{
	ArrayList<TString> list;
	ArrayList<int> index;

	ArrayList<unsigned short> &items = pGM->lsd.State.item_index;
	
	MgrItem mitem(pGM);

	for(int i=0; i<items.GetLength(); i++) {
		int itemID = items[i];

		LdbItem &item = pGM->ldb.dItem[itemID-1];

		index.Add(itemID);

		int itemcount = mitem.getItemCount(itemID);

		if (mitem.isUseable(itemID, true))
			list.Add(mitem.getName(itemID) + "\\L[148]- " + itemcount);
		else
			list.Add("\\C[3]" + mitem.getName(itemID) + "\\L[148]- " + itemcount);
	}

	pMenuSub->refreshNodes(list);
	menuSubId = index;
}

void ScrBattle3::chkMenuKeys() {
	int result;
	switch(curMenu) {
		case BMS_AUTO:
		{
			result = pMenuAuto->getInput();
			if (result != GRP_MENU_NOTWORK) {
				switch(result) {
					case 0://Manual
					case 1://Auto
						curMenu = BMS_HERO;
						pMenuAuto->setInControl(false);
						pMenuHero->setInControl(true);
						isBatAuto = (result == 1);

						menuMoveLeftFrame = SCR_BATTLE3_MENU_MOVING_FRAME;
						isMenuModeRight = false;
						break;
			
					case 2://Escape -> NotWork
						break;
				}
			}
			break;
		}

		case BMS_HERO:
		{
			if (batType == BTT_3A)
				break;

			result = pMenuHero->getInput();
			
			if (curSelHero != pMenuHero->getIndex()) {
				if (0<= curSelHero && curSelHero < nodeHeroLen)
					pNodeHero[curSelHero]->setHand(false);

				curSelHero = pMenuHero->getIndex();

				updateActionMenu();
			}

			if (0<= curSelHero && curSelHero < nodeHeroLen)
				pNodeHero[curSelHero]->setHand(true);

			if (result == GRP_MENU_CANCEL) {
				curMenu = BMS_AUTO;
				pMenuAuto->setInControl(true);
				pMenuHero->setInControl(false);

				menuMoveLeftFrame = SCR_BATTLE3_MENU_MOVING_FRAME;
				isMenuModeRight = true;

				if (0<= curSelHero && curSelHero < nodeHeroLen)
					pNodeHero[curSelHero]->setHand(false);
			} else if (result != GRP_MENU_NOTWORK) {
				if (turnPer[result] >= 1.0f) {
					updateActionMenu();

					curMenu = BMS_ACTION;
					pMenuHero->setInControl(false);
					pMenuAction->setInControl(true);

					if (batType == BTT_3C)
						pMenuAction->setVisible(true);

					if (0<= curSelHero && curSelHero < nodeHeroLen)
						pNodeHero[curSelHero]->setHand(false);
				}
			}


			break;
		}
		case BMS_ACTION:
		{
			result = pMenuAction->getInput();
			if (result == GRP_MENU_CANCEL) {
				if (batType == BTT_3A)
					break;

				curMenu = BMS_HERO;
				pMenuHero->setInControl(true);
				pMenuAction->setInControl(false);

				if (batType == BTT_3C)
					pMenuAction->setVisible(false);
			} else if (result != GRP_MENU_NOTWORK) {
				int actionID = pMenuActionId[result];

				if (actionID > 0) {
					LdbBattleCmdNode &ldbAct = pGM->ldb.dBatCmd.commends[actionID-1];

					lastBatCmdActType = actionID;

					switch(ldbAct.ctype) {
						case 0://Attack
						{
							if (!effvals.isAttackAll(curSelHero)) {
								lastMenu = curMenu;
								curMenu = BMS_TARGET_ENEMY;
								pMenuAction->setInControl(false);
								if (batType == BTT_3A)
									pMenuAction->setVisible(false);
								pMenuEnemy->setInControl(true);

								if (batType == BTT_3C)
									pMenuAction->setVisible(false);
							} else {
								addAction(curSelHero, 0, B3AT_ACT_ATTACK, 0);
								doMenuFinish();
							}

							break;
						}

						case 1://Skill Normal
						{
							curMenu = BMS_SKILL;
							pMenuAction->setInControl(false);
							pMenuSub->setInControl(true);
							pMenuSub->setVisible(true);
							pMenuSubCaption->setVisible(true);

							if (batType == BTT_3C)
								pMenuAction->setVisible(false);

							updateSkillMenu(-1);
							updateSubCaption(0);

							break;
						}

						case 2://Skill(Special Classify)
						{
							curMenu = BMS_SKILL;
							pMenuAction->setInControl(false);
							pMenuSub->setInControl(true);
							pMenuSub->setVisible(true);
							pMenuSubCaption->setVisible(true);

							if (batType == BTT_3C)
								pMenuAction->setVisible(false);

							int c = -1;
							for(int n=0; n<actionID; n++)
								if (pGM->ldb.dBatCmd.commends[n].ctype == 2)
									c++;

							updateSkillMenu(c);
							updateSubCaption(0);

							break;
						}

						case 3://Deffence
						{
							int actLdbID = battler.getID(curSelHero);
							LdbCharacter &ldbCh = pGM->ldb.dChar[actLdbID-1];
							int val = (ldbCh.super_guard)?2:1;
							battler.add(curSelHero, BAT_DEFENCE_TYPE, val);

							if (batType == BTT_3C)
								pMenuAction->setVisible(false);

							doMenuFinish();

							break;
						}

						case 4://Item
						{
							curMenu = BMS_ITEM;
							pMenuAction->setInControl(false);
							pMenuSub->setInControl(true);
							pMenuSub->setVisible(true);
							pMenuSubCaption->setVisible(true);

							if (batType == BTT_3C)
								pMenuAction->setVisible(false);

							updateItemMenu();
							updateSubCaption(0);

							break;
						}

						case 5://Escape
						{
							addAction(curSelHero, 0 , B3AT_ACT_ESCAPE, 0);
							doMenuFinish();
							break;
						}

						case 6://For Event(NotWork)
						{

							ScrBattle3ActionQueue node;
							node.actor_id = curSelHero;
							node.type = B3AT_ACT_TURN_START;
							node.using_id = lastBatCmdActType;
							addQueue(node);

							node.type = B3AT_ACT_TURN_END;
							addQueue(node);

							doMenuFinish();
							break;
						}
					} 
				} else {
					//Change
					doMenuFinish();
				}
			}
			break;
		}

		case BMS_ITEM:
		{
			result = pMenuSub->getInput();
			if (result == GRP_MENU_CANCEL) {
				curMenu = BMS_ACTION;
				pMenuAction->setInControl(true);
				pMenuAction->setVisible(true);
				pMenuSub->setInControl(false);
				pMenuSub->setVisible(false);	
				pMenuSubCaption->setVisible(false);

			} else if (result != GRP_MENU_NOTWORK) {
				int itemID = menuSubId[result];
				LdbItem &item = pGM->ldb.dItem[itemID-1];
				MgrItem mitem(pGM);

				lastItemId = itemID;

				if (mitem.isUseable(itemID, true)) {

					menuSubSel = itemID;

					if (item.Type != 9) {
						if (!mitem.isAllEffect(itemID)) {
							lastMenu = curMenu;
							curMenu = BMS_TARGET_HERO;
							pMenuHero->setInControl(true);
							pMenuHero->setIndex(0);
							pMenuSub->setInControl(false);
							pMenuSub->setVisible(false);
							pMenuSubCaption->setVisible(false);
						} else {
							MgrItem mitem(pGM);
							mitem.useItem(itemID);
							addAction(curSelHero, 0, B3AT_ACT_ITEM, itemID);
						}
					} else {
						int skillID = item.skill_id;

						MgrSkill mskill(pGM);

						menuSubSel = skillID;

						if (!mskill.isAllEffect(skillID)) {
							if (mskill.isEffMyTeam(skillID)) {
								curMenu = BMS_TARGET_HERO;
								pMenuHero->setIndex(0);
								pMenuHero->setInControl(true);
							} else {
								curMenu = BMS_TARGET_ENEMY;
								if (batType == BTT_3A)
									pMenuAction->setVisible(false);
								pMenuEnemy->setInControl(true);
							}
							pMenuSub->setInControl(false);
							pMenuSub->setVisible(false);
							pMenuSubCaption->setVisible(false);
							lastMenu = BMS_SKILL_BY_ITEM;
						} else {
							MgrItem mitem(pGM);
							mitem.useItem(itemID);
							addAction(curSelHero, 0, B3AT_ACT_SKILL, skillID);
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
				curMenu = BMS_ACTION;
				pMenuAction->setInControl(true);
				pMenuAction->setVisible(true);
				pMenuSub->setInControl(false);
				pMenuSub->setVisible(false);	
				pMenuSubCaption->setVisible(false);


			} else if (result != GRP_MENU_NOTWORK) {
				int skillID = menuSubId[result];
				MgrSkill mskill(pGM);

				if (mskill.isUseable(curSelHero, skillID, true)) {
					if (!mskill.isAllEffect(skillID)) {
						lastMenu = curMenu;
						menuSubSel = skillID;
						if (mskill.isEffMyTeam(skillID)) {
							curMenu = BMS_TARGET_HERO;
							pMenuHero->setIndex(0);
							pMenuHero->setInControl(true);
						} else {
							curMenu = BMS_TARGET_ENEMY;
							if (batType == BTT_3A)
								pMenuAction->setVisible(false);
							pMenuEnemy->setInControl(true);
						}
						pMenuSub->setInControl(false);
						pMenuSub->setVisible(false);
						pMenuSubCaption->setVisible(false);
					} else {
						addAction(curSelHero, 0, B3AT_ACT_SKILL, menuSubSel);
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
			setHand(menuEnemyId[pMenuEnemy->getIndex()]);
			if (result == GRP_MENU_CANCEL) {
				curMenu = BMS_ACTION;
				pMenuEnemy->setInControl(false);
				pMenuAction->setInControl(true);
				pMenuAction->setVisible(true);
				setHand(-1);

			} else if (result != GRP_MENU_NOTWORK) {
				int actID = curSelHero;
				int tgtID = menuEnemyId[result];
				if (lastMenu == BMS_SKILL)
					addAction(actID, tgtID, B3AT_ACT_SKILL, menuSubSel);
				else//Attack
					addAction(actID, tgtID, B3AT_ACT_ATTACK, 0);

				doMenuFinish();
			}

			break;
		}

		case BMS_TARGET_HERO:
		{
			result = pMenuHero->getInput();
			setHand(pMenuHero->getIndex());
			if (result == GRP_MENU_CANCEL) {
				curMenu = BMS_ACTION;
				pMenuHero->setIndex(curSelHero);
				pMenuAction->setInControl(true);
				pMenuAction->setVisible(true);
				pMenuAction->setIndex(curSelHero);
				setHand(-1);
			} else if (result != GRP_MENU_NOTWORK) {
				int actID = curSelHero;
				int tgtID = result;
				if (lastMenu == BMS_ITEM)
					addAction(actID, tgtID, B3AT_ACT_ITEM, menuSubSel);
				else//item
					addAction(actID, tgtID, B3AT_ACT_SKILL, menuSubSel);


				if (lastMenu != BMS_SKILL) {
					MgrItem mitem(pGM);
					mitem.useItem(lastItemId);
				}

				doMenuFinish();
			}

			break;
		}
	}
}

void ScrBattle3::updateSubCaption( int result )
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

	pMenuSubCaption->refreshNodes(list);

	menuSubLastRes = result;
}


void ScrBattle3::doMenuFinish()
{
	pMenuSub->setInControl(false);
	pMenuSub->setVisible(false);
	pMenuEnemy->setInControl(false);
	setHand(-1);
	turnPer[curSelHero] = 0.0f;

	for(int i=0; i<heroReadyQueue.GetLength(); i++)
		if (heroReadyQueue[i] == curSelHero) {
			heroReadyQueue.Remove(i);
			i--;
		};

	if (batType == BTT_3A) {
		if (heroReadyQueue.GetLength() == 0) {			
			curMenu = BMS_HERO;
			pMenuAction->setVisible(false);
			pMenuHero->setInControl(false);

			curSelHero = -1;
		} else {
			curMenu = BMS_ACTION;
			pMenuAction->setInControl(true);
			pMenuAction->setVisible(true);
			pMenuAction->setIndex(0);

			curSelHero = heroReadyQueue[0];
			heroReadyQueue.Remove(0);

			pMenuHero->setInControl(true);
			pMenuHero->setIndex(curSelHero);

			updateActionMenu();
		}
	} else {
		curMenu = BMS_HERO;
		pMenuHero->setInControl(true);
		pMenuAction->setInControl(false);
		
		if (heroReadyQueue.GetLength() > 0)
			pMenuHero->setIndex(heroReadyQueue[0]);
		if (batType == BTT_3C)
			pMenuAction->setVisible(false);
	}
}

void ScrBattle3::setCaption( TString str )
{
	if (str.isEmpty()) {
		pMenuCaption->setVisible(false);
	} else {
		ArrayList<TString> caption;
		caption.Add(str);

		pMenuCaption->refreshNodes(caption);
		pMenuCaption->setVisible(true);

		menuCaptionHideLeftFrame = 80;
	}
}

void ScrBattle3::updateCaption( int framedelta )
{
	if (menuCaptionHideLeftFrame>0) {
		menuCaptionHideLeftFrame -= framedelta;

		if (menuCaptionHideLeftFrame<=0)
			pMenuCaption->setVisible(false);
	}
}

void ScrBattle3::setHand( int id )
{
	if (id != curHanding) {
		if (0 <= curHanding && curHanding < nodeHeroLen) {
			pNodeHero[curHanding]->setHand(false);
		} else if (SCR_BATTLE_MAX_HEROS <= curHanding && curHanding < SCR_BATTLE_MAX_HEROS + nodeEnemyLen) {
			pNodeEnemy[curHanding - SCR_BATTLE_MAX_HEROS]->setHand(false);
		}

		if (0<= id && id < nodeHeroLen) {
			pNodeHero[id]->setHand(true);
		} else if (SCR_BATTLE_MAX_HEROS <= id && id < SCR_BATTLE_MAX_HEROS + nodeEnemyLen) {
			pNodeEnemy[id-SCR_BATTLE_MAX_HEROS]->setHand(true);
		}

		curHanding = id;
	}
}
