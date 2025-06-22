#include "AppDelegate.h"
#include "cocos2d.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "main"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

// using namespace cocos2d; // Optional: can be added globally or used with cocos2d:: prefix

extern "C"
{

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JniHelper::setJavaVM(vm);
    // Any essential very early C++ initializations could go here,
    // but most game logic starts in AppDelegate::applicationDidFinishLaunching().
    return JNI_VERSION_1_6; // Or JNI_VERSION_1_4 if targeting older NDKs/devices
}

/**
 * This function is called from the Java side when the surface is created.
 * It's part of the Cocos2d-x Android lifecycle.
 * In v3, you typically don't need to add much here as the engine handles
 * the director and view setup.
 */
void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv* env, jobject thiz, jint w, jint h)
{
    // cocos2d::Director::getInstance()->setViewport(); // Example if specific viewport setup needed early
    // This function is called after the GL context is created.
    // AppDelegate::applicationDidFinishLaunching() will be called after this.
}

// If your old main.cpp had other JNI functions:
// 1. Evaluate if they are still needed.
// 2. If yes, move them to relevant C++ classes.
// 3. Use JniHelper or manual JNI registration to expose them to Java if called from Java.
// 4. If they were utility functions for C++ only, they can remain in appropriate classes.

// The global g_DefaultGamesPath and g_pAppDelegate from the old main.cpp:
// - g_DefaultGamesPath logic, if needed, should be handled within your game's C++ code,
//   possibly initialized in AppDelegate or a file manager class.
// - AppDelegate instance is managed by the Cocos2d-x engine in v3.
}
