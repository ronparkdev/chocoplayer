#ifndef __R2K_SCENE_STATE__
#define __R2K_SCENE_STATE__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GRAPHIC/GrpFontL.h"
#include "../../GRAPHIC/GrpMenu.h"
#include "../../GRAPHIC/OBJECT/GrpFace.h"

USING_NS_CC;

class ScrMenuState : public ScreenLayer
{
	GameMain *m_main;
	int m_hid;

	GrpMsgbox *m_leftbg;
	GrpMsgbox *m_righttopbg;
	GrpMsgbox *m_rightmidbg;
	GrpMsgbox *m_rightbotbg;
	GrpMsgbox *m_moneybg;
	
	int m_key_esc_last;

	GrpFace *m_hFace;

	GrpFontL *m_hPos;

	GrpFontL *m_cName;
	GrpFontL *m_hName;

	GrpFontL *m_cJob;
	GrpFontL *m_hJob;

	GrpFontL *m_cTitle;
	GrpFontL *m_hTitle;

	GrpFontL *m_cState;
	GrpFontL *m_hState;

	GrpFontL *m_hLev;
	
	GrpFontL *m_money;

	GrpFontL *m_hHP;
	GrpFontL *m_hMP;
	GrpFontL *m_hExp;

	GrpFontL *m_hAtk;
	GrpFontL *m_hDef;
	GrpFontL *m_hSpt;
	GrpFontL *m_hAgi;

	GrpFontL *m_eWeapon;
	GrpFontL *m_eShield;
	GrpFontL *m_eArmor;
	GrpFontL *m_eHelmet;
	GrpFontL *m_eAcc;

public:

	ScrMenuState(GameMain *main, int hid);
	~ScrMenuState();

	void reloadShaders();

	void Draw(int framedelta);
	void chkKeys();

private:
};

#endif 
