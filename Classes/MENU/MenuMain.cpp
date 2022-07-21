#include "MenuMain.h"

#include "../COMMON/GlobalLangFile.h"
#include "../COMMON/MultiOutter.h"
#include "../SCENE/ScreenLayerManager.h"
#include "../GameMain.h"
#include "../AD/AdMain.h"

TString MenuMain::getCurMenu(TString &cmd) {
	int id;
	TString result;

	if (cmd.getLength() > 0) {
		if ((id = cmd.indexOf('|', 1)) != -1) {
			result = cmd.substring(1, id);
			cmd = cmd.substring(id);
		} else {
			result = cmd.substring(1);
			cmd = "";
		}
	}

	return result;
}

const char* MenuMain::_LN(const char* key) {
	return GlobalLangFile::getInstance()->getStringForKey(key);
}

void MenuMain::_PR(Json::Value &m, const TString &caption, const TString &name, const TString &type) {
	int id = 0;

	m["DO"] = "MENU";

	if (m["MENU"].isArray())
		id = m["MENU"].size();

	m["MENU"][id]["C"] = caption.getTextUTF8();	//실제 메뉴에 표시되는 텍스트
	m["MENU"][id]["N"] = name.getTextUTF8();	//위치스택에 표시할 이름
	m["MENU"][id]["T"] = type.getTextUTF8();	

	//타입, 
	//A:화살표 (S/ )
	//W:웹
	//N:일반
	//B:가로 바
	//S:스위치 (Y/N)
}

void MenuMain::_MN(Json::Value &m, const TString &captionName, const TString &name, const TString &type) {
	TString msg;
	msg.setTextUTF8(_LN(captionName.getTextUTF8()));
	
	_PR(m, msg, name, type);
}

void MenuMain::_TT(Json::Value &m, const TString &captionName) {
	m["TITLE"] = _LN(captionName.getTextUTF8());
}

TString MenuMain::_MSG( const TString &captionName )
{
	TString message;
	message.setTextUTF8(_LN(captionName.getTextUTF8()));
	return "{\"DO\":\"MESSAGE\", \"MESSAGE\":\"" + message + "\"}";
}

TString MenuMain::getMenu( const TString &cmd )
{
	TString next = cmd;
	TString cur = getCurMenu(next);

	AdMain::showLineAd();

	if (cur == "MAINMENU")
		return getMenuMain(next);
	else if (cur == "DETAILMENU")
		return getMenuGameDetail(next);
	else if (cur == "ENCODINGMENU")
		return getMenuEncoding(next);
	else if (cur == "NOTICEMENU")
		return getMenuNotice(next);
	else
		return R2K_MENU_RETURN_NOTHING;
}

TString MenuMain::getMenuMain( const TString &cmd )
{
	Json::Value m;
	ScreenLayerManager *slm = ScreenLayerManager::getInstance();
	GameMain *gm = GameMain::getInstance();

	TString result;
	TString next = cmd;
	TString cur = getCurMenu(next);

	_TT(m, "APPMENU_TITLE");

	if (cur.isEmpty()) {
		bool isToSave = true;
		bool isToTitle = true;
		bool isToGameList = true;

		switch((ScreenType) (gm->lsd.System.screen)) {
			case scr_app_main:	isToSave = isToTitle = isToGameList = false;break;
			case scr_title:		isToSave = isToTitle = false;break;
			case scr_battle:	isToSave = false;break;
		}
		
		_MN(m, "APPMENU_SET", "SETTING", "A");
		_MN(m, "APPMENU_FEEDBACK", "FEEDBACK", "A");
		_MN(m, "APPMENU_HELP", "HELP", "A");

		if (isToSave)	_MN(m, "APPMENU_TO_SAVE", "TO_SAVE", "A");
		if (isToTitle)	_MN(m, "APPMENU_TO_TITLE", "TO_TITLE", "A");
		if (isToGameList)_MN(m, "APPMENU_TO_GAMELIST", "TO_GAMELIST", "A");
		_MN(m, "APPMENU_QUIT_APP", "TO_QUIT", "A");
		
		if (!MultiOutter::isPro())
			_MN(m, "APPMENU_BUY_PRIMIUM", "BUY", "A");

		result.setTextUTF8(m.toStyledString().c_str());
		return result;

	} else if (cur == "SETTING") {
		return getMenuSetting(next);
	} else if (cur == "FEEDBACK") {
		return getMenuFeedback(next);
	} else if (cur == "HELP") {
		TString answer = getCurMenu(next);
		if (answer.isEmpty()) {	
			_TT(m, "APPMENU_HELP");
			_MN(m, "APPMENU_HELP_INST_RTP", "INST_RTP", "W");
			_MN(m, "APPMENU_HELP_INST_GAME", "INST_GAME", "W");
			_MN(m, "APPMENU_HELP_SUP_GAMES", "SUP_GAMES", "W");

			result.setTextUTF8(m.toStyledString().c_str());
			return result;
		} else if (answer == "INST_RTP") {
			MultiOutter::openCPWeb("manual.jsp#Install_RTP");
		} else if (answer == "INST_GAME") {
			MultiOutter::openCPWeb("manual.jsp#Install_Games");
		} else if (answer == "SUP_GAMES") {
			MultiOutter::openCPWeb("list.jsp");
		}
	} else if (cur == "TO_SAVE") {
		ScreenLayerManager::getInstance()->doAction(INCOMING_CMD_GOTO_SAVE);
	} else if (cur.substring(0, 3) == "TO_") {
		TString answer = getCurMenu(next);
		if (answer.isEmpty()) {	
			if (cur == "TO_TITLE")
				_MN(m, "APPMENU_TO_TITLE_CAPT", "NONE", "N");
			else if (cur == "TO_GAMELIST")
				_MN(m, "APPMENU_TO_GAMELIST_CAPT", "NONE", "N");
			else if (cur == "TO_QUIT")
				_MN(m, "APPMENU_QUIT_APP_CAPT", "NONE", "N");
			_MN(m, "APPMENU_YES", "YES", "A");
			_MN(m, "APPMENU_NO", "NO", "A");

			result.setTextUTF8(m.toStyledString().c_str());
			return result;
		} else if (answer == "YES") {
			ScreenLayerManager::getInstance()->setScrPaused(false);
			if (cur == "TO_TITLE")
				ScreenLayerManager::getInstance()->doAction(INCOMING_CMD_GOTO_TITLE);
			else if (cur == "TO_GAMELIST")
				ScreenLayerManager::getInstance()->doAction(INCOMING_CMD_GOTO_GAMELIST);
			else if (cur == "TO_QUIT")
				ScreenLayerManager::getInstance()->doAction(INCOMING_CMD_QUIT_APP);
		} else if (answer == "NO") {
			return R2K_MENU_RETURN_BACK;
		} else if (answer == "NONE") {
			return R2K_MENU_RETURN_NOTHING;
		}
	} else if (cur == "BUY") {
		TString answer = getCurMenu(next);
		if (answer.isEmpty()) {	
			_TT(m, "APPMENU_BUY_PRIMIUM");
			_MN(m, "APPMENU_BUY_PRIMIUM_BUY", "START", "A");
#if (SR_ANDROID == 1)
			_MN(m, "APPMENU_BUY_PRIMIUM_ANDROID1", "NONE", "N");
			_MN(m, "APPMENU_BUY_PRIMIUM_ANDROID2", "NONE", "N");
			_MN(m, "APPMENU_BUY_PRIMIUM_ANDROID3", "NONE", "N");
			_MN(m, "APPMENU_BUY_PRIMIUM_ANDROID4", "NONE", "N");
#else //#elif (SR_IPHONE == 1)
			_MN(m, "APPMENU_BUY_PRIMIUM_IOS1", "NONE", "N");
			_MN(m, "APPMENU_BUY_PRIMIUM_IOS2", "NONE", "N");
#endif

			result.setTextUTF8(m.toStyledString().c_str());
			return result;
		} else if (answer == "START") {
			MultiOutter::buyPro();
		} else if (answer == "NONE") {
			return R2K_MENU_RETURN_NOTHING;
		}
		MultiOutter::buyPro();
	}

	return R2K_MENU_RETURN_CLOSE;
}
