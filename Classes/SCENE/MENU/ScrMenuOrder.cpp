#include "ScrMenuOrder.h"

#include "ScrMenu.h"
#include "../MAP/ScrMap.h"

ScrMenuOrder::ScrMenuOrder( GameMain *main )
{
	m_main = main;

	LdbVoca &voca = m_main->ldb.dVoca;

	ArrayList<TString> tstr;

	m_menu = new GrpMenu(m_main, 2, 1, 70);
	m_menu->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(160)));
	m_menu->setVisible(false);
	this->addChild(m_menu,	0);

	tstr.Add("결정하기");
	tstr.Add("다시하기");
	m_menu->refreshNodes(tstr);

	m_menuL = new GrpMenu(m_main, 4, 1, 88);
	m_menuL->setPosition(ccp(SCR_POS_X(112),SCR_POS_Y(88)));
	this->addChild(m_menuL,	0);

	m_menuR = new GrpMenu(m_main, 4, 1, 88);
	m_menuR->setPosition(ccp(SCR_POS_X(208),SCR_POS_Y(88)));
	this->addChild(m_menuR,	0);

	m_hero_from = m_main->lsd.State.party;
	m_hero_length = m_hero_from.GetLength();
	for(int i=0; i<4; i++)
		m_hero_selected[i] = false;

	m_menu ->setInControl(false);
	m_menuL->setInControl(true);
	m_menuR->setInControl(false);
	updateMenu();
}

ScrMenuOrder::~ScrMenuOrder()
{
	this->removeChild(m_menu);
	this->removeChild(m_menuL);
	this->removeChild(m_menuR);
}

void ScrMenuOrder::reloadShaders()
{
	m_menuR->reloadShaders();
	m_menuL->reloadShaders();
}


void ScrMenuOrder::Draw( int framedelta )
{
	if (framedelta > 0)
		chkKeys();
}

void ScrMenuOrder::chkKeys()
{
	int result, selected = 0;
	for(int i=0; i<m_hero_length; i++)
		if (m_hero_selected[i])
			selected++;

	result = m_menuL->getInput();
	if (result == GRP_MENU_CANCEL) {
		if (selected == 0) {
			m_main->slm->setSettingScrMenu();
			m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
		} else {
			int hID = m_hero_to[selected-1];

			for(int i=0; i<m_hero_length; i++)
				if (m_hero_from[i] == hID)
					m_hero_selected[i] = false;

			m_hero_to.Remove(selected-1);
			updateMenu();
		}
	} else if (result != GRP_MENU_NOTWORK) {
		if (!m_hero_selected[result]) {
			m_hero_to.Add(m_hero_from[result]);
			m_hero_selected[result] = true;

			if (selected == m_hero_length-1) {
				m_menu->setInControl(true);
				m_menu->setVisible(true);
				m_menu->setIndex(0);
				m_menuL->setInControl(false);
			}
		}
		updateMenu();
	}

	result = m_menu->getInput();
	if (result == GRP_MENU_CANCEL || result == 1) {
		for(int i=0; i<m_hero_length; i++)
			m_hero_selected[i] = false;

		m_hero_to.RemoveAll();
		m_menu->setInControl(false);
		m_menu->setVisible(false);
		m_menuL->setInControl(true);
		updateMenu();
	} else if (result == 0) {
		m_main->lsd.State.party = m_hero_to;

		ScrMap::chkHeroPic(m_main, false);

		m_main->slm->setSettingScrMenu();
		m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
	}
}

void ScrMenuOrder::updateMenu() {
	ArrayList<TString> listL, listR;

	ArrayList<LdbCharacter> &characters = m_main->ldb.dChar;

	for(int i=0; i<m_hero_from.GetLength(); i++)
		listL.Add(m_hero_selected[i]?"":characters[m_hero_from[i]-1].name);

	for(int i=0; i<m_hero_to.GetLength(); i++)
		listR.Add(characters[m_hero_to[i]-1].name);

	m_menuL->refreshNodes(listL);
	m_menuR->refreshNodes(listR);
}