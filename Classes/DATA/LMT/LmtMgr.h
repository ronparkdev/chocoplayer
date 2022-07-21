#ifndef __R2K_LMT_MANAGER__
#define __R2K_LMT_MANAGER__
#include "LmtMain.h"

class ScrMapLmtArea{
public:
	int id;

	int left;
	int up;
	int right;
	int down;
};

class LmtMgr{
	LmtMain *m_lmt;

	LmtMapInfo m_curinfo;
	ArrayList<ScrMapLmtArea> m_curarea;

public:
	void init(LmtMain *lmt);

	void doMapChanged(int mapID);

	LmtMapInfo& getInfo();
	ArrayList<ScrMapLmtArea>& getArea();
	TString getName(int mapID);
};

#endif 
