#include "ScrMenuState.h"

#include "ScrMenu.h"
#include "../../COMMON/MgrCharacter.h"
#include "../../COMMON/MgrEquip.h"
#include "../../GRAPHIC/OBJECT/GrpFace.h"

ScrMenuState::ScrMenuState( GameMain *main, int hid )
{
	m_main = main;
	m_hid = hid;

	int texID = IBuffer::getInstance()->get(imgSystem, IMAGE_BUFFER_NOALPHA_PRESTR + m_main->lsd.System.system_name); 
	CCTexture2D *pTex = IBuffer::getInstance()->get(imgSystem, texID);

	m_leftbg = new GrpMsgbox(m_main, 124, 208);
	m_leftbg->setPosition(ccp(SCR_POS_X(124/2), SCR_POS_Y(208/2)));
	this->addChild(m_leftbg,	0);

	m_moneybg = new GrpMsgbox(m_main, 124, 32);
	m_moneybg->setPosition(ccp(SCR_POS_X(124/2), SCR_POS_Y(208 + 32/2)));
	this->addChild(m_moneybg,	0);

	m_righttopbg = new GrpMsgbox(m_main, 196, 64);
	m_righttopbg->setPosition(ccp(SCR_POS_X(124 + 196/2), SCR_POS_Y(0 + 64/2)));
	this->addChild(m_righttopbg,	0);

	m_rightmidbg = new GrpMsgbox(m_main, 196, 80);
	m_rightmidbg->setPosition(ccp(SCR_POS_X(124 + 196/2), SCR_POS_Y(64 + 80/2)));
	this->addChild(m_rightmidbg,	0);

	m_rightbotbg = new GrpMsgbox(m_main, 196, 96);
	m_rightbotbg->setPosition(ccp(SCR_POS_X(124 + 196/2), SCR_POS_Y(144 + 96/2)));
	this->addChild(m_rightbotbg,	0);

	

	LdbVoca *voca = &(m_main->ldb.dVoca);
	MgrCharacter mchar(m_main);
	MgrEquip mequip(m_main);
	int id = m_hid; 

	LsdCharacter &c = m_main->lsd.Character[id-1];
	m_hFace = new GrpFace(m_main);
	m_hFace	->set(c.face_name, c.face_index);
	m_hFace	->setPosition(ccp(SCR_POS_X(8+24), SCR_POS_Y(8+24)));
	this->addChild(m_hFace,	2);

	m_hPos	= new GrpFontL(m_main);
	m_hPos	->setText("\\L[108]\\C[1]전위",	pTex);
	m_hPos	->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(8)));
	this->addChild(m_hPos,	2);

	
	m_cName	= new GrpFontL(m_main);
	m_cName	->setText("\\C[1]이름",	pTex);
	m_cName	->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(56)));
	this->addChild(m_cName,	2);

	m_hName	= new GrpFontL(m_main);
	m_hName	->setText(mchar.getS(id, NAME),	pTex);
	m_hName	->setPosition(ccp(SCR_POS_X(42), SCR_POS_Y(72)));
	this->addChild(m_hName,	2);


	m_cJob	= new GrpFontL(m_main);
	m_cJob	->setText("\\C[1]직책",	pTex);
	m_cJob	->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(88)));
	this->addChild(m_cJob,	2);
	
	if (mchar.get(id, JOB) > 0) {
		m_hJob	= new GrpFontL(m_main);
		m_hJob	->setText(mchar.getS(id, JOB),	pTex);
		m_hJob	->setPosition(ccp(SCR_POS_X(42), SCR_POS_Y(104)));
		this->addChild(m_hJob,	2);
	}


	m_cTitle	= new GrpFontL(m_main);
	m_cTitle	->setText("\\C[1]별명",	pTex);
	m_cTitle	->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(120)));
	this->addChild(m_cTitle,	2);

	m_hTitle	= new GrpFontL(m_main);
	m_hTitle	->setText(mchar.getS(id, TITLE),	pTex);
	m_hTitle	->setPosition(ccp(SCR_POS_X(42), SCR_POS_Y(136)));
	this->addChild(m_hTitle,	2);


	m_cState	= new GrpFontL(m_main);
	m_cState	->setText("\\C[1]상태",	pTex);
	m_cState	->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(152)));
	this->addChild(m_cState,	2);

	m_hState	= new GrpFontL(m_main);
	m_hState	->setText( mchar.getS(id, STATE),	pTex);
	m_hState	->setPosition(ccp(SCR_POS_X(42), SCR_POS_Y(168)));
	this->addChild(m_hState,	2);

	m_hLev	= new GrpFontL(m_main);
	m_hLev	->setText(_ST("\\C[1]") + voca->level  + "\\C[0] " + mchar.get(id, LEVEL),	pTex);
	m_hLev	->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(184)));
	this->addChild(m_hLev,	2);


	//Money
	m_money = new GrpFontL(m_main);
	m_money->setText(_ST("\\L[108]") + m_main->lsd.State.money + _ST(" ") + m_main->ldb.dVoca.gold + _ST(">"));
	m_money->setPosition(ccp(SCR_POS_X(8), SCR_POS_Y(216)));
	this->addChild(m_money, 2);

	//Hp,Mp, Exp
	m_hHP	= new GrpFontL(m_main);
	m_hHP	->setText(_ST("\\C[1]") + voca->hp_short  + _ST("\\C[0]\\L[100]") + mchar.get(id, HP) + "/>\\L[180]" + mchar.get(id,MAXHP),	pTex);
	m_hHP	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(8)));
	this->addChild(m_hHP,	2);

	m_hMP	= new GrpFontL(m_main);
	m_hMP	->setText(_ST("\\C[1]") + voca->mp_short  + _ST("\\C[0]\\L[100]") + mchar.get(id, MP) + "/>\\L[180]" + mchar.get(id,MAXMP),	pTex);
	m_hMP	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(24)));
	this->addChild(m_hMP,	2);

	m_hExp	= new GrpFontL(m_main);
	m_hExp	->setText(_ST("\\C[1]") + voca->exp_short  + _ST("\\C[0]\\L[100]") + mchar.get(id, EXP) + _ST("/\\L[180]") + mchar.get(id, NEXTEXP),	pTex);
	m_hExp	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(40)));
	this->addChild(m_hExp,	2);

	//Stat
	m_hAtk	= new GrpFontL(m_main);
	m_hAtk	->setText(_ST("\\C[1]") + voca->attack + _ST("\\R[60]\\C[0]") + mchar.get(id, ATK),	pTex);
	m_hAtk	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(72)));
	this->addChild(m_hAtk,	2);

	m_hDef	= new GrpFontL(m_main);
	m_hDef	->setText(_ST("\\C[1]") + voca->defense + _ST("\\R[60]\\C[0]") + mchar.get(id, DEF),	pTex);
	m_hDef	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(88)));
	this->addChild(m_hDef,	2);

	m_hSpt	= new GrpFontL(m_main);
	m_hSpt	->setText(_ST("\\C[1]") + voca->spirit + _ST("\\R[60]\\C[0]") + mchar.get(id, SPT),	pTex);
	m_hSpt	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(104)));
	this->addChild(m_hSpt,	2);

	m_hAgi	= new GrpFontL(m_main);
	m_hAgi	->setText(_ST("\\C[1]") + voca->agility + _ST("\\R[60]\\C[0]") + mchar.get(id, AGI),	pTex);
	m_hAgi	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(120)));
	this->addChild(m_hAgi,	2);

	//Equipment
	m_eWeapon	= new GrpFontL(m_main);
	m_eWeapon	->setText(_ST("\\C[1]") + voca->weapon	+ _ST("\\R[60]\\C[0]") + mequip.getName(id, 1),	pTex);
	m_eWeapon	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(152)));
	this->addChild(m_eWeapon,	2);

	m_eShield	= new GrpFontL(m_main);
	m_eShield	->setText(_ST("\\C[1]") + voca->shield	+ _ST("\\R[60]\\C[0]") + mequip.getName(id, 2),	pTex);
	m_eShield	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(168)));
	this->addChild(m_eShield,	2);

	m_eArmor	= new GrpFontL(m_main);
	m_eArmor	->setText(_ST("\\C[1]") + voca->armor	+ _ST("\\R[60]\\C[0]") + mequip.getName(id, 3),	pTex);
	m_eArmor	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(184)));
	this->addChild(m_eArmor,	2);

	m_eHelmet	= new GrpFontL(m_main);
	m_eHelmet	->setText(_ST("\\C[1]") + voca->helmet	+ _ST("\\R[60]\\C[0]") + mequip.getName(id, 4),	pTex);
	m_eHelmet	->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(200)));
	this->addChild(m_eHelmet,	2);

	m_eAcc		= new GrpFontL(m_main);
	m_eAcc		->setText(_ST("\\C[1]") + voca->accessory+ _ST("\\R[60]\\C[0]") + mequip.getName(id, 5),	pTex);
	m_eAcc		->setPosition(ccp(SCR_POS_X(132), SCR_POS_Y(216)));
	this->addChild(m_eAcc,	2);

}

ScrMenuState::~ScrMenuState()
{

}

void ScrMenuState::reloadShaders()
{
	m_cName->reloadShaders();
	m_hName->reloadShaders();

	m_cJob->reloadShaders();
	m_hJob->reloadShaders();

	m_cTitle->reloadShaders();
	m_hTitle->reloadShaders();

	m_cState->reloadShaders();
	m_hState->reloadShaders();

	m_hLev->reloadShaders();

	m_money->reloadShaders();

	m_hHP->reloadShaders();
	m_hMP->reloadShaders();
	m_hExp->reloadShaders();

	m_hAtk->reloadShaders();
	m_hDef->reloadShaders();
	m_hSpt->reloadShaders();
	m_hAgi->reloadShaders();

	m_eWeapon->reloadShaders();
	m_eShield->reloadShaders();
	m_eArmor->reloadShaders();
	m_eHelmet->reloadShaders();
	m_eAcc->reloadShaders();
}

void ScrMenuState::Draw( int framedelta )
{
	if (framedelta > 0)
		chkKeys();
}

void ScrMenuState::chkKeys()
{
	int key_esc = m_main->ctl->getKey(rkc_esc);

	if (m_key_esc_last != key_esc && key_esc > 0) {
		m_main->slm->setSettingScrMenu();
		m_main->slm->setScreen(scr_menu, MENU_CHANGE_TIME);
	}

	m_key_esc_last = key_esc;
}

