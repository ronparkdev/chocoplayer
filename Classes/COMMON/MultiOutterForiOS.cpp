
#include "MultiOutter.h"
#include "../AppMulti.h"

#if (SR_IPHONE == 1)

void MultiOutterIOS_OpenSomething(const char *url);
void MultiOutterIOS_GetModel(char *str, int strlen);
void MultiOutterIOS_GetUniqueID(char *str, int strlen);
void MultiOutterIOS_OpenMenu(char *stack);
void MultiOutterIOS_OpenCloudPicker();
void MultiOutterIOS_removeFolder(char *path);

void MultiOutter::openMenu(const TString &stack) {
    MultiOutterIOS_OpenMenu(stack.getTextUTF8());
}

void MultiOutter::openGameAdd() {
    MultiOutterIOS_OpenCloudPicker();
}

void MultiOutter::openWeb(const TString &url) {
	MultiOutterIOS_OpenSomething(url.getTextUTF8());
}


void MultiOutter::openMovieReal(const TString &path) {

}

TString MultiOutter::getModel() {
	char buf[128];
	MultiOutterIOS_GetModel(buf, 128);

	return TString(buf);
}

TString MultiOutter::getUniqueID()
{
	char buf[256];
	MultiOutterIOS_GetUniqueID(buf, 512);

	TString str;
	str.setTextUTF8(buf);

	return str;
}

//IOS is not have adfree
bool MultiOutter::isPro()
{
	return true;
}

void MultiOutter::buyPro()
{

}

void MultiOutter::deleteFolder( const TString &path ) {
	MultiOutterIOS_removeFolder(path.getTextUTF8());
}

#endif
