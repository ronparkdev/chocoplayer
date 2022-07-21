#ifndef __R2K_SCENE_MENU_ORDER__
#define __R2K_SCENE_MENU_ORDER__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GRAPHIC/GrpMenu.h"

USING_NS_CC;

class ScrMenuOrder : public ScreenLayer
{
	GameMain *m_main;

	GrpMenu *m_menu;
	GrpMenu	*m_menuL;
	GrpMenu *m_menuR;

	int m_hero_length;
	bool m_hero_selected[4];
	ArrayList<short> m_hero_from;
	ArrayList<short> m_hero_to;
	
public:

	ScrMenuOrder( GameMain *main);
	~ScrMenuOrder();

	void reloadShaders();

	void Draw(int framedelta);
	void chkKeys();
	void updateMenu();
};

#endif 