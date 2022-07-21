#include "MenuMain.h"

#include "../DATA/ArrayList.h"
#include "../SCENE/APPMAIN/ScrAppMain.h"
#include "../SCENE/APPMAIN/ScrAppMainGameNode.h"

TString MenuMain::getMenuFeedback( const TString &cmd )
{
	Json::Value m;

	TString result;
	TString next = cmd;
	TString cur = getCurMenu(next);
	
	ArrayList<ScrAppMainGameNode> &gs = ScrAppMain::getList();

	if (cur.isEmpty()) {
		_TT(m, "APPMENU_FEEDBACK");
		_MN(m, "APPMENU_FEEDBACK_EMUL", "EMUL", "A");
		_MN(m, "APPMENU_FEEDBACK_APP", "APP", "A");
		_MN(m, "APPMENU_FEEDBACK_FUNC", "FUNC", "A");
		_MN(m, "APPMENU_FEEDBACK_ETC", "ETC", "A");

		result.setTextUTF8(m.toStyledString().c_str());
		return result;
	} else {
		if (cur == "EMUL") {
			return getMenuFeedbackGame(next);
		} else {
			m["DO"] = "FEEDBACK";
			m["FEEDBACK"]["TYPE"] = cur.getTextUTF8();

			result.setTextUTF8(m.toStyledString().c_str());
			return result;
		}
	}
}


TString MenuMain::getMenuFeedbackGame( const TString &cmd )
{
	Json::Value m;

	TString result;
	TString next = cmd;
	TString cur = getCurMenu(next);
	TString path = getCurMenu(next);
	TString hash = getCurMenu(next);
	TString name = getCurMenu(next);
	TString email = getCurMenu(next);
	TString detail = getCurMenu(next);
	TString s;
	int id;

	ArrayList<ScrAppMainGameNode> &gs = ScrAppMain::getList();

	if (cur.isEmpty()) {
		for(id=0; id<gs.GetLength(); id++) {
			ScrAppMainGameNode &g = gs[id];
			s.setText(id);
			_PR(m, g.getShortPath() + "(" + g.getName() + ")", s, "A");
		}

		m["TITLE"] = "Select Game";
		result.setTextUTF8(m.toStyledString().c_str());
		return result;
	} else {
		if (path.isEmpty()) {
			id = cur.getInt();
			ScrAppMainGameNode &g = gs[id];

			m["DO"] = "FEEDBACK";
			m["FEEDBACK"]["TYPE"] = "EMUL";
			m["FEEDBACK"]["PATH"] = g.getPath().getTextUTF8();
			m["FEEDBACK"]["HASH"] = g.getHash().getTextUTF8();
			m["FEEDBACK"]["NAME"] = g.getName().getTextUTF8();

			result.setTextUTF8(m.toStyledString().c_str());
			return result;
		} else {
			
			if (NetMain::getInstance()->sendBugReport(path, hash, name, email, detail))
				m["DO"] = "SUCCESS";
			else
				m["DO"] = "FAIL";

			result.setTextUTF8(m.toStyledString().c_str());
			return result;
		}
	}
}
