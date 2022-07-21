#include "GrpAniOne.h"

#include "../COMMON/ScreenCommon.h"
#include "../EVENT/EvtMain.h"
#include "../SCENE/MAP/ScrMapPublic.h"
#include "../SCENE/BATTLE/ScrBattle0.h"
#include "../SCENE/BATTLE/ScrBattle3.h"

#define GRP_ANIMATION_FRAME_PER_ONE 2

//For Map
GrpAniOne::GrpAniOne(GameMain *main)
{
	pHue = NULL;
	pLsd = &(main->lsd);
	isBattle = false;

	pLsdScr	= &(main->lsd.Screen);
	pSnd	= &(main->snd);
	m_ibuf	= IBuffer::getInstance();
	anis = &(main->ldb.dAnim);

	texId = -1;
	aniId = 0;
	isLoop = false;
	isBattle = false;
	skippedFrames = 0;

	for(int i=0; i<GRP_ANIMATION_MAX_NODE; i++) {
		pNodes[i] = new GrpAniNode();
		pNodes[i]->setVisible(false);
		this->addChild(pNodes[i], i);
	}

	this->autorelease();
}

//For Battle
GrpAniOne::GrpAniOne(GameMain *main, ScrBattleHueMgr *pHue, int objID)
{
	this->pHue = pHue;
	this->objID = objID;

	pLsd = NULL;
	pLsdScr	= new LsdScreen();
	pLsdScr->ani_id = 0;
	pSnd	= &(main->snd);
	m_ibuf	= IBuffer::getInstance();
	anis = &(main->ldb.dAnim);

	texId = -1;
	aniId = 0;
	isLoop = false;
	isBattle = true;

	for(int i=0; i<GRP_ANIMATION_MAX_NODE; i++) {
		pNodes[i] = new GrpAniNode();
		pNodes[i]->setVisible(false);
		this->addChild(pNodes[i], i);
	}

	this->autorelease();
}

GrpAniOne::~GrpAniOne()
{
	for(int i=0; i<GRP_ANIMATION_MAX_NODE; i++)
		this->removeChild(pNodes[i]);

	if (isBattle)
		delete pLsdScr;
}

// void GrpAnimationOne::reloadShaders()
// {
// 	//TODO why not use shader???
// }

LdbAni* GrpAniOne::getCurrentAni() {
	if (pLsdScr->ani_id <= 0)
		return NULL;
	else
		return &((*anis)[pLsdScr->ani_id-1]);
}

void GrpAniOne::play( int aniID )
{
	pLsdScr->ani_id = aniID;
	pLsdScr->ani_frameleft = 0;
	pLsdScr->ani_curframe = 0;
	skippedFrames = GRP_ANIMATION_FRAME_PER_ONE;
	isLoop = false;
}

void GrpAniOne::playLoop( int aniID )
{
	pLsdScr->ani_id = aniID;
	pLsdScr->ani_frameleft = 0;
	pLsdScr->ani_curframe = 0;
	skippedFrames = GRP_ANIMATION_FRAME_PER_ONE;
	isLoop = true;
}

bool GrpAniOne::isPlaying() {
	return (pLsdScr->ani_id > 0);
}

void GrpAniOne::Draw(int framedelta, bool isValFix)
{
	LdbAni *ani;
	LdbAniFrame *anif;
	LdbAniTiming *anit;
	int curframe, afterframe;
	int cellcount;
	int i, j, id;
	LsdObjectCommon *obj;

	if (pLsdScr->ani_id > 0) {
		if (pLsdScr->ani_id <= 0 || (*anis).GetLength() < pLsdScr->ani_id) {
			pLsdScr->ani_id = 0;
			return;
		}

		ani = &((*anis)[pLsdScr->ani_id-1]);

		if (!isBattle)
			obj = EvtMain::getEvent(pLsd, pLsdScr->ani_target);

		if (aniId != pLsdScr->ani_id && texName != ani->animation_name) {
			texName = ani->animation_name;

			texId = m_ibuf->get(imgBattle, texName);

			if (texId == -1)
				return;

			CCTexture2D *ptex = m_ibuf->get(imgBattle, texId);

			CCSpriteBatchNode *pbatch = CCSpriteBatchNode::createWithTexture(ptex);

			CCTexture2D *pbatchtex = pbatch->getTexture();

			for(int i=0; i<GRP_ANIMATION_MAX_NODE; i++) {
				pNodes[i]->Init(pbatchtex);
			}
		}

		aniId = pLsdScr->ani_id;

		if (texId == -1)
			return;

		this->setVisible(true);

		pLsdScr->ani_frameleft -= framedelta;
		int nextframe = (GRP_ANIMATION_FRAME_PER_ONE - pLsdScr->ani_frameleft) / GRP_ANIMATION_FRAME_PER_ONE;
		pLsdScr->ani_frameleft += GRP_ANIMATION_FRAME_PER_ONE * nextframe;

		curframe = pLsdScr->ani_curframe;
		afterframe = min(pLsdScr->ani_curframe + nextframe, ani->frames.GetLength());

		//CCLOG("%d", curframe);

		id = 0;
		if (curframe < ani->frames.GetLength()) {
			anif = &(ani->frames[curframe]);
			cellcount = min(anif->cells.GetLength(), GRP_ANIMATION_MAX_NODE);
			for(i=0; i<cellcount && id < GRP_ANIMATION_MAX_NODE; i++)
				pNodes[id++]->Draw(pLsdScr->ani_id, &(anif->cells[i]), 0, 0);
		}

		for(i = curframe; i < afterframe; i++)
		{
			anif = &(ani->frames[curframe/2]);
			for(j=0; j<ani->timings.GetLength(); j++)
			{
				anit = &(ani->timings[j]);
				if (anit->frame == i) {
					if (!anit->se.name.isEmpty())
						pSnd->PlaySound(anit->se);

					switch(anit->flash_scope) {
					case 1:
						{
							if (isBattle) {
								pHue->setFlash(objID, 1, 
									anit->flash_red, 
									anit->flash_green, 
									anit->flash_blue, 
									anit->flash_power, 
									anit->frame);
							} else if (obj != NULL) {
								obj->flash_fin_r = anit->flash_red;
								obj->flash_fin_g = anit->flash_green;
								obj->flash_fin_b = anit->flash_blue;
								obj->flash_cur_lev = anit->flash_power;
								obj->flash_frame_left = anit->frame;
								obj->flash_continuous = 1;
							}

							break;
						}
					case 2:
						{
							if (isBattle) {
								int scrID = SCR_BATTLE_MAX_HEROS + SCR_BATTLE_MAX_ENEMIES;
								pHue->setFlash(scrID, 1, 
									anit->flash_red, 
									anit->flash_green, 
									anit->flash_blue, 
									anit->flash_power, 
									anit->frame);

							} else {
								pLsdScr->flash_fin_r = anit->flash_red;
								pLsdScr->flash_fin_g = anit->flash_green;
								pLsdScr->flash_fin_b = anit->flash_blue;
								pLsdScr->flash_cur_lev = anit->flash_power;
								pLsdScr->flash_frame_left = anit->frame;
								pLsdScr->flash_continuous = 1;
							}

							break;
						}
					}
				}
			}
		}

		for(j=id; j<GRP_ANIMATION_MAX_NODE; j++)
			pNodes[j]->Clear();

		if (!isValFix) {
			pLsdScr->ani_curframe = afterframe;

			if (afterframe >= ani->frames.GetLength()) {
				if (isLoop) {
					pLsdScr->ani_curframe = 0;
					skippedFrames = 0;
				}else
					pLsdScr->ani_id = 0;

			} else {
				pLsdScr->ani_curframe = afterframe;
			}
		}
	}

	this->setVisible(pLsdScr->ani_id > 0);
}

int GrpAniOne::getAnimationFrame( GameMain *main, int id )
{
	ArrayList<LdbAni> &anis = main->ldb.dAnim;

	if (id <= 0 || id > anis.GetLength())
		return 0;

	return anis[id-1].frames.GetLength() * GRP_ANIMATION_FRAME_PER_ONE;
}
