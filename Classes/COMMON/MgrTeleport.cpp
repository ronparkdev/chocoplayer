#include "MgrTeleport.h"

#include "MgrState.h"

MgrTeleport::MgrTeleport( GameMain *main )
{
	pGM = main;
}

void MgrTeleport::setEscape( int mapid, int posx, int posy, int swid /*= 0*/ )
{
	ArrayList<LsdTeleport> &ts = pGM->lsd.Teleport;
	LsdTeleport t;
	t.mapid = mapid;
	t.pos_x = posx;
	t.pos_y = posy;
	t.switchid = swid;
	t.use_switch = (swid != 0);
	t.setIndex(0);

	if (ts.GetLength() == 0 || ts[0].getIndex() != 0)
		ts.Add(0, t);
	else
		ts[0] = t;
}

void MgrTeleport::moveEscape()
{
	MgrState mstate(pGM);
	ArrayList<LsdTeleport> &ts = pGM->lsd.Teleport;
	if (ts.GetLength() == 0 || ts[0].getIndex() != 0)
		return;

	LsdTeleport &t = ts[0];

	ScreenLayerManager *slm = ScreenLayerManager::getInstance();

	slm->setSettingScrMap(t.mapid, t.pos_x, t.pos_y, 0, true);
	slm->setScreen(scr_map);

	if (t.use_switch)
		mstate.setSw(t.switchid, true);
}

bool MgrTeleport::isEscapeable()
{
	ArrayList<LsdTeleport> &ts = pGM->lsd.Teleport;
	if (ts.GetLength() == 0 || ts[0].getIndex() != 0)
		return false;

	LsdTeleport &t = ts[0];
	return (pGM->lsd.System.able_escape && t.mapid > 0);
}

void MgrTeleport::addTeleport( int mapid, int posx, int posy, int swid /*= 0*/ )
{
	int i;
	bool haveMap;
	ArrayList<LsdTeleport> &ts = pGM->lsd.Teleport;
	LsdTeleport t;
	t.mapid = mapid;
	t.pos_x = posx;
	t.pos_y = posy;
	t.switchid = swid;
	t.use_switch = (swid != 0);

	if (ts.GetLength() == 0 || ts[0].getIndex() != 0)
		setEscape(0, 0, 0, 0);

	//Check already have map

	haveMap = false;
	for(i=1; i<ts.GetLength(); i++) {
		if (ts[i].mapid == mapid) {
			haveMap = true;
			break;
		} else if (mapid < ts[i].mapid) {
			break;
		}
	}

	if (haveMap) {
		ts[i] = t;
	} else {
		ts.Add(i, t);
	}

	for(i=0; i<ts.GetLength(); i++)
		ts[i].setIndex(0);
}

void MgrTeleport::removeTeleport( int mapid )
{
	int i;
	ArrayList<LsdTeleport> &ts = pGM->lsd.Teleport;
	for(i=1; i<ts.GetLength(); i++)
		if (ts[i].mapid == mapid) {
			ts.Remove(i);
			break;
		}

	for(i=0; i<ts.GetLength(); i++)
		ts[i].setIndex(0);
}

void MgrTeleport::getTeleportList( ArrayList<TString> &list )
{
	ArrayList<LsdTeleport> &ts = pGM->lsd.Teleport;

	list.RemoveAll();

	for(int i=0; i<ts.GetLength(); i++) {
		LsdTeleport &tel = ts[i];
		list.Add(pGM->lmtmgr.getName(tel.mapid));
	}
}

void MgrTeleport::moveTeleport( int id )
{
	MgrState mstate(pGM);
	ArrayList<LsdTeleport> &ts = pGM->lsd.Teleport;
	if (id < 0 || ts.GetLength() <= id)
		return;

	LsdTeleport &t = ts[id-1];

	ScreenLayerManager *slm = ScreenLayerManager::getInstance();

	slm->setSettingScrMap(t.mapid, t.pos_x, t.pos_y, 0, true);
	slm->setScreen(scr_map);

	if (t.use_switch)
		mstate.setSw(t.switchid, true);
}

bool MgrTeleport::isTeleportable()
{
	ArrayList<LsdTeleport> &ts = pGM->lsd.Teleport;
	if (ts.GetLength() == 0)
		return false;

	return (ts[0].getIndex() != 0 || ts.GetLength() > 1);
}

