#include "SndMain.h"

#if (SR_ANDROID == 1)

#include "../android/jni/JniHelper.h"

void SndMain::PlaySound( const GlbSound &val ) {
	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doPlaySound", 
		"(Ljava/lang/String;III)V"))
		return;

	if (val.name == "(OFF)")
		return;

	TString path = findSoundPath(val.name);

	if (path.isEmpty()) {
		//CCLOG((_ST("SndMain:PlaySound cant find file <") + val.name + ">").getTextUTF8());
		return;
	}

	jstring jpath = t.env->NewStringUTF(path.getTextUTF8());

	t.env->CallStaticVoidMethod(t.classID,t.methodID, 
		jpath, val.volume, val.tempo, val.balance);

	t.env->DeleteLocalRef(jpath);
	t.env->DeleteLocalRef(t.classID);
	
	//findSoundPath(m_gamepath + "/Sound", val.name);
}

void SndMain::StopSound() {
	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doStopSound", 
		"()V"))
		return;

	t.env->CallStaticVoidMethod(t.classID,t.methodID);
	t.env->DeleteLocalRef(t.classID);
}

void SndMain::PlayMusic( const GlbMusic &val ) {
	
	pLsd->System.music_current = val;

	if (val.name == "(OFF)") {
		StopMusic();
		return;
	}

	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doPlayMusic", 
		"(Ljava/lang/String;IIII)V"))
		return;

	if (val.name == "(OFF)")
		return;

	TString path = findMusicPath(val.name);

	if (path.isEmpty()) {
		//CCLOG((_ST("SndMain:PlaySound cant find file <") + val.name + ">").getTextUTF8());
		StopMusic();
		return;
	}

	jstring jpath = t.env->NewStringUTF(path.getTextUTF8());

	t.env->CallStaticVoidMethod(t.classID,t.methodID, 
		jpath, val.fadein, val.volume, val.tempo, val.balance);

	t.env->DeleteLocalRef(jpath);
	t.env->DeleteLocalRef(t.classID);
}

void SndMain::StopMusic() {

	pLsd->System.music_current.name = "(OFF)";

	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doStopMusic", 
		"()V"))
		return;

	t.env->CallStaticVoidMethod(t.classID,t.methodID);
	t.env->DeleteLocalRef(t.classID);
}

void SndMain::FadeoutMusic(int sec) {
	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doFadeoutMusic", 
		"(I)V"))
		return;

	t.env->CallStaticVoidMethod(t.classID,t.methodID, sec);
	t.env->DeleteLocalRef(t.classID);
}

void SndMain::onPause()
{
	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doPause", 
		"()V"))
		return;

	t.env->CallStaticVoidMethod(t.classID,t.methodID);
	t.env->DeleteLocalRef(t.classID);
}

void SndMain::onResume()
{
	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doResume", 
		"()V"))
		return;

	t.env->CallStaticVoidMethod(t.classID,t.methodID);
	t.env->DeleteLocalRef(t.classID);
}

void SndMain::onRelease()
{
	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doRelease", 
		"()V"))
		return;

	t.env->CallStaticVoidMethod(t.classID,t.methodID);
	t.env->DeleteLocalRef(t.classID);
}

void SndMain::doCheck(int framedelta)
{
	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doCheck", 
		"(I)V"))
		return;

	t.env->CallStaticVoidMethod(t.classID,t.methodID, framedelta);
	t.env->DeleteLocalRef(t.classID);
}


#endif
