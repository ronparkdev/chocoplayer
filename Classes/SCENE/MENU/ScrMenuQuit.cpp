#include "ScrMenuQuit.h"

#include "ScrMenu.h"
#include "../TITLE/ScrTitle.h"

ScrMenuQuit::ScrMenuQuit( GameMain *main )
{	
	m_main = main;

	LdbVoca &voca = m_main->ldb.dVoca;

	ArrayList<TString> tstr;

	m_menu = new GrpMenu(m_main, 2, 1, 52);
	m_menu->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(144)));
	this->addChild(m_menu,	0);

	tstr.Add(voca.yes);
	tstr.Add(voca.no);
	m_menu->refreshNodes(tstr);


	m_caption = new GrpMenu(m_main, 1, 1, 130);
	m_caption->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(68)));
	this->addChild(m_caption,	0);
	
	tstr.RemoveAll();
	tstr.Add(voca.exit_game_message);
	m_caption->refreshNodes(tstr);


	m_menu->setInControl(true);
	m_caption->setInControl(false);
}

ScrMenuQuit::~ScrMenuQuit()
{
	this->removeChild(m_menu);
	this->removeChild(m_caption);
}

void ScrMenuQuit::reloadShaders()
{
	m_menu->reloadShaders();
	m_caption->reloadShaders();
}

void ScrMenuQuit::Draw( int framedelta )
{
	if (framedelta > 0)
		chkKeys();
}

void ScrMenuQuit::chkKeys()
{
	int result = m_menu->getInput();
	if (result == GRP_MENU_CANCEL || result == 1) {
		m_main->slm->setSettingScrMenu();
		m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
	} else if (result == 0) {
		m_main->slm->setScreen(scr_title, MENU_CHANGE_TIME);
	}
}
