#ifndef __R2K_GAME_MAIN__
#define __R2K_GAME_MAIN__

#include "cocos2d.h"
#include "DATA/LDB/LdbMain.h"
#include "DATA/LMT/LmtMain.h"
#include "DATA/LMT/LmtMgr.h"
#include "DATA/LSD/LsdMain.h"
#include "DATA/LSD/LsdBuffer.h"

#include "COMMON/GlobalLangInGame.h"
#include "CONTROL/ctlMain.h"
#include "FILEIO/FileCTL.h"
#include "GRAPHIC/ImageBuffer.h"
#include "GRAPHIC/GrpRTP.h"
#include "GRAPHIC/GrpRTPUser.h"
#include "SOUND/SndMain.h"
#include "SCENE/ScreenLayerManager.h"
#include "TRANSLATE/TranslateMain.h"
#include "SCENE/APPMAIN/ScrAppMainGameNode.h"

class ScrGameListNode;

USING_NS_CC;

enum StartStageReturn{
	SSR_LOAD_SUCCESS = 0,
	SSR_LOAD_FAIL_FILE_CTL	= -1000, 
	SSR_LOAD_FAIL_LDB		= -1001,
	SSR_LOAD_FAIL_LMT		= -1002
};

class GameMain
{
public:
	static GameMain *g_instance;

	bool isPaused;

	TString pathGames;
	TString pathTmp;
	TString pathLang;
	TString pathGame;	//Game Path
	
	bool isArchive;		// 1 is archive?
	TString hashCode;
	int encId;

	LdbMain ldb;	//Fixed Data
	LmtMain	lmt;	//Map Tree
	LmtMgr lmtmgr;	//LMT File Helper
	LsdMain lsd;	//Saving Data
	LsdBuffer lsdbuf;	//Saving Data by Array(VER2.10 For Optimization)

	FileCTL fctl;	//File Manager(Include ZIP)
	GrpRTP rtp;		//RTP Manager
	GrpRTPUser rtpu;
	
	ScreenLayerManager *slm;	//Screen Manager
	ctlMain *ctl;	//Controler Manager
	SndMain snd;	//Sound Manager
	GlobalLangInGame inlang;
	TranslateMain trans;

	int curMapId;

	TString langCode;
	TString uniqID;
	TString deviceModel;

	bool isNeedNoticeRTP;
public:
	static GameMain *getInstance();

	GameMain();
	~GameMain();

	void appstart(TString gamespath);
	void appstart();

	int openGameWithBackground( ScrAppMainGameNode &node );
	void openGameWithMainThread();
	void quit();
	
	GameVersion getVersion();
	void onPause();
	void onResume();

	void initUserInfo();
	const TString& getLangCode() const;
	const TString& getUniqueID() const;
	const TString& getModel() const;
};

#endif