#ifndef __R2K_SCENE_MENU_EQUIP__
#define __R2K_SCENE_MENU_EQUIP__
#include "../../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../ScreenLayer.h"
#include "../../GRAPHIC/GrpFontL.h"
#include "../../GRAPHIC/GrpMenu.h"

USING_NS_CC;

class ScrMenuEquip : public ScreenLayer
{
	GameMain *m_main;
	int m_hid;

	GrpFontL	*m_explain;
	GrpMsgbox	*m_explainbg;
	GrpMenu		*m_equipmenu;
	GrpMenu		*m_statlist;
	GrpMenu		*m_itemlist;

	int m_skill_count;
	ArrayList<byte> m_skill_index;

	int m_curitem_type;
	ArrayList<unsigned short> m_item_index;
	int m_item_count;

	bool m_selectingitem;
	int m_last_index;
	bool m_two_hand_weapon;
public:

	ScrMenuEquip( GameMain *main, int hID);
	~ScrMenuEquip();

	void reloadShaders();

	void Draw(int framedelta);
	void chkKeys();

	void refStat(int itemID);
	void refEquip();
	void refItem();

private:
	TString getStatColor(int bef, int aft);
	void equipTest(int itemID, int &atk, int &def, int &spt, int &agi);

};

#endif 