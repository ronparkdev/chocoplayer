#include "ScrBattle0.h"

#include "../../DATA/GLB/GlbEvent.h"

void ScrBattle0::initEvent()
{
	evtStack.data.RemoveAll();
	evtStack.wait_left = 0;

	pEvtMain = new EvtMain(pGM, &evtStack, this, &battler);
	isEventNeedCheck = true;
	evtPage = 0;
}

void ScrBattle0::drawEvent(int framedelta) {
	if (framedelta == 0)
		return;
	
	pGM->lsd.System.frame_count += framedelta;

	if (isEventNeedCheck && evtStack.data.GetLength() == 0) {
		if (!chkEvent())
			isEventNeedCheck = false;
	}
	
	pEvtMain->doEvents(framedelta);

	GlbEventNode n;

	MgrState ms(pGM);

	while(battler.getEvent(n)) {
		switch(n.code) {

		case ecChangeMonsterHP:
			{
				int actID, inVal;
				actID = GetPara(n, 0) - 1 + SCR_BATTLE_MAX_HEROS;
				inVal = GetPara(n, 3);

				switch(GetPara(n, 2)) {
				//case 0:inVal = inVal; break;
				case 1:inVal = ms.getVar(inVal);break;
				case 2:inVal = battler.get(actID, MAXHP) * inVal / 100;break;
				}

				if (GetPara(n, 1) != 0)
					inVal *= -1;

				if (inVal == 0) {
					for(int i=0; i<SCR_BATTLE_MAX_ENEMIES; i++) {
						if (!battler.isAlive(i))
							continue;

						int curHP = battler.get(i, HP);

						if ( GetPara(n, 4) == 1 && curHP + inVal <= 0 )
							inVal = curHP - 1;

						battler.add(i, HP, inVal);
					}
				} else {
					int curHP = battler.get(actID, HP);

					if ( GetPara(n, 4) == 1 && curHP + inVal <= 0 )
						inVal = curHP - 1;

					battler.add(actID, HP, inVal);
				}

				if (inVal > 0)
					pGM->snd.PlaySound(pGM->lsd.System.se_enemy_damaged);
				break;
			}
		case ecChangeMonsterMP: /*ChangeMonsterMP*/
			{
				int actID, inVal;
				actID = GetPara(n, 0) - 1 + SCR_BATTLE_MAX_HEROS;
				inVal = GetPara(n, 3);

				switch(GetPara(n, 2)) {
				//case 0:inVal = inVal; break;
				case 1:inVal = ms.getVar(inVal);break;
				case 2:inVal = battler.get(actID, MAXMP) * inVal / 100;break;
				}

				if (GetPara(n, 1) != 0)
					inVal *= -1;

				if (inVal == 0) {
					for(int i=0; i<SCR_BATTLE_MAX_ENEMIES; i++) {
						if (!battler.isAlive(i))
							continue;

						battler.add(i, MP, inVal);
					}
				} else {
					battler.add(actID, MP, inVal);
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

				battler.setState(actID, inID, inVal);
				break;
			}
		case ecShowHiddenMonster: /*ShowHiddenMonster*/
			{
				int actID;
				actID = GetPara(n, 0) - 1 + SCR_BATTLE_MAX_HEROS;

				battler.add(actID, BAT_INVISIBLE, 0);
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
								battler.add(id, HP, -9999999);
						}
						break;
					}

				case 2:
					{
						escapeChanceHero = 1.0f;
						isActHeroEscape = true;
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
				setBackGround(n.string);
				break;
			}
		case ecShowBattleAnimation_B: /*ShowBattleAnimation_B*/
			{
				int aniID = GetPara(n, 0);
				ScrBattle0ActionQueue node;
				if (GetPara(n, 1) == -1) {
					for(int i=0; i<nodeEnemyLen; i++) {
						int id = i + SCR_BATTLE_MAX_HEROS;;

						if (!battler.isAlive(id))
							continue;

						ScrBattle0EnemyNode *enemy = pNodeEnemy[i];

						pAnis[id]->set(aniID, enemy->getPositionX(), enemy->getPositionY(), enemy->getHeight());
					}
				} else {
					int i = GetPara(n, 1);
					ScrBattle0EnemyNode *enemy = pNodeEnemy[i];
					pAnis[i]->set(aniID, enemy->getPositionX(), enemy->getPositionY(), enemy->getHeight());
				}

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

int ScrBattle0::GetPara(GlbEventNode &edata, int index) {
	if (index < edata.parameters.GetLength())
		return edata.parameters[index];
	else
		return 0;
}

bool ScrBattle0::chkEvent() {
	LdbEnemyGroup &group = pGM->ldb.dEnemys[batId-1];

	int i=0;
	for(i=evtPage; i<group.pages.GetLength(); i++) {
		LdbEnemyGroupEventPage &page = group.pages[i];

		if (!chkEventCond(&(page.condition)))
			continue;

		pEvtMain->doEvent(page.event_data);
		break;
	}

	if (evtPage >= group.pages.GetLength())
		return false;
	else{
		evtPage = i+1;
		return true;
	}
}

bool ScrBattle0::chkEventCond(LdbEnemyGroupEventCondition *c) {
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
					/* For RPG 2003
				case 7:
					{
						if (m_battle_type != BTT_0) {
							int id = c->turn_enemy_id-1 + SCR_BATTLE_MAX_HEROS;
							int turn = m_battler.get(id, BAT_TURN_COUNT);
							correctCondition &= chkTurn(turn,c->turn_actor_a,c->turn_actor_b);
						}
						break;
					}

				case 8:
					{
						if (m_battle_type != BTT_0) {
							int id = c->turn_actor_id-1;
							int turn = m_battler.get(id, BAT_TURN_COUNT);
							correctCondition &= chkTurn(turn,c->turn_actor_a,c->turn_actor_b);
						}
						break;
					}

				case 9:
					{
						correctCondition &= ((m_action_queue[0].actor_id == (c->command_actor_id-1)) &&
							(m_action_queue[0].using_id == c->command_id));

						//For make to wait untill finish event
						if (correctCondition) {
							ScrBattle0ActionQueue node = m_action_queue[0];
							//node.type = B3AT_ACT_TURN_END;
							m_action_queue.Remove(0);
							m_action_queue.Add(0, node);
						}
						break;
					}
					*/
				}
			}


			flag *= 2;
			flagid++;
		}
	}
	return correctCondition;
}

bool ScrBattle0::chkTurn(int val, int a, int b) {
	if (a == 0) {
		if (b != val)
			return false;
	} else if (!((val+a-+b)%a == 0))
		return false;

	return true;
}