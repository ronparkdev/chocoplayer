#ifndef __R2K_MENU_MAIN__
#define __R2K_MENU_MAIN__

#include "../DATA/TString.h"
#include "../FILEIO/lib_json/json.h"

#define R2K_MENU_RETURN_CLOSE "{\"DO\": \"CLOSE\"}"
#define R2K_MENU_RETURN_BACK "{\"DO\": \"BACK\"}"
#define R2K_MENU_RETURN_BACKREFRESH "{\"DO\": \"BACKREFRESH\"}"
#define R2K_MENU_RETURN_NOTHING "{\"DO\": \"NOTHING\"}"
#define R2K_MENU_RETURN_JOYSTICK "{\"DO\": \"JOYSTICK\"}"

class MenuMain{
public:
	static TString getMenu( const TString &cmd );
	
	static TString getMenuMain( const TString &cmd );
	static TString getMenuSetting( const TString &cmd );
	static TString getMenuFeedback( const TString &cmd );
	static TString getMenuFeedbackGame( const TString &cmd );

	static TString getMenuGameDetail( const TString &cmd );
	static TString getMenuEncoding( const TString &cmd );
	static void setMenuEncoding( const TString &spath, const TString &sisarchive, const TString &slang);

	static TString getMenuNotice( const TString &cmd );
	static TString getMenuNoticeRTP( const TString &cmd );
	static TString getMenuNoticeAgreeTerms( const TString &cmd );
	static TString getMenuNoticeFail( const TString &cmd );

private:
	static const char* _LN(const char* key);
	static void _PR(Json::Value &m, const TString &caption, const TString &name, const TString &type);
	static void _MN(Json::Value &m, const TString &captionName, const TString &name, const TString &type);
	static void _TT(Json::Value &m, const TString &captionName);
	static TString _MSG(const TString &captionName);
	static TString getCurMenu(TString &cmd);
};

#endif