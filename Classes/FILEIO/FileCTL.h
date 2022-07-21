#ifndef __R2K_GRAPHIC_FILE_CONTROL__
#define __R2K_GRAPHIC_FILE_CONTROL__
#include "../COMMON/DebugingCode.h"

#include <fstream>
#include "FileType.h"
#include "UnzipCTL.h"
#include "../DATA/TString.h"
#include "../GRAPHIC/GrpRTP.h"
#include "../GRAPHIC/GrpRTPUser.h"

class FileCTL
{
	GrpRTP * pRTP;
	GrpRTPUser * pRTPUser;

	bool isOpened;
	bool isArchive;

	TString openedPath;
	
	char *pBuf;
	int bufSize;
	
	UnzipCTL unzCtl;
	
	TString lastFileExt;

public:
	FileCTL();
	~FileCTL();

	void Init( GrpRTP *rtp, GrpRTPUser *rtpu );
	bool open( const TString &path, bool is_archive );
	void close();

	bool getData(File_Type_Name type, const TString &name, char** data, int* datasize);
	bool loadData( const TString &path, char** data, int* datasize );
	bool isReadable( File_Type_Name type, const TString &name );

	TString getLastExt();

	static TString findEtcFullPath( const TString & path, const TString & name );
	static TString findPictureFullPath( const TString & path, const TString & name );
	static TString findSoundFullPath( const TString & path, const TString & name );
	static TString findMusicFullPath( const TString & path, const TString & name );
	static TString findMovieFullPath( const TString & path, const TString & name );
};


#endif
