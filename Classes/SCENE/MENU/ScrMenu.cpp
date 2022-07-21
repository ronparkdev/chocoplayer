#include "ScrMenu.h"

#include "ScrMenuItem.h"
#include "ScrMenuSkill.h"
#include "ScrMenuEquip.h"
#include "ScrMenuState.h"
#include "ScrMenuOrder.h"
#include "ScrMenuQuit.h"
#include "../SAVE/ScrSave.h"
#include "../MAP/ScrMap.h"
#include "../../COMMON/MgrCharacter.h"
#include "../../GRAPHIC/OBJECT/GrpFace.h"


ScrMenu::ScrMenu( GameMain *main )
{
	m_main = main;

	LdbVoca *voca = &(m_main->ldb.dVoca);
	if (m_main->getVersion() == RPG2000) {
		m_menustr[0] = "Item";
		m_menustr[1] = "Skill";
		m_menustr[2] = "Equip";
		m_menustr[3] = "Save";
		m_menustr[8] = "Quit";
	} else {
		m_menustr[0] = voca->command_item;
		m_menustr[1] = voca->command_skill;
		m_menustr[2] = voca->menu_equipment;
		m_menustr[3] = voca->menu_save;
		m_menustr[8] = voca->menu_quit;
	}
	m_menustr[4] = voca->row;
	m_menustr[5] = voca->status;
	m_menustr[6] = voca->order;
	m_menustr[7] = voca->wait_off;
	m_menustr[9] = voca->wait_on;


	int texID = IBuffer::getInstance()->get(imgSystem, m_main->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	InitMenu();
	m_leftmenu = new GrpMenu(m_main, m_mlistlen, 1, 88);
	m_leftmenu->setPosition(ccp(SCR_POS_X(44), SCR_POS_Y((m_mlistlen+1)*16/2)));
	m_leftmenu->setInControl(true);
	this->addChild(m_leftmenu,	0);

	updateMenu();
	
	m_rightbg = new GrpMsgbox(m_main, 232, 240);
	m_rightbg->setPosition(ccp(SCR_POS_X(88 + 232/2), SCR_POS_Y(0 + 240/2)));
	this->addChild(m_rightbg,	0);

	m_moneybg = new GrpMsgbox(m_main, 88, 32);
	m_moneybg->setPosition(ccp(SCR_POS_X(0 + 88/2), SCR_POS_Y(208 + 32/2)));
	this->addChild(m_moneybg,	0);

	m_money = new GrpFontL(m_main);//208-16)/2
	m_money->setText(_ST("\\L[76]") + m_main->lsd.State.money + _ST(" ") + m_main->ldb.dVoca.gold + _ST(">"));
	m_money->setPosition(ccp(SCR_POS_X(0 + 8), SCR_POS_Y(208 + 8)));
	this->addChild(m_money, 1);

	m_selection = new GrpSelection(pTex, 168, 48);
	m_selection->setVisible(false);
	this->addChild(m_selection, 1);

	for(int i=0; i<4; i++) {
		m_hName[i]	= new GrpFontL(m_main);
		m_hTitle[i]	= new GrpFontL(m_main);
		m_hLev[i]	= new GrpFontL(m_main);
		m_hExp[i]	= new GrpFontL(m_main);
		m_hState[i]	= new GrpFontL(m_main);
		m_hHP[i]	= new GrpFontL(m_main);
		m_hMP[i]	= new GrpFontL(m_main);
		m_hFace[i]	= new GrpFace(m_main);

		this->addChild(m_hName[i],	2);
		this->addChild(m_hTitle[i],	2);
		this->addChild(m_hLev[i],	2);
		this->addChild(m_hExp[i],	2);
		this->addChild(m_hState[i],	2);
		this->addChild(m_hHP[i],	2);
		this->addChild(m_hMP[i],	2);
		this->addChild(m_hFace[i],	2);
	}

	ArrayList<short> &party = m_main->lsd.State.party;
	m_party = party;
	m_party_count = party.GetLength();

	m_selected_menu = -1;
	m_selected_hero = 0;

	m_main->lsd.System.screen = scr_menu;		
}

void ScrMenu::reloadShaders()
{
	m_leftmenu->reloadShaders();
	m_money->reloadShaders();
	for(int i=0; i<4; i++) {
		m_hName[i]->reloadShaders();
		m_hTitle[i]->reloadShaders();
		m_hLev[i]->reloadShaders();
		m_hExp[i]->reloadShaders();
		m_hState[i]->reloadShaders();
		m_hHP[i]->reloadShaders();
		m_hMP[i]->reloadShaders();
	}
}

void ScrMenu::Init() {
	//InitMenu();
	InitHeros();
}

void ScrMenu::InitMenu()
{
	m_mlistlen = 0;
	if (m_main->getVersion() == RPG2000) {
		m_mlistid[m_mlistlen++] = 0;
		m_mlistid[m_mlistlen++] = 1;
		m_mlistid[m_mlistlen++] = 2;
		m_mlistid[m_mlistlen++] = 3;
		m_mlistid[m_mlistlen++] = 5;
		m_mlistid[m_mlistlen++] = 8;
	} else {
		ArrayList<short> &cmds = m_main->ldb.dSys.menu_commands;
		for(int i=0; i<cmds.GetLength(); i++)
			m_mlistid[i] = cmds[i]-1;
		m_mlistlen += cmds.GetLength();
		m_mlistid[m_mlistlen++] = 8;
	}
}

void ScrMenu::updateMenu() {
	ArrayList<TString> menustrs;
	for(int i=0; i<m_mlistlen; i++) {
		int id = m_mlistid[i];
		if (id == 7 && m_main->lsd.System.battle_action_wait)
			id = 9;

		menustrs.Add(m_menustr[id]);
	}

	m_leftmenu->refreshNodes(menustrs);
}

void ScrMenu::InitHeros()
{
	LdbVoca *voca = &(m_main->ldb.dVoca);
	int texID = IBuffer::getInstance()->get(imgSystem, m_main->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	MgrCharacter mchar(m_main);
	for(int i=0; i<m_party_count && i<4; i++) {
		int id = m_party[i];
		int posY = 8 + 56*i;

		m_hName[i]	->setText(mchar.getS(id, NAME),	pTex);
		m_hName[i]	->setPosition(ccp(SCR_POS_X(152), SCR_POS_Y(posY)));

		m_hTitle[i]	->setText(mchar.getS(id, TITLE),	pTex);
		m_hTitle[i]	->setPosition(ccp(SCR_POS_X(246), SCR_POS_Y(posY)));

		m_hLev[i]	->setText(_ST("\\C[1]") + voca->lvl_short  + "\\C[0] " + mchar.get(id, LEVEL),	pTex);
		m_hLev[i]	->setPosition(ccp(SCR_POS_X(152), SCR_POS_Y(posY+16)));

		m_hExp[i]	->setText(_ST("\\L[44]") + mchar.get(id, EXP) + _ST("/\\L[88]") + mchar.get(id, NEXTEXP),	pTex);
		m_hExp[i]	->setPosition(ccp(SCR_POS_X(152), SCR_POS_Y(posY+32)));

		m_hState[i]	->setText( mchar.getS(id, STATE),	pTex);
		m_hState[i]	->setPosition(ccp(SCR_POS_X(190), SCR_POS_Y(posY+16)));

		m_hHP[i]	->setText(_ST("\\C[1]") + voca->hp_short  + _ST("\\C[0] \\L[45]") + mchar.get(id, HP) + "/>\\L[68]" + mchar.get(id,MAXHP),	pTex);
		m_hHP[i]	->setPosition(ccp(SCR_POS_X(246), SCR_POS_Y(posY+16)));

		m_hMP[i]	->setText(_ST("\\C[1]") + voca->mp_short  + _ST("\\C[0] \\L[45]") + mchar.get(id, MP) + "/>\\L[68]" + mchar.get(id,MAXMP),	pTex);
		m_hMP[i]	->setPosition(ccp(SCR_POS_X(246), SCR_POS_Y(posY+32)));

		LsdCharacter &c = m_main->lsd.Character[id-1];
		m_hFace[i]	->set(c.face_name, c.face_index);
		m_hFace[i]	->setPosition(ccp(SCR_POS_X(92+24 + (c.is_back_attack?8:0)), SCR_POS_Y(8+24+56*i)));
	}
}

ScrMenu::~ScrMenu()
{
	this->removeAllChildren();
}

void ScrMenu::Draw( int framedelta )
{
	if (framedelta > 0)
		chkKeys();
}

void ScrMenu::chkKeys()
{
	int key_esc = m_main->ctl->getKey(rkc_esc);
	int key_enter = m_main->ctl->getKey(rkc_enter);
	int key_arrow = m_main->ctl->getKey(rkc_arrow);

	if (m_key_esc_last != key_esc && key_esc > 0) {
		if (m_selected_menu == -1) {
			m_main->slm->setSettingScrMap();
			m_main->slm->setScreen(scr_map, MENU_CHANGE_TIME);
		} else {
			m_selected_menu = -1;
			refreshSelection();
		}
	}

	if (m_key_arrow_last != key_arrow && key_arrow > 0 && m_selected_menu != -1) {
		if (key_arrow == rkca_up)
			m_selected_hero = (m_selected_hero + (m_party_count-1)) % m_party_count;
		else 
		if (key_arrow == rkca_down)
			m_selected_hero = (m_selected_hero + 1) % m_party_count;
		
		refreshSelection();
	}

	if (m_key_enter_last != key_enter && key_enter > 0 && m_selected_menu != -1) {
		switch(m_selected_menu) {
			case 1://skill
				m_main->slm->setSettingScrMenu(scr_menu_skill, m_party[m_selected_hero]);
				m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
				break;

			case 2://Equip
				m_main->slm->setSettingScrMenu(scr_menu_equip, m_party[m_selected_hero]);
				m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
				break;

			case 4://row
				{
					int hID = m_party[m_selected_hero];
					
					m_main->lsd.Character[hID-1].is_back_attack  
						= !m_main->lsd.Character[hID-1].is_back_attack;

					InitHeros();
				}
				break;

			case 5://Status
				m_main->slm->setSettingScrMenu(scr_menu_state, m_party[m_selected_hero]);
				m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
				break;
		}
	}

	m_key_esc_last = key_esc;
	m_key_enter_last = key_enter;
	m_key_arrow_last = key_arrow;

	if (m_selected_menu == -1) {
		int result = m_leftmenu->getInput();
		if (result != GRP_MENU_NOTWORK) {
			switch(m_mlistid[result]) {
				case 1://skill
				case 2://Equip
				case 4://row
				case 5://Status
					m_selected_menu = m_mlistid[result];
					m_selected_hero = 0;
					refreshSelection();
					break;

				case 0://item
					m_main->slm->setSettingScrMenu(scr_menu_item);
					m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
					break;

				case 3://Save
					m_main->slm->setSettingScrSave(true, false);
					m_main->slm->setScreen(scr_save, MENU_CHANGE_TIME);
					break;

				case 6://Order
					m_main->slm->setSettingScrMenu(scr_menu_order);
					m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);

					break;

				case 7://WaitOn
					m_main->lsd.System.battle_action_wait 
						= !m_main->lsd.System.battle_action_wait;
					
					updateMenu();
					break;

				case 8://Quit
					m_main->slm->setSettingScrMenu(scr_menu_quit);
					m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
					break;
			}
		}
	}
}

void ScrMenu::refreshSelection() {
	bool visible = (m_selected_menu != -1);

	m_leftmenu->setInControl(!visible);
	
	m_selection->setVisible(visible);
	m_selection->setPosition(ccp(SCR_POS_X(148 + 168/2), SCR_POS_Y(8 + 56*m_selected_hero + 48/2)));
}
