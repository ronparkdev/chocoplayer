#include "ScrBattle3.h"

float scr_battle_background_moving_speed[] = 
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

void ScrBattle3::initBackGround() {
	for(int i=0; i<2; i++)
		bgSprs[i] = NULL;
}

void ScrBattle3::setBackGround( const ScrBattleBgInfo &bg_info )
{
	int i, r, c, texid;
	CCTexture2D *pTex;

	CCSprite *pSprite;

	bgName = bg_info;

	for(int i=0; i<2; i++)
		if (bgSprs[i] != NULL) {
			this->removeChild(bgSprs[i]);
			bgSprs[i] = NULL;
		}

	if (!bg_info.use_ab) {
		texid = IBuffer::getInstance()->get(imgBackdrop, IMAGE_BUFFER_NOALPHA_PRESTR + bg_info.bg);
		pTex = IBuffer::getInstance()->get(imgBackdrop, texid);

		bgSprs[0] = CCSpriteBatchNode::createWithTexture(pTex, 1);
		bgSprs[0]->setPosition(ccp(160, 120));
		this->addChild(bgSprs[0]);
		
		pSprite = CCSprite::createWithTexture(pTex);
		bgSprs[0]->addChild(pSprite);

		hue.init(SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES, bgSprs[0]);
	} else {
		for(i=0; i<2; i++) {
			TString bg_name;

			if (i==0)
				bg_name = bg_info.bg_a;
			else
				bg_name = bg_info.bg_b;

			int texid = IBuffer::getInstance()->get(imgPanorama, bg_name);

			CCTexture2D *pTex = IBuffer::getInstance()->get(imgPanorama, texid);

			bgW[i] = pTex->getContentSize().width;
			bgH[i] = pTex->getContentSize().height;

			if (bgH[i] == 0 || bgW[i] == 0)
				continue;

			bgSprs[i]->removeAllChildrenWithCleanup(true);

			if (bgH[i] < GAME_SCREEN_HEIGHT)
				bgR[i] = GAME_SCREEN_HEIGHT	/ bgH[i] + 1;
			else
				bgR[i] = 2;

			if (bgW[i] < GAME_SCREEN_WIDTH)
				bgC[i] = GAME_SCREEN_WIDTH	/ bgW[i] + 1;
			else
				bgC[i] = 2;

			bgSprs[i]->initWithTexture(pTex, bgR[i] * bgC[i]);
			bgSprs[i]->setVisible(true);


			hue.init(SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES, bgSprs[i]);

			i = 0;
			for(r=0; r<bgR[i]; r++)
				for(c=0; c<bgC[i]; c++)
				{
					pSprite = CCSprite::createWithTexture(bgSprs[i]->getTexture());
					pSprite->setPosition(ccp(bgW[i] * (c + 0.5f), -bgH[i] * (r + 0.5f)));
					bgSprs[i]->addChild(pSprite);

					if ((++i) == 1024)
						break;
				}
		}
	}
}

void ScrBattle3::drawBackGround(int framedelta) {
	int i;
	int m_background_x_int, m_background_x_def;
	int m_background_y_int, m_background_y_def;

	LsdMap &map = pGM->lsd.Map;
	LsdScreen &screen = pGM->lsd.Screen;

	for(i=0; i<2; i++) {
		if (bgSprs[i])
			continue;

		if (bgW == 0 || bgH == 0)
			return;

		//수평
		if (map.bg_hscroll && map.bg_hscrollauto) {
			if (map.bg_hscrollauto_speed != 0)
				bgX[i] += scr_battle_background_moving_speed[map.bg_hscrollauto_speed+8] * framedelta; 
		}

		m_background_x_int = m_background_x_def = (int)bgX[0];

		while(m_background_x_def < -bgW[i])
			m_background_x_def += bgW[i];

		while(m_background_x_def > 0)
			m_background_x_def -= bgW[i];

		bgX[i] += m_background_x_def - m_background_x_int;

		//수직
		if (map.bg_vscroll && map.bg_vscrollauto) {
			if (map.bg_vscrollauto_speed != 0)	
				bgY[i] += scr_battle_background_moving_speed[map.bg_vscrollauto_speed+8] * framedelta;
		}

		m_background_y_int = m_background_y_def = (int)bgY[0];

		bgSprs[i]->setPosition(ccp(
			SCR_POS_X(bgX[i]), 
			SCR_POS_Y(bgY[i])	
			));
	}
}
