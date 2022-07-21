#ifndef __R2K__CC_FOLDER__
#define __R2K__CC_FOLDER__
#include "../COMMON/DebugingCode.h"

#include "../DATA/TString.h"

#ifdef _WIN32
#include <io.h>
#include <conio.h>
#else
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#endif

#define CCFOLDER_PATH_MAX 1024

class TFolder{
private:
	bool isFolderOpened;

	TString openedPath;
	char openedPathC[CCFOLDER_PATH_MAX];

	bool isLastFolder;

#ifdef _WIN32
	_finddata_t m_fd;
	long m_handle;
#else
	DIR *m_dp;
	struct dirent *m_dirp;
#endif

public:
	TFolder();
	~TFolder();

	bool open(TString path);
	void close();
	bool next();
	bool isfolder();
	TString get();
	TString getName();

	static void chkFolder(TString path);
};

#endif