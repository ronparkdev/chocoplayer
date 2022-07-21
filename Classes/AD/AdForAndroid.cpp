#include "AdMain.h"
#include "../AppMulti.h"

#if (SR_ANDROID == 1)

#include "../android/jni/JniHelper.h"

void AdMain::showBigAd()
{
	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doShowAdBig", 
		"()V"))
		return;

	t.env->CallStaticVoidMethod(t.classID,t.methodID);
	t.env->DeleteLocalRef(t.classID);
}

void AdMain::hideBigAd()
{
	
}

void AdMain::showLineAd()
{
	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doShowAdLine", 
		"()V"))
		return;

	t.env->CallStaticVoidMethod(t.classID,t.methodID);
	t.env->DeleteLocalRef(t.classID);
}

void AdMain::hideLineAd()
{
	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doHideAdLine", 
		"()V"))
		return;

	t.env->CallStaticVoidMethod(t.classID,t.methodID);
	t.env->DeleteLocalRef(t.classID);
}

#endif