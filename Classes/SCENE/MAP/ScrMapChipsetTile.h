#ifndef __R2K_SCENE_MAP_CHIPSET_TILE__
#define __R2K_SCENE_MAP_CHIPSET_TILE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../../GRAPHIC/GrpShaderColor.h"

#define TILE_W 16
#define TILE_HW 8
#define _TILE(add_x) ((add_x)<<4)

class ScrMapChipSetTile{
public:
	ScrMapChipSetTile();
	~ScrMapChipSetTile();
	void Init( CCNode *pLayer, CCSpriteBatchNode **pSpriteBatch, GrpShaderColor *pShader );
	void Release();

	bool m_isused;

	int m_map_x;
	int m_map_y;
	int m_map_pos;

	int m_chipset_x;
	int m_chipset_y;
	int m_chipset_z;
	int m_chipset_w;
	int m_chipset_h;

	CCNode * m_pLayer;
	GrpShaderColor * m_pShader;

	CCSprite *m_pSprite;
	CCSpriteBatchNode **m_pSpriteBatch;
	CCSpriteBatchNode *m_pCurSpriteBatch;
	int m_last_chipset_level;

	bool m_is_need_refresh;
public:
	void Set(	float screen_x, float screen_y, 
				int map_x, int map_y, int map_pos, 
				int chipset_x, int chipset_y, int chipset_z, 
				int w, int h, 
				int tile_x, int tile_y,
				bool is_need_refresh);
	void Unset(CCNode *layer);
};

#endif