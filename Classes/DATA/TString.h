#ifndef __R2K_DATA__TSTRING__
#define __R2K_DATA__TSTRING__
#include "../COMMON/DebugingCode.h"

// 이 함수는 다국어를 지원하지않는 Windows의 MultiChar 문자열 대신에
// 다국어를 지원하는 WideChar중 UTF-8을 내부적으로 사용하고있습니다. 
// 또한 내부적으로 UTF-8를 사용하는 Cocos2d에 적합한 문자열입니다. 

// 기본적인 입력은 MultiChar이며, 언어별로 인코딩을 설정한 후에 입력하면됩니다.

#define _ST(str) TString(str)

#include <iostream>
#include <cmath>
#include "BaseStruct.h"

class TString{
private:
	char *pData;
	int length;
	static int defEncoding;
public:
	TString();
	TString(const char *cstr);
	TString(const std::string &str);
	TString(const TString &tstr);
	TString(int value);
	~TString();

    char* getTextUTF8() const;
    void getTextUTF8_s(char *cstr, int bufsize) const;
    void getText_s(char *cstr, int bufsize) const;
    
	void setText(const char *cstr);
	void setText(const std::string &str);	
	void setText(const TString &tstr);
	void setText(int value);				//정수를 받음
	void setText(int value, int strlen);	//정수를 받음. 남은 공간에는 0으로 채움
	void setTextUTF8(const char *cstr);		//UTF8 형태의 데이터를 입력받음
	void setTextHEX(int value);				//정수를 받아서 16진수형태로 입력
	void setChar(int c);					//UTF8 형태의 한문장을 입력받음

	int getInt();							//문장에서 숫자를 반환

	int getSize() const;					//UTF-8 타입의 byte크기를 반환
	int getLength() const;					//글자갯수를 반환
	int charAt(int pos) const;				//pos위치에 있는 글자를 반환
	int indexOf(int c) const;				//c 글자를 찾아서 위치를 반환, 없으면 -1
	int indexOf(int c, int startpos) const;	//startpos 이후 위치에있는 c 글자를 찾아서 위치를 반환, 없으면 -1
	int indexOf(const TString &findstr) const;	//findstr의 문장을 찾아 위치를 반환, 없으면 -1

	int lastIndexOf(int c) const;				//c 글자를 찾아서 위치를 반환, 없으면 -1
	int lastIndexOf(int c, int startpos) const;	//startpos 이후 위치에있는 c 글자를 찾아서 위치를 반환, 없으면 -1

	TString substring(int frompos) const;
	TString substring(int frompos, int topos) const; //frompos <= c < topos 위치에있는 문장을 반환함

	static void setDefEncoding(int encid);	
	int getEncoding() const;
	bool isEmpty() const; 
	
	void operator=(const char *cstr);
	void operator=(const TString &tstr);

	TString toUpper() const;
	TString toLower() const;

	void serialize(StructLoader &sl);

	friend bool operator==(const TString &tstr, const char *cstr);
	friend bool operator==(const char *cstr, const TString &tstr);
	friend bool operator==(const TString &tstr1, const TString &tstr2);

	friend bool operator!=(const TString &tstr, const char *cstr);
	friend bool operator!=(const char *cstr, const TString &tstr);
	friend bool operator!=(const TString &tstr1, const TString &tstr2);

	friend TString operator+(const TString &tstr, const char *cstr);
	friend TString operator+(const char *cstr, const TString &tstr);
	friend TString operator+(const TString &tstr1, const TString &tstr2);

	friend TString operator+(const TString &tstr, int num);
	friend TString operator+(int num, const TString &tstr);

	friend std::ostream& operator<<(std::ostream &os, TString &tstr);
};

bool operator==(const TString &tstr, const char *cstr);
bool operator==(const char *cstr, const TString &tstr);
bool operator==(const TString &tstr1, const TString &tstr2);

bool operator!=(const TString &tstr, const char *cstr);
bool operator!=(const char *cstr, const TString &tstr);
bool operator!=(const TString &tstr1, const TString &tstr2);

TString operator+(const TString &tstr, const char *cstr);
TString operator+(const char *cstr, const TString &tstr);
TString operator+(const TString &tstr1, const TString &tstr2);

TString operator+(const TString &tstr, int num);
TString operator+(int num, const TString &tstr);

#endif
