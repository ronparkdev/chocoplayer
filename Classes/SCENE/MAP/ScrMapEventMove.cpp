#include "ScrMap.h"

void ScrMap::chkEventMove(int framedelta, int index, LsdObjectCommon *obj) {
	LsdObjectCommon *objh = getHeroObject();

	bool last_route_wait = obj->move_route_custom && 
		(obj->move_route.move_commands[obj->move_route_index] == wait_moment);

	if (obj->move_frame_left <= 0) {
		obj->move_frame_left = 0;
		obj->jumping = false;

		switch(obj->ani_type) {
		case oanitype_none:
		case oanitype_dlock_none:

			if (!obj->ani_paused) {
				if (obj->ani_next_left_frame > 0) {
					obj->ani_next_left_frame -= framedelta;
				} else {
					obj->ani_walk |= 0x01;
					obj->ani_next_left_frame = 0;
				}
			} else {
				obj->ani_walk = 1;
			}

			break;

		case oanitype_walking:
		case oanitype_dlock_walking:
			if (!obj->ani_paused) {
				obj->ani_next_left_frame -= framedelta;
				if (obj->ani_next_left_frame <= 0) {
					obj->ani_walk = ((obj->ani_walk+1)%4);
					obj->ani_next_left_frame = DELAY_ANI[obj->move_speed]*2;
				}
			} else {
				obj->ani_walk = 1;
			}

			obj->move_frame_left -= DELAY_MOVE[obj->move_speed] * framedelta;

			break;


		case oanitype_rotate_clock:
			if (!obj->ani_paused) {
				obj->ani_next_left_frame -= framedelta;
				if (obj->ani_next_left_frame <= 0) {
					obj->direction = (obj->direction)%4 + 1;
					obj->ani_next_left_frame = DELAY_ANI[obj->move_speed];
				}
			}

			obj->move_frame_left -= DELAY_MOVE[obj->move_speed] * framedelta;

		default:
			break;
		}


		if (obj->wait_cur_frame < obj->wait_total_frame) {

			//이동하다 잠시 대기중인 시간
			obj->wait_cur_frame += framedelta;
		} else {
			obj->wait_cur_frame = 0;

			//이동을 시작해야됨

			if (obj->move_route_custom) {
				GlbMoveRoute &rt = obj->move_route;
				int move_cmd;

				bool move_end = false;
				int move_cnt = 0;

				while(!move_end) {

					move_cmd = rt.move_commands[obj->move_route_index++];

					if (!doEventMoving(index, move_cmd, obj, &rt, true))
						move_end = true;

					move_cnt++;

					if (rt.move_commands.GetLength() <= obj->move_route_index) {
						if (rt.repeat) {
							obj->move_route_index = 0;

							if (move_cnt == rt.move_commands.GetLength()) {
								//All NonWait Cmds 
								move_end = true;
							}
						} else {
							move_end = true;
							obj->move_route_custom = false;
							obj->move_route_index = obj->move_route_original_index;
						}
					}
				}
			}

			if (!obj->move_route_custom) {

				if (pEvt->isRuningSerial() && (!pGM->lsd.System.msg_moveevent || (index < 4)))
					return;

				if (index<4) {

					if (pGM->lsd.LocHero.ride_type != index || last_route_wait)
						return;

					int pos = pGM->ctl->getKey(rkc_arrow);
					int move_command;
					int face_command = 0;
					switch(pos) {
					case rkca_up:	
						move_command = move_up;		
						face_command = face_up;
						break;
					case rkca_down:	
						move_command = move_down;		
						face_command = face_down;
						break;
					case rkca_right:
						move_command = move_right;		
						face_command = face_right;	
						break;
					case rkca_left:	
						move_command = move_left;		
						face_command = face_left;
						break;
					default:		return;
					}

					doEventMoving(index, face_command, obj);
					doEventMoving(index, move_command, obj);
				} else {
					LmuEvent &event = mapData.Events[index-4];
					int pageid = ScrMapPublic::getEventPage(pGM, event);
					if (pageid == -1)
						return;
					switch((ObjectMoveType)event.Page[pageid].move_type) {
					case movetype_none:
						return;

					case movetype_random:
						doEventMoving(index, move_random, obj);
						break;

					case movetype_vertial:
						{
							if (obj->direction_moving != direct_up && obj->direction_moving != direct_down)
								obj->direction_moving = direct_down;

							int lastpos = obj->posX + obj->posY * mapWidth;
							doEventMoving(index, obj->direction_moving-1, obj);

							if (lastpos == obj->posX + obj->posY * mapWidth)
								obj->direction_moving = (obj->direction_moving == direct_up)?direct_down:direct_up;

							break;
						}

					case movetype_horizonal:
						{
							if (obj->direction_moving != direct_left && obj->direction_moving != direct_right)
								obj->direction_moving = direct_right;

							int lastpos = obj->posX + obj->posY * mapWidth;
							doEventMoving(index, obj->direction_moving-1, obj);

							if (lastpos == obj->posX + obj->posY * mapWidth)
								obj->direction_moving = (obj->direction_moving == direct_left)?direct_right:direct_left;

							break;
						}

					case movetype_to_hero:
						doEventMoveToHeroAI(index, obj);
						break;

					case movetype_away_from_hero:
						if (rand() % 4 == 0)
							doEventMoving(index, move_random, obj);
						else
							doEventMoving(index, move_towards_hero, obj);
						break;

					case movetype_custom:
						{
							int pageid = ScrMapPublic::getEventPage(pGM, mapData.Events[index-4]);

							if (pageid < 0)
								break;

							GlbMoveRoute &rt = mapData.Events[index-4].Page[pageid].move_route;
							int move_cmd;

							if (obj->move_route_index < rt.move_commands.GetLength() || rt.repeat) {
								bool move_end = false;
								int move_cnt = 0;
								while(!move_end) {
									move_cmd = rt.move_commands[obj->move_route_index++];

									if (!doEventMoving(index, move_cmd, obj, &rt, false))
										move_end = true;

									move_cnt++;

									if (rt.move_commands.GetLength() <= obj->move_route_index) {
										if (rt.repeat) {
											obj->move_route_index = 0;
										
											if (move_cnt == rt.move_commands.GetLength()) {
												//All NonWait Cmds 
												move_end = true;
											}
										} else {
											move_end = true;
										}
									}

								}
							}

						}
					}
				}
			}
		}
	}

	if (obj->move_frame_left>0) {
		//이동중

		switch(obj->ani_type) {
		case oanitype_none:
		case oanitype_dlock_none:
		case oanitype_walking:
		case oanitype_dlock_walking:
			if (!obj->ani_paused && !obj->jumping) {//anitype도 체크해야됨
				obj->ani_next_left_frame -= framedelta;
				if (obj->ani_next_left_frame <= 0) {
					obj->ani_walk = ((obj->ani_walk+1)%4);
					obj->ani_next_left_frame = DELAY_ANI[obj->move_speed];
				}
			}

			break;

		case oanitype_rotate_clock:
			if (!obj->ani_paused) {//anitype도 체크해야됨
				obj->ani_next_left_frame -= framedelta;
				if (obj->ani_next_left_frame <= 0) {
					obj->direction = (obj->direction)%4 + 1;
					obj->ani_next_left_frame = DELAY_ANI[obj->move_speed]*2;
				}
			}

		default:
			break;
		}

		obj->move_frame_left -= DELAY_MOVE[obj->move_speed] * framedelta;

		if (obj->move_frame_left <= 0) {
			obj->jumping = false;

			if (obj == objh) {
				doWalk();


				bool isEventRunning = pEvt->isRuningSerial();

				ArrayList<LmuEvent> &events = mapData.Events;
				for(int i=0; i<events.GetLength(); i++) {
					LsdObjectCommon &obji = pGM->lsd.Map.events[i];
					int pageID = ScrMapPublic::getEventPage(pGM, events[i]);
					if (pageID < 0)
						continue;

					LmuEventPage &page = events[i].Page[pageID];
					if (obji.map_id == curMapId && obji.posX == obj->posX && obji.posY == obj->posY) {
						if (page.draw_priority != drawpri_mid && !page.draw_dont_over && !isEventRunning &&
							!objh->move_route_custom) {//Added at 2.2.8
							pEvt->doTouchByHero(i, pageID);
						}
					}
				}
			}
		}
	}
}

void ScrMap::doEventMoveToHeroAI( int index, LsdObjectCommon * obj )
{
	LsdObjectCommon *objhero = getHeroObject();
	int defX = objhero->posX - obj->posX;
	int defY = objhero->posY - obj->posY;
	bool never_go_to_hero = true;
	int direction, movecmd;

	if (obj->move_route_original_index <= 0) {
		for(int i=0; i<10; i++) {
			if (abs(defX) == abs(defY)) {
				if (rand()%2 == 0) {
					if (defX < 0)
						direction = direct_left;
					else
						direction = direct_right;
				} else {
					if (defY < 0)
						direction = direct_up;
					else
						direction = direct_down;
				}
			} else {
				if (abs(defX) < abs(defY)) {
					if (defY < 0)
						direction = direct_up;
					else
						direction = direct_down;
				} else {
					if (defX < 0)
						direction = direct_left;
					else
						direction = direct_right;
				}
			}

			if ((abs(defX) == 1 && abs(defY) == 0) || 
				(abs(defX) == 0 && abs(defY) == 1) ||
				chkEventMoveAble(obj->posX, obj->posY, direction, index, obj, false)) {
					never_go_to_hero = false;
					break;
			}
		}

		if (never_go_to_hero)
			obj->move_route_original_index = rand() % 3 + 1;
	}

	if (obj->move_route_original_index > 0) {
		for(int i=0; i<7; i++) {
			switch(rand()%4) {
			case 0:direction = direct_up;break;
			case 1:direction = direct_down;break;
			case 2:direction = direct_right;break;
			case 3:direction = direct_left;break;
			}

			if (chkEventMoveAble(obj->posX, obj->posY, direction, index, obj, false))
				break;
		}

		obj->move_route_original_index--;
	}

	switch(direction) {
	case direct_up:		movecmd = move_up;break;
	case direct_down:	movecmd = move_down;break;
	case direct_right:	movecmd = move_right;break;
	case direct_left:	movecmd = move_left;break;
	default:
		return;
	}

	if (RANDF() < 0.1f) {
		doEventMoving(index, rand() % 4 + 1, obj);
	} else {
		doEventMoving(index, movecmd, obj);
	}
}

bool ScrMap::doEventMoving(int index, int move_cmd, LsdObjectCommon *obj, GlbMoveRoute *rt, bool is_custom_move) {

	bool isFace = false;
	bool isMove = false;
	int direction = -1;

	switch((MoveCommand)move_cmd) {
	case move_up:	
	case move_right:
	case move_down:
	case move_left:
	case move_upright:
	case move_downright:
	case move_downleft:
	case move_upleft:
	case move_random:
	case move_towards_hero:
	case move_away_from_hero:
	case move_forward:
		isMove = true;
		break;

	default:
		isMove = false;
	}
	switch((MoveCommand)move_cmd) {
	case face_up:	
	case face_right:
	case face_down:
	case face_left:
	case face_random:
	case face_towards_hero:
	case face_away_from_hero:
		isFace = true;
		break;

	default:
		isFace = false;
	}

	switch((MoveCommand)move_cmd) {
	case move_up:	
	case face_up:
		direction = direct_up;
		break;

	case move_right:
	case face_right:
		direction = direct_right;
		break;

	case move_down:
	case face_down:
		direction = direct_down;
		break;

	case move_left:
	case face_left:
		direction = direct_left;
		break;

	case move_upright:
		direction = direct_upright;
		break;

	case move_downright:
		direction = direct_downright;
		break;

	case move_downleft:
		direction = direct_downleft;
		break;

	case move_upleft:
		direction = direct_upleft;
		break;

	case move_random:
	case face_random:
		direction = rand() % 4+1;
		break;
	
	case move_forward:
		direction = obj->direction;
		break;
	
	case move_towards_hero:
		{
			LsdObjectCommon *objhero = getHeroObject();
			int defX = objhero->posX - obj->posX;
			int defY = objhero->posY - obj->posY;
			
			if (abs(defX) == abs(defY) || rand()%2 == 0) {
				if (rand()%2 == 0) {
					if (defX < 0)
						direction = direct_left;
					else
						direction = direct_right;
				} else {
					if (defY < 0)
						direction = direct_up;
					else
						direction = direct_down;
				}
			} else {
				if (abs(defX) < abs(defY)) {
					if (defY < 0)
						direction = direct_up;
					else
						direction = direct_down;
				} else {
					if (defX < 0)
						direction = direct_left;
					else
						direction = direct_right;
				}
			}
			break;
		}

	case move_away_from_hero:
		{
			LsdObjectCommon *objh = getHeroObject();
			int defX = objh->posX - obj->posX;
			int defY = objh->posY - obj->posY;
			if (abs(defX) == abs(defY)) {
				if (rand()%2 == 0) {
					if (defX < 0)
						direction = direct_right;
					else
						direction = direct_left;
				} else {
					if (defY < 0)
						direction = direct_down;				
					else
						direction = direct_up;
				}
			} else {
				int rnd_dir = rand()%3;
				if (abs(defX) < abs(defY)) {
					if (defY < 0) {
						if (rnd_dir == direct_up)
							rnd_dir = direct_left;
					} else {
						if (rnd_dir == direct_down)
							rnd_dir = direct_left;
					}
				} else {
					if (defX < 0) {

					} else {
						if (rnd_dir == direct_right)
							rnd_dir = direct_left;
					}
				}
				direction = rnd_dir;
			}
			break;
		}

	case face_towards_hero:
		{
			LsdObjectCommon *objh = getHeroObject();
			int defX = objh->posX - obj->posX;
			int defY = objh->posY - obj->posY;
			if (abs(defX) < abs(defY)) {
				if (defY < 0)
					direction = direct_up;
				else
					direction = direct_down;
			} else {
				if (defX < 0)
					direction = direct_left;
				else
					direction = direct_right;
			}
			break;
		}

	case face_away_from_hero:
		{
			LsdObjectCommon *objh = getHeroObject();
			int defX = objh->posX - obj->posX;
			int defY = objh->posY - obj->posY;
			if (abs(defX) < abs(defY)) {
				if (defY < 0)
					direction = direct_down;
				else
					direction = direct_up;
			} else {
				if (defX < 0)
					direction = direct_right;
				else
					direction = direct_left;
			}
			break;
		}

	case turn_degree_right:
		direction = (obj->direction)%4+1;
		break;

	case turn_degree_left:
		direction = (obj->direction + 2)%4+1;
		break;

	case turn_degree:
		direction = (obj->direction + 1)%4+1;
		break;

	case turn_degree_random:
		direction = (obj->direction + (rand()<0.5f)?0:2)%4+1;
		break;

	case wait_moment:
		if (obj->jumping)	
			return true;
		break;

	case begin_jump:
		if (!obj->jumping) {
			obj->jumping = true;
			obj->jumping_from_x = obj->posX;
			obj->jumping_from_y = obj->posY;
		}
		return true;

	case end_jump:
		if (obj->nowall || chkEventJumpAble(obj->posX, obj->posY, index)) {
			obj->move_frame_left = 300;
		} else {
			obj->posX = obj->jumping_from_x;
			obj->posY = obj->jumping_from_y;
			if (rt->skippable) {
				obj->move_route_index--;
			}
		}
		return false;

	case lock_facing:
		obj->direction_lock = true;
		return true;

	case unlock_facing:
		obj->direction_lock = false;
		return true;

	case increase_movement_speed:
		obj->move_speed = min(obj->move_speed+1, 6);
		return true;

	case decrease_movement_speed:
		obj->move_speed = max(obj->move_speed-1, 1);
		return true;

	case increase_movement_frequence:
		obj->move_freq = min(obj->move_freq+1, 8);
		return true;

	case decrease_movement_frequence:
		obj->move_freq = max(obj->move_freq-1, 1);
		return true;

	case walk_everywhere_on:
		obj->nowall = true;
		return true;

	case walk_everywhere_off:
		obj->nowall = false;
		return true;

	case stop_animation:
		obj->ani_paused = true;
		return true;

	case start_animation:
		obj->ani_paused = false;
		return true;

	case increase_transp:
		obj->transparent = min(obj->transparent+1,7);
		return true;

	case decrease_transp:
		obj->transparent = max(obj->transparent-1,0);
		return true;

	default:
		{
			int datalen = rt->move_commands.GetLength();
			char *data = new char[datalen];
			for(int i=0; i<datalen; i++)
				data[i] = rt->move_commands[i];

			StructLoader sl;
			sl.open(data, datalen);
			sl.setOffset(obj->move_route_index);

			switch((MoveCommand)move_cmd) {
			case switch_on:
				{
					int switchid;
					sl >> switchid;
					EvtMain::setSw(&(pGM->lsd), switchid, true);
					break;
				}

			case switch_off:
				{
					int switchid;
					sl >> switchid;
					EvtMain::setSw(&(pGM->lsd), switchid, false);
					break;
				}

			case change_graphic:
				{
					sl >> BaseStruct::size;
					sl >> obj->pic_name;
					sl >> obj->pic_index;
					obj->need_picname_check = true;
					break;
				}

			case play_sound_effect:
				GlbSound sound;
				sl >> BaseStruct::size;
				sl >> sound.name;
				sl >> sound.volume;
				sl >> sound.tempo;
				sl >> sound.balance;
				pGM->snd.PlaySound(sound);
				break;
			}

			obj->move_route_index = sl.getOffset();

			//delete []data;//it will be delete by structloader
			return true;
		}
	}

	int freq;
	if (is_custom_move)
		freq = rt->move_freq;
	else
		freq = obj->move_freq;

	if ((MoveCommand)move_cmd == wait_moment) {
		obj->wait_total_frame = DELAY_WAIT_MOMENT[freq];
	} else {
		obj->wait_total_frame = DELAY_WAIT[freq];
	}

	if (direction != -1) {
		if (isMove) {
			//Moving
			int posX = obj->posX;
			int posY = obj->posY;
			posX += ObjectDirectX[direction];
			posY += ObjectDirectY[direction];

			if (!chkPos(posX, posY))
				return false;
			
			if (!obj->jumping) {
				if (chkEventMoveAble(obj->posX, obj->posY, direction, index, obj, true)) {
					obj->move_frame_left = 300;
					obj->posX = posX;
					obj->posY = posY;
				} else {
					if (rt != NULL && !rt->skippable)
						obj->move_route_index--;
					return false;
				}
			} else {
				obj->posX = posX;
				obj->posY = posY;
				return true;
			}

		} else if (isFace) {
			//do nothing..
		} else if (move_cmd == wait_moment) {
			//do nothing..
		} else {
			obj->wait_total_frame = 0;	
		}

		obj->direction_moving = direction;

		if ((isFace || isMove) && index > 4) {
			LsdMapEvent *mapobj = (LsdMapEvent *)obj;
			mapobj->direction_event = 0;
		}

		if ((!isFixedDirection(obj) || isFace) && 
			obj->ani_type != oanitype_rotate_clock) {
				switch(direction) {
				case direct_upright:
				case direct_downright:
					direction = direct_right;
					break;

				case direct_downleft:
				case direct_upleft:
					direction = direct_left;
					break;
				}
				obj->direction = direction;
		}
	}

	return false;
}

TString ScrMap::getStringFromRoute( ArrayList<int> &cmds, int &index )
{
	int size = cmds[index++];
	int bufsize = size * 4 + 1;
	int tsize = 0;
	int isize = 0;

	if (size < 0)
		return "";

	char *buf = new char[bufsize];
	char *pbuf = buf;

	while(tsize < size) {
		tsize += 4;
		isize++;
	}

	buf[bufsize-1] = '\0';

	for(int i=0; i<isize && index < cmds.GetLength(); i++, pbuf += sizeof(int))
		*((int *)pbuf) = cmds[index++];

	return TString(buf);
}

bool ScrMap::isFixedDirection( LsdObjectCommon* obj )
{
	if (obj->direction_lock)
		return true;

	switch(obj->ani_type) {
	case oanitype_lock:
	case oanitype_dlock_none:
	case oanitype_dlock_walking:
	case oanitype_rotate_clock:
		return true;

	default:
		return false;
	}
}
