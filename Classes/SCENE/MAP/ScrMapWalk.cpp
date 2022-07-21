#include "ScrMap.h"

LsdObjectCommon* ScrMap::getHeroObject() {
	LsdObjectCommon *obj;
	switch(getRideType()) {
		case 0:obj = &(pGM->lsd.LocHero);break;
		case 1:obj = &(pGM->lsd.LocRide[0]);break;
		case 2:obj = &(pGM->lsd.LocRide[1]);break;
		case 3:obj = &(pGM->lsd.LocRide[2]);break;
	}
	return obj;
}

void ScrMap::initWalk(bool init) {
	if (init)
		mapWalkCnt = 0;
}

void ScrMap::doWalk() {

	MgrState mstate(pGM);
	MgrCharacter mchar(pGM);

	LsdObjectCommon *obj = getHeroObject();

	int terrID = getTerrainID(obj->posX, obj->posY);

	if (terrID <= 0 || terrID > pGM->ldb.dTerr.GetLength())
		return;

	LmtMapInfo &map = pGM->lmtmgr.getInfo();
	ArrayList<ScrMapLmtArea> &areas = pGM->lmtmgr.getArea();

	ArrayList<int> enemies;

	for(int n=0; n<map.encounters_EnemysID.GetLength(); n++) {
		LmtEnemiesInfo &enemy = map.encounters_EnemysID[n]; 
		enemies.Add(enemy.Monsters_ID);
	}

	for(int i=0; i<areas.GetLength(); i++) {
		if ( areas[i].left <= obj->posX && obj->posX <= areas[i].right &&
			areas[i].up <= obj->posY && obj->posY <= areas[i].down) {
			
			LmtMapInfo &areainfo = pGM->lmt.MInfo[areas[i].id];
				
			for(int n=0; n<areainfo.encounters_EnemysID.GetLength(); n++) {
				LmtEnemiesInfo &enemy = areainfo.encounters_EnemysID[n]; 
				enemies.Add(enemy.Monsters_ID);
			}
		}
	}
	LdbTerrain &terr = pGM->ldb.dTerr[terrID-1];

	if (enemies.GetLength() > 0 && RANDF() < terr.encounter_rate / 100.0f * 1.0f / map.encounter_steps) {
		int enemiesID = enemies[rand() % enemies.GetLength()];

		BatPos atkType = BPOS_NORMAL;
		if (atkType == 0 && (terr.special_flags & 0x1) && RANDF() < terr.special_back_party/100.0f)
			atkType = BPOS_BACKATTACK_HERO;//TODO CHK

		if (atkType == 0 && (terr.special_flags & 0x2) && RANDF() < terr.special_back_enemies/100.0f)
			atkType = BPOS_BACKATTACK_ENEMY;


		ScrBattleBgInfo bg_info;

		if (map.background_type <= 1) {
			bg_info.set(terr);
		} else {
			bg_info.set(map.background_name);
		}

		pGM->slm->setSettingScrBattle(atkType, enemiesID, bg_info, true, false, false);
		pGM->slm->setScreen(scr_battle);
	}

	bool needFlash = false;

	//Terrain Walking Damage
	if (terr.damage > 0) {
		
		for(int id=1; id<=pGM->ldb.dChar.GetLength(); id++)
			if (mstate.isPartyIn(id))
				mchar.add(id, HP, -terr.damage);

		needFlash = true;

		CCLOG("TERRAIN FLASH" + terrID);
	}

	if (terr.footstep.name != "" && (!terr.on_damage_se || (terr.damage > 0))) {
		pGM->snd.PlaySound(terr.footstep);
	}


	int walk = ++mapWalkCnt;

	for(int id=1; id<=pGM->ldb.dChar.GetLength(); id++) {
		if (!mstate.isPartyIn(id))
			continue;

		int condID = mchar.getTopState(id);

		if (condID > 0) {
			LdbCondition &cond = pGM->ldb.dCond[condID-1];

			if (cond.type == 0)
				continue;

			int effVal = 0;

			switch(cond.hp_change_type) {
				case CondChangeType_nothing:effVal = 0;break;
				case CondChangeType_gain:	effVal = 1;break;
				case CondChangeType_lose:	effVal = -1;break;
			}

			if (cond.hp_change_map_steps != 0)
				if (walk % cond.hp_change_map_steps == 0) {
					mchar.add(id, HP, effVal * cond.hp_change_map_val);
					if (effVal == -1)
						needFlash = true;

					CCLOG((_ST("CONDITION FLASH ") + condID).getTextUTF8());
				}
			switch(cond.mp_change_type) {
				case CondChangeType_nothing:effVal = 0;break;
				case CondChangeType_gain:	effVal = 1;break;
				case CondChangeType_lose:	effVal = -1;break;
			}

			if (cond.mp_change_map_steps != 0)
				if (walk % cond.mp_change_map_steps == 0)
					mchar.add(id, MP, effVal * cond.mp_change_map_val);

		}
	}

	if (needFlash) {
		GlbEventNode enode;
		GlbEvent event;

		enode.code = ecFlashScreen;
		enode.indent = 0;
		enode.parameters.SetLength(7);
		enode.parameters[0] = 31;
		enode.parameters[1] = 0;
		enode.parameters[2] = 0;
		enode.parameters[3] = 31;
		enode.parameters[4] = 5;
		enode.parameters[5] = 0;
		enode.parameters[6] = 1;
		event.data.Add(enode);

		pEvt->doEvent(event);
	}
}
