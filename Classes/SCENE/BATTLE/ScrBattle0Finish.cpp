#include "ScrBattle0.h"
#include "../../COMMON/ComMath.h"

void ScrBattle0::initBattleFinish() {
	isBatEscaped = false;
}

bool ScrBattle0::chkBattleFinish(int framedelta)
{
	if (framedelta <= 0)
		return true;

	GlobalLangInGame &lang = pGM->inlang;

	LdbVoca &voca = pGM->ldb.dVoca;

	for(int i=0; i<4; i++)
		pMenuProcRun[i]->removeText();

	curMsgRow = 0;

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

		ArrayList<TString> msg;

		if (isBatEscaped) {
			//Escaped(Go Away)
			msg.Add(voca.escape_success);
			finishType = BFT_ESCAPE;
		} else if (enemyAliveCnt==0) {
			//Team WIN!!
			msg.Add(voca.victory);
			
			MgrState mstate(pGM);
			MgrItem mitem(pGM);

			int exp = 0;
			int gold = 0;
			ArrayList<int> items;

			for(int i=0; i<nodeEnemyLen; i++) {
				if (!pNodeEnemy[i]->isShowed())
					continue;
				int ldbID = battler.getID(i + SCR_BATTLE_MAX_HEROS);

				LdbEnemy &enemy = pGM->ldb.dEnemy[ldbID-1];

				exp += enemy.take_exp;
				gold += enemy.take_money;

				if (enemy.drop_id > 0 && RANDF() < enemy.drop_chance / 100.0f)
					items.Add(enemy.drop_id);
			}

			msg.Add(exp + lang.getLang(Battle_GetExp) + voca.exp_received);

			for(int i=0; i<nodeHeroLen; i++) {
				battler.add(i, EXP, exp);

				battler.getLastLevelUpMsg(msg);
			}

			if (gold > 0) {
				msg.Add(voca.money_recieved_a + gold + voca.gold + voca.money_recieved_b);
			
				mstate.addMoney(gold);	
			}

			for(int i=0; i<items.GetLength(); i++) {
				msg.Add(mitem.getName(items[i]) + voca.item_recieved);
				mitem.addItemCount(items[i], 1);
			}

			pGM->snd.PlayMusic(pGM->lsd.System.music_battle_end);
			finishType = BFT_VICTORY;
		} else if (heroAliveCnt==0) {
			//Enemy WIN T^T
			msg.Add(voca.defeat);
			finishType = BFT_DEFEAT;
		}

		GlbEvent evt;
		GlbEventNode evtnode;
		evt.init();
		evtnode.indent = 0;
		for(int i=0; i<msg.GetLength(); i++) {
			evtnode.code = (i%4==0)?ecShowMessage:ecShowMessage_2;
			evtnode.string = msg[i];
			evt.data.Add(evtnode);
		}

		if (heroAliveCnt==0 && !isBatLoseToMap)
			evtnode.code = ecGameOver;
		else
			evtnode.code = ecTerminateBattle;
		evt.data.Add(evtnode);

		pEvtMain->doEvent(evt);
		return true;
	}
	return false;
}

void ScrBattle0::doEndByEvent()
{
	LsdMapEventStack *stack = pEvtMain->getLastStack(ecEnemyEncounter);
	
	if (stack == NULL)
		return;
	
	LsdMapEventData *stop = stack->data.GetTail();

	GlbEventNode &node = stop->event.data[stop->eventpos];
	int typeEscaped = GetPara(node, 3);
	int typeDefeat	= GetPara(node, 4);

	int findcode;
	switch(finishType) {
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

	pGM->snd.PlayMusic(pGM->lsd.System.music_before_battle);
	pGM->slm->setSettingScrMap();
	pGM->slm->setScreen(scr_map);
}