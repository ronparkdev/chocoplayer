#include "ScrMenuUse.h"

#include "ScrMenuItem.h"
#include "ScrMenuSkill.h"
#include "../../COMMON/ScreenCommon.h"
#include "../../COMMON/MgrItem.h"
#include "../../COMMON/MgrSkill.h"
#include "../../COMMON/MgrCharacter.h"
#include "../../COMMON/MgrState.h"
#include "../../GRAPHIC/OBJECT/GrpFace.h"

ScrMenuUse::ScrMenuUse( GameMain *main, int hID, int iID, bool isSkill) {
	m_main = main;

	m_isskill = isSkill;
	m_hid = hID;
	m_id = iID;
}

void ScrMenuUse::Init()
{

	int texID = IBuffer::getInstance()->get(imgSystem, m_main->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	m_namebg = new GrpMsgbox(m_main, 136, 32);
	m_namebg->setPosition(ccp(SCR_POS_X(0 + 136/2), SCR_POS_Y(0 + 32/2)));
	this->addChild(m_namebg, 0);

	m_name = new GrpFontL(m_main);
	m_name->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(8)));
	this->addChild(m_name, 2);


	m_countbg = new GrpMsgbox(m_main, 136, 32);
	m_countbg->setPosition(ccp(SCR_POS_X(0 + 136/2), SCR_POS_Y(32 + 32/2)));
	this->addChild(m_countbg, 0);

	m_count = new GrpFontL(m_main);
	m_count->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(40)));
	this->addChild(m_count, 2);

	m_herobg = new GrpMsgbox(m_main, 184, 240);
	m_herobg->setPosition(ccp(SCR_POS_X(136 + 184/2), SCR_POS_Y(0 + 240/2)));
	this->addChild(m_herobg, 0);

	for(int i=0; i<4; i++) {
		m_hName[i]	= new GrpFontL(m_main);
		m_hLev[i]	= new GrpFontL(m_main);
		m_hState[i]	= new GrpFontL(m_main);
		m_hHP[i]	= new GrpFontL(m_main);
		m_hMP[i]	= new GrpFontL(m_main);
		m_hFace[i]	= new GrpFace(m_main);

		this->addChild(m_hName[i],	2);
		this->addChild(m_hLev[i],	2);
		this->addChild(m_hState[i],	2);
		this->addChild(m_hHP[i],	2);
		this->addChild(m_hMP[i],	2);
		this->addChild(m_hFace[i],	2);
	}

	m_selected_hero = 0;

	LdbVoca &voca = m_main->ldb.dVoca;

	MgrItem mitem(m_main);
	MgrSkill mskill(m_main);
	MgrState mstate(m_main);

	if (!m_isskill) {
		m_name->setText(mitem.getName(m_id), pTex);
		m_isAllEffect = mitem.isAllEffect(m_id);

	} else {
		m_name->setText(mskill.getName(m_id), pTex);
		m_isAllEffect = mskill.isAllEffect(m_id);
	}

	if (!m_isAllEffect) {
		m_selection = new GrpSelection(pTex, 120, 48);
		m_selection->setPosition(ccp(SCR_POS_X(196 + 120/2), SCR_POS_Y(8 + 48/2)));
	} else {
		m_selection = new GrpSelection(pTex, 120, -8 + 56*mstate.getPartyLen());
		m_selection->setPosition(ccp(SCR_POS_X(196 + 120/2), SCR_POS_Y(8 + (-8 + 56 * mstate.getPartyLen())/2)));
	}
	this->addChild(m_selection, 1);


	refHero();
	refCount();
}

ScrMenuUse::~ScrMenuUse()
{

}

void ScrMenuUse::reloadShaders()
{
	m_name->reloadShaders();
	m_count->reloadShaders();

	for(int i=0; i<4; i++) {
		m_hName[i]->reloadShaders();
		m_hLev[i]->reloadShaders();
		m_hState[i]->reloadShaders();
		m_hHP[i]->reloadShaders();
		m_hMP[i]->reloadShaders();
	}
}

void ScrMenuUse::Draw( int framedelta )
{
	if (framedelta > 0)
		chkKeys();
}

void ScrMenuUse::chkKeys()
{
	int key_esc = m_main->ctl->getKey(rkc_esc);
	int key_enter = m_main->ctl->getKey(rkc_enter);
	int key_arrow = m_main->ctl->getKey(rkc_arrow);

	if (m_key_esc_last != key_esc && key_esc > 0) {
		if (!m_isskill) {
			m_main->slm->setSettingScrMenu(scr_menu_item);
			m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
		} else {
			m_main->slm->setSettingScrMenu(scr_menu_skill, m_hid);
			m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
		}
	}

	if (m_key_arrow_last != key_arrow && key_arrow > 0 && !m_isAllEffect) {
		if (key_arrow == rkca_up)
			m_selected_hero = (m_selected_hero + (m_party_count-1)) % m_party_count;
		else 
			if (key_arrow == rkca_down)
				m_selected_hero = (m_selected_hero + 1) % m_party_count;

		m_selection->setPositionY(SCR_POS_Y(8 + 48/2 + m_selected_hero * 56));
	}

	MgrItem mitem(m_main);
	MgrSkill mskill(m_main);
	MgrState mstate(m_main);

	bool something_did;
	if (m_key_enter_last != key_enter && key_enter > 0) {
		int targetid;
			
		if (!m_isAllEffect)
			targetid = m_main->lsd.State.party[m_selected_hero];
		else
			targetid = 0;

		if (!m_isskill) {
			LdbItem &item = m_main->ldb.dItem[m_id-1];
			if (item.Type != 9)
				mitem.useItem(targetid, m_id, false);
			else{
				something_did = mskill.useSkill(targetid, targetid, item.skill_id, false);
			
				if (something_did)
					mitem.useItem(targetid, m_id, true);
			}
		}else
			something_did = mskill.useSkill(m_hid, targetid, m_id, false);


		if (something_did) {
			refHero();
			refCount();
		}
	}

	m_key_esc_last = key_esc;
	m_key_enter_last = key_enter;
	m_key_arrow_last = key_arrow;
}

void ScrMenuUse::refHero()
{
	int texID = IBuffer::getInstance()->get(imgSystem, m_main->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);
	
	LdbVoca *voca = &(m_main->ldb.dVoca);
	MgrCharacter mchar(m_main);
	ArrayList<short> &party = m_main->lsd.State.party;
	m_party = party;
	m_party_count = party.GetLength();

	for(int i=0; i<party.GetLength() && i<4; i++) {
		int id = party[i];
		int posY = 8 + 56*i;

		m_hName[i]	->setText(mchar.getS(id, NAME),	pTex);
		m_hName[i]	->setPosition(ccp(SCR_POS_X(200), SCR_POS_Y(posY)));

		m_hLev[i]	->setText(_ST("\\C[1]") + voca->lvl_short  + "\\C[0] " + mchar.get(id, LEVEL),	pTex);
		m_hLev[i]	->setPosition(ccp(SCR_POS_X(200), SCR_POS_Y(posY+16)));

		m_hState[i]	->setText( mchar.getS(id, STATE),	pTex);
		m_hState[i]	->setPosition(ccp(SCR_POS_X(200), SCR_POS_Y(posY+32)));

		m_hHP[i]	->setText(_ST("\\C[1]") + voca->hp_short  + _ST("\\C[0] \\L[45]") + mchar.get(id, HP) + "/>\\L[68]" + mchar.get(id,MAXHP),	pTex);
		m_hHP[i]	->setPosition(ccp(SCR_POS_X(246), SCR_POS_Y(posY+16)));

		m_hMP[i]	->setText(_ST("\\C[1]") + voca->mp_short  + _ST("\\C[0] \\L[45]") + mchar.get(id, MP) + "/>\\L[68]" + mchar.get(id,MAXMP),	pTex);
		m_hMP[i]	->setPosition(ccp(SCR_POS_X(246), SCR_POS_Y(posY+32)));

		LsdCharacter &c = m_main->lsd.Character[id-1];

		m_hFace[i]	->set(c.face_name, c.face_index);
		m_hFace[i]	->setPosition(ccp(SCR_POS_X(144+24 + (c.is_back_attack?8:0)), SCR_POS_Y(8+56*i+24)));
	}
}

void ScrMenuUse::refCount()
{
	int texID = IBuffer::getInstance()->get(imgSystem, m_main->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);
	LdbVoca *voca = &(m_main->ldb.dVoca);

	MgrItem mitem(m_main);
	MgrSkill mskill(m_main);

	if (!m_isskill) {
		m_count->setText("\\C[1]"+voca->possessed_items+"\\L[116]\\C[0]"+mitem.getItemCount(m_id));
	} else {
		m_count->setText("\\C[1]"+voca->mp_cost+"\\L[116]\\C[0]"+mskill.getSpendMP(m_hid, m_id));
	}
}