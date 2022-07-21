#ifndef __R2K_GRAPHIC_UNZIP_CONTROL__
#define __R2K_GRAPHIC_UNZIP_CONTROL__
#include "../COMMON/DebugingCode.h"

#include <cocos2d.h>
USING_NS_CC;

#include <cstring>
#include "UnzipFolder.h"
#include "FileType.h"
#include "minizip/unzip.h"
#include "../DATA/TString.h"

using namespace std;
class UnzipCTL
{
	unzFile uf_;

	bool isOpened;

	UnzipFolder folders[R2K_FILE_TYPES];
	std::string gamePath;
	int gamePathLen;

	bool isRTP;
	int bufSize;
	char *pBuf;
	
	TString lastFileExt;
public:
	UnzipCTL();
	~UnzipCTL();

	bool open( const TString &path, bool isRTP = false );
	void close();
	bool findGameFolder(bool isRTP);
	bool getData(File_Type_Name type, const TString &name, char** data, int* datasize);
	bool isReadable( File_Type_Name type, const TString & name );
	
	void getFileList( ArrayList<TString> &fileList );
    void printFileList();

	TString getLastExt();
};

#endif
