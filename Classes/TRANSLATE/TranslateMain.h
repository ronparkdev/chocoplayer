#ifndef __R2K_TRANSLATE_MAIN__
#define __R2K_TRANSLATE_MAIN__
#include "../COMMON/DebugingCode.h"

#include "../FILEIO/FileCTL.h"
#include "../FILEIO/lib_json/json.h"
#include "../DATA/LDB/LdbMain.h"
#include "../NETWORK/NetMain.h"

class TranslateMain{
	Json::Value m_root;
	
	bool m_loaded;
	TString m_lastPath;
public:
	TranslateMain();
	~TranslateMain();

	bool Init();
	bool Init(const TString& path);
	bool getMsg(const TString &str, TString &res);
	bool getMsg(const TString &str, TString &res, int selcnt);
	bool getCharName(int charID,	TString &res);
	bool getItemName(int itemID,	TString &res);
	bool getItemDesc(int itemID,	TString &res);
	bool getSkillName(int skillID,	TString &res);
	bool getSkillDesc(int skillID,	TString &res);

	//static void saveDataToJson( const char *path, TRANSLATE_NET_STATE *state, int *cnt, int *cnttot);
	static void chkMsg(TString &str);
};

#endif