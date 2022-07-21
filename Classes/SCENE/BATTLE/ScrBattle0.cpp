#include "ScrBattle0.h"

ScrBattle0::ScrBattle0( GameMain *main, 
						int battle_pos, 
						int battle_id, 
						ScrBattleBgInfo bg_info, 
						bool escape_to_map, 
						bool lose_to_map,
						bool first_attack)
	:battler(main, battle_id, (BatPos)battle_pos),
	effVals(main, &battler),
	state(main, &battler),
	hue(main)
{
	pGM = main;

	batId = battle_id;
	batType = (BatType)pGM->ldb.dBatCmd.battle_type;
	batPos = (BatPos)battle_pos;
	isBatAutoPos = pGM->ldb.dBatCmd.enemy_auto_arrange;

	bgName = bg_info.bg;
	isBatEscapeable = escape_to_map;
	isBatLoseToMap = lose_to_map;
	isBatFirstAtk = first_attack;

	int texID_sys = IBuffer::getInstance()->get(imgSystem, pGM->lsd.System.system_name);
	CCTexture2D *tex_sys = IBuffer::getInstance()->get(imgSystem, texID_sys);

	pScreen = CCNode::create();
	this->addChild(pScreen, 0);

	pSprBg = new CCSprite();
	pSprBg->setPosition(ccp(160,160));
	pSprBg->autorelease();
	pScreen->addChild(pSprBg, 0);
	setBackGround(bgName);

	for(int id=0; id<SCR_BATTLE_MAX_ENEMIES; id++) {
		pAnis[id] = new GrpAniBat(pGM, &hue, id);
		pScreen->addChild(pAnis[id], 998);
	}

	initEvent();
	initPeople();
	initMenus();
	initBattleFinish();

	updateAutoMenu();
	updateActionMenu();
	updateHeroMenu();
	updateEnemyMenu();

	curSelHero = -1;
	turnCnt = 0;

	curMenu = BMS_PROCESS;
	finishType = BFT_EVENT;
	curMsgRow = 0;

	chkEvent();

	actFrameLeft = 0;

	pGM->lsd.System.music_before_battle = pGM->lsd.System.music_current;
	pGM->snd.PlaySound(pGM->lsd.System.se_battle);
	pGM->snd.PlayMusic(pGM->lsd.System.music_battle);

	heroCmdUsing.SetLength(4);
	heroCmdTgt.SetLength(4);
	heroCmdTgt.SetLength(4);

	pGM->lsd.System.screen = scr_battle;	
}

ScrBattle0::~ScrBattle0()
{
	this->removeChild(pScreen);
	this->removeChild(pSprBg);

	IBuffer::getInstance()->mapChanged();
}

void ScrBattle0::Init()
{

}

void ScrBattle0::reloadShaders()
{
	hue.reloadShaders();
	pEvtMain->reloadShaders();

	pMenuProcRes->reloadShaders();
	for(int i=0; i<4; i++)
		pMenuProcRun[i]->reloadShaders();

	pMenuAuto->reloadShaders();
	pMenuHero->reloadShaders();
	pMenuAction->reloadShaders();
	pMenuEnemy->reloadShaders();
	pMenuSub->reloadShaders();
	menuSubCaption->reloadShaders();
}

void ScrBattle0::Draw( int framedelta )
{
	for(int i=0; i<nodeEnemyLen; i++) {
		pNodeEnemy[i]->Draw(framedelta);
		pAnis[i]->Draw(framedelta);
	}

	updateMenuPos(framedelta);

	if (curMenu == BMS_PROCESS) {

		drawEvent(framedelta);

		if (pEvtMain->isRuningSerial())
			return;

		if (updateQueue(framedelta))
			return;

		if (chkBattleFinish(framedelta))
			return;

		if (doAction())
			return;

		doMenuStart();
	}else
		chkMenuKeys();
}

void ScrBattle0::chkKeys()
{

}

void ScrBattle0::setBackGround( TString bg_name )
{
	int tex_id = IBuffer::getInstance()->get(imgBackdrop, IMAGE_BUFFER_NOALPHA_PRESTR + bg_name);
	CCTexture2D *tex = IBuffer::getInstance()->get(imgBackdrop, tex_id);

	pSprBg->initWithTexture(tex);

	hue.init(SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES, pSprBg);
}


void ScrBattle0::initPeople() {
	LdbEnemyGroup &group = pGM->ldb.dEnemys[batId-1];

	nodeHeroLen = min(pGM->lsd.State.party.GetLength(), SCR_BATTLE_MAX_HEROS);
	nodeEnemyLen = min(group.members.GetLength(), SCR_BATTLE_MAX_ENEMIES);

	//Default Enemy Setting
	for(int i=0; i<nodeEnemyLen; i++) {
		int id = i+SCR_BATTLE_MAX_HEROS;

		LdbEnemyGroupMember &member = group.members[i];

		if (member.invisible)
			battler.add(id, BAT_INVISIBLE, 1);

		CCPoint point = ccp(SCR_POS_X(member.x), SCR_POS_Y(member.y));
		
		pNodeEnemy[i] = new ScrBattle0EnemyNode(pGM, &battler, id);
		pNodeEnemy[i]->setPosition(point);
		pNodeEnemy[i]->init(&hue);

		pScreen->addChild(pNodeEnemy[i], 1000 - point.y);
	}

	//Default Escape Chance Setting
	escapeChanceEnemy = effVals.getEscapePer(false);
	escapeChanceHero = effVals.getEscapePer(true);
}
