#include "ScrMenuTeleport.h"
#include "../../COMMON/MgrSkill.h"
#include "../../COMMON/MgrItem.h"
#include "../../COMMON/MgrTeleport.h"

ScrMenuTeleport::ScrMenuTeleport( GameMain *main, int id, int hid )
{
	m_main = main;
	m_hid = hid;
	m_id = id;

	ArrayList<TString> maplists;
	MgrTeleport mtele(m_main);
	mtele.getTeleportList(maplists);

	m_menu = new GrpMenu(main, 4, 2, 80);
	m_menu->refreshNodes(maplists);
	m_menu->setPosition(ccp(SCR_POS_X(160), SCR_POS_Y(200)));
	this->addChild(m_menu);
}
ScrMenuTeleport::~ScrMenuTeleport()
{

}

void ScrMenuTeleport::reloadShaders()
{
	m_menu->reloadShaders();
}

void ScrMenuTeleport::Draw( int framedelta )
{
	chkKeys();
}

void ScrMenuTeleport::chkKeys()
{
	bool goBack = false;

	int result = m_menu->getInput();
	if (result == GRP_MENU_CANCEL) {
		m_main->slm->setSettingScrMenu(scr_menu_skill, m_hid);
		m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
	} else if (result != GRP_MENU_NOTWORK) {
		MgrTeleport mtele(m_main);

		mtele.moveTeleport(result);
	}
}

void ScrMenuTeleport::Init()
{

}

