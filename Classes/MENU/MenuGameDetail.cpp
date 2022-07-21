#include "MenuMain.h"

#include "../GameMain.h"
#include "../COMMON/GlobalLang.h"
#include "../COMMON/MultiOutter.h"
#include "../SCENE/APPMAIN/ScrAppMain.h"
#include "../SCENE/APPMAIN/ScrAppMainGameNode.h"

TString MenuMain::getMenuGameDetail( const TString &cmd )
{
	Json::Value m;
	TString spath, sisarchive, cur, next, result;
	ScrAppMainGameNode node;

	next = cmd;
	spath = getCurMenu(next);
	sisarchive = getCurMenu(next);
	cur = getCurMenu(next);
	
	if (sisarchive == "Y")
		node.setArchive(spath);
	else
		node.setFolder(spath);

	node.loadSettings();

	if (node.getType() == GameNodeNothing)
		return R2K_MENU_RETURN_CLOSE;

	int encID = node.getEncID();
	TString sEnc;
	if (0 <= encID && encID < ENCODING_COUNT)
		sEnc = EncodingCaption[encID];
	else
		sEnc = "Not Selected";

	if (cur.isEmpty()) {
		_PR(m, "Path : "	+ node.getShortPath()	, "NONE", "N");
		_PR(m, "Name : "	+ node.getName()		, "NONE", "N");
		_PR(m, "Encoding : "+ sEnc					, "ENC", "A");
		_PR(m, "Delete this Game"					, "DEL", "A");

		m["TITLE"] = "Game Detail";
		result.setTextUTF8(m.toStyledString().c_str());
		return result;
	} else if (cur == "ENC") {
		TString answer = getCurMenu(next);
		if (answer.isEmpty()) {	
			for(int i=0; i<ENCODING_COUNT; i++)
				_PR(m, EncodingCaption[i], EncodingCaption[i], "A");

			m["TITLE"] = "Game Encoding";
			result.setTextUTF8(m.toStyledString().c_str());
			return result;
		} else {
			setMenuEncoding(spath, sisarchive, answer);

			m["DO"] = "BACK_REFRESH";
			m["REFRESH_TO"] = ("|DETAILMENU|" + spath + "|" + sisarchive).getTextUTF8();

			result.setTextUTF8(m.toStyledString().c_str());
			return result;
		}
	} else if (cur == "DEL") {
		TString answer = getCurMenu(next);
		if (answer.isEmpty()) {	
			_PR(m, "Delete it!(with savefile)", "YES", "A");
			_PR(m, "no no no", "NO", "N");

			m["TITLE"] = "Game Delete";
			result.setTextUTF8(m.toStyledString().c_str());
			return result;
		} else if (answer == "YES") {
			MultiOutter::deleteFolder(spath);

			if (GameMain::getInstance()->lsd.System.screen == scr_app_main) {
				ScrAppMain *layer = (ScrAppMain *)ScreenLayerManager::getInstance()->getCurLayer();
				layer->requestRemove(spath);
			}

			return R2K_MENU_RETURN_CLOSE;

		} else if (answer == "NO") {
			return R2K_MENU_RETURN_BACK;
		}
	} else if (cur == "NONE") {
		return R2K_MENU_RETURN_NOTHING;
	} else {
		return R2K_MENU_RETURN_CLOSE;
	}

	return R2K_MENU_RETURN_CLOSE;
}

TString MenuMain::getMenuEncoding( const TString &cmd )
{
	Json::Value m;
	TString spath, sisarchive, next, result;
	ScrAppMainGameNode node;

	next = cmd;
	spath = getCurMenu(next);
	sisarchive = getCurMenu(next);

	if (sisarchive == "Y")
		node.setArchive(spath);
	else
		node.setFolder(spath);

	if (node.getType() == GameNodeNothing)
		return R2K_MENU_RETURN_CLOSE;

	TString answer = getCurMenu(next);
	if (answer.isEmpty()) {	
		for(int i=0; i<ENCODING_COUNT; i++)
			_PR(m, EncodingCaption[i], EncodingCaption[i], "A");

		m["TITLE"] = "Game Encoding";
		result.setTextUTF8(m.toStyledString().c_str());
		return result;
	} else {
		setMenuEncoding(spath, sisarchive, answer);

		return R2K_MENU_RETURN_CLOSE;
	}
}

void MenuMain::setMenuEncoding( const TString &spath, const TString &sisarchive, const TString &slang )
{
	int i;
	ScrAppMainGameNode node;

	if (sisarchive == "Y")
		node.setArchive(spath);
	else
		node.setFolder(spath);

	node.loadSettings();
	for(i=0; i<ENCODING_COUNT; i++)
		if (slang == EncodingCaption[i]) {
			node.setEncID(i);
			node.saveSettings();
			break;
		}

	if (GameMain::getInstance()->lsd.System.screen == scr_app_main) {
		ScrAppMain *layer = (ScrAppMain *)ScreenLayerManager::getInstance()->getCurLayer();
		layer->requestRefresh(spath);
	}
}
