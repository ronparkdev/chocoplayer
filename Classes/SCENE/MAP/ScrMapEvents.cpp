#include "ScrMap.h"

#include <cmath>

void ScrMap::createMapEvents(bool init) {
	LsdMain &lsd			= pGM->lsd;
	LsdMap &lsd_map			= pGM->lsd.Map;
	LmuMain &lmu_map		= mapData;
	LmtMapInfo &lmt_map		= pGM->lmt.MInfo[curMapId-1];
	ArrayList<LsdMapEvent> &lsd_events	= lsd_map.events;
	ArrayList<LmuEvent> &lmu_events		= mapData.Events;
	int eventlen			= mapData.Events.GetLength();

	if (init) {
		lsd_map.init();

		lsd_map.bg_name			= lmt_map.background_name;

		lsd_map.bg_hscroll		= lmu_map.bg_hscroll;
		lsd_map.bg_hscrollauto	= lmu_map.bg_hscrollauto;
		lsd_map.bg_hscrollauto_speed	= lmu_map.bg_hscrollauto_speed;

		lsd_map.bg_vscroll		= lmu_map.bg_vscroll;
		lsd_map.bg_vscrollauto	= lmu_map.bg_vscrollauto;
		lsd_map.bg_vscrollauto_speed	= lmu_map.bg_vscrollauto_speed;

		lsd_map.encounter_rate	= lmt_map.encounter_steps;
		 
		lsd_events.SetLength(eventlen);

		for(int i=0; i<3; i++) {
			lsd.LocRide[i].isUsed = (lsd.LocRide[i].map_id == curMapId);
		}
	}

	for(int i=0; i<eventlen; i++)
		initMapEvent(init, i);


	mapEvtSprs.SetLength(eventlen+4);

	NeedMirror mirror_type;
	
	switch(mapData.scrollType) {
		case scroll_none:
			mirror_type = NeedMirror_None;
			break;
		case scroll_horizonal:
			mirror_type = NeedMirror_Horizonal;
			break;
		case scroll_vertical:
			mirror_type = NeedMirror_Vertical;
			break;
		case scroll_both:
			mirror_type = NeedMirror_Both;
			break;
	}

	for(int i=0; i<eventlen+4; i++)
		mapEvtSprs[i].init(pFg, i, mirror_type, mapWidth, mapHeight);

	initMapEventsChipset();

	LsdObjectCommon *obj;
	for(int i=0; i<eventlen+4; i++) {
		if (i<4) {
			if (i==0)
				obj = &(pGM->lsd.LocHero);
			else
				obj = &(pGM->lsd.LocRide[i-1]);
		} else {
			obj = &(lsd_events[i-4]);
		}

		if (obj->isUsed)
			mapEvtSprs[i].set(obj->pic_name);
		refreshEventShader(true, i, obj);
	}

	if (init)
		chkHeroPic(true);
}

void ScrMap::destroyMapEvents() {
	mapEvtSprs.SetLength(0);
}

void ScrMap::initMapEventsChipset() {
	LdbChipset &chipset = pGM->ldb.dChip[pGM->lsd.Map.chipset_id - 1];

	TString chipsetName = chipset.ChipsetName;
	chipsetBufId = IBuffer::getInstance()->get(imgChipSet, chipsetName);

	if (chipsetBufId == -1)
		return;

	CCTexture2D *pChipsetTexture = IBuffer::getInstance()->get(imgChipSet, chipsetBufId);

	for(int i=0; i<mapEvtSprs.GetLength(); i++)
		mapEvtSprs[i].init(pChipsetTexture);
}

inline void ScrMap::initMapEvent(bool init, int evtID) {

	LsdMapEvent *obj = &(pGM->lsd.Map.events[evtID]);
	LmuEvent &event	 = mapData.Events[evtID];

	if (init) {
		obj->init();
		obj->isUsed = true;
	}

	int pageID = ScrMapPublic::getEventPage(pGM, event);

	if (init) {

		obj->setIndex(event.getIndex());

		obj->map_id = curMapId;
		obj->posX = event.posX;
		obj->posY = event.posY;

		if (obj->evtpage < 0)
			return;

		LmuEventPage &page = mapData.Events[evtID].Page[pageID];

		obj->pic_index			= page.pic_id;
		obj->pic_name			= page.pic_name;

		obj->direction_moving   = obj->direction = page.direction + 1;
		obj->ani_walk			= page.ani_walk;
		obj->transparent		= (page.semi_trans?CHARSET_SEMITRANS:0);
		obj->move_freq			= page.move_freq;
		obj->draw_priority		= page.draw_priority;
		obj->ani_type			= page.ani_type;
		obj->move_speed			= page.move_speed;
		obj->move_route			= page.move_route;
		obj->wait_cur_frame		= obj->wait_total_frame	= DELAY_WAIT[1];
	}
}

bool ScrMap::chkPos(int &x, int &y) {
	ScrollType scrolltype = (ScrollType)mapData.scrollType;
	if (scrolltype == scroll_none || scrolltype == scroll_horizonal) {
		if (x<0 || mapWidth <= x)
			return false;
	} else {
		while(x<0)x+=mapWidth;
		while(x>=mapWidth)x-=mapWidth;
	}
	if (scrolltype == scroll_none || scrolltype == scroll_vertical) {
		if (y<0 || mapHeight <= y)
			return false;
	} else {
		while(y<0)y+=mapHeight;
		while(y>=mapHeight)y-=mapHeight;
	}
	return true;
}

void ScrMap::chkHeroPic(bool init)
{
	chkHeroPic(pGM, init);
}

void ScrMap::chkHeroPic( GameMain *main, bool init )
{
	LsdObjectCommon *obj = &(main->lsd.LocHero);

	MgrState mstate(main);
	if (mstate.getPartyLen() == 0) {
		obj->pic_name = "";
	} else {
		int hID = main->lsd.State.party[0];

		LsdCharacter &lsdCh = main->lsd.Character[hID-1];

		obj->pic_name	= lsdCh.pic_name;
		obj->pic_index	= lsdCh.pic_id;
		if (init)
			obj->transparent= lsdCh.pic_default_trans;
	}

	obj->need_picname_check = true;
}

