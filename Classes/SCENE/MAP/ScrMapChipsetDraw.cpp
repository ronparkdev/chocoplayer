#include "ScrMap.h"

int g_tile_x, g_tile_y, g_tile_z, g_tile_pid;
bool g_tile_need_refresh;

void ScrMap::drawChipSet(int framedelta)
{
	if (framedelta>0 || chipsetId == 0)
		chkChipSet();

// 	int pX = (int)(ScreenCommon::game_screen_x/TILE_W);
// 	int pY = (int)(ScreenCommon::game_screen_y/TILE_W);

	int pX = (int)(ScreenCommon::game_screen_x) >> 4;
	int pY = (int)(ScreenCommon::game_screen_y) >> 4;

	int seatime = seaTime/seaTimeDivide;
	if (seaAniType == 3)
		seaIndex = (seatime%6)>>1;
	else
	{
		if (seatime%8 < 6)
			seaIndex = (seatime%8)>>1;
		else
			seaIndex = 1;
	}
	seaModeX = (seatime%8)>>1;

	if (seaFrameLeft > 0) {
		seaFrameLeft -= framedelta;

		if (seaFrameLeft <= 0) {
			seaTime++;
			seaFrameLeft = 1;

			int tile_count = 0;
			for(int i=0; i<MAP_TILE_MAX && tile_count < mapTileEnabledLen; i++) {
				ScrMapChipSetTile &tile = mapTile[i];
				if (tile.m_isused && tile.m_is_need_refresh) {
					tile.Unset(this);
					mapTileQueue.push(i);
					mapTileEnabledLen--;
				}
			}
		}
	}

	//memset(m_map_tile_checked, 0, sizeof(byte)*m_map_tile_length); //2.10 Optimization

	// 	TString str = _ST("drawing tiles : ") + m_map_tiles;
	// 	debugtext->setString(str.getTextUTF8());

	int tile_count = 0;
	pFg->setPosition(ccp(SCR_POS_X(-ScreenCommon::game_screen_x), SCR_POS_Y(ScreenCommon::game_h-ScreenCommon::game_screen_y)));
	if (!pGM->lsd.Map.need_tile_refresh) {
		for(int i=0; i<MAP_TILE_MAX && tile_count < mapTileEnabledLen; i++) {
			ScrMapChipSetTile &tile = mapTile[i];
			if (tile.m_isused) {
				pMapTileChecked[tile.m_map_pos] = 1;
				tile_count++;
			}
		}
	} else {
		pGM->lsd.Map.need_tile_refresh = false;
		for(int i=0; i<MAP_TILE_MAX && tile_count < mapTileEnabledLen; i++) {
			ScrMapChipSetTile &tile = mapTile[i];
			if (tile.m_isused) {
				tile.Unset(this);
				mapTileQueue.push(i);
				mapTileEnabledLen--;
			}
		}
	}

	int dx, dy, did, cx, cy;
	for(g_tile_y = pY ; g_tile_y <= pY+GAME_SCREEN_TILE_HEIGHT ; g_tile_y++) {
		cy = dy = g_tile_y;

		if (dy >= mapHeight)dy -= mapHeight;
		if (dy < 0)dy += mapHeight;

		if (cy < 0) {
			if (mapHeight == GAME_SCREEN_TILE_HEIGHT)
				continue;

			cy += mapHeight;
		}

		for(g_tile_x = pX ; g_tile_x <= pX+GAME_SCREEN_TILE_WIDTH ; g_tile_x++)
		{
			cx = dx = g_tile_x;

			if (dx >= mapWidth)dx -= mapWidth;
			if (dx < 0)dx += mapWidth;
			did = dx + dy*(mapWidth);
			
			if (cx < 0) {
				if (mapWidth == GAME_SCREEN_TILE_WIDTH)
					continue;
				
				cx += mapWidth;
			}

			g_tile_pid = cx + cy*(mapWidth+GAME_SCREEN_TILE_WIDTH);

			if (pMapTileChecked[g_tile_pid] == 0) {
				drawChipsetTile(did);
			} else {
				pMapTileChecked[g_tile_pid] = 0;
			}
		}
	}

	tile_count = 0;
	for(int i=0; i<MAP_TILE_MAX && tile_count < mapTileEnabledLen; i++) {
		ScrMapChipSetTile &tile = mapTile[i];
		if (tile.m_isused) {
			if (pMapTileChecked[tile.m_map_pos] == 1) {
				pMapTileChecked[tile.m_map_pos] = 0;	//2.10 Optimization
				tile.Unset(this);
				mapTileQueue.push(i);
				mapTileEnabledLen--;
			} else {
				tile_count++;
			}
		}
	}

	refreshChipSetShader(framedelta, false);
}

inline void ScrMap::drawChipsetTile(int pid) {
	if (pid >= mapData.lowerMap.GetLength())
		return;

	LdbChipset &c = pGM->ldb.dChip[chipsetId-1];
	int LID = mapData.lowerMap[pid] ;
	int UID = mapData.upperMap[pid] ;

	int lowerCID = getChipsetID(LID);
	int upperCID = getChipsetID(UID);

	if (lowerCID > 17)lowerCID = pGM->lsd.Map.tile_lower[lowerCID-18] + 18;
	upperCID = pGM->lsd.Map.tile_upper[upperCID];

	int lowerflag, upperflag;

	if (lowerCID < c.passableDataLower.GetLength())
		lowerflag = c.passableDataLower[lowerCID];
	else
		lowerflag = 15;

	if (upperCID < c.passableDataUpper.GetLength())
		upperflag = c.passableDataUpper[upperCID];
	else
		upperflag = 15;

	if (0 > LID || LID > 12000 || 0 > UID || UID > 12000)
		return;

	
	if (g_tile_x < 0)
		g_tile_x += mapWidth;

	if (g_tile_y < 0)
		g_tile_y += mapHeight;

	g_tile_need_refresh = (LID < 4000);
	g_tile_z =  (((lowerflag&0x10) || (lowerflag&0x20))?sz_chipset_lower_top:sz_chipset_lower_bot);

	drawChipsetPart(LID);

	if (UID != 10000) {
		g_tile_z = (((upperflag&0x10)!=0x0)?sz_chipset_upper_top:sz_chipset_upper_bot);
		drawChipsetPart(UID);
	}
}

inline void ScrMap::drawChipsetPart(int chipID)
{
	if (chipID<3000)
	{
		int i = (chipID%50);
		int tx = (((int)(chipID/1000))%2)*3 + seaIndex;
		int ty = 4+((int)(chipID/2000))*3;

		switch(i)
		{
		case 0:
		{
			int ci = 0;
			tx = seaIndex;
			if (chipID<2000) {
				ci = chipID / 50;
				if (ci==1||ci%2==0||ci%3==0||ci%5==0) {
					DrawPart(_TILE(tx),_TILE(4));
					ty = 5;
				} else {
					DrawPart(_TILE(tx),_TILE(5));
					ty = 4;
				}
			} else {
				ci = (chipID-2000) / 50;
				if (ci==1||ci%2==0||ci%3==0||ci%5==0) {
					DrawPart(_TILE(tx),_TILE(7));
					ty = 6;
				} else {
					DrawPart(_TILE(tx),_TILE(6));
					ty = 7;
				}
			}
			switch(ci) {
			case 1:DrawPart(_TILE(tx)			,_TILE(ty)			,TILE_W ,TILE_W  ,0			,0);break;
			case 2:DrawPart(_TILE(tx) + TILE_HW,_TILE(ty)			,TILE_W ,TILE_W  ,TILE_HW	,0);break;
			case 4:DrawPart(_TILE(tx) 			,_TILE(ty) + TILE_HW,TILE_W ,TILE_W  ,0			,TILE_HW);break;
			case 8:DrawPart(_TILE(tx) + TILE_HW,_TILE(ty) + TILE_HW,TILE_W ,TILE_W  ,TILE_HW	,TILE_HW);break;

			case 3:DrawPart(_TILE(tx)			,_TILE(ty) 			,TILE_W ,TILE_HW ,0			,0);break;
			case 5:DrawPart(_TILE(tx)			,_TILE(ty) 			,TILE_HW,TILE_W  ,0			,0);break;
			case 10:DrawPart(_TILE(tx) + TILE_HW,_TILE(ty) 			,TILE_HW,TILE_W  ,TILE_HW	,0);break;
			case 12:DrawPart(_TILE(tx)			,_TILE(ty) + TILE_HW,TILE_W ,TILE_HW ,0			,TILE_HW);break;

			case 7:DrawPart(_TILE(tx)			,_TILE(ty)			,TILE_W ,TILE_W  ,0			,0);break;
			case 11:DrawPart(_TILE(tx) + TILE_HW,_TILE(ty)			,TILE_W ,TILE_W  ,TILE_HW	,0);break;
			case 13:DrawPart(_TILE(tx) 			,_TILE(ty) + TILE_HW,TILE_W ,TILE_W  ,0			,TILE_HW);break;
			case 14:DrawPart(_TILE(tx) + TILE_HW,_TILE(ty) + TILE_HW,TILE_W ,TILE_W  ,TILE_HW	,TILE_HW);break;
			}
			break;
		}

		case 1:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 2:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 3:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 4:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 5:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 6:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 7:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 8:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 9:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 10:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 11:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 12:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 13:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3),TILE_HW	,TILE_HW,0			,0);
			break;

		case 14:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 15:
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 16:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,TILE_W				,TILE_HW,TILE_W	,0			,0);
			break;

		case 17://
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,TILE_W				,TILE_HW,TILE_W	,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 18:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,TILE_W				,TILE_HW,TILE_W	,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 19://
			DrawPart(_TILE(tx)			,TILE_W);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 20:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(2)			,TILE_W	,TILE_HW,0			,0);
			break;

		case 21:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(2)			,TILE_W	,TILE_HW,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 22:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(2)			,TILE_W	,TILE_HW,0			,0);
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 23:
			DrawPart(_TILE(tx)			,_TILE(2)			,TILE_W	,TILE_HW,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 24:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,TILE_W				,TILE_HW,TILE_W	,TILE_HW	,0);
			break;

		case 25:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,TILE_W				,TILE_HW,TILE_W	,TILE_HW	,0);
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 26://
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,TILE_W				,TILE_HW,TILE_W	,TILE_HW	,0);
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 27:
			DrawPart(_TILE(tx)			,TILE_W);
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 28:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(2)+TILE_HW	,TILE_W	,TILE_HW,0			,TILE_HW);
			break;

		case 29:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(2)+TILE_HW	,TILE_W	,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 30:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,_TILE(2)+TILE_HW	,TILE_W	,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 31:
			DrawPart(_TILE(tx)			,_TILE(2)+TILE_HW	,TILE_W	,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 32:
			DrawPart(_TILE(tx)			,TILE_W);
			break;

		case 33:
			DrawPart(_TILE(tx)			,_TILE(2));
			break;

		case 34:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,TILE_W				,TILE_HW,TILE_W	,0			,0);
			DrawPart(_TILE(tx)			,_TILE(2)			,TILE_W	,TILE_HW,0			,0);
			DrawPart(_TILE(tx)			,0					,TILE_HW,TILE_HW,0			,0);
			break;

		case 35:
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx)			,TILE_W				,TILE_HW,TILE_W	,0			,0);
			DrawPart(_TILE(tx)			,_TILE(2)			,TILE_W	,TILE_HW,0			,0);
			DrawPart(_TILE(tx)			,0					,TILE_HW,TILE_HW,0			,0);
			break;

		case 36:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,TILE_W				,TILE_HW,TILE_W	,TILE_HW	,0);
			DrawPart(_TILE(tx)			,_TILE(2)			,TILE_W	,TILE_HW,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,0					,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 37:
			DrawPart(_TILE(tx)			,_TILE(3)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)+TILE_HW	,TILE_W				,TILE_HW,TILE_W	,TILE_HW	,0);
			DrawPart(_TILE(tx)			,_TILE(2)			,TILE_W	,TILE_HW,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,0					,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 38:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)+TILE_HW	,TILE_W				,TILE_HW,TILE_W	,TILE_HW	,0);
			DrawPart(_TILE(tx)			,_TILE(2)+TILE_HW	,TILE_W	,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)+TILE_HW	,TILE_HW			,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 39:
			DrawPart(_TILE(tx)			,_TILE(3)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx)+TILE_HW	,TILE_W				,TILE_HW,TILE_W	,TILE_HW	,0);
			DrawPart(_TILE(tx)			,_TILE(2)+TILE_HW	,TILE_W	,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)+TILE_HW	,TILE_HW			,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 40:
			DrawPart(_TILE(tx%3)		,_TILE(ty));
			DrawPart(_TILE(tx)			,TILE_W				,TILE_HW,TILE_W	,0			,0);
			DrawPart(_TILE(tx)			,_TILE(2)+TILE_HW	,TILE_W	,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)			,TILE_HW			,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 41:
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(3)			,TILE_HW,TILE_HW,TILE_HW	,0);
			DrawPart(_TILE(tx)			,TILE_W				,TILE_HW,TILE_W	,0			,0);
			DrawPart(_TILE(tx)			,_TILE(2)+TILE_HW	,TILE_W	,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)			,TILE_HW			,TILE_HW,TILE_HW,0			,TILE_HW);
			break;	

		case 42:
			DrawPart(_TILE(tx)			,TILE_W);
			DrawPart(_TILE(tx)			,0					,TILE_W	,TILE_HW,0			,0);
			break;

		case 43:
			DrawPart(_TILE(tx)			,_TILE(2));
			DrawPart(_TILE(tx)			,0					,TILE_HW,TILE_W	,0			,0);
			break;

		case 44:
			DrawPart(_TILE(tx)			,TILE_W);
			DrawPart(_TILE(tx)			,TILE_HW			,TILE_W	,TILE_HW,0			,TILE_HW);
			break;

		case 45:
			DrawPart(_TILE(tx)			,_TILE(2));
			DrawPart(_TILE(tx)+TILE_HW	,0					,TILE_HW,TILE_W	,TILE_HW	,0);
			break;

		case 46:
			DrawPart(_TILE(tx)	,0);
			break;

		default:
			return;
		}
	} else if (3000<=chipID && chipID< 4000)
	{
		//3045 3085 3143
		int i	,c;
		i = (chipID-3000)/50;
		c = seaModeX;
		if (0<=i && i<3 && 0<=c && c<4)
		{
			int tx=3+i;
			int ty=4+c;
			DrawPart(_TILE(tx)			,_TILE(ty));
		}
	} else if (4000<=chipID && chipID< 4600)
	{
		int tp = ((chipID-4000)/50);
		int tx = (tp%2)*3 + ((tp>=4)?6:0);
		int ty = 8+((int)(tp/2))*4 - ((tp>=4)?16:0);
		int i=(chipID-4000)%50;
		//Log.e("Error"	,"chipID:" + chipID + " tx:" + tx  + " ty:" + ty);
		switch (i) {
		case 0:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			break;

		case 1:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 2:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 3:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 4:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 5:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 6:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 7:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 8:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 9:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 10:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 11:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 12:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 13:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 14:
			DrawPart(_TILE(tx+1)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 15:
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 16:
			DrawPart(_TILE(tx)			,_TILE(ty+2));
			break;

		case 17:
			DrawPart(_TILE(tx)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 18:
			DrawPart(_TILE(tx)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 19:
			DrawPart(_TILE(tx+1)			,_TILE(ty+1));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 20:
			DrawPart(_TILE(tx+1)			,_TILE(ty+1));
			break;

		case 21:
			DrawPart(_TILE(tx+1)			,_TILE(ty+1));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 22:
			DrawPart(_TILE(tx+1)			,_TILE(ty+1));
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;


		case 23:
			DrawPart(_TILE(tx+1)			,_TILE(ty+1));
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 24:
			DrawPart(_TILE(tx+2)			,_TILE(ty+2));
			break;

		case 25:
			DrawPart(_TILE(tx+2)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 26:
			DrawPart(_TILE(tx+2)			,_TILE(ty+2));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 27:
			DrawPart(_TILE(tx+1)			,_TILE(ty+3));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 28:
			DrawPart(_TILE(tx+1)			,_TILE(ty+3));
			break;

		case 29:
			DrawPart(_TILE(tx+1)			,_TILE(ty+3));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 30:
			DrawPart(_TILE(tx+1)			,_TILE(ty+3));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 31:
			DrawPart(_TILE(tx+1)			,_TILE(ty+3));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 32:
			DrawPart(_TILE(tx)			,_TILE(ty+2)		,TILE_HW,TILE_W	,0			,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty+2)		,TILE_HW,TILE_W	,TILE_HW	,0);
			break;

		case 33:
			DrawPart(_TILE(tx+1)			,_TILE(ty+1)		,TILE_W	,TILE_HW,0			,0);
			DrawPart(_TILE(tx+1)			,_TILE(ty+3)+TILE_HW,TILE_W	,TILE_HW,0			,TILE_HW);
			break;

		case 34:
			DrawPart(_TILE(tx)			,_TILE(ty+1));
			break;

		case 35:
			DrawPart(_TILE(tx)			,_TILE(ty+1));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,TILE_HW	,TILE_HW);
			break;

		case 36:
			DrawPart(_TILE(tx+2)			,_TILE(ty+1));
			break;

		case 37:
			DrawPart(_TILE(tx+2)			,_TILE(ty+1));
			DrawPart(_TILE(tx+2)			,_TILE(ty)+TILE_HW	,TILE_HW,TILE_HW,0			,TILE_HW);
			break;

		case 38:
			DrawPart(_TILE(tx+2)			,_TILE(ty+3));
			break;

		case 39:
			DrawPart(_TILE(tx+2)			,_TILE(ty+3));
			DrawPart(_TILE(tx+2)			,_TILE(ty)			,TILE_HW,TILE_HW,0			,0);
			break;

		case 40:
			DrawPart(_TILE(tx)			,_TILE(ty+3));
			break;

		case 41:
			DrawPart(_TILE(tx)			,_TILE(ty+3));
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_HW,TILE_HW	,0);
			break;

		case 42:
			DrawPart(_TILE(tx)			,_TILE(ty+2)		,TILE_HW,TILE_W	,0			,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty+2)		,TILE_HW,TILE_W	,TILE_HW	,0);
			DrawPart(_TILE(tx)			,_TILE(ty)			,TILE_W	,TILE_HW,0			,0);
			break;

		case 43:
			DrawPart(_TILE(tx+1)			,_TILE(ty+1)		,TILE_W	,TILE_HW,0			,0);
			DrawPart(_TILE(tx+1)			,_TILE(ty+3)+TILE_HW,TILE_W	,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)			,_TILE(ty)			,TILE_HW,TILE_W	,0			,0);
			break;

		case 44:
			DrawPart(_TILE(tx)			,_TILE(ty+2)		,TILE_HW,TILE_W	,0			,0);
			DrawPart(_TILE(tx+2)+TILE_HW	,_TILE(ty+2)		,TILE_HW,TILE_W	,TILE_HW	,0);
			DrawPart(_TILE(tx)			,_TILE(ty)+TILE_HW	,TILE_W	,TILE_HW,0			,TILE_HW);
			break;

		case 45:
			DrawPart(_TILE(tx+1)			,_TILE(ty+1)		,TILE_W	,TILE_HW,0			,0);
			DrawPart(_TILE(tx+1)			,_TILE(ty+3)+TILE_HW,TILE_W	,TILE_HW,0			,TILE_HW);
			DrawPart(_TILE(tx)+TILE_HW	,_TILE(ty)			,TILE_HW,TILE_W	,TILE_HW	,0);
			break;

		case 46:
			DrawPart(_TILE(tx)			,_TILE(ty));
			break;

		default:
			return;
		}
	} else if (5000<=chipID && chipID<=5143) {
		int oriID = chipID-5000;
		int fixedID = pGM->lsd.Map.tile_lower[oriID];
		int tx = 12+fixedID%6 + ((int)fixedID/96)*6;
		int ty = (fixedID/6)%16;
		DrawPart(_TILE(tx)				,_TILE(ty)			,TILE_W ,TILE_W ,0			,0);
	} else if (10000<=chipID && chipID < 10144) {
		int oriID = chipID-10000;
		int fixedID = pGM->lsd.Map.tile_upper[oriID];
		int tx = 18 + fixedID % 6 + ((fixedID>=48)?6:0);
		int ty = 8 + (fixedID/6) - ((fixedID>=48)?16:0);

		DrawPart(_TILE(tx), _TILE(ty));
	}
	return;
}

int ScrMap::getChipsetID(int chipID)
{
	if (chipID<2000)//0,1
		return chipID/1000;
	else if (chipID<3000)//2,3
		return (chipID-2000)/500 + 2;
	else if (chipID<4000)//4,5
		return (chipID - 3000)/50+3;//TODO check
	else if (chipID<5000)//6,7
		return (chipID-4000)/50 + 6;
	else if (chipID<6000)
		return (chipID-5000)+18;
	else if (chipID>=10000)
		return (chipID-10000);
	else
		return 0;
}

inline void ScrMap::DrawPart(int chipset_x, int chipset_y)
{
	DrawPart(chipset_x	,chipset_y	,TILE_W	,TILE_W ,0	,0);
}

inline void ScrMap::DrawPart(int chipset_x, int chipset_y, int w, int h, int tile_x, int tile_y)
{
	//for(int i=0; i<MAP_TILE_MAX; i++) {

	if (mapTileQueue.empty())
		return;

	int i = mapTileQueue.front();
	mapTileQueue.pop();

	ScrMapChipSetTile &tile = mapTile[i];
	if (!tile.m_isused) {

		tile.Set(
			ScreenCommon::game_screen_x, ScreenCommon::game_screen_y, 
			g_tile_x, g_tile_y, g_tile_pid, 
			chipset_x, chipset_y, g_tile_z, 
			w, h, 
			tile_x, tile_y,
			g_tile_need_refresh
			);
		mapTileEnabledLen++;
		//break;
	}


	//}
	g_tile_z++;
}
