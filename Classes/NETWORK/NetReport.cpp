#include "NetMain.h"

#include "../FILEIO/lib_json/json.h"
#include "../GameMain.h"

void NetMain::sendAppStart()
{
	TString res;
	if (getData("cmd=AppStart", res)) {
		CCLOG("%s", res.getTextUTF8());
	}
}

bool NetMain::sendGameStart( const TString &path, const TString &hash, const TString &name )
{
	bool result;
	TString req, res, fixedName;

	if (name.isEmpty()) {
		fixedName = "null";
	} else {
		fixedName = name;
	}

	req = _ST("cmd=GameStart") + 
		"&user=" + getUserID() +
		"&path=" + path +
		"&name=" + fixedName +
		"&hash=" + hash;

	if (getData(req, res)) {
		Json::Value root;
		Json::Reader reader;
		reader.parse(res.getTextUTF8(), root);

		result = (((TString)root["result"].asString()) == "success");
	}

	return result;
}

bool NetMain::sendBugReport( const TString &path, const TString &hash, const TString &name, const TString &email, const TString &detail )
{
	bool result;
	TString req, res, fixedName;

	if (fixedName.isEmpty()) {
		fixedName = "null";
	} else {
		fixedName = name;
	}

	req = _ST("cmd=BugReport") + 
		"&user=" + getUserID() +
		"&path=" + path +
		"&name=" + fixedName +
		"&hash=" + hash +
		"&email=" + email +
		"&detail=" + detail;

	CCLOG("[REQ]%s", req.getTextUTF8());

	if (getData(req, res)) {
		CCLOG("[RES]%s", res.getTextUTF8());

		Json::Value root;
		Json::Reader reader;
		reader.parse(res.getTextUTF8(), root);

		result = (((TString)root["result"].asString()) == "success");
	}

	return result;
}

bool NetMain::getTranslatedText( const TString &src, TString &dst )
{
	bool result;
	TString req, res;

	req = _ST("cmd=Translate") + 
		"&user=" + getUserID() +
		"&text=" + getEncodedText(src) +
		"&tgt=" + GameMain::getInstance()->getLangCode();

	CCLOG("[REQ]%s", req.getTextUTF8());

	if (getData(req, res)) {
		CCLOG("[RES]%s", res.getTextUTF8());

		Json::Value root;
		Json::Reader reader;
		reader.parse(res.getTextUTF8(), root);

		result = (((TString)root["result"].asString()) == "success");
		if (result) {
			std::string ttext = root["text"].asCString();
			dst.setTextUTF8(ttext.c_str());
		}
	}

	return result;
}
