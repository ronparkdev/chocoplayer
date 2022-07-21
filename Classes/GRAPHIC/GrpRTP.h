#ifndef __R2K_GRAPHIC__RTP__
#define __R2K_GRAPHIC__RTP__
#include "../COMMON/DebugingCode.h"

#include "ImageType.h"
#include "GrpRTPNode.h"
#include "../COMMON/ScreenCommon.h"
#include "../DATA/TString.h"
#include "../DATA/ArrayList.h"
#include "../FILEIO/FileType.h"
#include "../FILEIO/UnzipCTL.h"

#define GRP_RTP_DATA "FileLangFit.dat"

static TString CURRENT_RTP_VERSION = "RTPVERSION1.1";

class GrpRTP{

	GameVersion ver;

	UnzipCTL fctl;

	ArrayList<GrpRTPNode> nodes;
	int nodeLen;
public:

	bool Init(const TString &rtppath);

	void setVersion( GameVersion game_ver );

	bool getData(File_Type_Name ctype, const TString &name, char **data, int *datasize );
	TString findName(File_Type_Name ctype, const TString &str );
};
#endif
