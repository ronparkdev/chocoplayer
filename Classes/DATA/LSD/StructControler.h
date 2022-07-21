#ifndef __R2K__DATA_LSDOLD_STRUCT_CONTROLER__
#define __R2K__DATA_LSDOLD_STRUCT_CONTROLER__

#include <iostream>
#include <fstream>
using namespace std;

#include "../TString.h"
#include "../ArrayList.h"

#define STRUCT_CONTROLER_PATH_MAX 1024

class StructControler{
public:

	fstream m_fs;

	char *fin;
	int finc, finid;

	char btype, blen;
	char *bbuf;

	bool FileLoadOpen( const TString &path );
	void FileLoadClose();

	bool ReadBool();

	bool ReadBoolFix();

	char ReadByte();

	short ReadShortFix();

	int ReadShortFixInv();

	int ReadUInt();

	int ReadUIntB();

	int ReadIntFixInv();

	long ReadAInt();

	double ReadDouble();

	TString ReadString();

	void ReadGarb();

	void ReadVecInt(ArrayList<int> &list);
};

#endif
