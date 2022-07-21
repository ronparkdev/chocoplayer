#include "MenuMain.h"

#include "../COMMON/MultiOutter.h"

TString MenuMain::getMenuNotice( const TString &cmd )
{
	TString next = cmd;
	TString cur = getCurMenu(next);

	if (cur == "RTP")
		return getMenuNoticeRTP(next);
	else if (cur == "FAIL")
		return getMenuNoticeFail(next);
	else
		return R2K_MENU_RETURN_CLOSE;

}

TString MenuMain::getMenuNoticeRTP( const TString &cmd )
{
	Json::Value m;
	TString cur, next, result;
	
	next = cmd;
	cur = getCurMenu(next);
	
	if (cur.isEmpty()) {	
		_MN(m, "NOTICE_RTP_CAPT", "NONE", "N");
		_MN(m, "NOTICE_RTP_YES", "Y", "W");
		_MN(m, "NOTICE_RTP_NO", "N", "A");

		result.setTextUTF8(m.toStyledString().c_str());
		return result;
	} else {
		if (cur == "Y") {
			MultiOutter::openCPWeb("manual.jsp#Install_RTP");
			return R2K_MENU_RETURN_CLOSE;
		} else if (cur == "N") {
			return R2K_MENU_RETURN_CLOSE;
		} else {
			return R2K_MENU_RETURN_NOTHING;
		}
	}
}

TString MenuMain::getMenuNoticeAgreeTerms( const TString &cmd ) {
	Json::Value m;
	TString cur, next, result;

	next = cmd;
	cur = getCurMenu(next);

	if (cur.isEmpty()) {	
		_MN(m, "For use this application, \nyou must agree terms and conditions", "NONE", "N");
		_MN(m, "See Terms of Use", "TERMS", "W");
		_MN(m, "See Private Policy", "PRIVATE", "W");
		_MN(m, "Agree all", "Y", "A");
		_MN(m, "Disagree (turn off the app)", "N", "A");

		result.setTextUTF8(m.toStyledString().c_str());
		return result;
	} else {
		if (cur == "TERMS") {
			MultiOutter::openWeb("http://chocolatlsweet.cafe24.com/choco/detail.jsp#Terms_of_Use");
			return R2K_MENU_RETURN_NOTHING;

		} else if (cur == "PRIVATE") {
			MultiOutter::openWeb("http://chocolatlsweet.cafe24.com/choco/detail.jsp#Privacy_Policy");
			return R2K_MENU_RETURN_NOTHING;

		} else if (cur == "Y") {
			return R2K_MENU_RETURN_CLOSE;
		
		} else if (cur == "N") {
			return R2K_MENU_RETURN_CLOSE;

		} else {
			return R2K_MENU_RETURN_NOTHING;
		}
	}
}

TString MenuMain::getMenuNoticeFail( const TString &cmd ) {
	Json::Value m;
	TString cur, next, result;

	next = cmd;
	cur = getCurMenu(next);

	_TT(m, "NOTICE_FAIL_TITLE");
	if (cur == "FILE") {
		_MN(m, "NOTICE_FAIL_FILE", "NONE", "N");
	} else if (cur == "LDB") {
		_MN(m, "NOTICE_FAIL_LDB", "NONE", "N");
	} else if (cur == "LMT") {
		_MN(m, "NOTICE_FAIL_LMT", "NONE", "N");
	} else {
		return R2K_MENU_RETURN_CLOSE;
	}

	result.setTextUTF8(m.toStyledString().c_str());
	return result;
}