#include "ScrMenuItem.h"

#include "ScrMenu.h"
#include "ScrMenuUse.h"
#include "../../COMMON/MgrItem.h"

ScrMenuItem::ScrMenuItem( GameMain *main )
{
	m_main = main;

	int texID = IBuffer::getInstance()->get(imgSystem, IMAGE_BUFFER_NOALPHA_PRESTR + m_main->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	m_explainbg = new GrpMsgbox(m_main,320, 32);
	m_explainbg->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(16)));	

	m_explain = new GrpFontL(main);
	m_explain->setPosition(ccp(SCR_POS_X(8),SCR_POS_Y(8)));

	m_itemmenu = new GrpMenu(m_main, 12, 2, 320);
	m_itemmenu->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(136)));

	this->addChild(m_explainbg,	0);
	this->addChild(m_explain,	0);
	this->addChild(m_itemmenu,	0);

	refreshItems();

	m_last_index = -1;
}

ScrMenuItem::~ScrMenuItem()
{

}

void ScrMenuItem::reloadShaders()
{
	m_explain->reloadShaders();
	m_itemmenu->reloadShaders();
}

void ScrMenuItem::Draw( int framedelta )
{
	if (framedelta > 0)
		chkKeys();
}

void ScrMenuItem::chkKeys()
{
	MgrItem mitem(m_main);
	int result = m_itemmenu->getInput();
	if (result == GRP_MENU_CANCEL) {
		m_main->slm->setSettingScrMenu();
		m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
	}else
	if (result != GRP_MENU_NOTWORK) {
		int itemid = m_item_index[result];

		if (mitem.isUseable(itemid, false)) {
			if (mitem.isNotSelect(itemid)) {
				mitem.useItem(1, itemid, true);
				refreshItems();
			} else {
				m_main->slm->setSettingScrMenu(scr_menu_use, 0, itemid, false);
				m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
			}
		}
	}

	if (m_last_index != m_itemmenu->getIndex() && m_item_count > 0) {
		m_last_index = m_itemmenu->getIndex();

		int itemID = m_item_index[m_last_index];

		m_explain->setText(mitem.getDesc(itemID));
	}	
}

void ScrMenuItem::refreshItems() {
	ArrayList<TString> menustrs;

	m_item_index.RemoveAll();
	m_item_count = 0;

	MgrItem mitem(m_main);

	int cnt;
	for(int i=0; i<m_main->ldb.dItem.GetLength(); i++) {
		cnt = m_main->lsdbuf.GetItem(i+1);
		if (cnt>0) {
			if (mitem.isUseable(i+1, false))
				menustrs.Add(mitem.getName(i+1) + _ST("\\L[150]: ") + cnt);
			else
				menustrs.Add("\\C[3]" + mitem.getName(i+1) + _ST("\\L[150]: ") + cnt);

			m_item_index.Add(i+1);

			m_item_count++;
		}
	}

	m_itemmenu->refreshNodes(menustrs);
	m_itemmenu->setInControl(true);
}
