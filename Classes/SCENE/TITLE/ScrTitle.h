#ifndef __R2K_SCENE_TITLE__
#define __R2K_SCENE_TITLE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GameMain.h"
#include "../../GRAPHIC/GrpMenu.h"


class ScrTitle : public ScreenLayer
{
	GameMain *pGM;

	GrpMenu *pMenu;

	CCSprite *pSprTitle;

	int debugInt;
public:
	ScrTitle(GameMain *main);
	~ScrTitle();

	void reloadShaders();
	void Draw(int framedelta);

};

#endif 
