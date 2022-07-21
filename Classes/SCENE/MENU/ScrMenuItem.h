#ifndef __R2K_SCENE_MENU_ITEM__
#define __R2K_SCENE_MENU_ITEM__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GRAPHIC/GrpFontL.h"
#include "../../GRAPHIC/GrpMenu.h"

USING_NS_CC;

class ScrMenuItem : public ScreenLayer
{
	GameMain *m_main;

	GrpFontL	*m_explain;
	GrpMsgbox	*m_explainbg;
	GrpMenu		*m_itemmenu;

	int m_last_index;

	int m_item_count;
	ArrayList<unsigned short> m_item_index;
public:

	ScrMenuItem(GameMain *main);
	~ScrMenuItem();

	void reloadShaders();

	void Draw(int framedelta);
	void chkKeys();
	void refreshItems();
private:

};

#endif 
