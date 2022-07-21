#ifndef __R2K_SCENE_BATTLE_FOR_2000_ACTION_QUEUE_NODE__
#define __R2K_SCENE_BATTLE_FOR_2000_ACTION_QUEUE_NODE__
#include "../../COMMON/DebugingCode.h"

#include "../../DATA/TString.h"
#include "../../DATA/GLB/GlbSound.h"

enum ScrBattle0ActionType{
	B0AT_BATTLE_START,
	B0AT_ACT_TURN_START,
	B0AT_ACT_TURN_END,
	B0AT_WAIT,
	B0AT_MESSAGE,
	B0AT_MESSAGE_CLEAR,
	B0AT_SCR_SHAKE,
	B0AT_ANIMATING,
	B0AT_SOUND,
	B0AT_ACTION,
	B0AT_ENEMY_FLASH,

	B0AT_ACT_ATTACK,
	B0AT_ACT_DOUBLEATTACK,
	B0AT_ACT_SKILL,
	B0AT_ACT_ESCAPE,
	B0AT_ACT_DEFENCE,
	B0AT_ACT_ITEM,
	B0AT_ACT_SELFDEST,
	B0AT_ACT_OBSERVING,
	B0AT_ACT_GATHER_POWER,
	B0AT_ACT_NOTHING,
	B0AT_ACT_TRANSFORM,
};

class ScrBattle0ActionQueue{
public:
	int actor_id;
	int target_id;
	int using_id;
	int effect_val;

	ScrBattle0ActionType type;
	ScrBattle0ActionType action_type;

	int total_frame;
	TString message;
	GlbSound sound;
};

#endif