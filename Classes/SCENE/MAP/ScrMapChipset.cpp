#include "ScrMap.h"

#include <cmath>

#define TILECHECK_USING 1;

void ScrMap::createMapChipset() {
	mapWidth = mapData.width;
	mapHeight = mapData.height;
	mapTilesLen = mapWidth * mapHeight;
	mapTileLen = (mapWidth+GAME_SCREEN_TILE_WIDTH) * (mapHeight+GAME_SCREEN_TILE_HEIGHT);

	if (pMapTileChecked != NULL)
		delete []pMapTileChecked;

	pMapTileChecked = new byte[mapTileLen]; 

	chipsetId = 0;
	chipsetBufId = -1;

	pGM->lsd.Map.chipset_id = mapData.chipSetID;

	refreshChipSetShader(0,true);

	pFg = CCNode::create();
	this->addChild(pFg, sz_chipset_lower_bot);

	int texid = IBuffer::getInstance()->get(imgPanorama, "");
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgPanorama, texid);
	for(int i=0; i<4; i++) {
		//m_map_chipset[i] = CCRenderTexture::create(ScreenCommon::game_w, ScreenCommon::game_h);
		pMapChipsetLayer[i] = CCSpriteBatchNode::createWithTexture(pTex, 1500);

		int chipset_level = 0;
		switch(i) {
			case 0:chipset_level = sz_chipset_lower_bot;break;
			case 1:chipset_level = sz_chipset_upper_bot;break;
			case 2:chipset_level = sz_chipset_lower_top;break;
			case 3:chipset_level = sz_chipset_upper_top;break;
		}

		pFg->addChild(pMapChipsetLayer[i], chipset_level);
		//m_map_chipset[i]->addChild(m_map_chipset_sprites[i]);
		//m_foreground->addChild(m_map_chipset[i], chipset_level);
	}

	maxScrX = _TILE(mapWidth);
	maxScrY = _TILE(mapHeight);

	memset(pMapTileChecked, 0, sizeof(byte)*mapTileLen); //2.10 Optimization
}

void ScrMap::initMapChipset()
{	

}

void ScrMap::destroyMapChipset() {
	if (pMapTileChecked != NULL)
		delete []pMapTileChecked;

	for(int i=0; i<MAP_TILE_MAX; i++)
		mapTile[i].Release();

	for(int i=0; i<4; i++)
		pFg->removeChild(pMapChipsetLayer[i]);
	
	this->removeChild(pFg);
}

void ScrMap::chkChipSet() {
	int chipset_id = pGM->lsd.Map.chipset_id;
	if (chipsetId == chipset_id ||
		chipset_id <= 0 || chipset_id > pGM->ldb.dChip.GetLength())
			return;

	LdbChipset &chipset = pGM->ldb.dChip[chipset_id - 1];

	TString chipsetName = chipset.ChipsetName;
	chipsetBufId = IBuffer::getInstance()->get(imgChipSet, chipsetName);

	seaAniType = chipset.Animation_Type;
	seaTime = 0;

	if (chipset.Animation_Speed == 0)
		seaTimeDivide = 12;
	else
		seaTimeDivide = 6;
	seaFrameLeft = 1;

	mapTileEnabledLen = 0;

	CCTexture2D *pTexture = IBuffer::getInstance()->get(imgChipSet, chipsetBufId);

	while(!mapTileQueue.empty())
		mapTileQueue.pop();

	for(int i=0; i<MAP_TILE_MAX; i++)
		mapTile[i].Release();

	for(int i=0; i<4; i++) {
		pMapChipsetLayer[i]->initWithTexture(pTexture, 1500);
		mapShader.setShader(pMapChipsetLayer[i]);
	}
	
	for(int i=0; i<MAP_TILE_MAX; i++) {
		mapTile[i].Init(this, pMapChipsetLayer, &mapShader);
		mapTileQueue.push(i);
	}

	chipsetId = chipset_id;

	initMapEventsChipset();
}

void ScrMap::refreshChipSetShader(int framedelta, bool force) {
	LsdScreen &scr = pGM->lsd.Screen;

	if (scr.scr_timeleft > 0) {
		scr.scr_timeleft -= framedelta;
		if (scr.scr_timeleft <= 0) {
			scr.scr_cur_r = scr.scr_fin_r;
			scr.scr_cur_g = scr.scr_fin_g;
			scr.scr_cur_b = scr.scr_fin_b;
			scr.scr_cur_gr= scr.scr_fin_gr;
		} else {
			scr.scr_cur_r += (scr.scr_fin_r - scr.scr_cur_r) / scr.scr_timeleft;
			scr.scr_cur_g += (scr.scr_fin_g - scr.scr_cur_g) / scr.scr_timeleft;
			scr.scr_cur_b += (scr.scr_fin_b - scr.scr_cur_b) / scr.scr_timeleft;
			scr.scr_cur_gr+= (scr.scr_fin_gr - scr.scr_cur_gr) / scr.scr_timeleft;
		}
		force = true;
	}

	if (scr.flash_continuous != 2) {
		if (scr.flash_frame_left > 0) {
			scr.flash_frame_left -= framedelta;
			if (scr.flash_frame_left <= 0)
				scr.flash_cur_lev = 0.0f;
			else
				scr.flash_cur_lev -= scr.flash_cur_lev / scr.flash_frame_left;

			force = true;
		}
	} else {
		scr.flash_continuous = 0;
		scr.flash_cur_lev = 0.0f;
		if (scr.flash_frame_left > 0) {
			scr.flash_frame_left = 0;
			force = true;
		}
	}

	isEventShaderNeedRefresh = force;

	if (force) {
		float flash_per = scr.flash_cur_lev / 31.0f;
		float per_r, per_g, per_b, per_gr;
		per_r = (scr.scr_cur_r-100.0f)/100.0f + scr.flash_fin_r * flash_per / 31.0f;
		per_g = (scr.scr_cur_g-100.0f)/100.0f + scr.flash_fin_g * flash_per / 31.0f;
		per_b = (scr.scr_cur_b-100.0f)/100.0f + scr.flash_fin_b * flash_per / 31.0f;
		per_gr = (scr.scr_cur_gr-100.0f)/100.0f + flash_per;
		mapShader.setColor(per_r, per_g, per_b, per_gr);
		bgShader.setColor(per_r, per_g, per_b, per_gr);
	}
}