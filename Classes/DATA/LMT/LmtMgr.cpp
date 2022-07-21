#include "LmtMgr.h"

#include "../../COMMON/DebugingCode.h"

void LmtMgr::init( LmtMain *lmt )
{
	m_lmt = lmt;
}

void LmtMgr::doMapChanged(int mapID)
{
	if ( mapID < 0 || m_lmt->MInfo.GetLength() <= mapID) {
		m_curinfo.init();
		CCLOG("lmtGetInfo : out of index : %d", mapID);
		return;
	}

	int n, lmtMapID = m_lmt->MTable[mapID-1];

	if (lmtMapID == -1) {
		m_curinfo.init();
		return;
	}

	ArrayList<LmtMapInfo> &m = m_lmt->MInfo;

	m_curinfo = m[mapID];
	LmtMapInfo &d = m_curinfo;
	
	//check for cycle! why occurred this... -_-? it is tool problem?
	bool *ismarked = new bool[m.GetLength()];
	for(int i=0; i<m.GetLength(); i++)
		ismarked[i] = false;

	bool findbg = false;
	bool findmus = false;
	bool findescape = false;
	bool findsave = false;
	bool findteleport = false;

	n = mapID;
	while(0<= n && n <m.GetLength() && !ismarked[n]) {
		if (!findbg) {
			d.background_name = m[n].background_name;
			d.background_type = m[n].background_type;
			if (m[n].background_type != 0)
				findbg = true;
		}
		if (!findmus) {
			d.music = m[n].music;
			d.music_type = m[n].music_type;
			if (m[n].music_type != 0)
				findmus = true;
		}
		if (!findescape) {
			d.escape = m[n].escape;
			if (m[n].escape != 0)
				findescape = true;
		}
		if (!findsave) {
			d.save = m[n].save;
			if (m[n].save != 0)
				findsave = true;
		}
		if (!findteleport) {
			d.teleport = m[n].teleport;
			if (m[n].teleport != 0)
				findteleport = true;
		}
		ismarked[n] = true;
		if (findbg && findmus && findescape && findsave && findteleport)
			break;

		
		n = m[n].parent_map;
	}

	m_curarea.RemoveAll();

	ScrMapLmtArea a;
	int j=0;
	for(int i = 0; i<m.GetLength(); i++)
		if (m[i].type == 2 && m[i].parent_map == mapID) {
			if (m[i].area.GetLength() > 0)
				a.left = 		m[i].area[0];


			if (m[i].area.GetLength() > 1)
				a.up = 		m[i].area[1];


			if (m[i].area.GetLength() > 2)
				a.right = 		m[i].area[2];


			if (m[i].area.GetLength() > 3)
				a.down = 		m[i].area[3];
			a.id = i;
			
			m_curarea.Add(a);

			j++;
		}

	m_curarea.ChangeMode(ArrayMode);

	delete []ismarked;
}

LmtMapInfo& LmtMgr::getInfo()
{
	return m_curinfo;
}

ArrayList<ScrMapLmtArea>& LmtMgr::getArea()
{
	return m_curarea;
}

TString LmtMgr::getName(int mapID)
{
	if ( mapID < 0 || m_lmt->MInfo.GetLength() <= mapID) {
		CCLOG("lmtGetInfo : out of index : %d", mapID);
		return "";
	}

	return m_lmt->MInfo[mapID].name;
}