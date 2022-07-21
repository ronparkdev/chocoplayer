#include "ScrMenuEquip.h"

#include "ScrMenu.h"
#include "../../COMMON/MgrEquip.h"
#include "../../COMMON/MgrCharacter.h"
#include "../../COMMON/MgrItem.h"

ScrMenuEquip::ScrMenuEquip( GameMain *main, int hID )
{
	m_main = main;
	m_hid = hID;

	int texID = IBuffer::getInstance()->get(imgSystem, IMAGE_BUFFER_NOALPHA_PRESTR + m_main->lsd.System.system_name);
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	m_explainbg = new GrpMsgbox(m_main, 320, 32);
	m_explainbg->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(16)));	

	m_explain = new GrpFontL(main);
	m_explain->setPosition(ccp(SCR_POS_X(8),SCR_POS_Y(8)));

	m_statlist = new GrpMenu(m_main, 5, 1, 120);
	m_statlist->setPosition(ccp(SCR_POS_X(60),SCR_POS_Y(32+16*6/2)));

	m_equipmenu = new GrpMenu(m_main, 5, 1, 200);
	m_equipmenu->setPosition(ccp(SCR_POS_X(220),SCR_POS_Y(32+16*6/2)));

	m_itemlist = new GrpMenu(m_main, 6, 2, 320);
	m_itemlist->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(32+16*6+ 112/2)));


	this->addChild(m_explainbg,	0);
	this->addChild(m_explain,	1);
	this->addChild(m_equipmenu,	0);
	this->addChild(m_statlist,	0);
	this->addChild(m_itemlist,	0);

	m_curitem_type = 1;

	m_two_hand_weapon = m_main->ldb.dChar[hID-1].use_two_hand;

	refEquip();
	refStat(-1);
	refItem();

	m_equipmenu->setInControl(true);
	m_selectingitem = false; 
}

ScrMenuEquip::~ScrMenuEquip()
{

}

void ScrMenuEquip::reloadShaders()
{
	m_explain->reloadShaders();
	m_equipmenu->reloadShaders();
	m_statlist->reloadShaders();
	m_itemlist->reloadShaders();
}

void ScrMenuEquip::Draw( int framedelta )
{
	if (framedelta > 0)
		chkKeys();
}

void ScrMenuEquip::chkKeys()
{
	if (!m_selectingitem) {
		if (m_last_index != m_equipmenu->getIndex()) {
			m_last_index = m_equipmenu->getIndex();

			m_curitem_type = m_last_index + 1;
			refItem();
		}

		int result = m_equipmenu->getInput();
		if (result == GRP_MENU_CANCEL) {
			m_main->slm->setSettingScrMenu();
			m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
		} else if (result != GRP_MENU_NOTWORK) {
			m_curitem_type = result + 1;
			m_selectingitem = true;

			m_equipmenu	->setInControl(false);
			m_itemlist	->setInControl(true);

			m_last_index = -1;

			refItem();
		}
	} else {
		if (m_last_index != m_itemlist->getIndex() && m_last_index > 0) {
			m_last_index = m_itemlist->getIndex();

			int itemID = m_item_index[m_last_index];

			if (itemID <= 0)
				m_explain->setText("");
			else
				m_explain->setText(m_main->ldb.dItem[itemID-1].desc);

			refStat(itemID);
			refItem();
		}

		int result = m_itemlist->getInput();
		if (result == GRP_MENU_CANCEL || result != GRP_MENU_NOTWORK) {
			if (result != GRP_MENU_NOTWORK) {
				int itemID = -1;
				if (result != GRP_MENU_CANCEL) {
					itemID = m_item_index[result];

					MgrEquip mequip(m_main);

					mequip.equip(m_hid, itemID, m_curitem_type);

				}

				refEquip();
				refStat(-1);
				refItem();
			}

			m_selectingitem = false;

			m_equipmenu	->setInControl(true);
			m_itemlist	->setInControl(false);

			m_last_index = -1;
		}
	}
}

void ScrMenuEquip::refStat(int itemID)
{
	MgrCharacter mchar(m_main);
	LdbVoca &voca = m_main->ldb.dVoca;
	ArrayList<TString> menustrs;

	int bef_atk, aft_atk;
	int bef_def, aft_def;
	int bef_spt, aft_spt;
	int bef_agi, aft_agi;

	bef_atk = mchar.get(m_hid, ATK);
	bef_def = mchar.get(m_hid, DEF);
	bef_spt = mchar.get(m_hid, SPT);
	bef_agi = mchar.get(m_hid, AGI);

	equipTest(itemID, aft_atk, aft_def, aft_spt, aft_agi);

	menustrs.Add(mchar.getS(m_hid, NAME));
	menustrs.Add("\\C[1]" + voca.attack		+ "\\L[84]\\C[0]" + bef_atk + "\\C[1]→>\\L[105]");
	menustrs.Add("\\C[1]" + voca.defense	+ "\\L[84]\\C[0]" + bef_def + "\\C[1]→>\\L[105]");
	menustrs.Add("\\C[1]" + voca.spirit		+ "\\L[84]\\C[0]" + bef_spt + "\\C[1]→>\\L[105]");
	menustrs.Add("\\C[1]" + voca.agility	+ "\\L[84]\\C[0]" + bef_agi + "\\C[1]→>\\L[105]");

	if (itemID >= 0) {
		equipTest(itemID, aft_atk, aft_def, aft_spt, aft_agi);

		menustrs[1] = menustrs[1] + getStatColor(bef_atk, aft_atk) + aft_atk + ">";
		menustrs[2] = menustrs[2] + getStatColor(bef_def, aft_def) + aft_def + ">";
		menustrs[3] = menustrs[3] + getStatColor(bef_spt, aft_spt) + aft_spt + ">";
		menustrs[4] = menustrs[4] + getStatColor(bef_agi, aft_agi) + aft_agi + ">";
	}

	m_statlist->refreshNodes(menustrs);
}

TString ScrMenuEquip::getStatColor(int bef, int aft) {
	if (bef<aft)
		return "\\C[2]";
	else if (bef==aft)
		return "\\C[0]";
	else 
		return "\\C[3]";
}

void ScrMenuEquip::refEquip()
{
	MgrCharacter mchar(m_main);
	MgrItem mitem(m_main);
	LdbVoca &voca = m_main->ldb.dVoca;
	ArrayList<TString> menustrs;
	GlbEquipment equip = mchar.getEquip(m_hid);
	
	menustrs.Add(_ST("\\C[1]") + voca.weapon + _ST("\\C[0]\\R[60]"));
		
	if (m_two_hand_weapon)
		menustrs.Add(_ST("\\C[1]") + voca.weapon + _ST("\\C[0]\\R[60]"));
	else
		menustrs.Add(_ST("\\C[1]") + voca.shield + _ST("\\C[0]\\R[60]"));

	menustrs.Add(_ST("\\C[1]") + voca.armor + _ST("\\C[0]\\R[60]"));
	menustrs.Add(_ST("\\C[1]") + voca.helmet + _ST("\\C[0]\\R[60]"));
	menustrs.Add(_ST("\\C[1]") + voca.accessory + _ST("\\C[0]\\R[60]"));

	if (equip.weapon_id > 0)
		menustrs[0] = menustrs[0] + mitem.getName(equip.weapon_id) + _ST(">");

	if (equip.shield_id > 0)
		menustrs[1] = menustrs[1] + mitem.getName(equip.shield_id) + _ST(">");
	
	if (equip.armor_id > 0)
		menustrs[2] = menustrs[2] + mitem.getName(equip.armor_id) + _ST(">");

	if (equip.helmet_id > 0)
		menustrs[3] = menustrs[3] + mitem.getName(equip.helmet_id) + _ST(">");

	if (equip.accessory_id > 0)
		menustrs[4] = menustrs[4] + mitem.getName(equip.accessory_id) + _ST(">");

	m_equipmenu->refreshNodes(menustrs);
}

void ScrMenuEquip::refItem()
{
	ArrayList<TString> menustrs;

	MgrEquip mequip(m_main);
	MgrItem mitem(m_main);

	m_item_index.RemoveAll();

	int cnt;
	for(int i=0; i<m_main->ldb.dItem.GetLength(); i++) {
		cnt = m_main->lsdbuf.GetItem(i+1);
		if (cnt == 0)
			continue;

		LdbItem &item = m_main->ldb.dItem[i];
		if ( (item.Type != m_curitem_type) && 
			!(item.Type == 1 && m_curitem_type == 2 && m_two_hand_weapon))
				continue;

		if (!mequip.isEquipable(m_hid, i+1))
			continue;

		menustrs.Add(mitem.getName(i+1) + _ST("\\L[150]: ") + cnt + _ST(">"));
		m_item_index.Add(i+1);
	}
	menustrs.Add("Not Equip");
	m_item_index.Add(0);
	m_item_count = m_item_index.GetLength();

	m_itemlist->refreshNodes(menustrs);
}

void ScrMenuEquip::equipTest(int itemID, int &atk, int &def, int &spt, int &agi) {
	m_main->lsdbuf.BackupItem();

	GlbEquipment				backup_equip		= m_main->lsd.Character[m_hid-1].cequip;
	MgrEquip mequip(m_main);
	MgrCharacter mchar(m_main);

	mequip.equip(m_hid, itemID, m_curitem_type);

	atk = mchar.get(m_hid, ATK);
	def = mchar.get(m_hid, DEF);
	spt = mchar.get(m_hid, SPT);
	agi = mchar.get(m_hid, AGI);

	m_main->lsdbuf.RestoreItem();
	m_main->lsd.Character[m_hid-1].cequip = backup_equip;
}