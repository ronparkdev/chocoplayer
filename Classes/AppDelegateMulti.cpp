#include "AppDelegate.h"

#include "AppMulti.h"
#include "COMMON/DebugingCode.h"

USING_NS_CC;

#if (SR_WIN == 1)

TString GetGamesPath() {
	TString PATH, PINN,POUT,PRTP,PTMP;
	PATH = "C:/Users/dev/";
	PINN = PATH + "r2k";
	POUT = "";
	PRTP = PATH + "r2k/main.84.com.sweetier.mrpg2kplayer.obb";
	PTMP = PATH + "r2k_tmp";
	
	return (PINN + "#" + POUT + "#" + PRTP + "#" + PTMP).getTextUTF8();
}

#endif

#if (SR_ANDROID == 1)

#include "../android/jni/JniHelper.h"

TString GetGamesPath() {
	cocos2d::JniMethodInfo t;
	
	if (!cocos2d::JniHelper::getStaticMethodInfo(t, "com/sweetier/mrpg2kplayer/chocoplayer", "getGamesPath", "()Ljava/lang/String;"))
		return "";
	
	jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID);
	
	const char *cstr = t.env->GetStringUTFChars(str,0);
	//CCLOG(cstr);
	
	TString tstr;
	tstr.setTextUTF8(cstr);
	
	t.env->ReleaseStringUTFChars(str,cstr);
	t.env->DeleteLocalRef(t.classID);
	
	return tstr;
}

#endif

#if (SR_IPHONE == 1)

TString MultiOutterIOS_GetTempFolder();

TString GetGamesPath() {
    TString path;
    path.setTextUTF8(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
    
    if (path.getLength() > 0 && path.charAt(path.getLength()-1) == '/')
        path = path.substring(0, path.getLength()-1);
    
    return path + "##" + path + "/rtp.dat#" + MultiOutterIOS_GetTempFolder();
}

#endif
