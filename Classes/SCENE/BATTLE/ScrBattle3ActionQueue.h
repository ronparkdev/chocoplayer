#ifndef __R2K_SCENE_BATTLE_FOR_2003_ACTION_QUEUE_NODE__
#define __R2K_SCENE_BATTLE_FOR_2003_ACTION_QUEUE_NODE__

enum ScrBattle3ActionType{
	B3AT_BATTLE_START,
	B3AT_ACT_TURN_START,
	B3AT_ACT_TURN_END,
	B3AT_WAIT,

	B3AT_CHAR_ANIMATING,
	B3AT_CHAR_MOVING_AND_ANIMATING,
	B3AT_CHAR_JUMPING,

	B3AT_WEAP_MOVING,
	B3AT_WEAP_UPPER_CURVED_MOVING,
	B3AT_WEAP_LOWER_CURVED_MOVING,
	
	B3AT_ANIMATING,

	B3AT_ACT_ATTACK,
	B3AT_ACT_SKILL,
	B3AT_ACT_ESCAPE,

	B3AT_ACT_ITEM,
	B3AT_ACT_SELFDEST,
	B3AT_ACT_CHANGE,

	B3AT_DIRECTING
};

class ScrBattle3ActionQueue{
public:
	int actor_id;
	int target_id;
	int using_id;
	int type_id;

	ScrBattle3ActionType type;

	CCPoint move_from;
	CCPoint move_to;

	int total_frame;
};

#endif