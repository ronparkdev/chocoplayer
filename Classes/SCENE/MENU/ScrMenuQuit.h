#ifndef __R2K_SCENE_MENU_QUIT__
#define __R2K_SCENE_MENU_QUIT__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GRAPHIC/GrpMenu.h"

USING_NS_CC;

class ScrMenuQuit : public ScreenLayer
{
	GameMain *m_main;

	GrpMenu *m_menu;
	GrpMenu *m_caption;
public:

	ScrMenuQuit( GameMain *main );
	~ScrMenuQuit();

	void reloadShaders();

	void Draw(int framedelta);
	void chkKeys();
};

#endif 