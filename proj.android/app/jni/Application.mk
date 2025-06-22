APP_STL := c++_static
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_LDFLAGS := -latomic

APP_ABI := armeabi-v7a arm64-v8a x86

# Support for NEON
APP_CPPFLAGS += -D__NEON__
LOCAL_ARM_NEON := true
