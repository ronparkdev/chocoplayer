#ifndef __R2K__UNZIP_FOLDER__
#define __R2K__UNZIP_FOLDER__
#include "../COMMON/DebugingCode.h"

#include "minizip/unzip.h"
#include "../DATA/ArrayList.h"

class UnzipFolder   
{
public:
	ArrayList<unz_file_pos> pos_start;
	ArrayList<int> pos_length;

	UnzipFolder();
	~UnzipFolder();
};

#endif
