#ifndef __R2K_SCENE_GAMEOVER__
#define __R2K_SCENE_GAMEOVER__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GRAPHIC/GrpMenu.h"

class ScrGameover: public ScreenLayer
{
	GameMain *pGM;

	GrpMenu *pMenu;

	CCSprite *pSprBG;
public:
	ScrGameover(GameMain *main);
	~ScrGameover();

	void reloadShaders();

	void Draw(int framedelta);
};

#endif 
