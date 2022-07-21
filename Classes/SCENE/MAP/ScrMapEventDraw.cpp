#include "ScrMap.h"

#include "../../COMMON/ComMath.h"

#define CHARSET_COL 4
#define CHARSET_ROW 2
#define CHARSET_WIDTH 24
#define CHARSET_HEIGHT 32

void ScrMap::drawEvents(int framedelta)
{
	int id = 0;

	drawEventOne(id++, framedelta, &(pGM->lsd.LocHero));
	for(int i=0; i<3; i++)
		drawEventOne(id++, framedelta, &(pGM->lsd.LocRide[i]));

	ArrayList<LsdMapEvent> &mapevents = pGM->lsd.Map.events;
	int eventlen = MIN(mapevents.GetLength(), mapEvtSprs.GetLength()-4);
	for(int i=0; i<eventlen; i++)
		drawEventOne(id++, framedelta, &(mapevents[i]));
}

void ScrMap::drawEventsAfterChipset()
{
	int id = 0;
	bool force = isEventShaderNeedRefresh;

	refreshEventShader(force, id++, &(pGM->lsd.LocHero));
	for(int i=0; i<3; i++)
		refreshEventShader(force, id++, &(pGM->lsd.LocRide[i]));

	ArrayList<LsdMapEvent> &events = pGM->lsd.Map.events;
	for(int i=0; i<events.GetLength(); i++) {
		refreshEventShader(force, id++, &(events[i]));
	}


	// 	for(int i=0; i<events.GetLength(); i++) {
	// 		LsdMapEvent &evt = events[i];
	// 		TString str;
	// 		str.setText(i, 3);
	// 		str = "[" + str + "]\t"; 
	// 		str = str + "mapID:" + evt.map_id + "\t"; 
	// 		str = str + "isUsed:" + (evt.isUsed?"TRUE":"FALSE") + "\t";
	// 		str = str + "isInvisible:" + (evt.invisible?"TRUE":"FALSE") + "\t";
	// 		str = str + "evtPage:" + evt.evtpage + "\t"; 
	// 		str = str + "posX:" + evt.posX + "\t"; 
	// 		str = str + "posY:" + evt.posY + "\t"; 
	// 
	// 		CCLog(str.getTextUTF8());
	// 	}

	isEventShaderNeedRefresh = false;
}

inline void ScrMap::drawEventOne(int index, long framedelta, LsdObjectCommon *obj) {

	bool isVisible = true;
	float pos_x, pos_y;
	float chk_pos_x, chk_pos_y;
	float per;
	int posZ;
	ScrMapEventNode *pNode = &(mapEvtSprs[index]);

	// 	if (index == 36 + 4)
	// 		CCLog((_ST("") + obj->posY).getTextUTF8());

	if (obj->map_id != curMapId || !obj->isUsed) {
		pNode->unset();
		return;
	}

	if (index < 4) {
		if (index == 0 && getRideType() != 0) {
			pNode->setVisible(false);
			isVisible = false;
		}
	} else {
		int eventid = index - 4;
		LmuEvent &lmuevent = mapData.Events[eventid];
		int pageid = ScrMapPublic::getEventPage(pGM, lmuevent);

		LsdMapEvent *mobj = (LsdMapEvent *)obj;

		if (mobj->evtpage == -2) {
			//for load
			mobj->evtpage = pageid;
		} else if (mobj->evtpage != pageid) {
			if (pageid >= 0) {
				LmuEventPage &page = mapData.Events[eventid].Page[pageid];

				if ((*pNode) != page.pic_name)
					pNode->set(page.pic_name);

				obj->pic_name = page.pic_name;
				obj->pic_index = page.pic_id;

				if (obj->direction == 0)
					obj->direction_moving = obj->direction = page.direction + 1;

				obj->ani_walk			= page.ani_walk;
				obj->transparent		= (page.semi_trans?3:0);
				obj->move_freq			= page.move_freq;
				obj->draw_priority		= page.draw_priority;
				obj->ani_type			= page.ani_type;
				obj->move_speed			= page.move_speed;
				obj->direction			= page.direction + 1;//TODO CHK
				obj->move_route_index	= 0;
				obj->move_route_original_index = 0;
			}

			//CCLOG((_ST("Event") + (eventid+1) + " PageChanged " + ((LsdMapEvent *)obj)->evtpage + "->" + pageid).getTextUTF8());
			mobj->evtpage = pageid;
		}

		if (pageid < 0) {
			pNode->unset();
			isVisible = false;
		}
	}

	chkEventMove(framedelta, index, obj);

	if (isVisible) {
		switch(obj->draw_priority) {
			case 0:posZ = sz_charset_bot;break;
			case 1:posZ = sz_charset_mid;break;
			case 2:posZ = sz_charset_top;break;
		}

		pNode->setZPos(posZ + obj->posY);
	}

	if (obj->move_frame_left>0) {
		per = obj->move_frame_left/300.0f;
		if (!obj->jumping) {
			pos_x = (obj->posX-ObjectDirectX[obj->direction_moving]*per)*TILE_W+TILE_HW;
			pos_y = (obj->posY-ObjectDirectY[obj->direction_moving]*per)*TILE_W;
		} else {
			pos_x = (obj->posX*(1.0f-per)+obj->jumping_from_x*per)*TILE_W+TILE_HW;
			pos_y = (obj->posY*(1.0f-per)+obj->jumping_from_y*per-sin(PI*per))*TILE_W;
		}
	} else {
		pos_x = obj->posX*TILE_W+TILE_HW;
		pos_y = obj->posY*TILE_W;
	}

	if (index == pGM->lsd.LocHero.ride_type) {
		refreshScrPos(pos_x, pos_y, framedelta, isScrNeedInit);
		isScrNeedInit = false;
	}

	obj->screen_pos_x = pos_x-ScreenCommon::game_screen_x;
	obj->screen_pos_y = pos_y-ScreenCommon::game_screen_y + TILE_HW;

	if (isVisible) {
		if (obj->pic_name.isEmpty())
			pos_y += TILE_HW;

		chk_pos_x = pos_x-ScreenCommon::game_screen_x;
		chk_pos_y = pos_y-ScreenCommon::game_screen_y;


		if (chk_pos_x < -CHARSET_WIDTH)
			chk_pos_x += _TILE(mapWidth);

		if (chk_pos_y < -CHARSET_HEIGHT)
			chk_pos_y += _TILE(mapHeight);

		if ((chk_pos_x > _TILE(GAME_SCREEN_TILE_WIDTH) + CHARSET_WIDTH) ||
			(chk_pos_y > _TILE(GAME_SCREEN_TILE_HEIGHT) + CHARSET_HEIGHT))
		{
			pNode->setVisible(false);
			//CCLOG((_ST("out of screen : ") + index).getTextUTF8());
		} else {
			int ani_walk = obj->ani_walk;
			if (ani_walk == 3)ani_walk = 1;

			if (obj->need_picname_check) {
				pNode->set(obj->pic_name);
				obj->need_picname_check = false;
			}

			if (pos_x > _TILE(mapWidth))
				pos_x -= _TILE(mapWidth);

			if (pos_y > _TILE(mapHeight))
				pos_y -= _TILE(mapHeight);


			int direction;

			LsdMapEvent* objevt = (LsdMapEvent*)obj;
			if (index >= 4 && 
				pEvt->getRunningSerialMapID() == index && 
				objevt->direction_event > 0) {
					direction = objevt->direction_event;
			} else {
				direction = obj->direction;
			}

			if (obj->pic_name.isEmpty()) {
				//use chipset
				int fixedID = pGM->lsd.Map.tile_upper[obj->pic_index];
				int tx = 18 + fixedID % 6 + ((fixedID>=48)?6:0);
				int ty = 8 + (fixedID/6) - ((fixedID>=48)?16:0);

				pNode->setTextureRect(_TILE(tx), _TILE(ty), TILE_W, TILE_W);
			} else {
				//use charset
				int pic_x = (((obj->pic_index%CHARSET_COL)*3)+ani_walk)*CHARSET_WIDTH;
				int pic_y = (((obj->pic_index/CHARSET_COL)*4)+direction-1)*CHARSET_HEIGHT;
				pNode->setTextureRect(pic_x, pic_y, CHARSET_WIDTH, CHARSET_HEIGHT);
			}

			pNode->setPos(pos_x + ScreenCommon::game_screen_x_for_event, pos_y);
			pNode->setVisible(!obj->invisible);
			pNode->setTrans(obj->transparent);
		}
	}
}

void ScrMap::refreshEventShader(bool force, int index, LsdObjectCommon *obj) {
	LsdScreen &scr = pGM->lsd.Screen;

	if (obj->flash_continuous != 2) {
		if (obj->flash_frame_left > 0) {
			if (obj->flash_frame_left == 1)
				obj->flash_cur_lev = 0.0f;
			else
				obj->flash_cur_lev -= obj->flash_cur_lev / obj->flash_frame_left;

			obj->flash_frame_left--;
			force = true;
		}
	} else {
		obj->flash_continuous = 0;
		obj->flash_cur_lev = 0.0f;
		if (obj->flash_frame_left > 0) {
			obj->flash_frame_left = 0;
			force = true;
		}
	}

	if (force) {
		float flash_event_per = obj->flash_cur_lev / 31.0f;
		float flash_per = scr.flash_cur_lev / 31.0f;
		mapEvtSprs[index].setColor(
			(scr.scr_cur_r-100.0f)/100.0f + scr.flash_fin_r * flash_per / 31.0f + obj->flash_fin_r * flash_event_per / 31.0f,
			(scr.scr_cur_g-100.0f)/100.0f + scr.flash_fin_g * flash_per / 31.0f + obj->flash_fin_g * flash_event_per / 31.0f,
			(scr.scr_cur_b-100.0f)/100.0f + scr.flash_fin_b * flash_per / 31.0f + obj->flash_fin_b * flash_event_per / 31.0f,
			(scr.scr_cur_gr-100.0f)/100.0f + flash_per + flash_event_per);
	}
}

int ScrMap::getEventFlag(int x, int y, int exid, bool isfrom) {
	ArrayList<LsdMapEvent> &events = pGM->lsd.Map.events;
	ArrayList<LmuEvent> &eventsLmu = mapData.Events;
	LdbChipset &c = pGM->ldb.dChip[mapData.chipSetID-1];
	for(int i=0; i<events.GetLength(); i++)
	{	
		LsdMapEvent &obj = events[i];

		int pageID = ScrMapPublic::getEventPage(pGM, eventsLmu[i]);
		if (pageID < 0 || !obj.isUsed || i == exid-4)//TODO
			continue;

		if (obj.posX == x && obj.posY == y) {
			if (pGM->getVersion() == RPG2000) {
				switch(obj.draw_priority) {
				case drawpri_low: 
				case drawpri_high: return 15;
				case drawpri_mid: return 0;
				}
			} else {
				if (obj.pic_name.isEmpty()) {
					int chipsetID = obj.pic_index+10000;

					if (chipsetID == 10000) {
						return 16;
						//return ((p.DrawPriority != 1 || isfrom)?15:0);
					} else {
						return c.passableDataUpper[getChipsetID(chipsetID)];
					}
				}
			}
		}
	}
	return 16;
}

void ScrMap::refreshScrPos(float cX, float cY, int framedelta, bool init) {

	LsdMap &map = pGM->lsd.Map;
	LsdScreen &scr = pGM->lsd.Screen;
	float curScrX, curScrY, panedScrX, panedScrY, deltaX, deltaY, tX, tY;

	ScrollType scroll_type = (ScrollType)mapData.scrollType;

	//X축 화면좌표 계산시작!

	//curScrX = 원래 있어야할 위치 + 에 PAN된 거리를 더함
	curScrX = cX - _TILE(9) - TILE_HW;

	//curScrX = curScrX에서 맵의 끝을 고려한 위치
	chkScrPosX(scroll_type, curScrX);

	curScrX += scr.pan_moved_x;

	//panedScrX = curScrX에서 PAN해야될 값을 더함
	if (scr.pan_move_x != 0.0f) {
		tX = DELAY_MOVE[scr.pan_move_speed] * framedelta / 300.0f * 16.0f;
		if (scr.pan_move_x > 0.0f) {
			if (scr.pan_move_x < tX)
				tX = scr.pan_move_x;
		} else {
			tX *= -1.0f;
			if (scr.pan_move_x > tX)
				tX = scr.pan_move_x;
		}
		panedScrX = curScrX + tX;
		chkScrPosX(scroll_type, panedScrX);
	}

	//pan_move_x = pan_move_x에서 맵의 끝을 고려한 위치
	chkScrPosX(scroll_type, curScrX);

	if (map.is_pan_lock && !init) {
		if (scr.pan_move_x != 0.0f)
			lastOriginalScrX = scrX += panedScrX-curScrX;
		else
			scrX = curScrX;
	} else {
		if (!init) {
			deltaX = curScrX - scrX;

			if (deltaX > 0) {
				if (curScrX >= lastOriginalScrX)
					lastOriginalScrX += deltaX;
			} else {
				if (curScrX <= lastOriginalScrX)
					lastOriginalScrX += deltaX;
			}
			scrX = curScrX;
		} else {
			lastOriginalScrX = scrX = curScrX;
		}
	}

	//shake를 고려한 화면을 출력
	if (isEffShakeAddX != 0.0f) {
		ScreenCommon::game_screen_x = lastOriginalScrX + isEffShakeAddX;

		// 		if (ScreenCommon::game_screen_x < 0)
		// 			ScreenCommon::game_screen_x += _TILE(m_map_width);

		if (isEffShakeAddX < 0.0f)
			pEffShakeHider->setPositionX(-isEffShakeAddX-16);
		else
			pEffShakeHider->setPositionX(-isEffShakeAddX + 320 + 16);

		pEffShakeHider->setVisible(true);
	} else {
		ScreenCommon::game_screen_x = lastOriginalScrX;
		pEffShakeHider->setVisible(false);
	}

	if (scr.pan_move_x != 0.0f) {
		deltaX = panedScrX - curScrX;
		if (deltaX == 0.0f) {
			scr.pan_move_x -= tX;
			scr.pan_x = scr.pan_moved_x += tX;
		} else {
			curScrX = panedScrX;
			scr.pan_move_x -= deltaX;
			scr.pan_x = scr.pan_moved_x += deltaX;
		}
	}

	ScreenCommon::game_screen_x_for_event = 0;

	if (ScreenCommon::game_screen_x < 0) {
		ScreenCommon::game_screen_x += _TILE(mapWidth);

		if ( mapData.scrollType == scroll_none || 
			mapData.scrollType == scroll_vertical)
			ScreenCommon::game_screen_x_for_event = _TILE(mapWidth);
	}

	if (ScreenCommon::game_screen_x > _TILE(mapWidth))
		ScreenCommon::game_screen_x -= _TILE(mapWidth);
	
	if (abs(lastScrX-ScreenCommon::game_screen_x) > maxScrX/2) {
		if (ScreenCommon::game_screen_x <= (maxScrX/2) && 
			lastScrX > (maxScrX/2)) {

				scr.screen_adjust_x += maxScrX;
		} else if (ScreenCommon::game_screen_x >= (maxScrX/2) && 
			lastScrX < (maxScrX/2)) {

				scr.screen_adjust_x -= maxScrX;
		}
	}

	lastScrX = ScreenCommon::game_screen_x;

	//같은방법으로 Y축계산

	curScrY = cY - _TILE(7);

	chkScrPosY(scroll_type, curScrY);

	curScrY += scr.pan_moved_y;

	if (scr.pan_move_y != 0.0f) {
		tY = DELAY_MOVE[scr.pan_move_speed] * framedelta / 300.0f * 16.0f;
		if (scr.pan_move_y > 0.0f) {
			if (scr.pan_move_y < tY)
				tY = scr.pan_move_y;
		} else {
			tY *= -1.0f;
			if (scr.pan_move_y > tY)
				tY = scr.pan_move_y;
		}
		panedScrY = curScrY + tY;

		chkScrPosY(scroll_type, panedScrY);
	}

	chkScrPosY(scroll_type, curScrY);

	if (map.is_pan_lock && !init) {

		if (scr.pan_move_y != 0.0f)
			ScreenCommon::game_screen_y = scrY += panedScrY-curScrY;
		else
			scrY = curScrY;
	} else {
		if (!init) {
			deltaY = curScrY - scrY;

			if (deltaY > 0) {
				if (curScrY >= ScreenCommon::game_screen_y)
					ScreenCommon::game_screen_y += deltaY;
			} else {
				if (curScrY <= ScreenCommon::game_screen_y)
					ScreenCommon::game_screen_y += deltaY;
			}
			scrY = curScrY;
		} else {
			ScreenCommon::game_screen_y = scrY = curScrY;
		}
	}

	if (scr.pan_move_y != 0.0f) {
		deltaY = panedScrY - curScrY;
		if (deltaY == 0.0f) {
			scr.pan_move_y -= tY;
			scr.pan_y = scr.pan_moved_y += tY;
		} else {
			curScrY = panedScrY;
			scr.pan_move_y -= deltaY;
			scr.pan_y = scr.pan_moved_y += deltaY;
		}
	}


	if (ScreenCommon::game_screen_y < 0)
		ScreenCommon::game_screen_y += _TILE(mapHeight);

	if (ScreenCommon::game_screen_y > _TILE(mapHeight))
		ScreenCommon::game_screen_y -= _TILE(mapHeight);

	if (abs(lastScrY-ScreenCommon::game_screen_y) > maxScrY/2) {
		if (ScreenCommon::game_screen_y <= (maxScrY/2) && 
			lastScrY > (maxScrY/2)) {

				scr.screen_adjust_y += maxScrY;
		} else if (ScreenCommon::game_screen_y >= (maxScrY/2) && 
			lastScrY < (maxScrY/2)) {

				scr.screen_adjust_y -= maxScrY;
		}
	}

	lastScrY = ScreenCommon::game_screen_y;
}

void ScrMap::chkScrPosX( ScrollType scroll_type, float &x )
{
	if (scroll_type == scroll_none || scroll_type == scroll_horizonal) {
		if (x < 0)x = 0;
		if (x+GAME_SCREEN_WIDTH > _TILE(mapWidth))
			x = _TILE(mapWidth) - GAME_SCREEN_WIDTH;
	} else {
		if (x < 0)
			x += _TILE(mapWidth);
	}
}

void ScrMap::chkScrPosY( ScrollType scroll_type, float &y )
{
	if (scroll_type == scroll_none || scroll_type == scroll_vertical) {
		if (y < 0)y = 0;
		if (y+GAME_SCREEN_HEIGHT > _TILE(mapHeight))
			y = _TILE(mapHeight) - GAME_SCREEN_HEIGHT;
	} else {
		if (y < 0)
			y += _TILE(mapHeight);
	}
}

int ScrMap::getTerrainID( int x, int y )
{
	int posid = x + y*mapWidth;

	if (posid < 0 || posid>=mapTilesLen || chipsetId <= 0)
		return 0;

	int cID = getChipsetID(mapData.lowerMap[posid]);
	if (cID > 17)cID = pGM->lsd.Map.tile_lower[cID-18] + 18;

	if (cID < pGM->ldb.dChip[chipsetId-1].TerrainData.GetLength())
		return pGM->ldb.dChip[chipsetId-1].TerrainData[cID];
	else
		return 1;
}
