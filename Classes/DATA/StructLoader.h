#ifndef __R2K_DATA__STRUCT_LOADER__
#define __R2K_DATA__STRUCT_LOADER__
#include "../COMMON/DebugingCode.h"

#define STRUCT_LOADER_BUFFER_MIN 2048
#define STRUCT_LOADER_PATH_MAX 512
#define CODE_END_OF_STRUCT 0

typedef unsigned char byte;

#include <iostream>
#include <fstream>
#include <cmath>

#ifdef _WIN32
#define __ISNAN		_isnan
#define __FINITE	_finite
#else
#define __ISNAN		isnan
#define __FINITE	finite
#endif

using namespace std;
class StructLoaderException_WrongStruct{};

enum StructLoaderMode{
	StructLoaderModeNone = -1,
	StructLoaderModeInit,
	StructLoaderModeRead,
	StructLoaderModeWrite
};

class StructLoader{
private:
    fstream m_fs;
    
    bool opened;
    char openedPath[STRUCT_LOADER_PATH_MAX];
	StructLoaderMode mode;

	bool memoryDoNotDealloc;
	char *pData;
	int dataLength;
	int offset;
public:
	StructLoader();
	~StructLoader();

	bool open(bool isRead, const char *path);
	bool open(char *data, int datalen, bool useSharedMemory = false);
	bool close();

	bool isOpened();
	void stopOpening();

	void allocForWrite(int newpos);
	bool isSizeReadable(int size);

	void setInitMode();
	StructLoaderMode getMode();

	bool isNextCode(int type);
	void intoCode();

	void finishStruct();
	void writeStructSize(int lastpos);

	int getOffset();
	void setOffset(int pos);
	int getLength();

	char* getRawData();

public:
	friend void readDouble	( StructLoader &sl,		   double &val );
	friend void writeDouble	( StructLoader &sl, const  double &val );

	friend void readFloat	( StructLoader &sl,		   float &val );
	friend void writeFloat	( StructLoader &sl,	const  float &val );
    

	friend StructLoader& operator<<(StructLoader &sl, const double &val);
	friend StructLoader& operator>>(StructLoader &sl,		double &val);
	friend StructLoader& operator<<(StructLoader &sl, const float &val);
	friend StructLoader& operator>>(StructLoader &sl,		float &val);
	friend StructLoader& operator<<(StructLoader &sl, const int &val);
	friend StructLoader& operator>>(StructLoader &sl,		int &val);
	friend StructLoader& operator<<(StructLoader &sl, const unsigned int &val);
	friend StructLoader& operator>>(StructLoader &sl,		unsigned int &val);
	friend StructLoader& operator<<(StructLoader &sl, const unsigned short &val);
	friend StructLoader& operator>>(StructLoader &sl,		unsigned short &val);
	friend StructLoader& operator<<(StructLoader &sl, const unsigned char &val);
	friend StructLoader& operator>>(StructLoader &sl,		unsigned char &val);
	friend StructLoader& operator<<(StructLoader &sl, const bool &val);
	friend StructLoader& operator>>(StructLoader &sl,		bool &val);
};

#endif
