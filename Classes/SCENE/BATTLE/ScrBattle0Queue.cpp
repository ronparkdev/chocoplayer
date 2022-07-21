#include "ScrBattle0.h"

#include "../../COMMON/ComMath.h"

#define SCR_BATTLE0_MIN_MESSAGE_FRAME 60

void ScrBattle0::addQueueMessage(TString str, bool wait) {
	ScrBattle0ActionQueue node;
	node.type = B0AT_MESSAGE;
	node.message = str;
	node.total_frame = 0;

	actQueue.Add(node);

	if (wait) {
		node.type = B0AT_WAIT;
		node.total_frame = SCR_BATTLE0_MIN_MESSAGE_FRAME;
		actQueue.Add(node);
	}
}

void ScrBattle0::addQueueMessageClear() {
	ScrBattle0ActionQueue node;
	node.type = B0AT_MESSAGE_CLEAR;
	node.total_frame = 0;
	actQueue.Add(node);
}

void ScrBattle0::addQueueSound( const GlbSound &sound )
{
	ScrBattle0ActionQueue node;
	node.type = B0AT_SOUND;
	node.sound = sound;
	node.total_frame = 0;
	actQueue.Add(node);
}

void ScrBattle0::addQueueAnimation(int tgtID, int usingID)
{
	ScrBattle0ActionQueue node;
	node.type = B0AT_ANIMATING;
	node.target_id = tgtID;
	node.using_id = usingID;
	node.total_frame = GrpAniOne::getAnimationFrame(pGM, usingID);
	actQueue.Add(node);
}

void ScrBattle0::addQueueAction(int actID, int tgtID, ScrBattle0ActionType type, int effVal, int usingID)
{
	ScrBattle0ActionQueue node;
	node.type = B0AT_ACTION;
	node.actor_id = actID;
	node.target_id = tgtID;
	node.using_id = usingID;
	node.effect_val = effVal;
	node.action_type = type;
	node.total_frame = 0;
	actQueue.Add(node);
}

void ScrBattle0::addQueueDamaged(int tgtID)
{
	if (!battler.isAlive(tgtID))
		return;
	ScrBattle0ActionQueue node;
	if (battler.isHero(tgtID)) {
		node.type = B0AT_SCR_SHAKE;
		node.total_frame = 20;
	} else {
		node.type = B0AT_ENEMY_FLASH;
		node.target_id = tgtID;
		node.total_frame = 20;
	}
	actQueue.Add(node);
}

bool ScrBattle0::updateQueue(int framedelta) {
	if (actFrameLeft > 0) {
		actFrameLeft -= framedelta;

		float per = (float)(actCurNode.total_frame - actFrameLeft)/actCurNode.total_frame;

		switch(actCurNode.type) {
		case B0AT_SCR_SHAKE:
			{
				pScreen->setPositionX(sin(per * PI * 3.0f) * 10.0f);
				break;
			}

		case B0AT_ENEMY_FLASH:
			{
				int i = actCurNode.target_id - SCR_BATTLE_MAX_HEROS;

				pNodeEnemy[i]->setAlpha((1.0f - sin(per * PI * 2.0f))/2.0f);
				break;
			}
		}

		if (actFrameLeft <= 0) {
			switch(actCurNode.type) {
			case B0AT_SCR_SHAKE:
				{
					pScreen->setPositionX(0.0f);
					break;
				}

			case B0AT_ENEMY_FLASH:
				{
					int i = actCurNode.target_id - SCR_BATTLE_MAX_HEROS;

					pNodeEnemy[i]->setAlpha(1.0f);
					break;
				}
			}
		}
	}

	if (actFrameLeft <= 0) {
		if (actQueue.GetLength() == 0)
			return false;

		actCurNode = actQueue[0];

		ScrBattle0ActionQueue &node = actCurNode;
		actQueue.Remove(0);

		actFrameLeft = node.total_frame;

		switch(node.type) {
		case B0AT_ACTION:
			{
				doActionEffect(node.actor_id, node.target_id, node.action_type, node.effect_val, node.using_id);
				break;
			}
		case B0AT_ANIMATING:
			{
				if (node.target_id == EFFECT_ALL_ENEMIES) {
					for(int i=0; i<nodeEnemyLen; i++) {
						int id = i + SCR_BATTLE_MAX_HEROS;

						if (!battler.isAlive(id))
							continue;

						ScrBattle0EnemyNode *enemy = pNodeEnemy[i];
						pAnis[i]->set(node.using_id, enemy->getPositionX(), enemy->getPositionY(), enemy->getHeight());
					}
				} else if (battler.isAlive(node.target_id) && !battler.isHero(node.target_id)) {
					int i = node.target_id-SCR_BATTLE_MAX_HEROS;
					ScrBattle0EnemyNode *enemy = pNodeEnemy[i];
					pAnis[i]->set(node.using_id, enemy->getPositionX(), enemy->getPositionY(), enemy->getHeight());
				}
				break;
			}
		case B0AT_MESSAGE:
			{
				if (curMsgRow == 0)
					for(int i=0; i<4; i++)
						pMenuProcRun[i]->removeText();

				pMenuProcRun[curMsgRow]->setText(node.message);
				curMsgRow = (curMsgRow+1)%4;
				break;
			}
		case B0AT_MESSAGE_CLEAR:
			{
				for(int i=0; i<4; i++)
					pMenuProcRun[i]->removeText();
				
				curMsgRow = 0;
				break;
			}
		}
	}
	return true;
}