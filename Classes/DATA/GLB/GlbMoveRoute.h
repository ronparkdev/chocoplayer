#ifndef __R2K_DATA__LMU_MOVE_ROUTE__
#define __R2K_DATA__LMU_MOVE_ROUTE__

#include <iostream>
#include "../BaseStruct.h"

enum MoveCommand{
	move_up,
	move_right,
	move_down,
	move_left,
	move_upright,
	move_downright,
	move_downleft,
	move_upleft,
	move_random,
	move_towards_hero,
	move_away_from_hero,
	move_forward,
	face_up,
	face_right,
	face_down,
	face_left,
	turn_degree_right,
	turn_degree_left,
	turn_degree,
	turn_degree_random,
	face_random,
	face_towards_hero,
	face_away_from_hero,
	wait_moment,
	begin_jump,
	end_jump,
	lock_facing,
	unlock_facing,
	increase_movement_speed,
	decrease_movement_speed,
	increase_movement_frequence,
	decrease_movement_frequence,
	switch_on,
	switch_off,
	change_graphic,
	play_sound_effect,
	walk_everywhere_on,
	walk_everywhere_off,
	stop_animation,
	start_animation,
	increase_transp,
	decrease_transp
};

class GlbMoveRoute : public BaseStruct
{
public:
	/*0B*/int move_commands_count;
	/*0C*/ArrayList<byte> move_commands;
	/*14*/int move_freq;
	/*15*/bool repeat;
	/*16*/bool skippable;

public:
	void init() {
		/*0B*/move_commands_count = 0;
		/*0C*/move_commands.RemoveAll();
		/*14*/int move_freq = 7;//Made by PSM
		/*15*/repeat	= true;
		/*16*/skippable	= false;
	}

	void serialize(StructLoader &sl) {
		if (sl.getMode() == StructLoaderModeRead)
			init();

		BaseStruct::serialize (sl, 0x0B, move_commands_count	, 0);
		//BaseStruct::Serialize (sl, 0x0C, move_commands);
		if (sl.getMode() == StructLoaderModeInit) {

		} else if (sl.getMode() == StructLoaderModeRead) {
			if (sl.isNextCode(0x0C)) {
				sl.intoCode();

				int i, size, poslimit;
				byte val;
				sl >> size;
				poslimit = sl.getOffset() + size;
				move_commands.SetLength(size);
				for(i=0; i<size && sl.getOffset() < poslimit; i++) {
					sl >> val;
					move_commands[i] = val;
				}
			}
		} else {
			sl << 0x0C;
			int startpos = sl.getOffset();
			for(int i=0; i<move_commands.GetLength(); i++)
				sl << move_commands[i];
			sl.writeStructSize(startpos);
		}

		BaseStruct::serialize (sl, 0x14, move_freq			,7);
		BaseStruct::serialize (sl, 0x15, repeat				,true);
		BaseStruct::serialize (sl, 0x16, skippable			,false);

		sl.finishStruct();
	}
};

#endif
