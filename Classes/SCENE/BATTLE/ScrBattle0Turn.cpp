#include "ScrBattle0.h"

#include "ScrBattleEffectValues.h"
#include "../../COMMON/ComMath.h"
#include "../../COMMON/MgrSkill.h"

void ScrBattle0::chkTurnPriority() {
	turnQueue.RemoveAll();

	LdbVoca &voca = pGM->ldb.dVoca;

	ArrayList<int> temp_list;

	int starti;
	int i, agi, agi_max = -1, agi_last_max = -1;
	int alive_cnt = 0;

	if (isActHeroEscape) {
		if (RANDF() < escapeChanceHero) {
			isBatEscaped = true;
			return;
		} else {
			escapeChanceHero += 0.1f;
			addQueueMessageClear();
			addQueueMessage(voca.escape_failure, true);
		}
		
		starti = SCR_BATTLE_MAX_HEROS;
	} else {
		starti = 0;
	}

	//Find Alive Count
	for(i=starti; i<SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES; i++) {
		if (!battler.isAlive(i))
			continue;

		alive_cnt++;
	}

	while(alive_cnt > 0) {

		//Find Max Agi

		agi_max = -1;

		for(i=starti; i<SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES; i++) {
			if (!battler.isAlive(i))
				continue;

			agi = battler.get(i, AGI);

			if ((agi_max == -1 || agi_max < agi) && 
				(agi_last_max == -1 || agi < agi_last_max))
				agi_max = agi;
		}	

		//Pick up people

		temp_list.RemoveAll();

		for(i=starti; i<SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES; i++) {
			if (!battler.isAlive(i))
				continue;

			agi = battler.get(i, AGI);

			if (agi != agi_max)
				continue;

			temp_list.Add(i);
			alive_cnt--;
		}

		//Push people into queue in random

		while(temp_list.GetLength() > 0) {
			int random = rand() % temp_list.GetLength();

			turnQueue.Add(temp_list[random]);
			temp_list.Remove(random);
		}

		agi_last_max = agi_max;
	}
}

void ScrBattle0::updateAction() {
	for(int i=0; i<SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES; i++) {
		if (!battler.isAlive(i))
			continue;
		
		heroActQueue[i].actor_id = i;
		
		int restriction = 0;

		int stateID = battler.getTopState(i);

		if (stateID > 0) {
			LdbCondition &cond = pGM->ldb.dCond[stateID-1];

			restriction = cond.restriction;
		}
		
		switch(restriction) {
		case 0:
			{
				if (battler.isHero(i)) {
					if (isActHeroAuto) {
						heroActQueue[i].target_id = getRandomPeople(i, !battler.isHero(i));
						heroActQueue[i].type = B0AT_ACT_ATTACK;
					} else {
						heroActQueue[i].target_id = heroCmdTgt[i];
						heroActQueue[i].using_id = heroCmdUsing[i];
						switch(heroCmdType[i]) {
							case 0:heroActQueue[i].type = B0AT_ACT_ATTACK;	break;
							case 1:heroActQueue[i].type = B0AT_ACT_SKILL;	break;
							case 2:heroActQueue[i].type = B0AT_ACT_DEFENCE;	break;
							case 3:heroActQueue[i].type = B0AT_ACT_ITEM;	break;
						}
					}
				} else {
					updateEnemyAction(i);
				}
				break;
			}

		case 1:
			{
				heroActQueue[i].type = B0AT_ACT_NOTHING;	
				break;
			}

		case 2:
			{
				//Attack Enemy
				heroActQueue[i].target_id = getRandomPeople(i, !battler.isHero(i));
				heroActQueue[i].type = B0AT_ACT_ATTACK;
				break;
			}
		case 3:
			{
				//Attack Team
				heroActQueue[i].target_id = getRandomPeople(i, battler.isHero(i));
				heroActQueue[i].type = B0AT_ACT_ATTACK;
				break;
			}
		}
	}
}

void ScrBattle0::updateEnemyAction( int id )
{
	int ldbID = battler.getID(id);

	LdbEnemy &enemy = pGM->ldb.dEnemy[ldbID-1];

	int findMaxPri = 101;
	int findCount = 0;
	int findAllCount = enemy.actions.GetLength();

	ArrayList<int> actionAble;
	while(findCount < findAllCount) {
		int findCurPri = 0;
		//Find Highest Rating
		for(int i=0; i<findAllCount; i++) {
			int pri = enemy.actions[i].rating;
			if (findCurPri <= pri && pri < findMaxPri) {
				findCurPri = pri;
			}
		}

		for(int i=0; i<findAllCount; i++) {
			LdbEnemyAction &act = enemy.actions[i];
			int pri = act.rating;

			if (pri != findCurPri)
				continue;

			bool isAble = false;

			switch(act.condition_type) {
			case 0://Normal
				isAble = true;break;

			case 1:
				{
					MgrState mstate(pGM);
					isAble = mstate.getSw(act.switch_id);
					break;
				} 

			case 2:
				{
					isAble = (act.condition_param1 * turnCnt 
						>= act.condition_param2);
					break;
				}

			case 3:
				{
					isAble = (act.condition_param1 <= nodeEnemyLen &&
						act.condition_param1 >= nodeEnemyLen);
					break;
				}

			case 4:
				{
					int hpPer = 100*battler.get(id, HP)/battler.get(id,MAXHP);
					isAble = (act.condition_param1 <= hpPer &&
						act.condition_param2 >= hpPer);
					break;
				}

			case 5:
				{
					int mpPer = 100*battler.get(id, MP)/battler.get(id,MAXMP);
					isAble = (act.condition_param1 <= mpPer &&
						act.condition_param2 >= mpPer);
					break;
				}

			case 6:
				{
					int lvAvg = 0;
					for(int id=0; id<nodeHeroLen; id++)
						lvAvg += battler.get(id, LEVEL);
					lvAvg /= nodeHeroLen;

					isAble = (act.condition_param1 <= lvAvg &&
						act.condition_param2 >= lvAvg);
					break;
				}


			case 7:
				{
					int heroDead = 0;
					for(int id=0; id<nodeHeroLen; id++) {
						int stateID = battler.getTopState(id);
						if (stateID == 1)
							heroDead++;
					}

					isAble = (act.condition_param1 <= heroDead &&
						act.condition_param2 >= heroDead);
					break;
				}

			}

			if (isAble)
				actionAble.Add(i);

			findCount++;
		}

		findMaxPri = findCurPri;

		if (actionAble.GetLength() > 0)
			break;
	}

	if (actionAble.GetLength() > 0) {
		int actionID = actionAble[rand() % actionAble.GetLength()];

		LdbEnemyAction &act = enemy.actions[actionID];

		ScrBattle0ActionQueue &node = heroActQueue[id];

		node.actor_id = id;
		node.target_id = getRandomPeople(id, true);

		int defAniID = 1;
		int defAniTime = GrpAniOne::getAnimationFrame(pGM, defAniID);

		switch(act.kind) {
		case 0:
			{
				switch(act.basic) {
					case 0:{//Normal Attack
						node.type = B0AT_ACT_ATTACK;

						break;
					}
					case 1:{//Double Attack
						for(int i=0; i<2; i++)
							node.type = B0AT_ACT_DOUBLEATTACK;
						break;
					}
					case 2:{//Defence
						node.type = B0AT_ACT_DEFENCE;
						break;
					}
					case 3:{//view heros... (do nothing)
						node.type = B0AT_ACT_OBSERVING;
						break;
					}
					case 4:{
						node.type = B0AT_ACT_GATHER_POWER;
						break;
					}
					case 5:{
						node.type = B0AT_ACT_SELFDEST;
						break;
					}
					case 6:{
						node.type = B0AT_ACT_ESCAPE;
						break;
					}
					case 7:{//do nothing
						node.type = B0AT_ACT_NOTHING;
						break;
					}
				}

				break;
			}

		case 1:
			{//Use Skill
				node.type = B0AT_ACT_SKILL;
				node.using_id = act.skill_id;
				break;
			}

		case 2:
			{//Change Monster
				node.type = B0AT_ACT_TRANSFORM;
				node.using_id = act.enemy_id;
				break;
			}
		}
	}
}

int ScrBattle0::getRandomPeople(int myID, bool isHero) {
	ArrayList<int> randomID;

	int stateID;
	for(int id=0; id<SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES; id++) {
		if (id == myID || 
			battler.isHero(id) != isHero ||
			battler.getID(id) <= 0)
			continue;

		stateID = battler.getTopState(id);

		if (stateID == 1)//dead
			continue;

		randomID.Add(id);
	}

	if (randomID.GetLength() == 0)
		return myID;
	else
		return randomID[rand() % randomID.GetLength()];
}