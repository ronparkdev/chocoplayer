#include "ScrBattle3.h"

void ScrBattle3::initPosition() {
	//Default Pos/Direction Setting
	if (isBatAutoPos) {
		CCPoint point;
		switch(batPos) {
		case BPOS_NORMAL:
			{
				for(int id=0; id<nodeHeroLen; id++)
					pNodeHero[id]->setDirection(false);

				for(int i=0; i<nodeEnemyLen; i++)
					pNodeEnemy[i]->setDirection(true);
				break;
			}
		case BPOS_BACKATTACK_ENEMY:
			{
				for(int id=0; id<nodeHeroLen; id++) {
					point = battler.getDefPos(id);
					point.x = 320.0f - point.x;
					battler.setDefPos(id, point);
				}

				for(int i=0; i<nodeEnemyLen; i++) {
					int id = i + SCR_BATTLE_MAX_HEROS;
					point = battler.getDefPos(id);
					point.x = 320.0f - point.x;
					battler.setDefPos(id, point);
					pNodeEnemy[i]->setDirection(false);
				}
				break;
			}
		case BPOS_BACKATTACK_HERO:
			{
				for(int i=0; i<nodeEnemyLen; i++) {
					int id = i+SCR_BATTLE_MAX_HEROS;
					pNodeEnemy[i]->setDirection(false);
				}
				break;
			}
		case BPOS_SIDEATTACK_ENEMY:
			{
				for(int id=0; id<nodeHeroLen; id++) {
					point = battler.getDefPos(id);
					point.x = 320.0f/2.0f;
					battler.setDefPos(id, point);
					pNodeHero[id]->setDirection(id%2 == 0);
				}
				for(int i=0; i<nodeEnemyLen; i++) {
					int id = i + SCR_BATTLE_MAX_HEROS;
					if (i%2 == 0) {
						point = battler.getDefPos(id);
						point.x = 320.0f - point.x;
						battler.setDefPos(id, point);
						pNodeEnemy[i]->setDirection(false);
					}
				}
				break;
			}
		case BPOS_SIDEATTACK_HERO:
			{
				for(int id=0; id<nodeHeroLen; id++) {
					point = battler.getDefPos(id);
					if (id%2 == 0) {
						point = battler.getDefPos(id);
						point.x = 320.0f - point.x;
						battler.setDefPos(id, point);
						pNodeHero[id]->setDirection(true);
					}
				}
				for(int i=0; i<nodeEnemyLen; i++) {
					int id = i + SCR_BATTLE_MAX_HEROS;
					point.x = 320.0f/2.0f;
					battler.setDefPos(id, point);
					pNodeEnemy[i]->setDirection(i%2 == 0);
				}
				break;
			}
		}
		for(int id=0; id<nodeHeroLen; id++) {
			point = battler.getDefPos(id);
			pNodeHero[id]->setPosition(point);
		}
		for(int i=0; i<nodeEnemyLen; i++) {
			int id = i + SCR_BATTLE_MAX_HEROS;
			point = battler.getDefPos(id);
			pNodeEnemy[i]->setPosition(point);
		}
	}
}

void ScrBattle3::updatePositionBattleStart() {
	if (isBatAutoPos) {
		switch(batPos) {
		case BPOS_NORMAL:
			{
				for(int id=0; id<nodeHeroLen; id++)
					pNodeHero[id]->setDirection(false);

				for(int i=0; i<nodeEnemyLen; i++)
					pNodeEnemy[i]->setDirection(true);
				break;
			}
		case BPOS_BACKATTACK_ENEMY:
			{
				for(int id=0; id<nodeHeroLen; id++)
					pNodeHero[id]->setDirection(true);
				break;
			}
		case BPOS_BACKATTACK_HERO:
			{
				for(int i=0; i<nodeEnemyLen; i++)
					pNodeEnemy[i]->setDirection(true);
				break;
			}
		case BPOS_SIDEATTACK_ENEMY:
			{
				for(int id=0; id<nodeHeroLen; id++) {
					pNodeHero[id]->setDirection(id%2 == 0);
				}
				for(int i=0; i<nodeEnemyLen; i++) {
					pNodeEnemy[i]->setDirection(i%2 != 0);
				}
				break;
			}
		case BPOS_SIDEATTACK_HERO:
			{
				for(int id=0; id<nodeHeroLen; id++) {
					pNodeHero[id]->setDirection(id%2 == 0);
				}
				for(int i=0; i<nodeEnemyLen; i++) {
					pNodeEnemy[i]->setDirection(i%2 == 0);
				}
				break;
			}
		}
	}
}

void ScrBattle3::updatePositionAction(int actID, int tgtID) {
	//if all target, not working
	if (tgtID < 0 || tgtID >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES)
		return;

	CCPoint actPoint = battler.getDefPos(actID);
	CCPoint tgtPoint = battler.getDefPos(tgtID);

	bool is_right = (actPoint.x < tgtPoint.x);

	if (battler.isHero(actID))
		pNodeHero[actID]->setDirection(is_right);
	else
		pNodeEnemy[actID-SCR_BATTLE_MAX_HEROS]->setDirection(is_right);
}

bool ScrBattle3::isRightOnAction(int actID, int tgtID) {
	if (tgtID < 0 || tgtID >= SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES) {
		if (battler.isHero(actID))
			return pNodeHero[actID]->getDirection();
		else
			return pNodeEnemy[actID-SCR_BATTLE_MAX_HEROS]->getDirection();
	}

	CCPoint actPoint = battler.getDefPos(actID);
	CCPoint tgtPoint = battler.getDefPos(tgtID);

	return actPoint.x < tgtPoint.x;
}