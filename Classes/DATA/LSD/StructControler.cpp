#include "StructControler.h"

#include <math.h>

bool StructControler::FileLoadOpen( const TString &path )
{
	char pathW[STRUCT_CONTROLER_PATH_MAX];
#if (SR_WIN == 1)
	path.getText_s(pathW,STRUCT_CONTROLER_PATH_MAX);
#else
	strcpy(pathW, path.getTextUTF8());
#endif

	m_fs.open( pathW , ios::in | ios::binary);

	if (m_fs.is_open()) {
		m_fs.seekg(0, ios::end);
		finc = (int)m_fs.tellg();
		finid = 0;
		fin = new char[finc];

		m_fs.clear(); 
		m_fs.seekg(0, ios::beg);
		m_fs.read(fin, finc);
		m_fs.close();

		return true;
	}

	return false;
}

void StructControler::FileLoadClose()
{
	delete []fin;
}

bool StructControler::ReadBool()
{
	long size = ReadAInt();

	if (size < 0)
		;

	return ReadBoolFix();
}

bool StructControler::ReadBoolFix()
{
	return ((fin[finid++]!=0)?true:false);
}

char StructControler::ReadByte()
{
	return fin[finid++];
}

short StructControler::ReadShortFix()
{
	short back = fin[finid];
	short front = fin[finid+1];
	if (back<0)
		back += 256;
	if (front<0)
		front += 256;
	short s = (short) (front * 256 + back);
	finid += 2;
	return s;
}

int StructControler::ReadShortFixInv()
{
	int s = ( fin[finid+1]*0x100 | fin[finid+0]);
	finid += 2;

	return s;
}

int StructControler::ReadUInt()
{
	int intsize = (int) ReadAInt();
	int pos = finid + intsize, val;
	if (intsize == 0)
		val = 0;
	else{
		val = (int) ReadAInt();
	}
	finid = pos;
	return val;
}

int StructControler::ReadUIntB()
{
	int i, s = 0;
	long intsize = ReadAInt();

	if (intsize < 0)
		;

	int chk;
	for(i = 0 ; i < intsize ; i++)
	{
		chk = fin[finid+i];
		if (chk < 0)
			chk += 128;
		s += pow(128.0f,i)*chk;
	}

	finid += intsize;

	return s;
}

long StructControler::ReadAInt()
{
	long intsize = 0;

	char i;
	do
	{
		i = fin[finid];
		if ((i & 0x80) == 0x80)
			i += 128;

		intsize = intsize*128 + i;

		finid++;
	}while((fin[finid-1] & 0x80) == 0x80);

	return intsize;
}

double StructControler::ReadDouble()
{
	double dOut = 1.0f;
	long size = ReadAInt();

	if (size < 0)
		;

	if (size != 8) {
		finid += size;
		return 0.0f;//Not IEEE Type!
	}
	bool b[64];	
	for(int i=0; i<8; i++) {
		int c = fin[finid + 7-i];
		for(int j=7; j>=0; j--) {
			b[i*8+j] = ((c&0x1) == 0x1?true:false);
			c >>= 1;
		}
	}

	finid += 8;

	int exp = 0;
	for(int i=10; i>=0; i--) {
		exp <<= 1;
		if (b[1+i])exp += 0x1;
	}
	exp -= 1023;

	double dT = 1.0f;
	for(int i=0; i<52; i++) {
		dT /= 2.0f;
		if (b[12+i])dOut += dT;
	}

	dOut *= pow(2.0f, exp);

	if (b[0])dOut *= -1.0f;

	return dOut;
}

TString StructControler::ReadString()
{
	int i;
	long strsize = ReadAInt();
	char *strbuf = new char[(int)strsize + 1];

	for(i = 0 ; i < strsize ; i++)
		strbuf[i] = fin[finid+i];
	strbuf[strsize] = '\0';

	finid += strsize;

	TString oStr;
	oStr = strbuf;
	delete []strbuf;

	return oStr;
}

void StructControler::ReadGarb()
{
	long intsize = ReadAInt();
	
	if (intsize < 0)
		;

	finid += intsize;
}

void StructControler::ReadVecInt( ArrayList<int> &list )
{
	long VectorSize = ReadAInt();
	long VectorIDLimit = finid + VectorSize;
	long VectorCount = ReadAInt();

	list.RemoveAll();

	if (VectorSize < 0)
		;

	if (VectorCount <= 0)
		return;

	list.SetLength(VectorCount);

	int i=0;
	while(finid < VectorIDLimit)
	{	        		
		list[i++] = (int)ReadAInt();
	}
}

int StructControler::ReadIntFixInv()
{
	int s = 0;
	for(int i=3; i>=0; i--)
		s = s << 8 | (fin[finid+i] & 0xFF);

	finid += 4;

	return s;
}
