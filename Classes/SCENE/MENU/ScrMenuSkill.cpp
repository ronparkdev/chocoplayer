#include "ScrMenuSkill.h"

#include "ScrMenu.h"
#include "ScrMenuUse.h"
#include "../../COMMON/MgrSkill.h"
#include "../../COMMON/MgrCharacter.h"

ScrMenuSkill::ScrMenuSkill( GameMain *main, int hID)
{
	m_main = main;
	m_hid = hID;

	int texID = IBuffer::getInstance()->get(imgSystem, m_main->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	m_explain = new GrpFontL(main);
	m_explain->setPosition(ccp(SCR_POS_X(8),SCR_POS_Y(8)));

	m_explainbg = new GrpMsgbox(m_main, 320, 32);
	m_explainbg->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(16)));

	m_state = new GrpFontL(main);
	m_state->setPosition(ccp(SCR_POS_X(8),SCR_POS_Y(40)));

	m_statebg = new GrpMsgbox(m_main, 320, 32);
	m_statebg->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(48)));		

	m_itemmenu = new GrpMenu(m_main, 10, 2, 320);
	m_itemmenu->setPosition(ccp(SCR_POS_X(160),SCR_POS_Y(152)));

	this->addChild(m_explain,	1);
	this->addChild(m_explainbg,	0);
	this->addChild(m_state,		1);
	this->addChild(m_statebg,	0);
	this->addChild(m_itemmenu,	0);

	ArrayList<TString> menustrs;
	MgrSkill mskill(main);

	m_skill_index.RemoveAll();
	m_skill_count = 0;
	for(int i=0; i<m_main->ldb.dSkill.GetLength(); i++) {
		if (m_main->lsdbuf.GetSkill(hID, i+1)) {
			if (mskill.isUseable(m_hid, i, false))
				menustrs.Add(mskill.getName(i+1) + _ST("\\L[150]: ") + mskill.getSpendMP(hID, i+1) + _ST(">"));
			else
				menustrs.Add("\\C[3]" + mskill.getName(i+1) + _ST("\\L[150]: ") + mskill.getSpendMP(hID, i+1) + _ST(">"));
			m_skill_index.Add(i+1);
			m_skill_count++;
		}
	}

	m_itemmenu->refreshNodes(menustrs);
	m_itemmenu->setInControl(true);

	m_last_index = -1;

	refStat();
}

ScrMenuSkill::~ScrMenuSkill()
{

}

void ScrMenuSkill::reloadShaders()
{
	m_explain->reloadShaders();
	m_state->reloadShaders();
	m_itemmenu->reloadShaders();
}

void ScrMenuSkill::Draw( int framedelta )
{
	if (framedelta > 0)
		chkKeys();
}

void ScrMenuSkill::chkKeys()
{
	int result = m_itemmenu->getInput();
	if (result == GRP_MENU_CANCEL) {
		m_main->slm->setSettingScrMenu();
		m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
	}else
	if (result != GRP_MENU_NOTWORK) {
		int skillid = m_skill_index[result];
		MgrSkill mskill(m_main);

		if (mskill.isUseable(m_hid, skillid)) {
			if (mskill.isNotSelect(skillid))
				mskill.useSkill(0, 0, skillid);
			else{
				m_main->slm->setSettingScrMenu(scr_menu_skill, m_hid, skillid, true);
				m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
			}
		}
	}

	if (m_last_index != m_itemmenu->getIndex() && m_skill_count > 0) {
		m_last_index = m_itemmenu->getIndex();

		MgrSkill mskill(m_main);
		m_explain->setText(mskill.getDesc(m_skill_index[m_last_index]));
	}

}

void ScrMenuSkill::refStat()
{
	int texID = IBuffer::getInstance()->get(imgSystem, m_main->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	LdbVoca *voca = &(m_main->ldb.dVoca);

	MgrCharacter mchar(m_main);
	TString str;
	str = mchar.getS(m_hid, NAME) + 
		  "\\R[80]\\C[1]"	+ voca->lvl_short  + "\\C[0] " + mchar.get(m_hid, LEVEL) + 
		  "\\R[125]"		+ mchar.getStateStr(m_hid) +
		  "\\R[180]\\C[1]"	+ voca->hp_short +
		  "\\L[220]\\C[0]"	+ mchar.get(m_hid, HP) + "/"
		  "\\L[245]\\C[0]"	+ mchar.get(m_hid, MAXHP) +
		  "\\R[250]\\C[1]"	+ voca->mp_short +
		  "\\L[288]\\C[0]"	+ mchar.get(m_hid, MP) + "/"
		  "\\L[307]\\C[0]"	+ mchar.get(m_hid, MAXMP);

	m_state->setText(str, pTex);
}

