
#include "GlobalLangFile.h"
#include "../AppMulti.h"


GlobalLangFile* GlobalLangFile::_instance = NULL;

string GlobalLangFile::g_temp_string = "";

GlobalLangFile* GlobalLangFile::getInstance()
{
	if (!_instance)
		_instance = new GlobalLangFile();
	return _instance;
}

void GlobalLangFile::removeInstance()
{
	if (_instance)
		delete _instance;

	_instance = NULL;
}


GlobalLangFile::GlobalLangFile()
{
	string fileName;
	// detect current language


#if (SR_WIN == 1)
	srand(time(NULL));
	switch(1) {
		case 0:fileName = "lang_kr.json";break;
		case 1:fileName = "lang_en.json";break;
	}
#else
	switch(CCApplication::sharedApplication()->getCurrentLanguage()) 
	{
	case kLanguageKorean:
		fileName = "lang_kr.json";
		break;
	case kLanguageJapanese:
		fileName = "lang_jp.json";
		break;
	default:
		CCLOG("GlobalLangFile::Unknown language. Use english");
		fileName = "lang_en.json";
		break;
	};
#endif
	// below we open, read and parse language data file with rapidjson library
	unsigned long size;
	const char* buf = (const char*)CCFileUtils::sharedFileUtils()->getFileData(fileName.c_str(), "r", &size);
	string content(buf);
	string clearContent = content.substr(0, content.rfind('}') + 1);

	document.Parse<0>(clearContent.c_str());
	if (document.HasParseError())
	{
		CCLOG("GlobalLangFile::Language file parsing error!");
		return;
	}
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

const char* GlobalLangFile::getStringForKey(string key)
{
    if (!document.IsNull() && !document[key.c_str()].IsNull())
        g_temp_string = document[key.c_str()].GetString();
    else
        g_temp_string = "";
    
	replaceAll(g_temp_string, "\\n", "\n");

	return g_temp_string.c_str();
}