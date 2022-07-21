#include "ScrBattle3.h"

#include "../../DATA/GLB/GlbEvent.h"

void ScrBattle3::initEvent()
{
	evtStack.data.RemoveAll();
	evtStack.wait_left = 0;

	pEvtMain = new EvtMain(pGM, &evtStack, this, &battler);
	isEventNeedCheck = false;
	evtPage = 0;
}

void ScrBattle3::chkEvent(int framedelta) {
	pGM->lsd.System.frame_count += framedelta;
	
	if (isEventNeedCheck && evtStack.data.GetLength() == 0) {
		if (!chkEventRun())
			isEventNeedCheck = false;
	}
	
// 	LsdMapEventData *evt = m_event_stack.data.GetTail();
// 	if (evt != NULL)
// 		CCLog((_ST("eventpos : ") + evt->eventpos).getTextUTF8());

	pEvtMain->doEvents(framedelta);

	GlbEventNode n;

	MgrState ms(pGM);

	while(battler.getEvent(n)) {
		switch(n.code) {
		case ecChangeMonsterHP:
			{
				bool issound = false;
				int i, actID, inVal, inVal2;
				actID = GetPara(n, 0) - 1 + SCR_BATTLE_MAX_HEROS;
				inVal = GetPara(n, 3);

				if (actID < SCR_BATTLE_MAX_HEROS) {
					for(int i=SCR_BATTLE_MAX_HEROS; i<SCR_BATTLE_MAX_HEROS+nodeEnemyLen; i++) {
						if (!battler.isAlive(i))
							continue;

						switch(GetPara(n, 2)) {
							case 0:inVal2 = inVal; break;
							case 1:inVal2 = ms.getVar(inVal);break;
							case 2:inVal2 = battler.get(i, MAXHP) * inVal / 100;break;
							default:inVal2 = 0;
						}

						if (GetPara(n, 1) != 0)
							inVal2 *= -1;

						int curHP = battler.get(i, HP);

						if ( GetPara(n, 4) == 0 && curHP + inVal2 <= 0 )
							inVal2 = curHP - 1;

						battler.add(i, HP, inVal2);

						if (inVal2 != 0) {
							showDamage(i, inVal2);
							issound = true;
						}
					}
				} else {
					i = actID;

					switch(GetPara(n, 2)) {
					case 0:inVal2 = inVal; break;
					case 1:inVal2 = ms.getVar(inVal);break;
					case 2:inVal2 = battler.get(actID, MAXHP) * inVal / 100;break;
					default:inVal2 = 0;
					}

					if (GetPara(n, 1) != 0)
						inVal2 *= -1;

					int curHP = battler.get(i, HP);

					if ( GetPara(n, 4) == 1 && curHP + inVal2 <= 0 )
						inVal2 = curHP - 1;

					battler.add(i, HP, inVal2);

					if (inVal2 != 0) {
						showDamage(i, inVal2);
						issound = true;
					}
				}

				if (issound)
					pGM->snd.PlaySound(pGM->lsd.System.se_enemy_damaged);

				break;
			}
		case ecChangeMonsterMP: /*ChangeMonsterMP*/
			{
				int i, actID, inVal, inVal2;
				actID = GetPara(n, 0) - 1 + SCR_BATTLE_MAX_HEROS;
				inVal = GetPara(n, 3);

				if (actID < SCR_BATTLE_MAX_HEROS) {
					for(int i=SCR_BATTLE_MAX_HEROS; i<SCR_BATTLE_MAX_HEROS+nodeEnemyLen; i++) {
						if (!battler.isAlive(i))
							continue;

						switch(GetPara(n, 2)) {
						case 0:inVal2 = inVal; break;
						case 1:inVal2 = ms.getVar(inVal);break;
						case 2:inVal2 = battler.get(i, MAXMP) * inVal / 100;break;
						default:inVal2 = 0;
						}

						if (GetPara(n, 1) != 0)
							inVal2 *= -1;

						battler.add(i, MP, inVal2);
					}
				} else {
					i = actID;

					switch(GetPara(n, 2)) {
					case 0:inVal2 = inVal; break;
					case 1:inVal2 = ms.getVar(inVal);break;
					case 2:inVal2 = battler.get(actID, MAXMP) * inVal / 100;break;
					default:inVal2 = 0;
					}

					if (GetPara(n, 1) != 0)
						inVal2 *= -1;

					battler.add(i, MP, inVal2);
				}
				break;
			}
		case ecChangeMonsterCondition: /*ChangeMonsterCondition*/
			{
				int actID, inID;
				bool inVal;
				actID = GetPara(n, 0) - 1 + SCR_BATTLE_MAX_HEROS;
				inID = GetPara(n, 2) - 1;
				inVal = (GetPara(n, 2) == 0);//TODO CHK

				if (actID < SCR_BATTLE_MAX_HEROS) {
					for(int i=SCR_BATTLE_MAX_HEROS; i<SCR_BATTLE_MAX_HEROS+nodeEnemyLen; i++) {
						if (!battler.isAlive(i))
							continue;

						battler.setState(i, inID, inVal);
					}
				} else {
					battler.setState(actID, inID, inVal);
				}
				break;
			}
		case ecShowHiddenMonster: /*ShowHiddenMonster*/
			{
				int actID;
				actID = GetPara(n, 0) - 1 + SCR_BATTLE_MAX_HEROS;

				if (actID < SCR_BATTLE_MAX_HEROS) {
					for(int i=SCR_BATTLE_MAX_HEROS; i<SCR_BATTLE_MAX_HEROS+nodeEnemyLen; i++) {
						if (!battler.isAlive(i))
							continue;

						battler.add(i, BAT_INVISIBLE, 0);
					}
				} else {
					battler.add(actID, BAT_INVISIBLE, 0);
				}
				
				break;
			}
		case ecForceFlee:
			{
				switch(GetPara(n, 0)) {
				case 0:
					{
						//HeroEscape
						isBatEscaped = true;
						break;
					}

				case 1:
					{
						//All Enemy Escape
						for(int i=0; i<nodeEnemyLen; i++) {
							int id = i + SCR_BATTLE_MAX_HEROS;

							if (battler.isAlive(id))
								addAction(id, 0, B3AT_ACT_ESCAPE, 0);
						}
						break;
					}

				case 2:
					{
						int id = GetPara(n, 1) + SCR_BATTLE_MAX_HEROS;
						addAction(id, 0, B3AT_ACT_ESCAPE, 0);
						break;
					}

				}
				break;
			}
		case ecEnableCombo:
			{
				break;
			}
		case ecChangeBattleBG: /*ChangeBattleBG*/
			{
				ScrBattleBgInfo flag;

				flag.use_ab = false;
				flag.bg = n.string;
				
				setBackGround(flag);
				break;
			}
		case ecShowBattleAnimation_B: /*ShowBattleAnimation_B*/
			{
				ScrBattle3ActionQueue node;

				int id, targetid = GetPara(n, 1);

				if (GetPara(n,3) == 1) {
					//Hero
					if (targetid == -1)
						node.target_id = EFFECT_ALL_HEROS;
					else{
						for(id=0; id<nodeHeroLen; id++) {
							if (battler.getID(id) == targetid) {
								node.target_id = id;
								break;
							}
						}

						if (id == nodeHeroLen)
							break;
					}
				} else {
					//Enemy
					if (targetid == -1)
						node.target_id = EFFECT_ALL_ENEMIES;
					else
						node.target_id = targetid + SCR_BATTLE_MAX_HEROS;
				}
				
				node.type = B3AT_ANIMATING;
				node.using_id = GetPara(n, 0);

				doQueue(node);

				break;
			}
		case ecTerminateBattle: /*TerminateBattle*/
			{
				doEndByEvent();
				break;
			}
		}
	}
}

int ScrBattle3::GetPara(GlbEventNode &edata, int index) {
	if (index < edata.parameters.GetLength())
		return edata.parameters[index];
	else
		return 0;
}

bool ScrBattle3::chkEventRun() {
	LdbEnemyGroup &group = pGM->ldb.dEnemys[batId-1];
	
	int i=0;
	for(i=evtPage; i<group.pages.GetLength(); i++) {
		LdbEnemyGroupEventPage &page = group.pages[i];

		if (!chkEventCond(&(page.condition)))
			continue;

		pEvtMain->doEvent(page.event_data);
		break;
	}

	if (evtPage == group.pages.GetLength())
		return false;
	else{
		evtPage = i+1;
		return true;
	}
}

bool ScrBattle3::isEventRunning() {
	return pEvtMain->isRuningSerial();
}

bool ScrBattle3::chkEventCond(LdbEnemyGroupEventCondition *c) {
	int flag = 1, flagid = 0;
	bool correctCondition;

	MgrState mstate(pGM);

	if (c->flags == 0)
		correctCondition = false;
	else{
		correctCondition = true;
		while(flag <= c->flags) {
			if ((c->flags & flag) == flag) {
				switch(flagid) {
					case 0:
					{
						if (!mstate.getSw(c->switch_a_id))
							correctCondition = false;
						break;
					}
					case 1:
					{
						if (!mstate.getSw(c->switch_b_id))
							correctCondition = false;
						break;
					}

					case 2:
					{
						if (!(mstate.getVar(c->variable_id) >= c->variable_value))
							correctCondition = false;
						break;
					}

					case 3:
					{
						correctCondition &= chkTurn(turnCnt,c->turn_a,c->turn_b);
						break;
					}

					case 4:
					{
						int alive = 0;
						for(int n=0; n<nodeHeroLen; n++) {
							int id = n;
							if (battler.isAlive(id))
								alive++;
						}

						int val = 100-100*alive/nodeHeroLen;

						if (!(c->fatigue_min <= val && val <=c->fatigue_max))
							correctCondition = false;
						break;
					} 

					case 5:
					{
						int id = c->enemy_id-1 + SCR_BATTLE_MAX_HEROS;

						int hpPer = 100 * battler.get(id, HP) / 
							battler.get(id, MAXHP);
						if (!(c->enemy_hp_min <= hpPer && hpPer <=c->enemy_hp_max))
							correctCondition = false;
						break;
					}

					case 6:
					{
						int id = c->actor_id-1;

						int hpPer = 100 * battler.get(id, HP) / 
							battler.get(id, MAXHP);
						if (!(c->enemy_hp_min <= hpPer && hpPer <=c->enemy_hp_max))
							correctCondition = false;
						break;	   
					}

					case 7:
					{
						if (batType != BTT_0) {
							int id = c->turn_enemy_id-1 + SCR_BATTLE_MAX_HEROS;
							int turn = battler.get(id, BAT_TURN_COUNT);
							correctCondition &= chkTurn(turn,c->turn_actor_a,c->turn_actor_b);
						}
						break;
					}

					case 8:
					{
						if (batType != BTT_0) {
							int id = c->turn_actor_id-1;
							int turn = battler.get(id, BAT_TURN_COUNT);
							correctCondition &= chkTurn(turn,c->turn_actor_a,c->turn_actor_b);
						}
						break;
					}

					case 9:
					{
						int actID = battler.getID(actQueue[0].actor_id);
						correctCondition &= ((actID == c->command_actor_id) &&
												(actQueue[0].using_id == c->command_id));
						
						//For make to wait untill finish event
						if (correctCondition) {
							ScrBattle3ActionQueue node = actQueue[0];
							node.type = B3AT_ACT_TURN_END;
							actQueue.Remove(0);
							actQueue.Add(0, node);
						}
						break;
					}
				}
			}

			flag *= 2;
			flagid++;
		}
	}
	return correctCondition;
}

bool ScrBattle3::chkTurn(int val, int a, int b) {
	if (a == 0) {
		if (b != val)
			return false;
	} else if (!((val+a-+b)%a == 0))
		return false;

	return true;
}
