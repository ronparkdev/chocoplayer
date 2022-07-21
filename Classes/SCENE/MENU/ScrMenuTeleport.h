#ifndef __R2K_SCENE_MENU_TELEPORT__
#define __R2K_SCENE_MENU_TELEPORT__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GameMain.h"
#include "../../GRAPHIC/GrpMenu.h"

USING_NS_CC;

class ScrMenuTeleport : public ScreenLayer
{
	GameMain *m_main;

	bool m_isSkill;

	int m_hid;
	int m_id;

	GrpMenu *m_menu;

	int m_key_esc_last;
	int m_key_arrow_last;
	int m_key_enter_last;
public:

	ScrMenuTeleport( GameMain *main, int id, int hid );
	~ScrMenuTeleport();

	void reloadShaders();

	void Draw(int framedelta);
	void chkKeys();

private:
	void Init();
};

#endif 