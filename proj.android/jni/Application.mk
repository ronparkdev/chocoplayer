APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -lm -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1 -std=c++0x
APP_CFLAGS := -Wno-error=format-security -Wno-format-security -fexceptions -Wno-write-strings
APP_CXXFLAGS := -fpermissive 