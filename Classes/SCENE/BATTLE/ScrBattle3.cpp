#include "ScrBattle3.h"

ScrBattle3::ScrBattle3( GameMain *main, 
						int battle_pos, 
						int battle_id, 
						const ScrBattleBgInfo &bg_info, 
						bool escape_to_map, 
						bool lose_to_map,
						bool first_attack)
	:battler(main, battle_id, (BatPos)battle_pos),
	 effvals(main, &battler),
	 state(main, &battler),
	 hue(main)
{

	pGM = main;
	ArrayList<TString> tlist;

	batId = battle_id;
	batType = (BatType)pGM->ldb.dBatCmd.battle_type;
	batPos = (BatPos)battle_pos;
	isBatAutoPos = pGM->ldb.dBatCmd.enemy_auto_arrange;

	bgName = bg_info;
	isBatEscapeable = escape_to_map;
	isBatLoseToMap = lose_to_map;
	isBatFirstAtk = first_attack;

	isBatStart = false;
	isBatEnd = false;
	
	isMenuModeRight = false;
	menuMoveLeftFrame = 0;

	menuSubSel = -1;
	menuSubLastRes = 0;

	lastItemId = 0;


	int texID_sys = IBuffer::getInstance()->get(imgSystem, pGM->lsd.System.system_name);
	CCTexture2D *tex_sys = IBuffer::getInstance()->get(imgSystem, texID_sys);

	int texID_sys2 = IBuffer::getInstance()->get(imgSystem2, pGM->ldb.dSys.system2_name);
	CCTexture2D *tex_sys2 = IBuffer::getInstance()->get(imgSystem2, texID_sys2);


	//Init BackGround
	initBackGround();
	setBackGround(bgName);

	for(int id=0; id<SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES; id++) {
		pAnis[id] = new GrpAniBat(pGM, &hue, id);
		this->addChild(pAnis[id], 998);
	}
	
	isBatAuto = false;
	curSelHero = -1;
	curHanding = -1;
	turnCnt = -1;

	isEventNeedCheck = false;
	evtPage = 0;

	endType = BFT_EVENT;

	for(int i=0; i<SCR_BATTLE3_MAX_NUMBER; i++) {
		pDmgNum[i] = new GrpBatDam(pGM);
		this->addChild(pDmgNum[i], 999);
	}

	initPeople();
	initPosition();
	initMenus();
	initBattleFinish();
	initEvent();

	updateAutoMenu();
	updateHeroMenu();
	updateEnemyMenu();

	//Battle Start!!
	ScrBattle3ActionQueue node;
	node.actor_id = -1;

	if (batPos == BPOS_BACKATTACK_HERO ||
		batPos == BPOS_SIDEATTACK_HERO ||
		isBatFirstAtk)
			setCaption(pGM->ldb.dVoca.special_combat);

	node.type = B3AT_WAIT;
	node.total_frame = 60;//Game start after 60frame = 1sec
	addQueue(node);

	node.type = B3AT_BATTLE_START;
	addQueue(node);

	node.type = B3AT_ACT_TURN_START;
	addQueue(node);

	node.type = B3AT_ACT_TURN_END;
	addQueue(node);

	isBatStart = isBatEnd = isBatEscaped = false;
	pGM->lsd.System.music_before_battle = pGM->lsd.System.music_current;
	pGM->snd.PlaySound(pGM->lsd.System.se_battle);
	pGM->snd.PlayMusic(pGM->lsd.System.music_battle);

	pGM->lsd.System.screen = scr_battle;	
}

ScrBattle3::~ScrBattle3()
{
	IBuffer::getInstance()->mapChanged();
}

void ScrBattle3::Init()
{

}

void ScrBattle3::reloadShaders()
{
	hue.reloadShaders();
	pBatEndFont->reloadShaders();
	pEvtMain->reloadShaders();

	for(int i=0; i<SCR_BATTLE3_MAX_NUMBER; i++)
		pDmgNum[i]->reloadShaders();

	pMenuCaption->reloadShaders();
	pMenuAuto->reloadShaders();
	pMenuHero->reloadShaders();
	pMenuAction->reloadShaders();
	pMenuEnemy->reloadShaders();
	pMenuSub->reloadShaders();
	pMenuSubCaption->reloadShaders();
}


void ScrBattle3::Draw( int framedelta )
{
// 	TString str = "";
// 	str = str + (isEventRunning()	?"ERUN OK ":"ERUN NO ");
// 	str = str + (isGameRunning()	?"GRUN OK ":"GRUN NO ");
// 	str = str + (m_battle_start		?"BSTT OK ":"BSTT NO ");
// 	str = str + (m_battle_end		?"BEND OK ":"BEND NO ");
// 	str = str + "CMENU" + m_cur_menu;
// 	CCLog(str.getTextUTF8());
// 
// 	if (isEventRunning()) {
// 		CCLog("");
// 		CCLog((_ST("EVENT_STACK_LEN : ") + m_event_stack.data.GetLength()).getTextUTF8());
// 		for(int i=0; i<m_event_stack.data.GetLength(); i++) {
// 			LsdMapEventData &datas = m_event_stack.data[i];
// 			
// 			CCLog((_ST("\tSTACK[") + i + "] EVENT_LEN : " + datas.event_length).getTextUTF8());
// 	
// 			for(int j=0; j<datas.event_length; j++) {
// 				GlbEventNode &evt = datas.event.data[j];
// 
// 				CCLog((_ST("\t\tEVENT[") + j + "] CODE : " + evt.code).getTextUTF8());
// 			}
// 		}
// 		CCLog("");
// 	}

	for(int i=0; i<nodeHeroLen; i++) {
		pNodeHero[i]->Draw(framedelta);
		pGazeTn[i]->refresh(turnPer[i]);
	}

	for(int i=0; i<nodeEnemyLen; i++) {
		pNodeEnemy[i]->Draw(framedelta);
		pNodeWeapon[i]->Draw(framedelta);
	}

	updateTurnPer(framedelta);
	updateQueue(framedelta);
	updateBattleFinish(framedelta);
	updateCaption(framedelta);
	updateMenuPos(framedelta);

	for(int id=0; id<SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES; id++)
		pAnis[id]->Draw(framedelta);

	for(int i=0; i<SCR_BATTLE3_MAX_NUMBER; i++)
		pDmgNum[i]->Draw(framedelta);

	hue.Draw(framedelta);

	chkEvent(framedelta);

	if (!isEventRunning() && isBatStart)
		chkKeys();
}

void ScrBattle3::chkKeys()
{
	chkMenuKeys();
}

bool ScrBattle3::isGameRunning() {

	if (!isBatStart || isBatEnd)
		return false;

	switch(curMenu) {
		case BMS_HERO:
		case BMS_ACTION:
		case BMS_TARGET_ENEMY:
		case BMS_TARGET_HERO:
		{
			if (!isEventRunning())
				return true;
		}
	}

	if (actQueue.GetLength() != 0) {
		if (actQueue[0].type != B3AT_ACT_TURN_END) {
			return true;
		}
	}

	return false;
}

void ScrBattle3::initPeople()
{
	LdbEnemyGroup &group = pGM->ldb.dEnemys[batId-1];

	nodeHeroLen = min(pGM->lsd.State.party.GetLength(), SCR_BATTLE_MAX_HEROS);
	nodeEnemyLen = min(group.members.GetLength(), SCR_BATTLE_MAX_ENEMIES);

	int texID_sys2 = IBuffer::getInstance()->get(imgSystem2, pGM->ldb.dSys.system2_name);
	CCTexture2D *tex_sys2 = IBuffer::getInstance()->get(imgSystem2, texID_sys2);


	//Default Hero Setting
	for(int id=0; id<nodeHeroLen; id++) {
		pNodeHero[id] = new ScrBattle3HeroNode(pGM, &battler, &hue, id, tex_sys2);

		CCPoint point = battler.getDefPos(id);
		pNodeHero[id]->setPosition(point);
		this->addChild(pNodeHero[id], 1000 - point.y);

		pNodeHero[id]->init(&hue);

		turnPer[id] = 0.0f;

	}


	//Default Enemy Setting
	for(int i=0; i<nodeEnemyLen; i++) {
		int id = i+SCR_BATTLE_MAX_HEROS;

		if (group.members[i].invisible)
			battler.add(id, BAT_INVISIBLE, 1);

		pNodeEnemy[i] = new ScrBattle3EnemyNode(pGM, &battler, id, tex_sys2);

		CCPoint point = battler.getDefPos(id);
		pNodeEnemy[i]->setPosition(point);
		this->addChild(pNodeEnemy[i], 1000 - point.y);

		pNodeEnemy[i]->init(&hue);

		turnPer[id] = 0.0f;
	
		pNodeWeapon[i] = new ScrBattle3WeaponNode(pGM);
		pNodeWeapon[i]->setVisible(false);
		pNodeWeapon[i]->init(&hue);
		this->addChild(pNodeWeapon[i], 999);
	}

	//Default Escape Chance Setting
	escapeChanceEnemy = effvals.getEscapePer(false);
	escapeChanceHero = effvals.getEscapePer(true);


	//Start Turn Percent Setting
	switch(batPos) {
	case BPOS_BACKATTACK_ENEMY:
	case BPOS_SIDEATTACK_ENEMY:
		{
			for(int i=0; i<nodeEnemyLen; i++) {
				int id = i + SCR_BATTLE_MAX_HEROS;
				turnPer[id] = 0.99f;
			}
			break;
		}

	case BPOS_BACKATTACK_HERO:
	case BPOS_SIDEATTACK_HERO:
		{
			for(int id=0; id<nodeHeroLen; id++)
				turnPer[id] = 0.99f;
			break;
		}	
	}

	if (isBatFirstAtk)
		for(int id=0; id<nodeHeroLen; id++)
			turnPer[id] = 0.99f;

}
