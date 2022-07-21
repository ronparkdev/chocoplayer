#include "GameMain.h"

#include "COMMON/DebugingCode.h"

#include "AppMulti.h"
#include "FILEIO/TFolder.h"
#include "COMMON/GlobalLangFont.h"
#include "COMMON/MultiOutter.h"
#include "COMMON/MgrSetting.h"
#include "AD/AdMain.h"
#include "TRANSLATE/TranslateMain.h"
#include "SCENE/APPMAIN/ScrAppMain.h"
#include "NETWORK/NetMain.h"

GameMain * GameMain::g_instance = NULL;

GameMain * GameMain::getInstance()
{
	return g_instance;
}

GameMain::GameMain()
	:lsdbuf(this)
{
	g_instance = this;

	isPaused = false;

	slm = NULL;

	ctl = new ctlMain();
	ctl->retain();
}

GameMain::~GameMain() {
	ctl->release();

	if (g_instance == this)
		g_instance = NULL;
}


void GameMain::appstart(TString gamespath) {
	try{
		int p1, p2, p3;

		TString inn_path, ext_path, rtp_path;

		TString uid = MultiOutter::getUniqueID();
		CCLog(("UserID:" + uid).getTextUTF8());

		MgrSetting::getInstance()->LoadAll();

		p1 = gamespath.indexOf('#');
		p2 = gamespath.indexOf('#', p1+1);
		p3 = gamespath.indexOf('#', p2+1);

		inn_path	= gamespath.substring(0, p1);
		ext_path	= gamespath.substring(p1+1, p2);
		rtp_path	= gamespath.substring(p2+1, p3);
		pathTmp	= gamespath.substring(p3+1);

		if (!inn_path.isEmpty() && inn_path.charAt(inn_path.getLength()-1) != '/')
			inn_path = inn_path + "/";

		if (!ext_path.isEmpty() && ext_path.charAt(ext_path.getLength()-1) != '/')
			ext_path = ext_path + "/";

		if (!pathTmp.isEmpty() && pathTmp.charAt(pathTmp.getLength()-1) != '/')
			pathTmp = pathTmp + "/";

		pathLang = inn_path + "TRANS/";

        CCLOG(("PATH_IN  : " + inn_path).getTextUTF8());
        CCLOG(("PATH_EX  : " + ext_path).getTextUTF8());
		CCLOG(("PATH_RTP : " + rtp_path).getTextUTF8());
		CCLOG(("PATH_TMP : " + pathTmp).getTextUTF8());
        
		TFolder::chkFolder(inn_path);
		TFolder::chkFolder(pathTmp);

		pathGames = inn_path;// + "#" + ext_path;
		
		rtp.Init(rtp_path);
		if (!rtpu.Init(inn_path)) {
			isNeedNoticeRTP = true;
		} else {
			isNeedNoticeRTP = false;
		}

		fctl.Init(&rtp, &rtpu);
		lmtmgr.init(&lmt);
		snd.Init(&lsd, &fctl, pathTmp);
		
		IBuffer::getInstance()->setFileCTL(&fctl);
		
		ctl->Init();

		initUserInfo();

		GlobalLangFont::Init();
		ctl->updateSetting();

		slm = ScreenLayerManager::create();
		slm->Init(this);

		CCScene *pScene = ScreenLayerManager::scene(slm, this);
		CCDirector::sharedDirector()->runWithScene(pScene);

		slm->setScreen(scr_app_main);
	}catch(...) {
		doToast("ERROR GameMain::appstart()");
		CCLOG("ERROR GameMain::appstart()");
	}
}

void GameMain::appstart() {
	slm->setScreen(scr_app_main);
	snd.StopMusic();
	snd.StopSound();
	snd.removeTemp();
}

int GameMain::openGameWithBackground( ScrAppMainGameNode &node ) {
	node.getGameData(pathGame, isArchive, hashCode, encId);

	NetMain::getInstance()->sendGameStart(pathGame, hashCode, node.getName());

	//CCLOG((_ST("Main::openGameWithBackground ") + m_gamepath).getTextUTF8());

	TString::setDefEncoding(encId);
	if (!fctl.open(pathGame, isArchive))
		return SSR_LOAD_FAIL_FILE_CTL;

	StructLoader sl;

	char *data;
	int datasize;

	try{
		if (	fctl.getData(fileFolder, "RPG_RT.ldb", &data, &datasize) &&
			sl.openWithNoFree(data, datasize)) {
			CCLOG("datalen %d", datasize);
			ldb.init();
			ldb.serialize(sl);
			sl.close();
		} else {
			return SSR_LOAD_FAIL_LDB;
		}

		if (	fctl.getData(fileFolder, "RPG_RT.lmt", &data, &datasize) &&
			sl.openWithNoFree(data, datasize)) {
			lmt.init();
			lmt.serialize(sl);
			sl.close();
		} else {
			return SSR_LOAD_FAIL_LMT;
		}

		if (lmt.Info.party_map_id <= 0)
			return SSR_LOAD_FAIL_LMT;

	}catch(...) {
		return SSR_LOAD_FAIL_FILE_CTL;
	}

	lsdbuf.Init();
	trans.Init();

	rtp.setVersion(getVersion());
	rtpu.setVersion(getVersion());

	return SSR_LOAD_SUCCESS;
}

void GameMain::openGameWithMainThread() {
	IBuffer::getInstance()->release();

	GlobalLangFont::setLang((Encoding)encId);
	inlang.setLang((Encoding)encId);
	snd.setPath(pathGame);
}

void GameMain::quit() {
	
}

GameVersion GameMain::getVersion()
{
	if (ldb.dSys.ldb_id == 2003)
		return RPG2003;
	else
		return RPG2000;
}

void GameMain::onPause()
{
	if (!isPaused) {
		isPaused = true;
		if (slm != NULL)
			slm->onPause();
	}
}

void GameMain::onResume()
{
	if (isPaused) {
		isPaused = false;
		if (slm != NULL)
			slm->onResume();

		AdMain::chkAd();
	}
}


void GameMain::initUserInfo()
{	
	//CCLog((_ST("Lang Detected") + (int)CCApplication::sharedApplication()->getCurrentLanguage()).getTextUTF8());

	switch(CCApplication::sharedApplication()->getCurrentLanguage()) 
	{
	case kLanguageEnglish:		langCode = "wn";break;
	case kLanguageChinese:		langCode = "zh";break;
	case kLanguageFrench:		langCode = "fr";break;
	case kLanguageItalian:		langCode = "it";break;
	case kLanguageGerman:		langCode = "de";break;
	case kLanguageSpanish:		langCode = "es";break;
	case kLanguageDutch:		langCode = "nl";break;
	case kLanguageRussian:		langCode = "ru";break;
	case kLanguageKorean:		langCode = "ko";break;
	case kLanguageJapanese:		langCode = "jp";break;
	case kLanguageHungarian:	langCode = "hu";break;
	case kLanguagePortuguese:	langCode = "pt";break;
	default:					langCode = "??";break;
	};

	uniqID = MultiOutter::getUniqueID();
	deviceModel = MultiOutter::getModel();
}

const TString& GameMain::getModel() const
{
	return deviceModel;
}

const TString& GameMain::getLangCode() const
{
	return langCode;
}

const TString& GameMain::getUniqueID() const
{
	return uniqID;
}
