#ifndef __R2K_NETWORK_MAIN__
#define __R2K_NETWORK_MAIN__
#include "../COMMON/DebugingCode.h"

#include "cocos2d.h"
USING_NS_CC;

#include "../DATA/TString.h"

class NetMain{
	static NetMain *g_pInstance;

	TString serverHost;
	TString lastUserID;

public:
	static NetMain *getInstance();
	static void removeInstance();

	NetMain();
	~NetMain();

	bool getData(const TString &request, TString &html);

	void sendAppStart();
	bool sendGameStart(const TString &path, const TString &hash, const TString &name);
	bool sendBugReport(const TString &path, const TString &hash, const TString &name, const TString &email, const TString &detail);

	bool getTranslatedText( const TString &src, TString &dst );

	const TString& getUserID();
	bool getASCEncryptedString(const TString &input, TString &output);

	TString getEncodedText( const TString &input );
};

#endif