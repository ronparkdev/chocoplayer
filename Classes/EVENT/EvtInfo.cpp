#include "EvtMain.h"

LsdCharacter* EvtMain::getCharacter( int index )
{
	ArrayList<LsdCharacter> &character = pGM->lsd.Character;
	if (index < 1 || index > character.GetLength())
		return NULL;
	else
		return &(character[index-1]);
}

LsdObjectCommon* EvtMain::getEvent(int my_eventid, int finding_eventid)
{
	int id;
	ArrayList<LsdMapEvent> &map_events = pGM->lsd.Map.events;
	
	if (finding_eventid < 0)
		return NULL;
	
	if (finding_eventid < 10000) {
		if ((id = mapEventRealId[finding_eventid]) == -1)
			return NULL;
		else
			return &(map_events[id]);
	} else {
		switch(finding_eventid) {
		case 10001:return &(pGM->lsd.LocHero);
		case 10002:return &(pGM->lsd.LocRide[0]);
		case 10003:return &(pGM->lsd.LocRide[1]);
		case 10004:return &(pGM->lsd.LocRide[2]);
		case 10005:
			{
				if (my_eventid < 0 || (id = mapEventRealId[my_eventid]) == -1)
					return NULL;
				else 
					return &(map_events[id]);
			}

		default:
			for(int i=0; i<map_events.GetLength(); i++) {
				if (pMapData->Events[i].getIndex() == finding_eventid)
					return &(map_events[i]);
			}

			return NULL;
		}
	}
}

LsdObjectCommon* EvtMain::getEvent( LsdMain *lsd, int finding_eventid )
{
	ArrayList<LsdMapEvent> &map_events = lsd->Map.events;
	switch(finding_eventid) {
	case 10001:return &(lsd->LocHero);
	case 10002:return &(lsd->LocRide[0]);
	case 10003:return &(lsd->LocRide[1]);
	case 10004:return &(lsd->LocRide[2]);
	case 10005:return NULL;
	default:
		for(int i=0; i<map_events.GetLength(); i++) {
			if (map_events[i].getIndex() == finding_eventid)
				return &(map_events[i]);
		}

		return NULL;
	}
}

void EvtMain::setSw( LsdMain *main, int index, bool val )
{
	ArrayList<bool> &sws = main->System.switches;
	if (index < 1 || index > sws.GetLength())
		return;
	else
		sws[index-1] = val;
}

// void EvtMain::addCInfo( int heroID, StatType type, int addVal )
// {
// 
// }
// 
// void EvtMain::setEquip( LsdCharacter* c, int inVal )
// {
// 	
// }
// 
// void EvtMain::unsetEquip( LsdCharacter* c, int unEquipType )
// {
// 
// }
// 
// 
// int EvtMain::getCInfo( int heroID, StatType type )
// {
// 	return 0;
// }
// 
// void EvtMain::setState(int heroID, int stateID, bool val)
// {
// 
// }
// 
// void EvtMain::clearState( int inID )
// {
// 
// }
// 
// bool EvtMain::isCurEquip( GlbEquipment equip, int itemID )
// {
// 	return false;
// }
// 
// bool EvtMain::getState( int heroID, int stateID )
// {
// 	return 0;
// }

int EvtMain::getEventID( int n )
{
	return pMapData->Events[n].getIndex();
}


