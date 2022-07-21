#ifndef __R2K_SCENE_MENU_SKILL__
#define __R2K_SCENE_MENU_SKILL__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GRAPHIC/GrpFontL.h"
#include "../../GRAPHIC/GrpMenu.h"

USING_NS_CC;

class ScrMenuSkill : public ScreenLayer
{
	GameMain *m_main;
	int m_hid;

	GrpFontL	*m_explain;
	GrpMsgbox	*m_explainbg;
	GrpFontL	*m_state;
	GrpMsgbox	*m_statebg;
	GrpMenu		*m_itemmenu;

	int m_last_index;

	int m_skill_count;
	ArrayList<int> m_skill_index;
	
public:

	ScrMenuSkill( GameMain *main, int hID);
	~ScrMenuSkill();

	void reloadShaders();

	void Draw(int framedelta);
	void chkKeys();
	void refStat();
private:

};

#endif 