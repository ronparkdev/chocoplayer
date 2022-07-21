#include "ScrMap.h"

float scr_map_background_moving_speed[] = 
{
	-8.0f,
	-4.0f,
	-2.0f,
	-1.0f,
	-0.5f,
	-0.25f,
	-0.125f,
	-0.0625f,
	0.0f,
	0.0625f,
	0.125f,
	0.25f,
	0.5f,
	1.0f,
	2.0f,
	4.0f,
	8.0f
};

void ScrMap::createBackGround()
{
	pBgSprBN = NULL;
	bgTexName = "";

	int texid = IBuffer::getInstance()->get(imgPanorama, "");
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgPanorama, texid);

	pBgSprBN = CCSpriteBatchNode::createWithTexture(pTex, 4);
	this->addChild(pBgSprBN, sz_background);

	for(int i=0; i<4; i++) {
		pBgSprs[i] = CCSprite::createWithTexture(pTex);
		pBgSprBN->addChild(pBgSprs[i]);
	}
}

void ScrMap::initBackGround(bool init)
{
	LsdMap &lsdmap = pGM->lsd.Map;

	if (isInited) {
		lsdmap.bg_name = mapData.bg_name;
		lsdmap.bg_hscroll = mapData.bg_hscroll;
		lsdmap.bg_vscroll = mapData.bg_vscroll;
		lsdmap.bg_hscrollauto = mapData.bg_hscrollauto;
		lsdmap.bg_vscrollauto = mapData.bg_vscrollauto;
		lsdmap.bg_hscrollauto_speed = mapData.bg_hscrollauto_speed;
		lsdmap.bg_vscrollauto_speed = mapData.bg_vscrollauto_speed;
	}

	setBackGround(
		lsdmap.bg_name,
		lsdmap.bg_hscroll,
		lsdmap.bg_vscroll,
		lsdmap.bg_hscrollauto,
		lsdmap.bg_vscrollauto,
		lsdmap.bg_hscrollauto_speed,
		lsdmap.bg_vscrollauto_speed
		);
}

void ScrMap::destroyBackGround() {
	this->removeChild(pBgSprBN);
}


void ScrMap::setBackGround( const TString &name )
{
	LsdMap &map = pGM->lsd.Map;


	float x, y;
	int i, r, c;
	bool match_as_map = false;
	if (!name.isEmpty()) {

		int texid = IBuffer::getInstance()->get(imgPanorama, IMAGE_BUFFER_NOALPHA_PRESTR + name);

		CCTexture2D *pTex = IBuffer::getInstance()->get(imgPanorama, texid);

		bgW = pTex->getContentSize().width;
		bgH = pTex->getContentSize().height;

		if (bgH == 0 || bgW == 0)
			return;

		if (bgW == _TILE(mapWidth) &&
			bgH == _TILE(mapHeight))
				match_as_map = true;

		pBgSprBN->removeAllChildrenWithCleanup(true);

		if (match_as_map && !map.bg_vscroll && (mapData.scrollType == scroll_none || mapData.scrollType == scroll_horizonal))
			bgR = 1;
		else if (bgH < GAME_SCREEN_HEIGHT)
			bgR = GAME_SCREEN_HEIGHT	/ bgH + 1;
		else
			bgR = 2;

		if (match_as_map && !map.bg_hscroll && (mapData.scrollType == scroll_none || mapData.scrollType == scroll_vertical))
			bgC = 1;
		else if (bgW < GAME_SCREEN_WIDTH)
			bgC = GAME_SCREEN_WIDTH	/ bgW + 1;
		else
			bgC = 2;

		pBgSprBN->initWithTexture(pTex, bgR * bgC);
		pBgSprBN->setVisible(true);


		bgShader.setShader(pBgSprBN);

		i = 0;
		for(r=0; r<bgR; r++)
			for(c=0; c<bgC; c++)
			{
				x = bgW * (c + 0.5f);
				y = bgH * (r + 0.5f);
				pBgSprs[i] = CCSprite::createWithTexture(pBgSprBN->getTexture());
				pBgSprs[i]->setPosition(ccp(x, -y));

				pBgSprBN->addChild(pBgSprs[i]);

				if ((++i) == 1024)
					break;
			}
	} else {
		if (pBgSprBN != NULL)
			pBgSprBN->setVisible(false);

		bgW = 0;
		bgH = 0;
	}

	map.bg_name = name;
	bgTexName = name;
}

void ScrMap::setBackGround(const TString &name, bool hs, bool vs, bool hsa, bool vsa, int hsa_speed, int vsa_speed)
{
	LsdMap &map = pGM->lsd.Map;

	bgX = bgY = 0;

	isBgNeedRefresh = false;

	map.bg_hscroll = hs;
	map.bg_vscroll = vs;
	map.bg_hscrollauto = hsa;
	map.bg_vscrollauto = vsa;
	map.bg_hscrollauto_speed = hsa_speed;
	map.bg_vscrollauto_speed = vsa_speed;

	setBackGround(name);
}

int scr_map_last_screen_x;
int scr_map_last_screen_y;

void ScrMap::drawBackGround(int framedelta)
{
	//int i, r, c, r_s, r_e, c_s, c_e, t_x, t_y;

	int m_background_x_int, m_background_x_def;
	int m_background_y_int, m_background_y_def;
	LsdMap &map = pGM->lsd.Map;
	LsdScreen &screen = pGM->lsd.Screen;

	float screen_x = ScreenCommon::game_screen_x + screen.screen_adjust_x;
	float screen_y = ScreenCommon::game_screen_y + screen.screen_adjust_y;

	if (bgTexName != pGM->lsd.Map.bg_name && framedelta > 0)
		setBackGround(pGM->lsd.Map.bg_name);

	if (bgTexName.isEmpty())
		return;

	if (bgW == 0 || bgH == 0)
		return;

	if (!isBgNeedRefresh) {
		bgLastScrX = screen_x;
		bgLastScrY = screen_y;
		isBgNeedRefresh = true;
	}

	//수평
	if (map.bg_hscroll) {
		//움직임의 반이 움직임

		float def = bgLastScrX - screen_x;

		bgX += def / 2.0f;

		if (map.bg_hscrollauto && map.bg_hscrollauto_speed != 0)
			bgX += scr_map_background_moving_speed[map.bg_hscrollauto_speed+8] * framedelta; 

		bgLastScrX = screen_x;
	} else {
		if (bgW == 320 || 
			mapData.scrollType == scroll_horizonal || 
			mapData.scrollType == scroll_both)
		{
			//화면에 고정됨
			bgX = 0.0f;

		} else {
			bgX = -screen_x;
		}
	}

	m_background_x_int = m_background_x_def = (int)bgX;

	while(m_background_x_def < -bgW)
		m_background_x_def += bgW;

	while(m_background_x_def > 0)
		m_background_x_def -= bgW;

	bgX += m_background_x_def - m_background_x_int;

	//수직
	if (map.bg_vscroll) {
		//움직임의 반이 움직임

		float def = bgLastScrY - screen_y;

		bgY += def / 2.0f;

		if (map.bg_vscrollauto && map.bg_vscrollauto_speed != 0)	
			bgY += scr_map_background_moving_speed[map.bg_vscrollauto_speed+8] * framedelta;

		bgLastScrY = screen_y;
	} else {
		if (bgW == 240 || 
			mapData.scrollType == scroll_vertical || 
			mapData.scrollType == scroll_both)
		{
			//화면에 고정됨
			bgY = 0.0f;

		} else {
			bgY = -screen_y;
		}
	}

	m_background_y_int = m_background_y_def = (int)bgY;

	while(m_background_y_def < -bgH)
		m_background_y_def += bgH;

	while(m_background_y_def > 0)
		m_background_y_def -= bgH;

	bgY += m_background_y_def - m_background_y_int;

	pBgSprBN->setPosition(ccp(
		SCR_POS_X(bgX), 
		SCR_POS_Y(bgY)	
		));

	float x, y;
	float xf, xt, yf, yt;
	int r, c, a, b, i = 0;

	for(r=0; r<bgR; r++) {

		if (r == 0)
			yf = -bgY;
		else
			yf = 0;

		if (r == bgR - 1) {
			if (r == 0 && yf + ScreenCommon::game_h >= bgH) {
				yt = bgH;
			} else {
				b = bgR * bgH;
				a = (int)-bgY;
				while(a + ScreenCommon::game_h < b)
					a += ScreenCommon::game_h;

				if ((yt = a % bgH) == 0)
					yt = bgH;
			}
		}else
			yt = bgH;

		y = bgH * r + (yf + yt) * 0.5f;

		for(c=0; c<bgC; c++)
		{

			if (c == 0)
				xf = -bgX;
			else
				xf = 0;

			if (c == bgC - 1) {
				if (c == 0 && xf + ScreenCommon::game_w >= bgW) {
					xt = bgW;
				} else {
					b = bgC * bgW;
					a = (int)-bgX;
					while(a + ScreenCommon::game_w < b)
						a += ScreenCommon::game_w;

					if ((xt = a % bgW) == 0)
						xt = bgW;
				}
			}else
				xt = bgW;

			x = bgW * c + (xf + xt) * 0.5f;

			pBgSprs[i]->setTextureRect(CCRect(xf, yf, xt-xf, yt-yf));
			pBgSprs[i]->setPosition(ccp(x, -y));

			if ((++i) == 1024)
				break;
		}
	}

	scr_map_last_screen_x = bgX;
	scr_map_last_screen_y = bgY;

}