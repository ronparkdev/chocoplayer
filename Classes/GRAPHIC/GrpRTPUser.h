#ifndef __R2K_GRAPHIC__RTP_USER__
#define __R2K_GRAPHIC__RTP_USER__
#include "../COMMON/DebugingCode.h"

#include "ImageType.h"
#include "GrpRTPNode.h"
#include "../COMMON/ScreenCommon.h"
#include "../DATA/TString.h"
#include "../DATA/ArrayList.h"
#include "../FILEIO/FileType.h"
#include "../FILEIO/UnzipCTL.h"

#define GRP_RTP_USER_DATA "RTP.zip"

class GrpRTPUser{

	GameVersion ver;

	UnzipCTL fctl;

	ArrayList<GrpRTPNode> nodes;
	int nodeLen;
public:

	bool Init(const TString &rootpath);

	void setVersion( GameVersion game_ver );

	bool getData(File_Type_Name ctype, const TString &name, char **data, int *datasize );
	TString findName(File_Type_Name ctype, const TString &str );
};

#endif
