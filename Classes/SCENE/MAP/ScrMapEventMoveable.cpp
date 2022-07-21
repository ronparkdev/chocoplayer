#include "ScrMap.h"

bool ScrMap::isMoveableToHero(int evtID, int direction) {

	LdbChipset &c = pGM->ldb.dChip[mapData.chipSetID-1];
	ArrayList<LsdMapEvent> &events = pGM->lsd.Map.events;
	LsdMapEvent &obj = events[evtID];

	if (obj.pic_name.isEmpty()) {
		//using chipset event

		switch(obj.draw_priority) {
		case drawpri_low:
			{
				int chipsetID = obj.pic_index+10000;
				if (chipsetID == 10000)
					break;

				if (chipsetID < c.passableDataUpper.GetLength()) {
					int pass = c.passableDataUpper[chipsetID];
					switch((ObjectDirect)direction) {
					case direct_up:
						return ((pass & 8) != 0);
					case direct_right:
						return ((pass & 4) != 0);
					case direct_down:
						return ((pass & 1) != 0);
					case direct_left:
						return ((pass & 2) != 0);
					default:
						return true;
					}
				}else
					return true;
			}

		case drawpri_mid:
			return false;

		case drawpri_high:
			return true;

		default:
			return false;
		}
	} else {
		switch(obj.draw_priority) {
		case drawpri_mid:
			return false;

		default:
			return true;
		}
	}
	return false;
}

int ScrMap::getChipsetFlag(int posX, int posY) {
	return getChipsetFlag(posX, posY, -1, false);
}

int ScrMap::getChipsetFlag(int x, int y, int exid, bool isfrom)
{
	int lowerflag, upperflag;
	int fCanMove, fCantMove;
	int id = (int) (x + y*mapWidth);

	ArrayList<LsdMapEvent> &events = pGM->lsd.Map.events;
	ArrayList<LmuEvent> &eventsLmu = mapData.Events;
	LdbChipset &c = pGM->ldb.dChip[mapData.chipSetID-1];
	int lowerCID = getChipsetID(mapData.lowerMap[id]);
	int upperCID = getChipsetID(mapData.upperMap[id]);

	if (lowerCID > 162)
		lowerCID = 0;
	if (upperCID > 144)
		upperCID = 0;

	if (lowerCID > 17)lowerCID = pGM->lsd.Map.tile_lower[lowerCID-18] + 18;
	upperCID = pGM->lsd.Map.tile_upper[upperCID];


	if (lowerCID < c.passableDataLower.GetLength())
		lowerflag = c.passableDataLower[lowerCID];
	else
		lowerflag = 15;

	if (upperCID < c.passableDataUpper.GetLength())
		upperflag = c.passableDataUpper[upperCID];
	else
		upperflag = 15;

	//	eventflag = getEventFlag(x,y, exid, isfrom);

	//if this tile is square
	if (lowerflag & 0x20) {
		if ( y > 0 ) {
			//BLABLA passableDataLower NOT USE?
			int uppos = x + (y-1)*mapWidth;
			int uplowerCID = getChipsetID(mapData.lowerMap[uppos]);

			if (lowerCID != uplowerCID)
				lowerflag = 0x1F;

		} else {
			lowerflag = 0x1F;
		}
	}

	fCantMove = 0;

	if (upperCID == 0)
		fCanMove = lowerflag;
	else{
		//Star UpperChipset
		if ((upperflag & 0x10) == 0x10) {
			fCantMove = (upperflag ^ 0x0F) & 0x0F;
			//fCantMove = !upperflag;
			fCanMove = lowerflag;
		}else
			fCanMove = upperflag;
	}

	for(int i=0; i<events.GetLength(); i++)
	{	

		LsdMapEvent &obj = events[i];

		int pageID = ScrMapPublic::getEventPage(pGM, eventsLmu[i]);
		if ( !obj.isUsed || pageID < 0 || i == exid-4 )//TODO
			continue;

		if (obj.posX != x || obj.posY != y || obj.nowall)//Fixed at 2.2.10 for 괴이증후군
			continue;

		if (obj.pic_name.isEmpty() && obj.draw_dontover == drawpri_low) {
			int chipsetID = obj.pic_index+10000;
			if (chipsetID == 10000)
				break;

			if (chipsetID < c.passableDataUpper.GetLength())
				fCanMove |= c.passableDataUpper[getChipsetID(chipsetID)];
			else
				fCanMove |= 15;
		}
	}

	if (pGM->getVersion() == RPG2000 && isfrom)
		return fCanMove & 0xF; //Need Mork Checking
	else
		return (fCanMove & (~fCantMove)) & 0xF;
}

bool ScrMap::chkEventMoveAble(int fromX, int fromY, int direction, int index, LsdObjectCommon *obj, bool eventRun)
{
	bool fromleader = false;
	bool toleader = false;
	int toX = fromX + ObjectDirectX[direction];
	int toY = fromY + ObjectDirectY[direction];

	if (!chkPos(toX, toY))
		return false;

	if (obj->nowall)
		return true;


	//비행기를 타고있는게 아니라면
	if (getRideType() != 3) {
		//가려는곳에 이벤트가 있는지 체크
		LsdObjectCommon *objh = getHeroObject();
		LsdObjectCommon *objride = pGM->lsd.LocRide;
		ArrayList<LmuEvent> &events = mapData.Events;
		if (objh != obj && getRideType() == 0) {
			if (objh->map_id == curMapId && !objh->nowall &&
				objh->posX == toX && objh->posY == toY) {
					//rider
					if (index < 4)
						return false;

					int pageID = ScrMapPublic::getEventPage(pGM, events[index-4]);
					if (pageID < 0)
						return false;

					LmuEventPage &page = events[index-4].Page[pageID];
					if (eventRun && !pEvt->isRuningSerial())
						pEvt->doTouchByEvent(index-4, pageID);

					if (!isMoveableToHero(index - 4, direction)) {
						if (!isFixedDirection(obj))
							obj->direction = direction;

						return false;
					}
			}
		}

		for(int i=0; i<2; i++) {
			if (objride[i].map_id == curMapId && !objride[i].nowall &&
				objride[i].posX == toX && objride[i].posY == toY)
				return false;
		}

		bool isEventRunning =  pEvt->isRuningSerial();

		if (obj == objh || obj->draw_priority == drawpri_mid || obj->draw_dontover) {

			for(int i=0; i<events.GetLength(); i++) {
				LsdObjectCommon &obji = pGM->lsd.Map.events[i];
				int pageID = ScrMapPublic::getEventPage(pGM, events[i]);
				if (pageID < 0 || !obji.isUsed)
					continue;

				LmuEventPage &page = events[i].Page[pageID];
				if (obji.map_id == curMapId && obji.posX == toX && obji.posY == toY && !obji.nowall &&
					(page.draw_priority == drawpri_mid || (obj != objh && page.draw_dont_over)) )
				{

					if (obj == objh && eventRun && !objh->move_route_custom)
					{//Added at 2.2.8
						pEvt->doTouchByHero(i, pageID);
						pEvt->doTouchByEvent(i, pageID);
					}

					return false;
				}
				if (obji.map_id == curMapId && obji.posX == objh->posX && obji.posY == objh->posY) {
					if (!obji.pic_name.isEmpty() && page.draw_priority == drawpri_low) {
						fromleader = true;
					}
				}
			}

		}
	}

	if (getRideType() > 0) {
		int tID = getTerrainID(toX, toY);

		if (tID == 0)
			return false;

		LdbTerrain &terr = pGM->ldb.dTerr[tID-1];

		if (getRideType() == 1) {
			if (terr.boat_pass)
				return true;
		} else if (getRideType() == 2) {
			if (terr.ship_pass)
				return true;
		} else {//getRideType() == 3
			if (terr.airship_pass)
				return true;
		}
	}

	int lastPassiveF	= getChipsetFlag(fromX, fromY, index, true);
	int curPassiveF		= getChipsetFlag(toX, toY);
	int curPassiveXF	= getChipsetFlag(fromX, toY);
	int curPassiveYF	= getChipsetFlag(toX, fromY);

	//what???
	// 	if (d.MoveType == 3 && m_main->getVersion() == RPG2000)
	// 		return (curPassiveF != 0);

	switch((ObjectDirect)direction) {
	case direct_up:
		if (!((lastPassiveF&8) && (curPassiveF&1)))
			return false;
		break;
	case direct_right:
		if (!((lastPassiveF&4) && (curPassiveF&2)))
			return false;
		break;
	case direct_down:
		if (!((lastPassiveF&1) && (curPassiveF&8)))
			return false;
		break;
	case direct_left:
		if (!((lastPassiveF&2) && (curPassiveF&4)))
			return false;
		break;
	case direct_upright:
		if (!(((lastPassiveF&4) && (curPassiveYF&2))	||
			((lastPassiveF&8) && (curPassiveXF&1))	))
			return false;
		break;
	case direct_downright:
		if (!(((lastPassiveF&4) && (curPassiveYF&2))	||
			((lastPassiveF&1) && (curPassiveXF&8))	))
			return false;
		break;
	case direct_downleft:
		if (!(((lastPassiveF&2) && (curPassiveYF&4))	||
			((lastPassiveF&1) && (curPassiveXF&8))	))
			return false;
		break;
	case direct_upleft:
		if (!(((lastPassiveF&2) && (curPassiveYF&4))	||
			((lastPassiveF&8) && (curPassiveXF&1))	))
			return false;
		break;
	}

	return true;
}

bool ScrMap::chkEventJumpAble(int toX, int toY, int index)
{
	int curPassiveF		= getChipsetFlag(toX, toY, index, true);
	return ((curPassiveF & 0x15) != 0);
}