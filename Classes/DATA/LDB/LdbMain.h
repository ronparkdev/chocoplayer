#ifndef __R2K_DATA__LDB_MAIN__
#define __R2K_DATA__LDB_MAIN__

#include <iostream>
#include "../BaseStruct.h"

#include "LdbAni.h"
#include "LdbAttribute.h"
#include "LdbCharacter.h"
#include "LdbChipset.h"
#include "LdbCommonEvent.h"
#include "LdbCondition.h"
#include "LdbEnemy.h"
#include "LdbEnemyGroup.h"
#include "LdbItem.h"
#include "LdbJob.h"
#include "LdbSkill.h"
#include "LdbSystem.h"
#include "LdbTerrain.h"
#include "LdbVoca.h"
#include "LdbBattleCmd.h"
#include "LdbBattlerAni.h"
#include "LdbSwitch.h"
#include "LdbVariable.h"

#define LDB_HEADER "LcfDataBase"

class LdbMain : public BaseStruct
{
public: 
	TString header;

public:
	ArrayList<LdbCharacter>		dChar;
	ArrayList<LdbSkill>			dSkill;
	ArrayList<LdbItem>			dItem;
	ArrayList<LdbEnemy>			dEnemy;
	ArrayList<LdbEnemyGroup>	dEnemys;
	ArrayList<LdbTerrain>		dTerr;
	ArrayList<LdbAttribute>		dAttr;
	ArrayList<LdbCondition>		dCond;
	ArrayList<LdbAni>			dAnim;
	ArrayList<LdbChipset>		dChip;
	LdbVoca						dVoca;
	LdbSystem					dSys;
	ArrayList<LdbSwitch>		dSw;
	ArrayList<LdbVariable>		dVar;
	ArrayList<LdbCommonEvent>	dComEvt;
	LdbBattleCmd				dBatCmd;
	ArrayList<LdbJob>			dJob;
	ArrayList<LdbBattlerAni>	dBatrAnim;

public:
	void init() {
		header = LDB_HEADER;

		dChar	.RemoveAll();
		dSkill	.RemoveAll();
		dItem	.RemoveAll();
		dEnemy	.RemoveAll();
		dEnemys	.RemoveAll();
		dTerr	.RemoveAll();
		dAttr	.RemoveAll();
		dCond	.RemoveAll();
		dAnim	.RemoveAll();
		dChip	.RemoveAll();
		dSw		.RemoveAll();
		dVar	.RemoveAll();
		dComEvt	.RemoveAll();
		dJob	.RemoveAll();
		dBatrAnim.RemoveAll();
	}

	void serialize(StructLoader &sl) {
		init();
		BaseStruct::serialize(sl, header);
		if (header == LDB_HEADER) {
			BaseStruct::serialize(sl, 0x0B, dChar	);
			BaseStruct::serialize(sl, 0x0C, dSkill	);
			BaseStruct::serialize(sl, 0x0D, dItem	);
			BaseStruct::serialize(sl, 0x0E, dEnemy	);	
			BaseStruct::serialize(sl, 0x0F, dEnemys	);
			BaseStruct::serialize(sl, 0x10, dTerr	);
			BaseStruct::serialize(sl, 0x11, dAttr	);
			BaseStruct::serialize(sl, 0x12, dCond	);
			BaseStruct::serialize(sl, 0x13, dAnim	);
			BaseStruct::serialize(sl, 0x14, dChip	);
			BaseStruct::serialize (sl, 0x15, &dVoca	);
			BaseStruct::serialize (sl, 0x16, &dSys	);
			BaseStruct::serialize(sl, 0x17, dSw		);
			BaseStruct::serialize(sl, 0x18, dVar		);
			BaseStruct::serialize(sl, 0x19, dComEvt	);
			BaseStruct::serializeEmpty(sl, 0x1A);
			BaseStruct::serializeEmpty(sl, 0x1B);
			BaseStruct::serializeEmpty(sl, 0x1C);
			BaseStruct::serialize (sl, 0x1D, &dBatCmd	);
			BaseStruct::serialize(sl, 0x1E, dJob		);
			BaseStruct::serialize(sl, 0x20, dBatrAnim);
		}
	}
};

#endif
