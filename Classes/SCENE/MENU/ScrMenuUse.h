#ifndef __R2K_SCENE_MENU_USE__
#define __R2K_SCENE_MENU_USE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GRAPHIC/GrpFontL.h"
#include "../../GRAPHIC/GrpMenu.h"
#include "../../GRAPHIC/OBJECT/GrpFace.h"

USING_NS_CC;

class ScrMenuUse : public ScreenLayer
{
	GameMain *m_main;

	int m_hid;
	int m_id;

	bool m_isAllEffect;

	bool m_isskill;

	GrpFontL	*m_name;
	GrpMsgbox	*m_namebg;

	GrpFontL	*m_count;
	GrpMsgbox	*m_countbg;

	GrpMsgbox	*m_herobg;

	GrpFace *m_hFace[4];

	GrpFontL *m_hName[4];
	GrpFontL *m_hLev[4];
	GrpFontL *m_hState[4];
	GrpFontL *m_hHP[4];
	GrpFontL *m_hMP[4];

	GrpSelection *m_selection;
	
	ArrayList<short> m_party;
	int m_party_count;

	int m_selected_hero;

	int m_key_esc_last;
	int m_key_arrow_last;
	int m_key_enter_last;
public:

	ScrMenuUse( GameMain *main, int hID, int iID, bool isSkill);
	~ScrMenuUse();

	void Draw(int framedelta);
	void chkKeys();

	void reloadShaders();

	void refHero();
	void refCount();
private:
	void Init();
};

#endif 