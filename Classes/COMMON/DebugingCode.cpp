#include "DebugingCode.h"

#include "../AppMulti.h"
#include "../DATA/TString.h"

#include "cocos2d.h"
USING_NS_CC;

int last_clock;

inline void CCLOGT(const TString &str) {
	CCLOG("%s", str.getTextUTF8());
}

void initClock() {
	last_clock = (int)clock();
}

int defClock() {
	int cur_clock = (int)clock();
	int def_clock = cur_clock - last_clock;
	last_clock = cur_clock;
	return def_clock;
}

void defClockPrint(TString name) {
	int cur_clock = (int)clock();
	int def_clock = cur_clock - last_clock;
	last_clock = cur_clock;

	if (def_clock > 1)
		CCLOG("%s : %d", name.getTextUTF8(), def_clock);
}

#if (SR_WIN == 1)
void doToast( const TString &str )
{
    
}
#endif

#if (SR_ANDROID == 1)
#include "cocos2d.h"
#include "../android/jni/JniHelper.h"
void doToast( const TString &str )
{
// it make error.. why? 
// Logcat say "invalid address or address of corrupt block 0x63fe0238 passed to dlfree"


	cocos2d::JniMethodInfo t;

	if (!cocos2d::JniHelper::getStaticMethodInfo(t, 
		"com/sweetier/mrpg2kplayer/chocoplayer", 
		"doToast", 
		"(Ljava/lang/String;)V"))
		return;

	TString tstr = str;

	jstring jstr = t.env->NewStringUTF(tstr.getTextUTF8());

	t.env->CallStaticVoidMethod(t.classID, t.methodID, jstr);

	t.env->DeleteLocalRef(jstr);
	t.env->DeleteLocalRef(t.classID);
}
#endif

#if (SR_IPHONE == 1)
void doToast( const TString &str )
{
    
}
#endif
