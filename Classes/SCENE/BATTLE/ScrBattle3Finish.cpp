#include "ScrBattle3.h"

#include "../../COMMON/ComMath.h"
#include "../../EVENT/EvtMain.h"

void ScrBattle3::initBattleFinish()
{
	int texID_sys = IBuffer::getInstance()->get(imgSystem, pGM->lsd.System.system_name);
	CCTexture2D *tex_sys = IBuffer::getInstance()->get(imgSystem, texID_sys);

	pBatEndMsgbox = new GrpMsgbox(pGM, 320, 32);
	pBatEndMsgbox->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(16)));
	pBatEndMsgbox->setVisible(false);
	this->addChild(pBatEndMsgbox, 1003);

	pBatEndMsgboxDown = new GrpSysDown(tex_sys);
	pBatEndMsgboxDown->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(32)));
	pBatEndMsgboxDown->setVisible(false);
	this->addChild(pBatEndMsgboxDown, 1004);

	pBatEndFont = new GrpFont(pGM);
	pBatEndFont->setPosition(ccp(SCR_POS_X(8),SCR_POS_Y(8)));
	this->addChild(pBatEndFont, 1004);
}


void ScrBattle3::chkBattleFinish()
{
	GlobalLangInGame &lang = pGM->inlang;

	int heroAliveCnt = 0;
	int enemyAliveCnt = 0;
	for(int id=0; id<nodeHeroLen; id++) {
		if (battler.isAlive(id))
			heroAliveCnt++;
	}

	for(int i=0; i<nodeEnemyLen; i++) {
		int id = i + SCR_BATTLE_MAX_HEROS;
		if (battler.isAlive(id))
			enemyAliveCnt++;
	}

	if (enemyAliveCnt == 0 || heroAliveCnt == 0 || isBatEscaped) {
		actQueue.RemoveAll();

		isBatEnd = true;

		pMenuAuto->setInControl(false);
		pMenuAction->setInControl(false);
		pMenuHero->setInControl(false);
		pMenuEnemy->setInControl(false);
		pMenuSub->setInControl(false);

		curFrame = 0;
		batEndMsgRow = -1;

		LdbVoca &voca = pGM->ldb.dVoca;

		if (isBatEscaped) {
			//Escaped(Go Away)

			if (!voca.escape_success.isEmpty())
				batEndMsg.Add(voca.escape_success + "\\!");

			isBatEndReturningToMap = true;

			endType = BFT_ESCAPE;
		} else if (enemyAliveCnt==0) {
			pGM->snd.PlayMusic(pGM->lsd.System.music_battle_end);

			//Team WIN!!
			ScrBattle3ActionQueue node;
			node.using_id = 10;
			node.type_id = 2;
			node.type = B3AT_CHAR_ANIMATING;
			node.total_frame = 999999;

			for(int id=0; id<nodeHeroLen; id++) {
				if (battler.isAlive(id)) {
					node.actor_id = id;
					addQueue(node);
				}
			}

			batEndMsg.Add(voca.victory + "\\!");

			int take_exp = 0;
			int take_money = 0;
			ArrayList<int> take_item;

			for(int i=0; i<nodeEnemyLen; i++) {
				int id = i + SCR_BATTLE_MAX_HEROS;

				if (pNodeEnemy[i]->isShowed()) {
					int ldbID = battler.getID(id);

					LdbEnemy &enemy = pGM->ldb.dEnemy[ldbID-1];

					take_exp += enemy.take_exp;
					take_money += enemy.take_money;
					if (enemy.drop_id > 0 && RANDF() < enemy.drop_chance / 100.0f)
						take_item.Add(enemy.drop_id);
				}
			}

			if (take_exp > 0) {
				batEndMsg.Add(take_exp + lang.getLang(Battle_GetExp) + voca.exp_received + "\\!");

				for(int id=0; id<nodeHeroLen; id++)
					battler.add(id, EXP, take_exp);
			}

			if (take_money > 0) {
				batEndMsg.Add(voca.money_recieved_a + " " + take_money + voca.gold + voca.money_recieved_b + "\\!");

				MgrState mstate(pGM);
				mstate.addMoney(take_money);
			}

			if (take_item.GetLength() > 0) {
				for(int i=0; i<take_item.GetLength(); i++) {
					MgrItem mitem(pGM);
					batEndMsg.Add(mitem.getName(take_item[i]) + lang.getLang(Battle_GetItem) + voca.item_recieved + "\\!");
					mitem.addItemCount(take_item[i], 1);
				}
			}

			isBatEndReturningToMap = true;

			endType = BFT_VICTORY;
		} else if (heroAliveCnt==0) {
			//Enemy WIN T^T

			if (!voca.defeat.isEmpty())
				batEndMsg.Add(voca.defeat + "\\!");

			isBatEndReturningToMap = isBatLoseToMap;

			endType = BFT_DEFEAT;
		}

		if (batEndMsg.GetLength() > 0) {
			batEndLastKey = pGM->ctl->getKey(rkc_enter);
			pBatEndMsgbox->setVisible(true);
			pBatEndMsgboxDown->setVisible(true);
		}
	}
}

void ScrBattle3::updateBattleFinish(int framedelta) {

	if (!isBatStart || !isBatEnd || framedelta <= 0)
		return;

	curFrame += framedelta;

	if (batEndMsgRow == -1 || 
		(pBatEndFont->refresh(curFrame) && !pBatEndFont->isWaitKey())) {
		int pos = ++batEndMsgRow;

		if (pos < batEndMsg.GetLength()) {
			pBatEndFont->setText(batEndMsg[pos]);
			pBatEndFont->showStart(curFrame);
		} else {
			if (isBatEndReturningToMap) {
				doEndByEvent();

				pGM->slm->setSettingScrMap();
				pGM->slm->setScreen(scr_map);
			} else {
				pGM->slm->setScreen(scr_gameover);
			}
			isBatStart = false;
		}
	}
}

void ScrBattle3::doEndByEvent()
{
	LsdMapEventStack *stack = pEvtMain->getLastStack(ecEnemyEncounter);
	
	if (stack != NULL) {
		LsdMapEventData *stop = stack->data.GetTail();

		GlbEventNode &node = stop->event.data[stop->eventpos];
		int typeEscaped = GetPara(node, 3);
		int typeDefeat	= GetPara(node, 4);

		int findcode;
		switch(endType) {
		case BFT_VICTORY:	findcode = ecVictoryHandler;break;
		case BFT_DEFEAT:	findcode = ecDefeatHandler;break;
		case BFT_ESCAPE:	findcode = (typeEscaped == 1)?ecEndBattle:ecEscapeHandler;break;
		default:			findcode = -1;
		}

		if (typeEscaped == 2 || typeDefeat == 1) {
			bool finded = false;
			int n;
			for(n = stop->eventpos+1; n < stop->event.data.GetLength() - 1; n++) {
				if (stop->event.data[n].indent == stop->event.data[stop->eventpos].indent) {
					if (stop->event.data[n].code == ecEnemyEncounter)
						break;

					if (stop->event.data[n].code == findcode || stop->event.data[n].code == ecEndBattle) {
						finded = true;
						break;
					}
				}
			}

			if (finded)
				stop->eventpos = n+1;
			else
				stop->eventpos++;
		} else {
			stop->eventpos++;
		}
	}
	pGM->snd.PlayMusic(pGM->lsd.System.music_before_battle);
	pGM->slm->setSettingScrMap();
	pGM->slm->setScreen(scr_map);
}