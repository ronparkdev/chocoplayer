#ifndef __R2K_DATA__LSD_MAIN__
#define __R2K_DATA__LSD_MAIN__

#include <iostream>
#include "../BaseStruct.h"
#include "../SysTime.h"
#include "../LDB/LdbMain.h"

#include "LsdPreview.h"
#include "LsdSystem.h"
#include "LsdScreen.h"
#include "LsdPicture.h"
#include "LsdCharacter.h"
#include "LsdState.h"
#include "LsdTeleport.h"
#include "LsdMap.h"
#include "LsdLocationHero.h"
#include "LsdLocationRide.h"
#include "LsdCommonEvent.h"

#define LSD_HEADER "LcfSaveData"

class LsdMain : public BaseStruct
{
public: 
	TString header;

public:
	LdbMain * m_link;

	LsdPreview				Preview;
	LsdSystem				System;
	LsdScreen				Screen;
	ArrayList<LsdPicture>	Picture;
	LsdLocationHero			LocHero;
	LsdLocationRide			LocRide[3];
	ArrayList<LsdCharacter>	Character;
	LsdState				State;
	ArrayList<LsdTeleport>	Teleport;
	LsdMap					Map;


	int						what70;
	LsdMapEventStack		EventMap;
	ArrayList<LsdCommonEvent>EventCommon;
public:
	void newgame(LdbMain &ldb, LmtMain &lmt) {
		StructLoader sl_init;
		sl_init.setInitMode();
		serialize(sl_init);

		System.newgame(ldb);

		Picture.SetLength(50);

		LocHero.newgame(ldb, lmt);
		for(int i=0; i<3; i++)
			LocRide[i].newgame(i, ldb, lmt);

		Character.SetLength(m_link->dChar.GetLength());
		for(int i=0; i<Character.GetLength(); i++) {
			Character[i].link(&ldb.dChar[i]);
			Character[i].newgame(ldb, ldb.dChar[i]);
		}

		State.newgame(ldb);

		Map.events.RemoveAll();
		EventMap.data.RemoveAll();
		EventCommon.SetLength(m_link->dComEvt.GetLength());
	}

	void serialize(StructLoader &sl) {

		if (sl.getMode() == StructLoaderModeInit) {
			LsdTeleport tel;
			tel.mapid = 0;
			tel.pos_y = 0;
			tel.pos_x = 0;
			tel.use_switch = true;
			tel.switchid = 1;
			tel.setIndex(0);
			if (Teleport.GetLength() == 0) {
				Teleport.Add(tel);
			} else if (Teleport[0].getIndex() != 0) {
				Teleport.Add(0, tel);	
			}
		}

		if (sl.getMode() == StructLoaderModeWrite) {
			LsdPreview &pre = Preview;

			pre.Time = SysTime::getTime();
			pre.Hero1HP = pre.Hero1Level = 0;
			pre.Hero1Name = "";

			for(int i=0; i<4; i++) {
				pre.HeroFaceID[i] = 0;
				pre.HeroFaceName[i] = "";
			}

			ArrayList<short> &party = State.party;
			for(int i=0; i<party.GetLength(); i++) {
				int heroID = party[i];
				if (heroID == 0)
					break;

				LsdCharacter &c = Character[heroID-1];

				if (i == 0) {
					pre.Hero1Name	= c.name;
					pre.Hero1Level	= c.clevel;
					pre.Hero1HP		= c.chp;
				}


				pre.HeroFaceID[i] = c.face_index;
				pre.HeroFaceName[i] = c.face_name;
			}

		}

		BaseStruct::serialize(sl, header, (TString)(LSD_HEADER));
		if (header == LSD_HEADER) {
			BaseStruct::serialize (sl, 0x64, &Preview);
			BaseStruct::serialize (sl, 0x65, &System);
			BaseStruct::serialize (sl, 0x66, &Screen);
			BaseStruct::serialize(sl, 0x67, Picture);

			BaseStruct::serialize (sl, 0x68, &LocHero);
			for(int i=0; i<3; i++)
				BaseStruct::serialize (sl, 0x69 + i, &LocRide[i]);

			BaseStruct::serialize(sl, 0x6C, Character);
			BaseStruct::serialize (sl, 0x6D, &State);
			BaseStruct::serialize(sl, 0x6E, Teleport);
			BaseStruct::serialize (sl, 0x6F, &Map);

			BaseStruct::serialize (sl, 0x70, what70);
			BaseStruct::serialize (sl, 0x71, &EventMap);
			BaseStruct::serialize(sl, 0x72, EventCommon);
		}
	}
	
	void link(LdbMain *ldb) {
		m_link = ldb;

		System.link(&(ldb->dSys));

		Character.SetLength(ldb->dChar.GetLength());
		for(int i=0; i<ldb->dChar.GetLength(); i++) {
			Character[i].link(&(ldb->dChar[i]));
		}
	}
};

#endif
