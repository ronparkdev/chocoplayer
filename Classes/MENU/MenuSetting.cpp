#include "MenuMain.h"

#include "../COMMON/MultiOutter.h"
#include "../COMMON/MgrSetting.h"
#include "../SCENE/ScreenLayerManager.h"
#include "../GameMain.h"

TString MenuMain::getMenuSetting( const TString &cmd )
{
	Json::Value m;
	TString result;
	TString next = cmd;
	TString cur = getCurMenu(next);
	TString answer = getCurMenu(next);
	TString val = getCurMenu(next);
	bool valB = (val == "Y");
	
	if (cur.isEmpty()) {
		_TT(m, "APPMENU_SET");
		_MN(m, "APPMENU_SET_VKEY",	"VKEY", "N");
		_MN(m, "APPMENU_SET_JOY",	"JOYSTICK", "A");
		_MN(m, "APPMENU_SET_ETC",	"ETC", "A");

		result.setTextUTF8(m.toStyledString().c_str());
		return result;
	} else if (cur == "VKEY") {
		ScreenLayerManager::getInstance()->doAction(INCOMING_CMD_OPEN_VKEY);
		return R2K_MENU_RETURN_CLOSE;
	} else if (cur == "JOYSTICK") {
		ScreenLayerManager::getInstance()->doAction(INCOMING_CMD_OPEN_JOY);
		return R2K_MENU_RETURN_CLOSE;
		/*
		if (answer.isEmpty()) {
			//이건 네이티브단에서 처리..ㅎㅎ
			return R2K_MENU_RETURN_JOYSTICK;
		} else {
			int keyTgt = answer.getInt();
			int keyJoy = answer.getInt();

			MgrSetting *setting;
			if ((setting = MgrSetting::getInstance()) != NULL) {
				setting->setInt(RST_JOYSTICK_KEYCODE + keyTgt, keyJoy);
			}
		}
		*/
	} else if (cur == "ETC") {
		MgrSetting *s = MgrSetting::getInstance();
		if (answer.isEmpty()) {

			_TT(m, "APPMENU_SET_ETC");
			_MN(m, "APPMENU_SET_ETC_DEBUG",		"DEBUG",		s->getBool(RST_ETC_DEBUG)		?"SY":"SN");
			_MN(m, "APPMENU_SET_ETC_AUTOFIRE",	"AAUTOFIRE",	s->getBool(RST_ETC_AUTOMSG)		?"SY":"SN");
			_MN(m, "APPMENU_SET_ETC_FRAMESKIP",	"FRAMERATE",	s->getBool(RST_ETC_FRAMESKIP)	?"SY":"SN");
			_MN(m, "APPMENU_SET_ETC_VSCREEN",	"VSCREEN",		s->getBool(RST_ETC_IS_VERTICAL)	?"SY":"SN");
			_MN(m, "APPMENU_SET_ETC_TRNAS_MSG",	"TRANSMSG",		s->getBool(RST_TRANSLATE_LDB)	?"SY":"SN");
			
			result.setTextUTF8(m.toStyledString().c_str());
			return result;
		} else if (answer == "DEBUG") {
			s->setBool(RST_ETC_DEBUG,		valB);
			s->SaveAll();
		} else if (answer == "AAUTOFIRE") {
			s->setBool(RST_ETC_AUTOMSG,		valB);
			s->SaveAll();
		} else if (answer == "FRAMERATE") {
			s->setBool(RST_ETC_FRAMESKIP,	valB);
			s->SaveAll();
		} else if (answer == "VSCREEN") {
			s->setBool(RST_ETC_IS_VERTICAL, valB);
			s->SaveAll();
			if (!MultiOutter::isProA() && !valB) {
				return _MSG("APPMENU_SET_ETC_VSCREEN_LIMITED");
			}
		} else if (answer == "TRANSMSG") {
			s->setBool(RST_TRANSLATE_LDB, valB);
			s->SaveAll();
		}
		return R2K_MENU_RETURN_NOTHING;
	}
	return R2K_MENU_RETURN_NOTHING;
}