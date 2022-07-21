
#ifndef __R2K_COMMON_GLOBAL_LANGUAGE_FILE__
#define __R2K_COMMON_GLOBAL_LANGUAGE_FILE__

//code : http://discuss.cocos2d-x.org/t/tutorial-multiple-language-support/16237

#include <cocos2d.h>

#include <CocoStudio/Json/rapidjson/document.h>
#include <CocoStudio/Json/rapidjson/rapidjson.h>

USING_NS_CC;
using namespace rapidjson;

#include <iostream>
#include <cstring>
using std::string;

class GlobalLangFile{
	Document document; // current document with language data
	GlobalLangFile(); // constructor is private
	static GlobalLangFile* _instance;
	static string g_temp_string;
public:
	static GlobalLangFile* getInstance();
	static void removeInstance();

	const char* getStringForKey(string key);
};

#endif
