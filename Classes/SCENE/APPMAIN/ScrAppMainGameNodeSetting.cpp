#include "ScrAppMainGameNode.h"

#include "../../COMMON/ScreenCommon.h"
#include "../../FILEIO/InfoFile.h"
#include "../../FILEIO/TFolder.h"
#include "../../FILEIO/TSecure.h"
#include "../../COMMON/GlobalLang.h"
#include "../../DATA/LDB/LdbMainSimple.h"
#include "../../GameMain.h"

#define NOT_FIND "##NOT_FIND##";

#define VAL_TITLE "TITLE_NAME"
#define VAL_TITLE_ENCODING "TITLE_ENCODING"
#define VAL_HASH "LDB_MD5_CODE"
#define VAL_VER "GAME_VERSION"
#define VAL_VER_R2K0 "RPG2000"
#define VAL_VER_R2K3 "RPG2003"
#define VAL_ENCODING "GAME_ENCODING"
#define VAL_COLOR "GAME_COLOR"
#define VAL_NAME "GAME_NAME"

#define TITLE_CAN_LOAD 0
#define TITLE_IS_UNIBYTE -1
#define TITLE_CANT_LOAD -2

void ScrAppMainGameNode::loadSettings()
{
	TString str_game_ver, str_encoding, str_color, str_title_encoding;
	InfoFile info(pathSetting);

	sTitleName = info.getValue(VAL_TITLE);
	sHashCode = info.getValue(VAL_HASH);
	sGameName = info.getValue(VAL_NAME);
	str_title_encoding = info.getValue(VAL_TITLE_ENCODING);

	if (sTitleName.isEmpty() || sHashCode.isEmpty() || sGameName.isEmpty() || str_title_encoding.isEmpty()) {
		reloadSettings();
		saveSettings();
	} else {
		str_game_ver = info.getValue(VAL_VER);
		if (str_game_ver == "RPG2000")
			versionID = RPG2000;
		else if (str_game_ver == "RPG2003")
			versionID = RPG2003;
		else
			versionID = 0;

		str_encoding = info.getValue(VAL_ENCODING);


		sEncId = -1;
		for(int i=0; i<ENCODING_COUNT; i++)
			if (EncodingCaption[i] == str_encoding) {
				sEncId = i;
				break;
			}

		//CCLog("Load Enc %s->%d" , str_encoding.getTextUTF8(), sEncId);

		sTitleEncID = str_title_encoding.getInt();

		TString::setDefEncoding(sEncId);

		str_color = info.getValue(VAL_COLOR);

		if (str_color.isEmpty())
			sColor = -1;
		else
			sColor = str_color.getInt();
	}
}

void ScrAppMainGameNode::saveSettings()
{
	CCLog("ScrAppMainGameNode::saveSettings()");

	TString str_game_ver, str_encoding;

	TFolder::chkFolder(pathSetting);

	switch((GameVersion)versionID) {
	case RPG2000:str_game_ver = "RPG2000";break;
	case RPG2003:str_game_ver = "RPG2003";break;
	default:str_game_ver = "NOT_SURE";break;
	}

	if (0<=sEncId && sEncId<ENCODING_COUNT)
		str_encoding = EncodingCaption[sEncId];
	else
		str_encoding = "NOT_SURE";

	CCLog("Save Enc %d->%s" , sEncId, str_encoding.getTextUTF8());

	InfoFile info(pathSetting);	
	info.setValue(VAL_TITLE,	sTitleName);
	info.setValue(VAL_TITLE_ENCODING, sTitleEncID);
	info.setValue(VAL_VER,		str_game_ver);
	info.setValue(VAL_ENCODING,	str_encoding);
	info.setValue(VAL_HASH,		sHashCode);
	info.setValue(VAL_COLOR,	sColor);
	info.setValue(VAL_NAME,		sGameName);
	info.flush();
}

void ScrAppMainGameNode::reloadSettings()
{
	CCLog("ScrAppMainGameNode::reloadSettings()");

	int encID, result;
	FileCTL fctl;

	char *ldbData;
	int ldbDataSize;

	if (fctl.open(path, (type == GameNodeArchive))) {
		if (fctl.getData(fileFolder, "RPG_RT.ldb", &ldbData, &ldbDataSize))
		{
			for(encID=0; encID<ENCODING_COUNT; encID++) {
				result = reloadSettingsTry(&fctl, ldbData, ldbDataSize, encID);

				if (result == TITLE_IS_UNIBYTE)
					break;
				else if (result == TITLE_CAN_LOAD) {
					sTitleEncID = encID;

					break;
				}
			}

			if (sHashCode.isEmpty())
				sHashCode = TSequre::getMD5(ldbData, ldbDataSize);
		}

		if (fctl.getData(fileFolder, "RPG_RT.ini", &ldbData, &ldbDataSize)) {
			InfoFile info(ldbData, ldbDataSize);

			TString::setDefEncoding(sEncId);

			TString name;
			name.setText(info.getValue("GameTitle").getTextUTF8());

			if (name.isEmpty())
				sGameName = "#NoInfo";
			else
				sGameName = name;
		}

		fctl.close();
	}
}

int ScrAppMainGameNode::reloadSettingsTry( FileCTL *fctl, char * ldbData, int ldbDataSize, int encID )
{
	LdbMainSimple ldb;
	StructLoader sl;
	if (!sl.openWithNoFree(ldbData, ldbDataSize))
		return TITLE_CANT_LOAD;

	TString::setDefEncoding(encID);
	ldb.serialize(sl);
	sl.close();

	TString &title = ldb.dSys.title_name;

	//It is Multibyte Char? && Check Only First Encoding
	if (encID == 0) {

		if (ldb.dSys.ldb_id == 2003)
			versionID = RPG2003;
		else
			versionID = RPG2000;

// 		for(i=0; i<title.getLength(); i++) {
// 			int c = title.charAt(i);
// 
// 			if (c < 0 || c >= 128)
// 				break;
// 		}
// 
// 		if (i == title.getLength()) {
// 			titleName = title;
// 
// 			return TITLE_IS_UNIBYTE;
// 		}
	}

	GameMain *gm = GameMain::getInstance();

	gm->rtp.setVersion((GameVersion)versionID);
	gm->rtpu.setVersion((GameVersion)versionID);

	FileCTL fctl2;
	fctl2.Init(&gm->rtp, &gm->rtpu);
	if (fctl2.open(path, (type == GameNodeArchive))) {
		if (fctl2.isReadable(fileTitle, title)) {
			sTitleName = title;
			return TITLE_CAN_LOAD;
		}

		fctl2.close();
	}

	if (gm->rtp.findName(fileTitle, title).isEmpty() || 
		gm->rtpu.findName(fileTitle, title).isEmpty())
	{
		sTitleName = title;
		return TITLE_CAN_LOAD;
	}

	return TITLE_CANT_LOAD;
}
