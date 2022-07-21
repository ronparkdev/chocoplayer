#include "ScrMap.h"

void ScrMap::initRide() {

}

void ScrMap::updateRide() {
	if (getRideType() == 0)
		return;

	LsdObjectCommon *objHero = &(pGM->lsd.LocHero);
	LsdObjectCommon *obj;

	switch(getRideType()) {
		case 1:obj = &(pGM->lsd.LocRide[0]);break;
		case 2:obj = &(pGM->lsd.LocRide[1]);break;
		case 3:obj = &(pGM->lsd.LocRide[2]);break;
	}

	objHero->posX = obj->posX;
	objHero->posY = obj->posY;
	objHero->direction = obj->direction;
	objHero->move_frame_left = 0;
}

int ScrMap::getRideType() {
	return pGM->lsd.LocHero.ride_type;
}

void ScrMap::doRide() {
	LsdObjectCommon *obj;

	switch(getRideType()) {
		case 0:obj = &(pGM->lsd.LocHero);break;
		case 1:obj = &(pGM->lsd.LocRide[0]);break;
		case 2:obj = &(pGM->lsd.LocRide[1]);break;
		case 3:obj = &(pGM->lsd.LocRide[2]);break;
	}

	ArrayList<LsdMapEvent> &events = pGM->lsd.Map.events;
	int direction = obj->direction;
	int posX = obj->posX; 
	int posY = obj->posY; 

	int posXd = obj->posX + ObjectDirectX[direction]; 
	int posYd = obj->posY + ObjectDirectY[direction];
	bool posdMoveable = chkPos(posXd, posYd);

	LsdObjectCommon *objtgt;

	if (getRideType() == 0) {
		//Small/Big Boat
		for(int i=0; i<3; i++) {
			objtgt = &(pGM->lsd.LocRide[i]);

			if (curMapId != objtgt->map_id)
				continue;

			if ((i < 2 && objtgt->posX == posXd && objtgt->posY == posYd && posdMoveable) || 
				(i == 2 && objtgt->posX == posX && objtgt->posY == posY)) {
					setRideType(i+1);
					break;
			}
		}
	} else {
		if (getRideType() != 3) {
			if (chkRideLeaveable(posXd, posYd) && posdMoveable)
				doRideLeave(posXd, posYd);
		} else {
			if (chkRideLeaveable(posX, posY))
				doRideLeave(posX, posY);
		}
	}
}

void ScrMap::doRideLeave(int toX, int toY) {

	pGM->lsd.LocHero.ride_type = 0;
	pGM->lsd.LocHero.posX = toX;
	pGM->lsd.LocHero.posY = toY;
}

void ScrMap::setRideType(int val) {
	//I dont know why, but init speed when ride.
	//Why?..
	
	pGM->lsd.LocHero.ride_type = val;

	//Small Boat
	pGM->lsd.LocRide[0].move_speed = 4;
	//Big Boat
	pGM->lsd.LocRide[1].move_speed = 4;
	//Flyer
	pGM->lsd.LocRide[2].move_speed = 4;

	pGM->lsd.LocRide[0].draw_priority = 0;
}


bool ScrMap::chkRideLeaveable( int toX, int toY )
{
	LsdObjectCommon *obj;

	switch(getRideType()) {
		case 1:obj = &(pGM->lsd.LocRide[0]);break;
		case 2:obj = &(pGM->lsd.LocRide[1]);break;
		case 3:obj = &(pGM->lsd.LocRide[2]);break;
	}

	if (obj->nowall)
		return true;
	ScrollType scrolltype = (ScrollType)mapData.scrollType;
	if (scrolltype == scroll_none || scrolltype == scroll_horizonal) {
		if (toX<0 || mapWidth <= toX)
			return false;
	} else {
		while(toX<0)toX+=mapWidth;
		while(toX>=mapWidth)toX-=mapWidth;
	}
	if (scrolltype == scroll_none || scrolltype == scroll_vertical) {
		if (toY<0 || mapHeight <= toY)
			return false;
	} else {
		while(toY<0)toY+=mapHeight;
		while(toY>=mapHeight)toY-=mapHeight;
	}

	//가려는곳에 이벤트가 있는지 체크
	LsdLocationRide *objride = pGM->lsd.LocRide;
	ArrayList<LmuEvent> &events = mapData.Events;

	for(int i=0; i<3; i++) {
		if (getRideType()-1 != i && objride[i].map_id == curMapId && !objride[i].nowall &&
			objride[i].posX == toX && objride[i].posY == toY)
			return false;
	}

	for(int i=0; i<events.GetLength(); i++) {
		LsdObjectCommon &iobj = pGM->lsd.Map.events[i];
		int pageID = ScrMapPublic::getEventPage(pGM, events[i]);
		if (pageID < 0 || !iobj.isUsed)
			continue;

		LmuEventPage &page = events[i].Page[pageID];
		if (iobj.map_id == curMapId && iobj.posX == toX && iobj.posY == toY) {
			if ((page.draw_priority == drawpri_mid || page.draw_dont_over) && !obj->nowall) {
				return false;
			}
		}
	}

	if (getRideType() > 0) {
		int tID = getTerrainID(toX, toY);

		if (tID == 0)
			return false;

		LdbTerrain &terr = pGM->ldb.dTerr[tID-1];

		if (getRideType() == 3 && !terr.airship_land)
			return false;
	}

	int curPassiveF		= getChipsetFlag(toX, toY);

	if ((curPassiveF&15) == 0)
		return false;

	return true;
}
