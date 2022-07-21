#ifndef __R2K_SCREEN_APP_MAIN_GAME_NODE__
#define __R2K_SCREEN_APP_MAIN_GAME_NODE__

#include "cocos2d.h"
USING_NS_CC;

#include "../../DATA/TString.h"
#include "../../FILEIO/FileCTL.h"

enum GameNodeType{
	GameNodeNothing,
	GameNodeHelp,
	GameNodeArchive,
	GameNodeFolder
};

enum GameNodeState{
	GameNodeTitleNotLoaded,
	GameNodeTitlePrepared,
	GameNodeTitleLoaded
};

class ScrAppMainGameNode
{
private:
	GameNodeType type;
	TString path;

	TString pathSetting;
	int		sTitleEncID;
	TString sTitleName;
	TString sGameName;
	TString sHashCode;
	int		sEncId;
	int		sColor;

	GameNodeState stateTitle;
	CCTexture2D *pTexTitle;
	CCImage *pImgTitle;
	int versionID;

	float loadedTime;
	bool isLoadFailed;
public:
	ScrAppMainGameNode();
	~ScrAppMainGameNode();

	void set(const TString & path, bool isArchive);
	void setFolder(const TString &path);
	void setArchive(const TString &path);
	void setHelp();

	void reload();
	
	GameNodeType getType();
	GameNodeState getState();
	CCTexture2D* getTitleTex();

	void loadTitleWithMainThread();
	void loadDataWithSubThread();

	bool isNeedLoading();
	void setTitleImage(CCImage *image);
	float getLoadedTime();

	TString getName();
	TString getPath();
	TString getShortPath();
	TString getHash();
	int getEncID();
	void setEncID(int encId);
	bool isArchive();

	void getGameData( TString &gamepath, bool &archive, TString &hashCode, int &encodingID );

	void loadSettings();
	void saveSettings();
	void reloadSettings();
	int reloadSettingsTry( FileCTL *fctl, char * ldbData, int ldbDataSize, int encID );
};

#endif