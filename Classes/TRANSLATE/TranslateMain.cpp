#include "TranslateMain.h"

#include "../GameMain.h"
#include "../FILEIO/TSecure.h"
#include "../COMMON/MgrSetting.h"

#include "cocos-ext.h"
USING_NS_CC_EXT;


TranslateMain::TranslateMain()
	:m_loaded(false)
{

}

TranslateMain::~TranslateMain()
{
	m_root.clear();
}

bool TranslateMain::Init()
{
	GameMain *main = GameMain::getInstance();

	if (main == NULL)
		return false;

	return Init(main->pathLang + main->hashCode + ".lang");
}

bool TranslateMain::Init(const TString& path)
{
	if (m_lastPath != path) {
		ifstream is(path.getTextUTF8(), ios::binary);
	
		Json::Reader reader;

		m_root.clear();
		
		m_loaded = reader.parse(is, m_root);

		return m_loaded;
	} else {
		return true;
	}
}

bool TranslateMain::getMsg( const TString &str, TString &res, int selcnt )
{
	bool success = false, result;
	TString list[4], msg, trans;
	int from = 0, to, listcnt = 0;

	while((to = str.indexOf('\n', from)) >= 0 && listcnt < 4) {
		list[listcnt++] = str.substring(from, to);
		from = to + 1;
	}
	

	if (listcnt-selcnt > 0) {
		for(int i=0; i<listcnt-selcnt; i++)
			msg = msg + list[i] + "\n";

		chkMsg(msg);

		//CCLog(msg.getTextUTF8());

		result = getMsg(msg, trans);
		success |= result;
		if (result)
			res = trans;
		else
			res = msg;
	}

	for(int i=_MAX(listcnt-selcnt,0); i<listcnt; i++) {
		chkMsg(list[i]);
		result = getMsg(list[i], trans);
		success |= result;
		if (result)
			res = res + "\n" + trans;
		else
			res = res + "\n" + list[i];
	}

	return success;
}

bool TranslateMain::getMsg( const TString &str, TString &res )
{
	MgrSetting *s = MgrSetting::getInstance();
	if (s->getBool(RST_TRANSLATE_LDB))
		return NetMain::getInstance()->getTranslatedText(str, res);
	else 
		return false;
	/*
	if (!m_loaded)
		return false;

	TString hash = TSequre::getMD5(str);

	TString vmap, vevt, vpage, vdata;

	if (m_root.isNull())
		return false;

	Json::Value &jsaying = m_root["SAYING"];
	if (jsaying.isNull())
		return false;

	Json::Value &jtrans = jsaying[hash.getTextUTF8()];
	if (jtrans.isNull())
		return false;

	res.setTextUTF8(jtrans.asString().c_str());

	return true;
	*/
}

bool TranslateMain::getCharName( int charID, TString &res )
{
	return false;
}

bool TranslateMain::getItemName( int itemID, TString &res )
{
	if (!m_loaded)
		return false;

	if (m_root.isNull())
		return false;

	Json::Value &jitems = m_root["ITEM"];
	if (!jitems.isArray() || itemID <= 0 || jitems.size() < itemID)
		return false;

	Json::Value &jitem = jitems[itemID-1];

	Json::Value &jval = jitem["NAME"];
	if (jval.isNull())
		return false;

	res.setTextUTF8(jval.asString().c_str());

	return true;
}

bool TranslateMain::getItemDesc( int itemID, TString &res )
{
	if (!m_loaded)
		return false;

	if (m_root.isNull())
		return false;

	Json::Value &jitems = m_root["ITEM"];
	if (!jitems.isArray() || itemID <= 0 || jitems.size() < itemID)
		return false;

	Json::Value &jitem = jitems[itemID-1];

	Json::Value &jval = jitem["DESC"];
	if (jval.isNull())
		return false;

	res.setTextUTF8(jval.asString().c_str());

	return true;
}

bool TranslateMain::getSkillName( int skillID, TString &res )
{
	if (!m_loaded)
		return false;

	if (m_root.isNull())
		return false;

	Json::Value &jitems = m_root["SKILL"];
	if (!jitems.isArray() || skillID <= 0 || jitems.size() < skillID)
		return false;

	Json::Value &jitem = jitems[skillID-1];

	Json::Value &jval = jitem["NAME"];
	if (jval.isNull())
		return false;

	res.setTextUTF8(jval.asString().c_str());

	return true;
}

bool TranslateMain::getSkillDesc( int skillID, TString &res )
{
	if (!m_loaded)
		return false;

	if (m_root.isNull())
		return false;

	Json::Value &jitems = m_root["SKILL"];
	if (!jitems.isArray() || skillID <= 0 || jitems.size() < skillID)
		return false;

	Json::Value &jitem = jitems[skillID-1];

	Json::Value &jval = jitem["DESC"];
	if (jval.isNull())
		return false;

	res.setTextUTF8(jval.asString().c_str());

	return true;
}
