 #include "ScreenLayerManager.h"

#include "../GameMain.h"
#include "../AppMulti.h"
#include "../AppMultiTime.h"
#include "../COMMON/ScreenCommon.h"
#include "../COMMON/MgrSetting.h"
#include "../COMMON/MultiOutter.h"
#include "../COMMON/GlobalLangFont.h"
#include "../DATA/TString.h"
#include "../GRAPHIC/ImageBuffer.h"
#include "../SOUND/SndMain.h"
#include "../AD/AdMain.h"

#include "APPVKEY/ScrAppVKey.h"
#include "APPJOY/ScrAppJoy.h"

#define BLOCK_WIDTH 8

ScreenLayerManager * ScreenLayerManager::gInstance = NULL;

ScreenLayerManager * ScreenLayerManager::getInstance()
{
	return gInstance;
}

CCScene* ScreenLayerManager::scene( ScreenLayerManager *slm, GameMain *main)//ctlMain *ctl, IBuffer *ibuf )
{
	CCScene *scene = CCScene::create();

	scene->addChild(slm);

	return scene;
}

bool ScreenLayerManager::init()
{
	if ( !CCLayerCTL::init() )
	{
		return false;
	}

	return true;
}

void ScreenLayerManager::onTouch( Touch_Type type, int id, float x, float y )
{
	float fx, fy;

	if (pCurLayer != NULL) {
		if (pCurLayer->isFullScreen()) {
			pCurLayer->onTouch(type, id, x, ScreenCommon::app_h - y);
		} else {
			fx = (x - ScreenCommon::game_x) / ScreenCommon::game_s ;
			fy = ScreenCommon::game_h - (y - ScreenCommon::game_y) / ScreenCommon::game_s ;

			if (type != touchtype_down || 
				(0<=fx && fx<ScreenCommon::game_w && 
				 0<=fy && fy<ScreenCommon::game_h)) {
					pCurLayer->onTouch(type, id, fx, fy);
			}

			if (type == touchtype_down && !isSubJoyOpened && !isSubVKeyOpened) {
				if (limitFrameLeft == 0) {
					MultiOutter::openMenu("|MAINMENU|BUY");
				} else {
					setScrPaused(false);
				}
			}
		}
	}
}

void ScreenLayerManager::Init( GameMain * main )
{
	pGM = main;
	setCtl(main->ctl);
}

ScreenLayerManager::ScreenLayerManager()
{
	gInstance = this;

	pCurLayer = NULL;
	changeFrameTotal = changeFrameLeft = 0;

	aniTypeShow = aniTypeHide = 0;
	lastAniType = -1;

// 	m_pCurScreen = CCRenderTexture::create
// 		((int)floor(ScreenCommon::game_w * ScreenCommon::game_s), 
// 		(int)floor(ScreenCommon::game_h * ScreenCommon::game_s));
// 	m_pCurScreen->retain();
	pCurScreenSpr = NULL;

	pGlProg = NULL;
	lastGLSLFileName = NULL;

	incomingAction = INCOMING_CMD_NONE;

	isFirst = true;
	isScrHided = false;
	isMapHided = false;
	isFrameNeedRefresh = false;
	chkFrame = 0;
	totFrame = 0;
	lastFrameDelta = 0;

	scrLastType = scrType = scr_app_main;

	this->setKeypadEnabled(true);

	//Screen Side Hider
	for(int i=0; i<2; i++) {
		pSprBlack[i] = CCSprite::create("bg_black.png");
		this->addChild(pSprBlack[i], sz_controler-1);
	}

	if (!ScreenCommon::game_is_vertical) {
		float sizeBlackW = (ScreenCommon::app_w - GAME_SCREEN_WIDTH *ScreenCommon::game_s)/2.0f;
		float sizeBlackH = ScreenCommon::app_h;

		pSprBlack[0]->setScaleX(sizeBlackW);
		pSprBlack[0]->setScaleY(sizeBlackH);
		pSprBlack[0]->setPosition(ccp(sizeBlackW/2.0f, sizeBlackH/2.0f));

		pSprBlack[1]->setScaleX(sizeBlackW);
		pSprBlack[1]->setScaleY(sizeBlackH);
		pSprBlack[1]->setPosition(ccp(ScreenCommon::app_w - sizeBlackW/2.0f, sizeBlackH/2.0f));
	} else {
		float sizeW = ScreenCommon::app_w;
		float sizeGame = ScreenCommon::game_h * ScreenCommon::game_s;
		float sizeNotGame = ScreenCommon::app_h - sizeGame;

		pSprBlack[0]->setScaleX(sizeW);
		pSprBlack[0]->setScaleY(sizeNotGame);
		pSprBlack[0]->setPosition(ccp(sizeW*0.5f, sizeNotGame*0.5f));
	}

	//Sub Screens
	isSubVKeyOpened = isSubJoyOpened = false;
	pSubVKey = NULL;
	pSubJoy = NULL;

	//Screen Pause
	isScrPaused = false;

	float screenX = ScreenCommon::game_w * ScreenCommon::game_s;
	float screenY = ScreenCommon::game_h * ScreenCommon::game_s;
	float centerX = ScreenCommon::game_x + screenX * 0.5f;
	float centerY = ScreenCommon::game_y + screenY * 0.5f;

	pSprPausedFade = CCSprite::create("bg_black.png");
	pSprPausedFade->setScaleX(screenX);
	pSprPausedFade->setScaleY(screenY);
	pSprPausedFade->setPositionX(centerX);
	pSprPausedFade->setPositionY(centerY);
	pSprPausedFade->setOpacity(125);
	pSprPausedFade->setVisible(false);
	this->addChild(pSprPausedFade, sz_controler-2);

	pSprPausedCaption = CCSprite::create("scr_paused.png");
	pSprPausedCaption->setScale((screenX * 0.6f) / pSprPausedCaption->getContentSize().width);
	pSprPausedCaption->setPositionX(centerX);
	pSprPausedCaption->setPositionY(centerY);
	pSprPausedCaption->setVisible(false);
	this->addChild(pSprPausedCaption, sz_controler-1);

	//Setting Load
	MgrSetting *s = MgrSetting::getInstance();
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->setDisplayStats(s->getBool(RST_ETC_DEBUG));

	initTimeLimit();
}

ScreenLayerManager::~ScreenLayerManager()
{
	if (gInstance == this)
		gInstance = NULL;//TODO

	if (pCurLayer != NULL) {
		this->removeChild(pCurLayer);
		pCurLayer->release();
	}

	if (pGlProg != NULL) {
		pGlProg->release();
		pGlProg = NULL;
	}

	doFinAni();
}

bool ScreenLayerManager::isChanging()
	{return (changeFrameLeft > 0);}
bool ScreenLayerManager::isHided()
	{return isScrHided;}

void ScreenLayerManager::onPause() {
	this->setTouchEnabled(false);

	setScrPaused(true);
}

void ScreenLayerManager::onResume() {
	this->setTouchEnabled(true);

	pGM->ctl->updateSetting();
	pGM->trans.Init();

	if (pCurLayer != NULL)
		pCurLayer->reloadSetting();

	MgrSetting *s = MgrSetting::getInstance();
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->setDisplayStats(s->getBool(RST_ETC_DEBUG));
}

void ScreenLayerManager::onChangedOrientation()
{
	//NotUse
}

void ScreenLayerManager::keyMenuClicked()
{
	if (!isSubVKeyOpened && !isSubJoyOpened)
		MultiOutter::openMenu("|MAINMENU");
}

ScreenLayer* ScreenLayerManager::getCurLayer()
	{return pCurLayer;}

void ScreenLayerManager::setDefaultPosition() {
	if (pCurLayer->isFullScreen()) {
		pCurLayer->setPosition(ccp(0.0f, 0.0f));
		pCurLayer->setScale(1.0f);

		for(int i=0; i<2; i++)
			pSprBlack[i]->setVisible(false);

		pGM->ctl->setVisible(false);
	} else {
		pCurLayer->setPosition(ccp(ScreenCommon::game_x, ScreenCommon::game_y));
		pCurLayer->setScale(ScreenCommon::game_s);

		pSprBlack[0]->setVisible(true);
		pSprBlack[1]->setVisible(!ScreenCommon::game_is_vertical);

		pGM->ctl->setVisible(true);
	}
}

void ScreenLayerManager::doScanAndHide(bool doevent)
{
	if (pCurLayer == NULL)
		return;

	CCRenderTexture *pRendTex = CCRenderTexture::create(ScreenCommon::game_w,  ScreenCommon::game_h);

	pCurLayer->setVisible(true);
	pCurLayer->setPosition(ccp(ScreenCommon::game_x_init, ScreenCommon::game_y_init));
	pCurLayer->setRotation(0.0f);
	pCurLayer->setScale(1.0f);

	pRendTex->clear(0.0f, 0.0f, 0.0f, 255.0f);
	pRendTex->begin();
	pCurLayer->Draw(doevent?1:0);
	pCurLayer->visit();
	pRendTex->end();

	if (pCurScreenSpr != NULL)
		this->removeChild(pCurScreenSpr);

	setDefaultPosition();

	CCTexture2D *pTex = pRendTex->getSprite()->getTexture();
	pTex->setAliasTexParameters();

	pCurScreenSpr = CCSprite::createWithTexture(pTex);
	pCurScreenSpr->setVisible(false);
	this->addChild(pCurScreenSpr, sz_controler-2);

	pCurLayer->setVisible(false);
}

void ScreenLayerManager::setScreen(ScreenType type, float changetime)
{
	if (isChanging())
		return;

	scrLastType = scrType;
	GameMain::getInstance()->lsd.System.screen = scrType = type;

	checkTimeLimit(scrType);
	setAniType(scrType, scrLastType);

	//changetime = 0.2f;//FORDEBUG
 	//m_Hide_Animation_Type = m_Show_Animation_Type = rand() % 19;//FORDEBUG
	
	if (type == scr_title)
		isMapHided = false;

	if (isFirst) {
		//if not have last screen
		addScreen(type);

		//m_pCurLayer->setVisible(true);

		isFirst = false;
		isScrHided = false;
		isScrChange = false;
	} else if (!isScrHided) {
		//start fade out

		//2.2.10 씨발!!! 좆같이코딩해놨네.. 메뉴나 세이브에서 복귀할때는 HIDE가 풀림...
		if (scrLastType == scr_menu || scrLastType == scr_save)
			isMapHided = false;

		if (aniTypeShow == 20 && aniTypeHide == 20) {
			addScreen(type);
		} else {
			isScrHided = true;
			isScrChange = true;

			changeTime = changetime;

			if (pCurLayer != NULL) {
				doScanAndHide(false);

				changeFrameTotal = changeFrameLeft 
					= (int)(changetime * FRAME_PER_SEC);

				pCurLayer->setVisible(false);
				doReadyAni(aniTypeHide);
				doRunAni(1.0f);
			}
		}
	} else {
		if (type != scr_map) {
			isScrHided = false;
			changeFrameTotal = changeFrameLeft 
				= (int)(changeTime * FRAME_PER_SEC);

			doFinAni();

			addScreen(scrType);
			doScanAndHide(true);

			doReadyAni(aniTypeShow);
			doRunAni(0.0f);
		} else {
			//if change screen immediately 
			addScreen(type);

			pCurLayer->setVisible(!isScrHided);

			isFirst = false;
			isScrChange = false;
		}
	}

	AdMain::chkAd(type);
}

bool ScreenLayerManager::Hide(int type)
{
	if (isChanging())
		return false;

	if (isScrHided)
		return true;

	isScrHided = true;
	isMapHided = true;
	isScrChange = false;
	changeFrameTotal = changeFrameLeft 
		= (int)(SCREEN_HIDE_TIME * FRAME_PER_SEC);

	//m_pChangeFrameTotal = m_pChangeFrameLeft = 4000;//FORDEBUG

	doScanAndHide(false);
		
	doReadyAni(type);
	doRunAni(1.0f);

	return false;	
}

bool ScreenLayerManager::Show(int type)
{
	if (isChanging())
		return false;

	if (!isScrHided)
		return true;

	isScrHided = false;
	isMapHided = false;
	isScrChange = false;
	changeFrameTotal = changeFrameLeft 
		= (int)(SCREEN_HIDE_TIME * FRAME_PER_SEC);

	//m_pChangeFrameTotal = m_pChangeFrameLeft = 4000;//FORDEBUG

	doScanAndHide(true);

	doReadyAni(type);
	doRunAni(0.0f);

	return false;
}

void ScreenLayerManager::fixTotalFrame() {
#if (SR_IPHONE == 1)
	int expect_totalframe = ((int)(getTime() * 60.0f)) % 240;
#else
	const long chkclock = CLOCKS_PER_SEC << 2;
	int expect_totalframe = (clock() % chkclock) * 240 / chkclock;
#endif

	totFrame = expect_totalframe;
}

void ScreenLayerManager::draw()
{
	int framedelta = getFrameDelta();

	int key_menu = getCtl()->getKey(rkc_menu);
	if (lastKeyMenu != key_menu) {
		getCtl()->setKey(rkc_menu, 0);

		if (key_menu > 0)
			keyMenuClicked();
	}

	if (incomingAction != INCOMING_CMD_NONE) {
		switch((IncomingCommand)incomingAction) {
		case INCOMING_CMD_OPEN_VKEY:
			setSceneVKeyVisible(true);
			break;
		
		case INCOMING_CMD_OPEN_JOY:
			setSceneJoyVisible(true);
			break;

		case INCOMING_CMD_GOTO_TITLE:
			setScreen(scr_title);
			break;

		case INCOMING_CMD_GOTO_SAVE:
			setSettingScrSave(true, false);
			setScreen(scr_save);
			break;

		case INCOMING_CMD_GOTO_GAMELIST:
			setScreen(scr_app_main);
			break;

		case INCOMING_CMD_QUIT_APP:
			CCDirector::sharedDirector()->end();
#if (SR_IPHONE == 1)
            exit(0);
#endif
                
			break;

		}
		incomingAction = INCOMING_CMD_NONE;
	}

	windowsKeyInput();

	if (gcFrameLeft > 0) {
		if (gcFrameLeft == 1)
			IBuffer::getInstance()->mapChanged();

		gcFrameLeft--;
	}
	 
	drawTimeLimit(framedelta);

	if (isChanging()) {
		//Screen Changing

		changeFrameLeft -= framedelta;

		float per = (float)(changeFrameTotal - changeFrameLeft) / changeFrameTotal;

		if (isScrHided)
			doRunAni(1.0f - per);
		else
			doRunAni(per);

		if (!isChanging()) {
			//Change is finish

			if (isScrChange) {
				//Fadein - Fadeout
				if (isScrHided) {
					if (scrType != scr_map || !isMapHided) {
						//fade out is finish, and it will fade in
						isScrHided = false;
						changeFrameTotal = changeFrameLeft 
							= (int)(changeTime * FRAME_PER_SEC);

						addScreen(scrType);
						doScanAndHide(true);

						doReadyAni(aniTypeShow);
						doRunAni(0.0f);
					} else {
						addScreen(scrType);
						pCurLayer->setVisible(false);
					}
				} else {
					//fade in is finished
					pCurLayer->setVisible(true);

					doFinAni();
					
					fixTotalFrame();

					gcFrameLeft = 1;
				}
			} else {
				//Show or Hide
				if (!isScrHided)
					pCurLayer->setVisible(true);

				doFinAni();
			}
		}
	} else {
		//if menu is closed, enable touch and refresh setting

		if (!isSubVKeyOpened && pSubVKey != NULL) {
			pSubVKey->retain();
			pSubVKey->autorelease();
			this->removeChild(pSubVKey);
			pSubVKey = NULL;
		}

		if (!isSubJoyOpened && pSubJoy != NULL) {
			pSubJoy->retain();
			pSubJoy->autorelease();
			this->removeChild(pSubJoy);
			pSubJoy = NULL;
		}

		//Draw Screen
		if (!isScrPaused) {
			pCurLayer->Draw(framedelta);
		} else {
			for(int i=0; i<R2K_KEYCODE_COUNT; i++)
				if (pGM->ctl->getKey((R2K_KEYCODE)i) != 0) {
					if (limitFrameLeft == 0) {
						MultiOutter::openMenu("|MAINMENU|BUY");
					} else {
						setScrPaused(false);
					}
					break;
				}
		}
	}

	if (!isSubVKeyOpened && !isSubJoyOpened) {
		chkFrame += framedelta;
		if (chkFrame > 6) {
			pGM->snd.doCheck(chkFrame);
			chkFrame = 0;
		}
	}
}

void ScreenLayerManager::reloadShaders()
{
	CCLog("ScreenLayerManager::reloadShaders()");
	if (pGlProg != NULL) {
// 		m_glProgram->reset();
// 		m_glProgram->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, ccPositionTextureA8Color_frag);
// 		m_glProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
// 		m_glProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
// 		m_glProgram->link();
// 		m_glProgram->updateUniforms();
// 		m_glProgram->use();
	}

	lastGLSLFileName = NULL;

	if (pCurLayer != NULL)
		pCurLayer->reloadShaders();
}


void ScreenLayerManager::doAction( int type )
{
	incomingAction = type;
}

void ScreenLayerManager::setSceneVKeyVisible(bool isVisible)
{
	if (isVisible != isSubVKeyOpened) {
		isSubVKeyOpened = isVisible;

		if (isVisible) {
			pSubVKey = ScrAppVKey::create();

			this->addChild(pSubVKey, sz_controler);
			this->setTouchEnabled(false);

			if (pCurLayer != NULL)
				pCurLayer->setTouchEnabled(false);

			setScrPaused(true);
		} else {
			this->setTouchEnabled(true);
			pGM->ctl->updateSetting();

			if (pCurLayer != NULL)
				pCurLayer->setTouchEnabled(true);

			refreshScrPaused();
		}

	}
}

void ScreenLayerManager::setSceneJoyVisible( bool isVisible )
{
	if (isVisible != isSubJoyOpened) {
		isSubJoyOpened = isVisible;

		if (isVisible) {
			pSubJoy = ScrAppJoy::create();

			this->addChild(pSubJoy, sz_controler);
			this->setTouchEnabled(false);

			if (pCurLayer != NULL)
				pCurLayer->setTouchEnabled(false);

			setScrPaused(true);
		} else {
			this->setTouchEnabled(true);
			pGM->ctl->updateSetting();

			if (pCurLayer != NULL)
				pCurLayer->setTouchEnabled(true);

			refreshScrPaused();
		}
	}
}

int ScreenLayerManager::getFrameDelta()
{
	int framedelta = 1;

	MgrSetting *s = MgrSetting::getInstance();

#if (SR_IPHONE == 1)
	int expect_totalframe = ((int)(getTime() * 60.0f)) % 240;
#else
	const long chkclock = CLOCKS_PER_SEC << 2;
	int expect_totalframe = (clock() % chkclock) * 240 / chkclock;
#endif

	if (getCtl()->getKey(rkc_turbo) > 0) {
		framedelta = 16;
		totFrame = expect_totalframe;
	} else if (s->getBool(RST_ETC_FRAMESKIP)) {
		if (isFrameNeedRefresh) {
			totFrame = expect_totalframe;
			isFrameNeedRefresh = false;
		} else if (expect_totalframe < 120 && 120 <= totFrame) {
			if (lastFrameDelta <= 0)
				framedelta = 1;
			else
				framedelta = lastFrameDelta;

			totFrame = expect_totalframe;
		} else {
			framedelta = expect_totalframe - totFrame;
			if (framedelta <= 0)
				framedelta = 1;
			if (framedelta >= 4)
				framedelta = 4;

			totFrame += framedelta;
		}
	}

	lastFrameDelta = framedelta;

	return framedelta;
}

void ScreenLayerManager::refreshScrPaused() {
	setScrPaused(isScrPaused);
}

void ScreenLayerManager::setScrPaused( bool isPaused )
{
	switch(pGM->lsd.System.screen) {
	case scr_app_main:
		return;
	}

	bool isVisible = isPaused && !isSubJoyOpened && !isSubVKeyOpened;
	bool isSoundPlay = !isPaused && !isSubJoyOpened && !isSubVKeyOpened;

	pSprPausedFade->setVisible(isVisible);
	pSprPausedCaption->setVisible(isVisible);

	pSprPausedFade->stopAllActions();
	if (isVisible) {
		CCFadeTo *fadeIn = CCFadeTo::create(1.0f, 0);
		CCFadeTo *fadeOut = CCFadeTo::create(1.0f, 255);

		CCSequence *actSeq = CCSequence::createWithTwoActions(fadeOut, fadeIn);
		CCRepeatForever *actRep = CCRepeatForever::create(actSeq);

		pSprPausedCaption->setOpacity(255);
		pSprPausedCaption->runAction(actRep);
	}

	SndMain &snd = GameMain::getInstance()->snd;
	if (isSoundPlay) {
		snd.onResume();
	} else {
		snd.onPause();
	}

	this->isScrPaused = isPaused;
}
