#include "ScrMapChipSetTile.h"

#include "../../COMMON/ScreenCommon.h"

ScrMapChipSetTile::ScrMapChipSetTile()
	:m_isused(false), m_pSpriteBatch(NULL), m_pSprite(NULL)
	,m_last_chipset_level(-1)
{}

ScrMapChipSetTile::~ScrMapChipSetTile()
{

}

void ScrMapChipSetTile::Init( CCNode *pLayer, CCSpriteBatchNode **pSpriteBatch, GrpShaderColor *pShader )
{
	if (m_pSpriteBatch != NULL && m_pSprite != NULL) {
		m_pCurSpriteBatch->removeChild(m_pSprite, false);
		m_pSprite = NULL;
	}

	m_pLayer = pLayer;
	m_pSpriteBatch = pSpriteBatch;
	m_pShader = pShader;

	m_last_chipset_level = -1;
}

void ScrMapChipSetTile::Release() {
	if (m_pCurSpriteBatch != NULL && m_pSprite != NULL) {
		m_pCurSpriteBatch->removeChild(m_pSprite, false);
		m_pSprite = NULL;
	}
	m_last_chipset_level = -1;
}

void ScrMapChipSetTile::Unset(CCNode *layer)
{
	if (m_isused) {
		if (m_pSprite != NULL) {
// 			pCurSpriteBatch->removeChild(pSprite, false);
// 			pSprite = NULL;
// 			m_last_chipset_level = -1;
			m_pSprite->setVisible(false);
		}
		m_isused = false;
	}
}

void ScrMapChipSetTile::Set( 
	float screen_x, float screen_y, 
	int map_x, int map_y, 
	int map_pos, 
	int chipset_x, int chipset_y, int chipset_z, 
	int chipset_w, int chipset_h,
	int add_x, int add_y,
	bool is_need_refresh)
{
	if (!m_isused) {
		int chipset_level = 0;
		if (chipset_z >= sz_chipset_upper_top)
			chipset_level = 3;
		else if (chipset_z >= sz_chipset_lower_top)
			chipset_level = 2;
		else if (chipset_z >= sz_chipset_upper_bot)
			chipset_level = 1;
		else if (chipset_z >= sz_chipset_lower_bot)
			chipset_level = 0;

		if (m_last_chipset_level != chipset_level) {
			CCSpriteBatchNode *pLastSpriteBatch = NULL;
			CCSprite *pLastSprite = NULL;
			if (m_last_chipset_level >= 0) {
				pLastSpriteBatch = m_pSpriteBatch[m_last_chipset_level];
				pLastSprite = m_pSprite;
			}

			m_pCurSpriteBatch = m_pSpriteBatch[chipset_level];

			CCTexture2D *pTexture = m_pCurSpriteBatch->getTexture();
			m_pSprite = CCSprite::createWithTexture(pTexture);
			m_pCurSpriteBatch->addChild(m_pSprite, chipset_z);

// 			TString p1, p2, str;
// 			p1.setTextHEX((int)m_pSprite);
// 			p2.setTextHEX((int)m_pCurSpriteBatch);
// 			CCLog((p1 + "=>" + p2).getTextUTF8());

			if (pLastSpriteBatch != NULL) {
				pLastSpriteBatch->removeChild(pLastSprite, false);
// 				p1.setTextHEX((int)pLastSprite);
// 				p2.setTextHEX((int)pLastSpriteBatch);
// 				CCLog((p1 + "X>" + p2).getTextUTF8());
			}

			m_last_chipset_level = chipset_level;
		} else {
			m_pSprite->setVisible(true);
			m_pCurSpriteBatch->reorderChild(m_pSprite, chipset_z);
		}

		m_pSprite->setTextureRect(CCRect(chipset_x, chipset_y ,chipset_w, chipset_h));
		m_pSprite->setPosition(ccp(
									SCR_POS_X(map_x*TILE_W+(chipset_w>>1)+add_x), 
									SCR_POS_Y(map_y*TILE_W+(chipset_h>>1)+add_y)
								));

		this->m_chipset_x = chipset_x;
		this->m_chipset_y = chipset_y;
		this->m_chipset_z = chipset_z;

		this->m_chipset_w = chipset_w;
		this->m_chipset_h = chipset_h;

		this->m_map_x = map_x;
		this->m_map_y = map_y;
		this->m_map_pos = map_pos;

		this->m_is_need_refresh = is_need_refresh;

		m_isused = true;
	}
}
