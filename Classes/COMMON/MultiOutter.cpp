#include "MultiOutter.h"

#include "../AppMulti.h"
#include "../GameMain.h"

#include "../FILEIO/lib_json/json.h"

void MultiOutter::openMovie(const TString &name) {
	TString path = GameMain::getInstance()->snd.findMoviePath(name);
	if (path.isEmpty()) {
		openMovieReal(path);
		GameMain::getInstance()->onPause();
	}
}

#if (SR_WIN == 1)

#include <windows.h>
#include "../MENU/MenuMain.h"

void MultiOutter::openMenu(const TString &stack) {

	//ScreenLayerManager::getInstance()->doAction(INCOMING_CMD_OPEN_VKEY);
	//ScreenLayerManager::getInstance()->doAction(INCOMING_CMD_OPEN_JOY);
	CCLog("%s, %s", stack.getTextUTF8(), MenuMain::getMenu(stack).getTextUTF8());
}

void MultiOutter::openGameAdd() {
	CCLOG("openMenu");
}

void MultiOutter::openWeb(const TString &url) {}
void MultiOutter::openMovieReal(const TString &path) {}

bool MultiOutter::isPro() {return false;}
void MultiOutter::buyPro() {}

TString MultiOutter::getModel() {return "PC";}
TString MultiOutter::getUniqueID() {
	return "it_is_test_unique_id";
}

void MultiOutter::deleteFolder( const TString &spath ) {
	char buf[1024] = {0, };
	sprintf(buf, "rm -rf %s", spath.getTextUTF8());
	
	system(buf);
}

#endif

#include "../SCENE/APPMAIN/ScrAppMain.h"

TString MultiOutter::getInfo( int type )
{
	Json::FastWriter writer;
	TString result;

	switch((IncomingCommand)type) {
	case INCOMING_CMD_GET_GAMES_INFO:
		{
			Json::Value root;
			ArrayList<ScrAppMainGameNode> &gs = ScrAppMain::getList();

			Json::Value games(Json::arrayValue);
			for(int i=0; i<gs.GetLength(); i++) {
				ScrAppMainGameNode &g = gs[i];
				Json::Value node;

				node["path"] = g.getShortPath().getTextUTF8();
				node["name"] = g.getName().getTextUTF8();
				node["hash"] = g.getHash().getTextUTF8();

				games[i] = node;
			}

			root["games"] = games;

			result.setTextUTF8(writer.write(root).c_str());
			break;
		}

	case INCOMING_CMD_GET_SUPPORT_MENUS:
		{
			Json::Value root;
			int screen = GameMain::getInstance()->lsd.System.screen;

			bool isToSave = true;
			bool isToTitle = true;
			bool isToGameList = true;
			
			switch((ScreenType) screen) {
			case scr_app_main:
				isToSave = isToGameList = isToTitle = false;
				break;

			case scr_title:
				isToSave = isToTitle = false;
				break;

			case scr_battle:
				isToSave = false;
				break;
			}

			root["to_save"]		= isToSave;
			root["to_title"]	= isToTitle;
			root["to_gamelist"]	= isToGameList;

			result.setTextUTF8(writer.write(root).c_str());
			break;
		}
	}

	return result;
}

void MultiOutter::closedGameAdd()
{
	if (GameMain::getInstance()->lsd.System.screen == scr_app_main) {
		ScrAppMain *layer = (ScrAppMain *)ScreenLayerManager::getInstance()->getCurLayer();
		layer->requestRefreshAll();
	}
}

void MultiOutter::openCPWeb( const TString &param )
{
	TString url, sparam, squestion, ssharp;
	int pquestion, psharp;

	if ((pquestion = param.indexOf("?")) == -1)
		pquestion = param.getLength();

	if ((psharp = param.indexOf("#")) == -1)
		psharp = param.getLength();

	sparam = param.substring(0, _MIN(pquestion, psharp));
	if (pquestion < psharp) {
		squestion	= param.substring(pquestion, psharp);
		ssharp		= param.substring(psharp);
	} else {
		squestion	= param.substring(pquestion);
		ssharp		= param.substring(psharp, pquestion);
	}

	if (squestion.isEmpty())
		squestion = "?deviceLang=" + GameMain::getInstance()->getLangCode();
	else
		squestion = squestion + "&deviceLang=" + GameMain::getInstance()->getLangCode();

#if (SR_ANDROID == 1)
	squestion = squestion + "&deviceType=android";
#else // (SR_IPHONE == 1)	
	squestion = squestion + "&deviceType=ios";
#endif

	url = "http://chocolatlsweet.cafe24.com/choco/" + sparam + squestion + ssharp;

	openWeb(url);
}

bool MultiOutter::isProA()
{
#if (SR_ANDROID == 1)
	return isPro();
#else
	return true;
#endif
}


bool MultiOutter::isProI()
{
#if (SR_IPHONE == 1)
	return isPro();
#else
	return true;
#endif
}
