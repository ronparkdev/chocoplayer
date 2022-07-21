
#include "MultiOutter.h"
#include "../AppMulti.h"

#if (SR_ANDROID == 1)

#include "../android/jni/JniHelper.h"
#include <jni.h>

#include "ComMath.h"
#include "GameMain.h"

#include "MgrSetting.h"
#include "../MENU/MenuMain.h"

void MultiOutter::openMenu(const TString &stack) {
	cocos2d::JniMethodInfo t;
	if (!cocos2d::JniHelper::getStaticMethodInfo(t,
		"com/sweetier/mrpg2kplayer/chocoplayer",
		"doOpenMenu", "(Ljava/lang/String;)V"))
		return;

	jstring jstack = t.env->NewStringUTF(stack.getTextUTF8());

	t.env->CallStaticVoidMethod(t.classID, t.methodID, jstack);
	t.env->DeleteLocalRef(jstack);
}

void MultiOutter::openGameAdd() {
	cocos2d::JniMethodInfo t;
	if (!cocos2d::JniHelper::getStaticMethodInfo(t,
		"com/sweetier/mrpg2kplayer/chocoplayer",
		"doOpenGameAdd", "()V"))
		return;

	t.env->CallStaticVoidMethod(t.classID, t.methodID);
}

void MultiOutter::openWeb(const TString &url) {
	cocos2d::JniMethodInfo t;
	if (!cocos2d::JniHelper::getStaticMethodInfo(t,
		"com/sweetier/mrpg2kplayer/chocoplayer",
		"doOpenWeb",
		"(Ljava/lang/String;)V"))
		return;

	jstring jurl = t.env->NewStringUTF(url.getTextUTF8());

	t.env->CallStaticVoidMethod(t.classID, t.methodID, jurl);
	t.env->DeleteLocalRef(jurl);
	t.env->DeleteLocalRef(t.classID);
}

void MultiOutter::openMovieReal(const TString &path) {
	cocos2d::JniMethodInfo t;
	if (!cocos2d::JniHelper::getStaticMethodInfo(t,
		"com/sweetier/mrpg2kplayer/chocoplayer",
		"doOpenMovie",
		"(Ljava/lang/String;)V"))
		return;

	jstring jpath = t.env->NewStringUTF(path.getTextUTF8());

	t.env->CallStaticVoidMethod(t.classID, t.methodID, jpath);
	t.env->DeleteLocalRef(jpath);
	t.env->DeleteLocalRef(t.classID);
}

TString MultiOutter::getModel()
{
	JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t
		, "com/sweetier/mrpg2kplayer/chocoplayer" //org/cocos2dx/lib/Cocos2dxActivity
		, "getModel"
		, "()Ljava/lang/String;"))
		return "FIND_FUNCTION_FAIL";

	jstring jstr = NULL; 
	jstr = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);

	const char *cstr = t.env->GetStringUTFChars(jstr,0);

	TString tstr;
	tstr.setTextUTF8(cstr);

	t.env->ReleaseStringUTFChars(jstr,cstr);
	t.env->DeleteLocalRef(t.classID);

	return tstr;
}

TString MultiOutter::getUniqueID()
{
	JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t
		, "com/sweetier/mrpg2kplayer/chocoplayer" //org/cocos2dx/lib/Cocos2dxActivity
		, "getUserID"
		, "()Ljava/lang/String;"))
		return "FIND_FUNCTION_FAIL";

	jstring jstr = NULL; 
	jstr = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);

	const char *cstr = t.env->GetStringUTFChars(jstr,0);

	TString tstr;
	tstr.setTextUTF8(cstr);

	t.env->ReleaseStringUTFChars(jstr,cstr);
	t.env->DeleteLocalRef(t.classID);

	return tstr;
}

bool MultiOutter::isPro()
{
	cocos2d::JniMethodInfo t;
	if (!cocos2d::JniHelper::getStaticMethodInfo(t,
		"com/sweetier/mrpg2kplayer/chocoplayer",
		"isPro",
		"()Z"))
		return false;

	jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
	t.env->DeleteLocalRef(t.classID);

	return result;
}

void MultiOutter::buyPro()
{
	cocos2d::JniMethodInfo t;
	if (!cocos2d::JniHelper::getStaticMethodInfo(t,
		"com/sweetier/mrpg2kplayer/chocoplayer",
		"doBuyPro",
		"()V"))
		return;

	t.env->CallStaticVoidMethod(t.classID, t.methodID);
	t.env->DeleteLocalRef(t.classID);
}

void MultiOutter::deleteFolder( const TString &spath ) {
	cocos2d::JniMethodInfo t;
	if (!cocos2d::JniHelper::getStaticMethodInfo(t,
		"com/sweetier/mrpg2kplayer/chocoplayer",
		"doDeleteFolder",
		"(Ljava/lang/String;)V"))
		return;

	jstring jpath = t.env->NewStringUTF(spath.getTextUTF8());

	t.env->CallStaticVoidMethod(t.classID, t.methodID, jpath);
	t.env->DeleteLocalRef(jpath);
	t.env->DeleteLocalRef(t.classID);
}

#define MULTI_OUTTER_FILE_MAX 256


#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL Java_com_sweetier_mrpg2kplayer_menu_MgrMenu_getMenu
	( JNIEnv* env, jobject obj, jstring jcmd)
{
	TString cmd, result;
	const char *scmd = (*env).GetStringUTFChars(jcmd, NULL);
	cmd.setTextUTF8(scmd);
	(*env).ReleaseStringUTFChars(jcmd, scmd);

	jstring jresult = (*env).NewStringUTF(MenuMain::getMenu(cmd).getTextUTF8());

	return jresult;
}

JNIEXPORT jint JNICALL Java_com_sweetier_mrpg2kplayer_menu_MgrMenu_getValue
								( JNIEnv* env, jobject obj, jint jtype)
{
	MgrSetting *setting;
	if ((setting = MgrSetting::getInstance()) == NULL)
		return -2;
	
	return setting->getAutoInt(jtype);
}


JNIEXPORT void JNICALL Java_com_sweetier_mrpg2kplayer_menu_MgrMenu_setValue
	( JNIEnv* env, jobject obj, jint jtype, jint jvalue)
{
	MgrSetting *setting;
	if ((setting = MgrSetting::getInstance()) == NULL)
		return;

	setting->setAutoInt(jtype, jvalue);
}

JNIEXPORT jboolean JNICALL Java_com_sweetier_mrpg2kplayer_menu_MgrMenu_doBugReport
	( JNIEnv* env, jobject obj, jstring jpath, jstring jhash, jstring jemail, jstring jdetail)
{
	bool result;
	TString path, hash, email, detail;

	const char *spath	= (*env).GetStringUTFChars(jpath, NULL);
	const char *shash	= (*env).GetStringUTFChars(jhash, NULL);
	const char *semail	= (*env).GetStringUTFChars(jemail, NULL);
	const char *sdetail = (*env).GetStringUTFChars(jdetail, NULL);
	path	.setTextUTF8(spath);
	hash	.setTextUTF8(shash);
	email	.setTextUTF8(semail);
	detail	.setTextUTF8(sdetail);

	result = NetMain::getInstance()->sendBugReport(path, hash, "", email, detail);

	(*env).ReleaseStringUTFChars(jdetail, sdetail);
	(*env).ReleaseStringUTFChars(jemail, semail);
	(*env).ReleaseStringUTFChars(jhash, shash);
	(*env).ReleaseStringUTFChars(jpath, spath);

	return result;
}

JNIEXPORT jstring JNICALL Java_com_sweetier_mrpg2kplayer_chocoplayer_doAction
	( JNIEnv* env, jobject obj, jint jtype)
{
	switch((IncomingCommand)jtype) {
	case INCOMING_CMD_OPEN_VKEY:
	case INCOMING_CMD_GOTO_TITLE:
	case INCOMING_CMD_GOTO_GAMELIST:
	case INCOMING_CMD_QUIT_APP:
		ScreenLayerManager::getInstance()->doAction(jtype);
		return NULL;

	case INCOMING_CMD_CLOSE_GAME_ADD:
		MultiOutter::closedGameAdd();
		return NULL;

	case INCOMING_CMD_GET_GAMES_INFO:
	case INCOMING_CMD_GET_SUPPORT_MENUS:
		return (*env).NewStringUTF(MultiOutter::getInfo(jtype).getTextUTF8());
	}

	return NULL;
}

#ifdef __cplusplus
}
#endif

#endif
