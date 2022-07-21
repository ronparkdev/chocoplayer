#ifndef __R2K_INFO_FILE__
#define __R2K_INFO_FILE__
#include "../COMMON/DebugingCode.h"

#include <fstream>
#include "../DATA/TString.h"
#include "../DATA/ArrayList.h"

#define INFO_FILE_PATH_MAX 1024

using namespace std;

class InfoFile
{
	TString openedPath;
	ArrayList<TString> datName;
	ArrayList<TString> datValue;

public:
	InfoFile(TString path);
	InfoFile( char *data, int datasize );
	~InfoFile();

	TString getValue( const TString &name );
	void setValue(const TString &name, const TString &value);
	void flush();
};


#endif