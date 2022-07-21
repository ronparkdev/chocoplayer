#include "ScrBattle3.h"

#include "../../COMMON/MgrSkill.h"
#include "../../COMMON/MgrState.h"
#include "../../COMMON/ComMath.h"
#include "../../GRAPHIC/GrpAniBat.h"

#define ACT_MOVEX_HERO 30
#define ACT_MOVEY_HERO 30
#define ACT_MOVEY_WEAPON 50

#define ACT_TIME_HEROMOVE 20
#define ACT_TIME_WEAPON_SLOW 30
#define ACT_TIME_WEAPON_NORMAL 20
#define ACT_TIME_WEAPON_FAST 10

#define ACT_TIME_DEFENCE 60
#define ACT_TIME_ITEM 50
#define ACT_TIME_SKILL 30
#define ACT_TIME_ETC 60
#define ACT_TIME_ESCAPE 120
#define	ACT_TIME_SELFDEST 60

#define ACT_TIME_HERO_DAMAGED 30

#define TURN_TIME_MIN 5.0f

#define EFFECT_ALL_HEROS -1
#define EFFECT_ALL_ENEMIES -2

void ScrBattle3::updateTurnPer(int timedelta) {

	if (!isGameRunning()) {
		//CCLOG("TURNPER : NOTRUN");
		return;
	} else {
		//CCLOG("TURNPER : TRUN");
	}

	float ag, agMax = 0.0f, req_time, req_per;
	int stateID, restrict;
	for(int id=0; id<SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES; id++) {
		if (battler.getID(id) <= 0)
			continue;

		stateID = battler.getTopState(id);

		if (stateID > 0) {
			restrict = pGM->ldb.dCond[stateID-1].restriction;

			if (restrict == 1)//Dead
				continue;
		} else if (!battler.isAlive(id))
			continue;

		ag = battler.get(id, AGI);

		if (ag > agMax)
			agMax = ag;
	}

	bool in_queue[SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES];

	memset(in_queue, 0, sizeof(bool) * (SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES));

	for(int i=0; i<actQueue.GetLength(); i++) {
		int id = actQueue[i].actor_id;
		if (0<= id && id < SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES)
			in_queue[id] = true;
	}

	for(int id=0; id<SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES; id++) {
		if (battler.getID(id) <= 0 || in_queue[id])
			continue;

		stateID = battler.getTopState(id);

		restrict = 0;

		if (stateID > 0) {
			 restrict = pGM->ldb.dCond[stateID-1].restriction;

			 if (restrict == 1)//Cant Act
				 continue;
		} else if (!battler.isAlive(id))
			continue;

		ag = battler.get(id, AGI);

		req_time = TURN_TIME_MIN * sqrt(agMax / ag);

		req_per = timedelta / (req_time * FRAME_PER_SEC);

		if (turnPer[id] < 1.0f) {
			turnPer[id] += req_per;
			if (turnPer[id] >= 1.0f) {
				turnPer[id] = 1.0f;

				doTurnFull(id);

			}
		}
	}
}

void ScrBattle3::doTurnFull( int id )
{
	int stateID = battler.getTopState(id);

	int restrict = 0;
	if (stateID > 0)
		restrict = pGM->ldb.dCond[stateID-1].restriction;

	bool isHero = battler.isHero(id);

	ScrBattle3ActionQueue node;
	node.actor_id = id;
	node.using_id = 0;//Right Hand Attack
	node.type = B3AT_ACT_ATTACK;

	if (isHero && restrict == 0 && isBatAuto)
		restrict = 2;

	switch(restrict) {
		case 0:
		{
			if (isHero) {
				int i=0;
				for(i=0; i<heroReadyQueue.GetLength(); i++)
					if (heroReadyQueue[i] == id)
						break;

				if (i == heroReadyQueue.GetLength()) {
					heroReadyQueue.Add(id);

					if (batType == BTT_3A && curSelHero == -1) {
						pMenuHero->setInControl(true);
						pMenuHero->setIndex(id);

						curMenu = BMS_ACTION;
						pMenuAction->setInControl(true);
						pMenuAction->setVisible(true);
						pMenuAction->setIndex(0);

						curSelHero = id;
						updateActionMenu();
						pMenuAction->setInControl(true);
					}
				}
			} else {
				doEnemyAction(id);
			}
			break;
		}

		case 2:
		{
			//Attack Enemy
			addAction(id, getRandomPeople(id, !isHero), B3AT_ACT_ATTACK, 0);
			turnPer[id] = 0.0f;
			break;
		}

		case 3:
		{
			//Attack Team
			addAction(id, getRandomPeople(id, isHero), B3AT_ACT_ATTACK, 0);
			turnPer[id] = 0.0f;
			break;
		}
	}
}

void ScrBattle3::doEnemyAction( int id )
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
		int actID = actionAble[rand() % actionAble.GetLength()];

		LdbEnemyAction &act = enemy.actions[actID];

		ScrBattle3ActionQueue node;
		node.actor_id = id;
		node.target_id = getRandomPeople(id, true);

		int defAniID = 1;
		int defAniTime = GrpAniOne::getAnimationFrame(pGM, defAniID);

		node.type = B3AT_ACT_TURN_START;
		node.using_id = 0;
		addQueue(node);

		switch(act.kind) {
			case 0:
			{
				switch(act.basic) {
					case 0:{//Normal Attack
						addAction(node.actor_id, node.target_id, B3AT_ACT_ATTACK, 0);

						break;
					}
					case 1:{//Double Attack
						for(int i=0; i<2; i++)
							addAction(node.actor_id, node.target_id, B3AT_ACT_SKILL, 0);
						break;
					}
					case 2:{//Defence
						battler.add(id, BAT_DEFENCE_TYPE, 1);
						break;
					}
					case 3:{//view heros... (do nothing)
						break;
					}
					case 4:{
						battler.add(id, BAT_POWER_UP, 1);
						break;
					}
					case 5:{
						addAction(id, EFFECT_ALL_HEROS, B3AT_ACT_SELFDEST, 0);
						break;
					}
					case 6:{
						addAction(id, 0, B3AT_ACT_ESCAPE, 0);
						break;
					}
					case 7:{//do nothing
						break;
					}
				}

				break;
			}

			case 1:
			{//Use Skill
				MgrSkill mskill(pGM);
				int tgtID;
				if (mskill.isEffMyTeam(act.skill_id))
					tgtID = getRandomPeople(id, false);
				else
					tgtID = getRandomPeople(id, true);

				addAction(id, tgtID, B3AT_ACT_SKILL, act.skill_id);
				break;
			}

			case 2:
			{//Change Monster
				addAction(id, 0, B3AT_ACT_CHANGE, act.enemy_id);
				break;
			}
		}

		node.actor_id = id;
		node.type = B3AT_ACT_TURN_END;
		addQueue(node);
	}

	turnPer[id] = 0.0f;
}

void ScrBattle3::chkTurn()
{
	for(int id=0; id<SCR_BATTLE_MAX_HEROS+SCR_BATTLE_MAX_ENEMIES; id++) {
		if (battler.getID(id) <= 0)
			continue;

		int stateID = battler.getTopState(id);

		if (stateID <= 0)
			continue;

		int restrict = pGM->ldb.dCond[stateID-1].restriction;

		switch(restrict) {
		case 1:
			//Cant Act
			removeQueue(id);

		case 2:
		case 3:
			if (battler.isHero(id)) {
				for(int i=0; i<heroReadyQueue.GetLength(); i++)
					if (heroReadyQueue[i] == id) {
						heroReadyQueue.Remove(i);
						i--;
					}
			}
		}

	}
}

int ScrBattle3::getRandomPeople(int myID, bool isHero) {
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

void ScrBattle3::addAction(int actID, int tgtID, ScrBattle3ActionType type, int etcID) {
	bool actIsHero = battler.isHero(actID);
	int actLdbID = battler.getID(actID);
	bool isActRightDirection = false;
	GlbEquipment equip;

	if (actIsHero)
		equip = battler.getEquip(actID);

	ScrBattle3ActionQueue node;
	node.actor_id = actID;

	const CCPoint defPos = battler.getDefPos(actID);

	node.type = B3AT_ACT_TURN_START;
	node.using_id = lastBatCmdActType;
	addQueue(node);

	switch(type) {
		case B3AT_ACT_ATTACK:
		{
			bool isUseTwoHands = false;
			bool isUseRight = false;
			bool isUseLeft = false;
			bool isAttackAll = false; 

			int atkCntR = 1,	atkCntL = 1;
			int atkAniR = 1,	atkAniL = 1;
			int atkTimeR = 0,	atkTimeL = 0;

			LdbItem *item;

			battler.add(actID, BAT_DEFENCE_TYPE, 0);

			if (actIsHero) {
				if (etcID>0) {
					LdbCharacter &ldbCh = pGM->ldb.dChar[actLdbID-1];

					if (ldbCh.use_two_hand && equip.shield_id > 0)
						isUseTwoHands = true;
				}

				atkAniL = atkAniR = pGM->ldb.dChar[actLdbID-1].unarmed_ani;
			}

			atkTimeL = atkTimeR = GrpAniOne::getAnimationFrame(pGM, 1);

			if (actIsHero && equip.weapon_id > 0) {
				isUseRight = true;
				item = &(pGM->ldb.dItem[equip.weapon_id-1]);

				atkAniR = item->animation_id;
				atkTimeR = GrpAniOne::getAnimationFrame(pGM, atkAniR);

				if (item->dual_attack)atkCntR *= 2;
				if (item->attack_all)isAttackAll = true;

				if (actLdbID-1 < item->animation_data.GetLength()) {
					LdbBattleAniData &ani = item->animation_data[actLdbID-1];
					atkCntR *= ani.attacks +1;

					if (ani.ranged) {
						switch(ani.ranged_speed) {
							case 0:atkTimeR = ACT_TIME_WEAPON_FAST;break;
							case 1:atkTimeR = ACT_TIME_WEAPON_NORMAL;break;
							case 2:atkTimeR = ACT_TIME_WEAPON_SLOW;break;
						}
					}
				}
			}

			if (actIsHero && isUseTwoHands && equip.shield_id > 0) {
				isUseLeft = true;
				item = &(pGM->ldb.dItem[equip.shield_id-1]);

				atkAniL = item->animation_id;
				atkTimeL = GrpAniOne::getAnimationFrame(pGM, atkAniL);

				if (item->dual_attack)atkCntL *= 2;
				if (item->attack_all)isAttackAll = true;

				if (actLdbID-1 < item->animation_data.GetLength()) {
					LdbBattleAniData &ani = item->animation_data[actLdbID-1];
					atkCntL *= ani.attacks +1;

					if (ani.ranged) {
						switch(ani.ranged_speed) {
						case 0:atkTimeL = ACT_TIME_WEAPON_FAST;break;
						case 1:atkTimeL = ACT_TIME_WEAPON_NORMAL;break;
						case 2:atkTimeL = ACT_TIME_WEAPON_SLOW;break;
						}
					}
				}
			}

			if (isAttackAll) {
				if (battler.isHero(tgtID))
					tgtID = EFFECT_ALL_ENEMIES;
				else
					tgtID = EFFECT_ALL_HEROS;
			}

			node.actor_id = actID;
			node.target_id = tgtID;

			isActRightDirection = isRightOnAction(actID, tgtID);

			node.type = B3AT_DIRECTING;
			addQueue(node);

			//MoveFront

			if (isUseRight || isUseLeft) {
				if (actLdbID-1 < item->animation_data.GetLength()) {
					LdbBattleAniData &ani = item->animation_data[actLdbID-1];
					
					if (ani.movement > 0) {
						switch(ani.movement) {
							case 1:
							{
								node.move_to = ccp(defPos.x + ACT_MOVEX_HERO * (isActRightDirection?1:-1), defPos.y);
								node.total_frame = ACT_TIME_HEROMOVE;
								node.using_id = 8;//WalkingLeft
								node.type_id = 2;//Walking
								node.type = B3AT_CHAR_MOVING_AND_ANIMATING;
								addQueue(node);
								break;
							}

							case 2:
							{
								node.move_to = ccp(defPos.x + ACT_MOVEX_HERO * (isActRightDirection?1:-1), defPos.y);
								node.total_frame = ACT_TIME_HEROMOVE;
								node.type = B3AT_CHAR_JUMPING;
								addQueue(node);
								break;
							}

							case 3:
							{
								node.total_frame = ACT_TIME_HEROMOVE;
								node.type = B3AT_CHAR_MOVING_AND_ANIMATING;
								if (isAttackAll) {
									node.move_to = ccp(defPos.x + ACT_MOVEX_HERO * (isActRightDirection?1:-1), defPos.y);
									
								} else {
									CCPoint pos = battler.getDefPos(tgtID);
									ScrBattle3EnemyNode *enemy = pNodeEnemy[tgtID-SCR_BATTLE_MAX_HEROS];
									node.move_to = ccp(pos.x + (enemy->getWidth()/2 + HERO_SIZE/2) * (isActRightDirection?-1:1), pos.y);
								}
								node.using_id = 8;//WalkingLeft
								node.type_id = 2;//Walking
								addQueue(node);
								break;
							}
						}
					}
				}
			}
			
			//Attack

			if (!isUseLeft && !isUseRight) {
				node.type = B3AT_ACT_ATTACK;
				node.using_id = 0;//Right Hand Attack
				addQueue(node);
			}

			if (!actIsHero) {
				int tmpActID = node.actor_id;
				node.actor_id = node.target_id;
				node.using_id = 5;
				node.type_id = 0;
				node.total_frame = ACT_TIME_HERO_DAMAGED;
				node.type = B3AT_CHAR_ANIMATING;
				addQueue(node);

				node.actor_id = tmpActID;
			}

			if (isUseRight) {
				item = &(pGM->ldb.dItem[equip.weapon_id-1]);

				bool isAtkInOther = isAttackAllInOther(actLdbID, item);

				for(int i=0; i<atkCntR; i++) {
					for(int j=0; j<nodeEnemyLen; j++) {
						int id = j + SCR_BATTLE_MAX_HEROS;

						if (!isAtkInOther) {
							j = 9999;
							id = tgtID;
						} else {
							int stateID = battler.getTopState(j);
							if (stateID == 1)
								continue;
						}

						addWeaponRangeAni(actLdbID, item, true, actID, tgtID);

						node.using_id = atkAniR;
						node.type = B3AT_ANIMATING;
						addQueue(node);

						node.using_id = 1;//Right Hand Attack
						node.type_id = 0;
						node.total_frame = atkTimeR;
						node.type = B3AT_CHAR_ANIMATING;
						addQueue(node);

						if (!isWeaponRangeAni(actLdbID, item))
							addWait(atkTimeR);

						node.target_id = id;
						node.using_id = 0;//Right Hand Attack
						node.type = B3AT_ACT_ATTACK;
						addQueue(node);

						addWeaponRangeAni(actLdbID, item, false, actID, tgtID);
					}
				}
			}

			if (isUseLeft) {
				item = &(pGM->ldb.dItem[equip.shield_id-1]);

				bool atkInOther = isAttackAllInOther(actLdbID, item);

				for(int i=0; i<atkCntL; i++) {
					for(int j=0; j<nodeEnemyLen; j++) {
						int id = j + SCR_BATTLE_MAX_HEROS;

						if (!atkInOther) {
							j = 9999;
							id = tgtID;
						} else {
							int stateID = battler.getTopState(j);
							if (stateID == 1)
								continue;
						}

						addWeaponRangeAni(actLdbID, item, true, actID, tgtID);

						node.using_id = atkAniL;
						node.type = B3AT_ANIMATING;
						addQueue(node);

						node.using_id = 2;//Left Hand Attack
						node.type_id = 0;
						node.total_frame = atkTimeL;
						node.type = B3AT_CHAR_ANIMATING;
						addQueue(node);

						if (!isWeaponRangeAni(actLdbID, item))
							addWait(atkTimeL);

						node.target_id = id;
						node.using_id = 1;//Left Hand Attack
						node.type = B3AT_ACT_ATTACK;
						addQueue(node);

						addWeaponRangeAni(actLdbID, item, false, actID, tgtID);
					}
				}
			}

			//MoveBack

			node.target_id = tgtID;

			if (isUseRight || isUseLeft) {
				if (actLdbID-1 < item->animation_data.GetLength()) {
					LdbBattleAniData &ani = item->animation_data[actLdbID-1];

					if (ani.movement > 0) {
						switch(ani.movement) {
							case 1:
							{
								node.move_to = defPos;
								node.total_frame = ACT_TIME_HEROMOVE;
								node.using_id = 9;//WalkingRight
								node.type_id = 2;//Walking
								node.type = B3AT_CHAR_MOVING_AND_ANIMATING;
								addQueue(node);
								break;
							}

							case 2:
							{
								node.move_to = defPos;
								node.total_frame = ACT_TIME_HEROMOVE;
								node.type = B3AT_CHAR_JUMPING;
								addQueue(node);
								break;
							}

							case 3:
							{
								node.total_frame = ACT_TIME_HEROMOVE;
								node.move_to = defPos;
								node.using_id = 9;//WalkingRight
								node.type_id = 2;//Walking
								node.type = B3AT_CHAR_MOVING_AND_ANIMATING;
								addQueue(node);
								break;
							}
						}
					}
				}
			}


			if (actIsHero) {
				if (isAttackAll)
					tgtID = EFFECT_ALL_ENEMIES;

				pEvtMain->setLastAction(actID, lastBatCmdActType, tgtID);
			}
			break;
		}

		case B3AT_ACT_SKILL:
		{
			int skillID = etcID;

			LdbSkill *skill = &(pGM->ldb.dSkill[skillID-1]);
			MgrSkill mskill(pGM);

			bool isAttackAll = mskill.isAllEffect(skillID); 
			int atkAni = skill->ani_id;
			int atkTime = GrpAniOne::getAnimationFrame(pGM, atkAni);
				
			int actIsHero = battler.isHero(actID);
			int actLdbID = battler.getID(actID);

			battler.add(actID, BAT_DEFENCE_TYPE, 0);

			if (isAttackAll) {
				if (battler.isHero(actID) ^ mskill.isEffMyTeam(skillID))
					tgtID = EFFECT_ALL_ENEMIES;
				else
					tgtID = EFFECT_ALL_HEROS;
			}

			node.actor_id = actID;
			node.target_id = tgtID;

			isActRightDirection = isRightOnAction(actID, tgtID);

			node.type = B3AT_DIRECTING;
			addQueue(node);

			//MoveFront

			if (actIsHero && actLdbID-1 < skill->battler_ani.GetLength()) {
				LdbBattleAniData &ani = skill->battler_ani[actLdbID-1];

				if (ani.movement > 0) {
					switch(ani.movement) {
						case 1:
						{
							node.move_to = ccp(defPos.x + ACT_MOVEX_HERO * (isActRightDirection?1:-1), defPos.y);
							node.total_frame = ACT_TIME_HEROMOVE;
							node.using_id = 8;//WalkingLeft
							node.type_id = 2;//Walking
							node.type = B3AT_CHAR_MOVING_AND_ANIMATING;
							addQueue(node);
							break;
						}

						case 2:
						{
							node.move_to = ccp(defPos.x + ACT_MOVEX_HERO * (isActRightDirection?1:-1), defPos.y);
							node.total_frame = ACT_TIME_HEROMOVE;
							node.type = B3AT_CHAR_JUMPING;
							addQueue(node);
							break;
						}

						case 3:
						{
							node.total_frame = ACT_TIME_HEROMOVE;
							node.type = B3AT_CHAR_MOVING_AND_ANIMATING;
							if (isAttackAll) {
								node.move_to = ccp(defPos.x + ACT_MOVEX_HERO * (isActRightDirection?1:-1), defPos.y);

							} else {
								CCPoint pos = battler.getDefPos(tgtID);
								ScrBattle3EnemyNode *enemy = pNodeEnemy[tgtID-SCR_BATTLE_MAX_HEROS];
								node.move_to = ccp(pos.x + (enemy->getWidth()/2 + HERO_SIZE/2) * (isActRightDirection?-1:1), pos.y);
							}
							node.using_id = 8;//WalkingLeft
							node.type_id = 2;//Walking
							addQueue(node);
							break;
						}
					}
				}
			}

			//Skill

			if (actIsHero && actLdbID-1 < skill->battler_ani.GetLength()) {
				LdbBattleAniData &ani = skill->battler_ani[actLdbID-1];

				node.using_id = ani.skill_pose;
				node.type_id = 1;
				node.total_frame = ACT_TIME_SKILL;
				node.type = B3AT_CHAR_ANIMATING;
				addQueue(node);

				node.type = B3AT_WAIT;
				addQueue(node);
			}

			node.using_id = atkAni;
			node.type = B3AT_ANIMATING;
			addQueue(node);

			node.total_frame = atkTime;
			node.type = B3AT_WAIT;
			addQueue(node);

			node.using_id = etcID;
			node.type = B3AT_ACT_SKILL;
			addQueue(node);

			//MoveBack

			if (actIsHero && actLdbID-1 < skill->battler_ani.GetLength()) {
				LdbBattleAniData &ani = skill->battler_ani[actLdbID-1];

				if (ani.movement > 0) {
					switch(ani.movement) {
						case 1:
						{
							node.move_to = defPos;
							node.total_frame = ACT_TIME_HEROMOVE;
							node.using_id = 9;//WalkingRight
							node.type_id = 2;//Walking
							node.type = B3AT_CHAR_MOVING_AND_ANIMATING;
							addQueue(node);
							break;
						}

						case 2:
						{
							node.move_to = defPos;
							node.total_frame = ACT_TIME_HEROMOVE;
							node.type = B3AT_CHAR_JUMPING;
							addQueue(node);
							break;
						}

						case 3:
						{
							node.total_frame = ACT_TIME_HEROMOVE;
							node.move_to = defPos;
							node.using_id = 9;//WalkingRight
							node.type_id = 2;//Walking
							node.type = B3AT_CHAR_MOVING_AND_ANIMATING;
							addQueue(node);
							break;
						}
					}
				}
			}

			if (actIsHero)
				pEvtMain->setLastAction(actID, lastBatCmdActType, tgtID);
			
			break;
		}

		case B3AT_ACT_ITEM:
		{
			node.actor_id = actID;
			node.target_id = tgtID;
			node.using_id = etcID;
			node.type = B3AT_ACT_SKILL;
			
			pEvtMain->setLastAction(actID, lastBatCmdActType, tgtID);
			break;
		}

		case B3AT_ACT_ESCAPE:
		{
			node.total_frame = ACT_TIME_ESCAPE;

			if (battler.isHero(node.actor_id)) {
				if (RANDF() > escapeChanceHero) {
					escapeChanceHero += 0.1f;
					break;
				}
			} else {
				if (RANDF() > escapeChanceEnemy) {
					escapeChanceEnemy += 0.1f;
					break;
				}
			}

			if (battler.isHero(node.actor_id)) {
				node.type = B3AT_CHAR_ANIMATING;
				node.using_id = 9;
				node.type_id = 2;
				for(int id=0; id<nodeHeroLen; id++) {
					node.actor_id = id;
					addQueue(node);
				}
				isBatEscaped = true;
			}
				
			node.actor_id = actID;
			node.type = B3AT_ACT_ESCAPE;
			addQueue(node);

			break;
		}

		case B3AT_ACT_SELFDEST:
		{
			node.total_frame = ACT_TIME_SELFDEST;

			node.target_id = EFFECT_ALL_HEROS;
			node.type = B3AT_ACT_ATTACK;
			node.using_id = 0;//Right Hand Attack
			addQueue(node);

			node.type = B3AT_ACT_SELFDEST;
			addQueue(node);

			break;
		}

		case B3AT_ACT_CHANGE:
		{
			node.type = B3AT_ACT_CHANGE;
			node.using_id = etcID;
			addQueue(node);
			break;
		}
	}

	node.type = B3AT_ACT_TURN_END;
	addQueue(node);
}

bool ScrBattle3::isAttackAllInOther(int actLdbID, LdbItem *item) {
	if (actLdbID-1 >= item->animation_data.GetLength())
		return false;

	LdbBattleAniData &ani = item->animation_data[actLdbID-1];

	if (ani.ranged)
		return false;

	return (item->ranged_target == 3);
}

void ScrBattle3::addWeaponRangeAni(int actLdbID, LdbItem *item, bool isGoing, int actID, int tgtID) {
	if (actLdbID-1 >= item->animation_data.GetLength())
		return;

	LdbBattleAniData &ani = item->animation_data[actLdbID-1];

	if (!ani.ranged)
		return;

	ScrBattle3ActionQueue n_go, n_bk;
	int frame;

	if (!item->ranged_trajectory) {
		n_go.type = n_bk.type = B3AT_WEAP_MOVING;
	} else {
		n_go.type = B3AT_WEAP_UPPER_CURVED_MOVING;
		n_bk.type = B3AT_WEAP_LOWER_CURVED_MOVING;
	}

	n_go.actor_id = n_bk.target_id = actID;
	n_go.target_id = n_bk.actor_id = tgtID;
	
	//n_go.using_id = n_bk.using_id = tgtID - SCR_BATTLE_MAX_HEROS;
	n_go.using_id = n_bk.using_id = actLdbID;
	n_go.type_id = n_bk.type_id = ani.ranged_anim;

	n_go.move_from = n_bk.move_to = battler.getDefPos(actID);
	switch(item->ranged_target) {
	case 0://Attack-- One
	case 3://Atack all, Move to all enemies in order
		{
			n_go.move_to = n_bk.move_from = battler.getDefPos(tgtID);
			break;
		}
	case 1://Attack all, Move to center of enemies
		{
			CCPoint centerpos(ccp(0,0));
			int alive_enemies = 0;
			for(int i=0; i<nodeEnemyLen; i++) {
				int id = i + SCR_BATTLE_MAX_HEROS;
				
				int stateID = battler.getTopState(id);

				if (stateID == 1)
					continue;

				centerpos.x += pNodeEnemy[i]->getPositionX();
				centerpos.y += pNodeEnemy[i]->getPositionY();

				alive_enemies++;
			}

			centerpos.x /= alive_enemies;
			centerpos.y /= alive_enemies;

			n_go.move_to = n_bk.move_from = centerpos;
			break;
		}
	case 2://Attack all, Move to each enemies
		{
			n_go.target_id = n_bk.actor_id = EFFECT_ALL_ENEMIES;
			break;
		}
	}

	switch(ani.ranged_speed) {
		case 0:frame = ACT_TIME_WEAPON_FAST;break;
		case 1:frame = ACT_TIME_WEAPON_NORMAL;break;
		case 2:frame = ACT_TIME_WEAPON_SLOW;break;
		default:frame = 0;
	}

	n_go.total_frame = n_bk.total_frame = frame;

	addQueue(isGoing?n_go:n_bk);
}

bool ScrBattle3::isWeaponRangeAni( int actLdbID, LdbItem * item )
{
	if (actLdbID-1 >= item->animation_data.GetLength())
		return false;

	LdbBattleAniData &ani = item->animation_data[actLdbID-1];

	return ani.ranged;
}

void ScrBattle3::addWait( int frame )
{
	ScrBattle3ActionQueue node;
	node.type = B3AT_WAIT;
	node.total_frame = frame;

	addQueue(node);
}

void ScrBattle3::updateQueue(int framedelta) {
	if (actQueue.GetLength() == 0) {
		//CCLOG("QUEUE : NONE");
		return;
	}

	//CCLOG((_ST("QUEUE : FRAME") + m_action_frame_left + " TYPE" + (int)m_action_queue.GetHead()->type).getTextUTF8());

	if (actFrameLeft > 0) {
		ScrBattle3ActionQueue &node = actQueue[0];
		CCNode *obj;
		
		float totper = (float)(node.total_frame-actFrameLeft) / node.total_frame;
		float frameper = (float)framedelta / actFrameLeft;

		if (totper > 1.0f)
			totper = 1.0f;

		if (frameper > 1.0f)
			frameper = 1.0f;
		
		switch(node.type) {
			case B3AT_CHAR_MOVING_AND_ANIMATING:
			{
				obj = pNodeHero[node.actor_id];

				CCPoint offset = obj->getPosition();

				offset.x = node.move_to.x * frameper 
							+ offset.x * (1.0f - frameper);
				offset.y = node.move_to.y * frameper 
							+ offset.y * (1.0f - frameper);

				obj->setPosition(offset);

				break;
			}
		
			case B3AT_CHAR_JUMPING:
			{
				obj = pNodeHero[node.actor_id];

				CCPoint offset;

				offset.x = node.move_to.x * totper 
						+ node.move_from.x * (1.0f - totper);
				offset.y = node.move_to.y * totper 
						+ node.move_from.y * (1.0f - totper) 
						+ sin(totper * PI) * ACT_MOVEY_HERO;

				obj->setPosition(offset);
				
				break;
			}

			case B3AT_WEAP_MOVING:
			{
				if (node.actor_id == EFFECT_ALL_ENEMIES) {
					for(int i=0; i<nodeEnemyLen; i++) {
						int id = i + SCR_BATTLE_MAX_HEROS;

						int stateID = battler.getTopState(id);

						if (stateID == 1)
							continue;

						obj = pNodeEnemy[i];

						CCPoint offset;

						offset.x = node.move_to.x * totper 
							+ obj->getPositionX() * (1.0f - totper);
						offset.y = node.move_to.y * totper 
							+ obj->getPositionY() * (1.0f - totper);

						pNodeWeapon[i]->setPosition(offset);
					}
				} else if (node.target_id == EFFECT_ALL_ENEMIES) {
					for(int i=0; i<nodeEnemyLen; i++) {
						int id = i + SCR_BATTLE_MAX_HEROS;

						int stateID = battler.getTopState(id);

						if (stateID == 1)
							continue;

						obj = pNodeEnemy[i];

						CCPoint offset;

						offset.x = obj->getPositionX() * totper 
							+ node.move_from.x * (1.0f - totper);
						offset.y = obj->getPositionY() * totper 
							+ node.move_from.y * (1.0f - totper);

						pNodeWeapon[i]->setPosition(offset);
					}
				} else {
					obj = pNodeWeapon[node.using_id];

					CCPoint offset;

					offset.x = node.move_to.x * totper 
						+ node.move_from.x * (1.0f - totper);
					offset.y = node.move_to.y * totper 
						+ node.move_from.y * (1.0f - totper);

					obj->setPosition(offset);
				}
				break;
			}

			case B3AT_WEAP_UPPER_CURVED_MOVING:
			case B3AT_WEAP_LOWER_CURVED_MOVING:
			{
				if (node.actor_id == EFFECT_ALL_ENEMIES) {
					for(int i=0; i<nodeEnemyLen; i++) {
						int id = i + SCR_BATTLE_MAX_HEROS;

						int stateID = battler.getTopState(id);

						if (stateID == 1)
							continue;

						obj = pNodeEnemy[i];

						CCPoint offset;

						offset.x = node.move_to.x * totper 
							+ obj->getPositionX() * (1.0f - totper);
						offset.y = node.move_to.y * totper 
							+ obj->getPositionY() * (1.0f - totper)
							+ sin(totper * PI) * ACT_MOVEY_WEAPON 
							* ((node.type == B3AT_WEAP_LOWER_CURVED_MOVING)?-1:1);

						pNodeWeapon[i]->setPosition(offset);
					}
				} else if (node.target_id == EFFECT_ALL_ENEMIES) {
					for(int i=0; i<nodeEnemyLen; i++) {
						int id = i + SCR_BATTLE_MAX_HEROS;

						int stateID = battler.getTopState(id);

						if (stateID == 1)
							continue;

						obj = pNodeEnemy[i];

						CCPoint offset;

						offset.x = obj->getPositionX() * totper 
							+ node.move_from.x * (1.0f - totper);
						offset.y = obj->getPositionY() * totper 
							+ node.move_from.y * (1.0f - totper) 
							+ sin(totper * PI) * ACT_MOVEY_WEAPON 
							* ((node.type == B3AT_WEAP_LOWER_CURVED_MOVING)?-1:1);

						pNodeWeapon[i]->setPosition(offset);
					}
				} else {
					int weapID;
					if (battler.isHero(node.actor_id))
						weapID = node.target_id - SCR_BATTLE_MAX_HEROS;
					else
						weapID = node.actor_id - SCR_BATTLE_MAX_HEROS;

					obj = pNodeWeapon[weapID];

					CCPoint offset;

					offset.x = node.move_to.x * totper 
						+ node.move_from.x * (1.0f - totper);
					offset.y = node.move_to.y * totper 
						+ node.move_from.y * (1.0f - totper) 
						+ sin(totper * PI) * ACT_MOVEY_WEAPON 
						* ((node.type == B3AT_WEAP_LOWER_CURVED_MOVING)?-1:1);

					obj->setPosition(offset);
				}
				
				break;
			}

			case B3AT_ACT_SELFDEST:
			{
				if (battler.isHero(node.actor_id))
					break;

				int id = node.actor_id-SCR_BATTLE_MAX_HEROS;

				pNodeEnemy[id]->setScale(1.0f + totper);

				break;
			}

			case B3AT_ACT_ESCAPE:
			{
				for(int id=0; id<nodeHeroLen; id++)
					pNodeHero[id]->setPositionX(pNodeHero[id]->getPositionX() 
								+ framedelta * 2.0f * (pNodeHero[id]->getDirection()?-1.0f:1.0f));

				break;
			}
		}		
		actFrameLeft -= framedelta;
	} else {
		actFrameLeft = 0;

		switch(actQueue[0].type) {
		case B3AT_WEAP_MOVING:
		case B3AT_WEAP_LOWER_CURVED_MOVING:
		case B3AT_WEAP_UPPER_CURVED_MOVING:
			{
				for(int i=0; i<nodeEnemyLen; i++)
					pNodeWeapon[i]->setVisible(false);
				break;
			}

		}

		actQueue.Remove(0);
		
		if (actQueue.GetLength() > 0) {
			ScrBattle3ActionQueue &node = actQueue[0];

			doQueue(node);
		}
	}
}

void ScrBattle3::addQueue(const ScrBattle3ActionQueue &node) {

	switch(node.type) {
	case B3AT_ACT_ATTACK:
		CCLOG("AD__B3AT_ACT_ATTACK");break;
	case B3AT_ACT_SKILL:
		CCLOG("AD__B3AT_ACT_SKILL");break;
	case B3AT_ACT_ESCAPE:
		CCLOG("AD__B3AT_ACT_ESCAPE");break;
	case B3AT_ANIMATING:
		CCLOG("AD__B3AT_ANIMATING");break;
	case B3AT_CHAR_MOVING_AND_ANIMATING:
		CCLOG("AD__B3AT_CHAR_MOVING_AND_ANIMATING");break;
	case B3AT_CHAR_ANIMATING:
		CCLOG("AD__B3AT_CHAR_ANIMATING");break;
	case B3AT_CHAR_JUMPING:
		CCLOG("AD__B3AT_CHAR_JUMPING");break;
	case B3AT_WEAP_UPPER_CURVED_MOVING:
		CCLOG("AD__B3AT_WEAP_UPPER_CURVED_MOVING");break;
	case B3AT_WEAP_LOWER_CURVED_MOVING:
		CCLOG("AD__B3AT_WEAP_LOWER_CURVED_MOVING");break;
	case B3AT_WAIT:
		CCLOG("AD__B3AT_WAIT");break;
	default:
		CCLOG("AD__%d", node.type);break;
	}

	actQueue.Add(node);

	if (actQueue.GetLength() == 1)
		doQueue(actQueue[0]);
}

void ScrBattle3::doQueue(ScrBattle3ActionQueue &node) {
	bool isEffectAll = false;
	int effVal = 0;

	switch(node.type) {
	case B3AT_ACT_ATTACK:
		CCLOG("DO__B3AT_ACT_ATTACK");break;
	case B3AT_ACT_SKILL:
		CCLOG("DO__B3AT_ACT_SKILL");break;
	case B3AT_ACT_ESCAPE:
		CCLOG("DO__B3AT_ACT_ESCAPE");break;
	case B3AT_ANIMATING:
		CCLOG("DO__B3AT_ANIMATING");break;
	case B3AT_CHAR_MOVING_AND_ANIMATING:
		CCLOG("DO__B3AT_CHAR_MOVING_AND_ANIMATING");break;
	case B3AT_CHAR_ANIMATING:
		CCLOG("DO__B3AT_CHAR_ANIMATING");break;
	case B3AT_CHAR_JUMPING:
		CCLOG("DO__B3AT_CHAR_JUMPING");break;
	case B3AT_WEAP_UPPER_CURVED_MOVING:
		CCLOG("DO__B3AT_WEAP_UPPER_CURVED_MOVING");break;
	case B3AT_WEAP_LOWER_CURVED_MOVING:
		CCLOG("DO__B3AT_WEAP_LOWER_CURVED_MOVING");break;
	case B3AT_WAIT:
		CCLOG("DO__B3AT_WAIT");break;
	default:
		CCLOG("DO__%d", node.type);break;
	}

	switch(node.type) {
		case B3AT_BATTLE_START:
		{
			isBatStart = true;
			break;
		}

		case B3AT_ACT_TURN_START:
		{
			hue.setFlash(node.actor_id, 0, 31, 31, 31, 10, 20);

			turnCnt++;
			if (0<=node.actor_id && node.actor_id<SCR_BATTLE_MAX_HEROS+SCR_BATTLE3_MAX_NUMBER)
				battler.add(node.actor_id, BAT_TURN_COUNT, 1);

			pEvtMain->setLastActID(node.actor_id);

			isEventNeedCheck = true;
			evtPage = 0;
			break;
		}

		case B3AT_ACT_TURN_END:
		{
			updateHeroMenu();
			updateEnemyMenu();
			chkBattleFinish();
			updatePositionBattleStart();
			break;
		}

		case B3AT_WAIT:
		{
			actFrameLeft = node.total_frame;
			break;
		}
		

		case B3AT_ACT_ATTACK:
		{
			if (node.target_id == EFFECT_ALL_HEROS) {
				for(int id=0; id<nodeHeroLen; id++) {
					int stateID = battler.getTopState(id);

					if (stateID == 1)
						continue;

					doAttack(node.actor_id, id, node.using_id == 1);
				}
			} else if (node.target_id == EFFECT_ALL_ENEMIES) {
				for(int i=0; i<nodeEnemyLen; i++) {
					int id = i + SCR_BATTLE_MAX_HEROS;
					int stateID = battler.getTopState(id);

					if (stateID == 1)
						continue;

					doAttack(node.actor_id, id, node.using_id == 1);
				}
			} else {
				doAttack(node.actor_id, node.target_id, node.using_id == 1);
			}
			break;
		}

		case B3AT_ACT_SKILL:
		{
			if (node.target_id == EFFECT_ALL_HEROS) {
				for(int id=0; id<nodeHeroLen; id++) {
					int stateID = battler.getTopState(id);

					if (stateID == 1)
						continue;

					doSkill(node.actor_id, id, node.using_id);
				}
			} else if (node.target_id == EFFECT_ALL_ENEMIES) {
				for(int i=0; i<nodeEnemyLen; i++) {
					int id = i + SCR_BATTLE_MAX_HEROS;
					int stateID = battler.getTopState(id);

					if (stateID == 1)
						continue;

					doSkill(node.actor_id, id, node.using_id);
				}
			} else {
				doSkill(node.actor_id, node.target_id, node.using_id);
			}

			break;
		}

		case B3AT_ANIMATING:
		{
			CCPoint point;
			int tgtID = node.target_id;
			if (tgtID == EFFECT_ALL_ENEMIES) {
				for(int i=0; i<nodeEnemyLen; i++) {
					int id = i + SCR_BATTLE_MAX_HEROS;
					int stateID = battler.getTopState(id);
					if (stateID == 1)
						continue;

					point = battler.getDefPos(id);
					pAnis[id]->set(node.using_id, point.x, point.y, pNodeEnemy[i]->getWidth());
				}
			} else if (tgtID == EFFECT_ALL_HEROS) {
				for(int id=0; id<nodeHeroLen; id++) {
					int stateID = battler.getTopState(id);
					if (stateID == 1)
						continue;

					point = battler.getDefPos(id);
					pAnis[id]->set(node.using_id, point.x, point.y, HERO_SIZE);
				}
			} else {
				point = battler.getDefPos(node.target_id);
				if (battler.isHero(node.target_id))
					pAnis[node.target_id]->set(node.using_id, point.x, point.y,
							HERO_SIZE);
				else
					pAnis[node.target_id]->set(node.using_id, point.x, point.y, 
							pNodeEnemy[node.target_id - SCR_BATTLE_MAX_HEROS]->getWidth());

			}
			break;
		}

		case B3AT_CHAR_MOVING_AND_ANIMATING:
		{
			actFrameLeft = node.total_frame;
			pNodeHero[node.actor_id]->getPosition(&node.move_from.x, &node.move_from.y);
		}

		case B3AT_CHAR_ANIMATING:
		{
			pNodeHero[node.actor_id]
				->setAnimation(node.using_id, node.type_id, node.total_frame);
			break;
		}

		case B3AT_WEAP_MOVING:
		case B3AT_WEAP_UPPER_CURVED_MOVING:
		case B3AT_WEAP_LOWER_CURVED_MOVING:
		{
			//actLdbID, range_ani_id
			LdbBattlerAniExt &weap = pGM->ldb.dBatrAnim[node.using_id-1].weapon_data[node.type_id];

			if (node.actor_id == EFFECT_ALL_ENEMIES || node.target_id == EFFECT_ALL_ENEMIES) {
				for(int i=0; i<nodeEnemyLen; i++) {
					int id = i + SCR_BATTLE_MAX_HEROS;
					int stateID = battler.getTopState(id);

					if (stateID == 1)
						continue;

					pNodeWeapon[i]->set(weap.battler_name, weap.battler_index);
					pNodeWeapon[i]->setVisible(true);
				}

				if (node.actor_id == EFFECT_ALL_ENEMIES) {
					pNodeHero[node.target_id]->getPosition(&node.move_to.x, &node.move_to.y);
				} else {
					pNodeHero[node.actor_id]->getPosition(&node.move_from.x, &node.move_from.y);
				}
			} else {
				int weapID;
				if (battler.isHero(node.actor_id))
					weapID = node.target_id - SCR_BATTLE_MAX_HEROS;
				else
					weapID = node.actor_id - SCR_BATTLE_MAX_HEROS;

				pNodeWeapon[weapID]->set(weap.battler_name, weap.battler_index);
				pNodeWeapon[weapID]->setVisible(true);

				if (battler.isHero(node.actor_id))
					pNodeHero[node.actor_id]->getPosition(&node.move_from.x, &node.move_from.y);
				else
					pNodeEnemy[node.actor_id-SCR_BATTLE_MAX_HEROS]->getPosition(&node.move_from.x, &node.move_from.y);

				if (battler.isHero(node.target_id))
					pNodeHero[node.target_id]->getPosition(&node.move_to.x, &node.move_to.y);
				else
					pNodeEnemy[node.target_id-SCR_BATTLE_MAX_HEROS]->getPosition(&node.move_to.x, &node.move_to.y);

			}
			actFrameLeft = node.total_frame;
			break;
		}

		case B3AT_CHAR_JUMPING:
		{
			actFrameLeft = node.total_frame;
			pNodeHero[node.actor_id]->getPosition(&node.move_from.x, &node.move_from.y);
			break;
		}

		case B3AT_ACT_ESCAPE:
		{
			if (battler.isHero(node.actor_id)) {
				updatePositionBattleStart();
				actFrameLeft = node.total_frame;
			} else {
				battler.add(node.actor_id, HP, -9999999);
			}

			break;
		}

		case B3AT_ACT_SELFDEST:
		{
			battler.add(node.actor_id, HP, -9999999);
			actFrameLeft = node.total_frame;
			break;
		}

		case B3AT_DIRECTING:
		{
			updatePositionAction(node.actor_id, node.target_id);
			break;
		}

		case B3AT_ACT_CHANGE:
		{
			int id = node.actor_id - SCR_BATTLE_MAX_HEROS;
			pNodeEnemy[id]->change(node.using_id);
			break;
		}
	}
	
	chkTurn();
}

void ScrBattle3::removeQueue( int id )
{
	int i = 0;
	for(; i<actQueue.GetLength(); i++)
		if (actQueue[i].type != B3AT_ACT_TURN_END)
			break;

	i++;

	while(i < actQueue.GetLength()) {
		if (actQueue[i].actor_id == id) {
			actQueue.Remove(i);
		} else {
			i++;
		}
	}
}	
