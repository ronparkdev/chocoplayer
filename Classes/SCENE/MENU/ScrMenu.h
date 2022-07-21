#ifndef __R2K_SCENE_MENU__
#define __R2K_SCENE_MENU__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GameMain.h"
#include "../../GRAPHIC/GrpFontL.h"
#include "../../GRAPHIC/GrpMenu.h"
#include "../../GRAPHIC/OBJECT/GrpFace.h"

USING_NS_CC;

#define SCR_SAVE_COUNT 15
#define SCR_SAVE_COUNTPERPAGE 3

class ScrMenu : public ScreenLayer
{
	GameMain *m_main;
	GrpMenu *m_leftmenu;
	GrpMsgbox *m_rightbg;
	GrpMsgbox *m_moneybg;
	GrpSelection *m_selection;

	TString m_menustr[10];
	int m_mlistid[10];
	int m_mlistlen;

	int m_key_esc_last;
	int m_key_enter_last;
	int m_key_arrow_last;

	GrpFontL *m_money;

	GrpFontL *m_hName[4];
	GrpFontL *m_hTitle[4];
	GrpFontL *m_hLev[4];
	GrpFontL *m_hExp[4];
	GrpFontL *m_hState[4];
	GrpFontL *m_hHP[4];
	GrpFontL *m_hMP[4];

	GrpFace *m_hFace[4];

	int m_selected_menu;
	int m_selected_hero;
	
	ArrayList<short> m_party;
	int m_party_count;
public:

	ScrMenu(GameMain *main);
	~ScrMenu();

	void reloadShaders();

	void Init();
	void InitMenu();
	void InitHeros();
	void updateMenu();

	void Draw(int framedelta);
	void chkKeys();

private:
	void refreshSelection();
};

#endif 
