LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

# Define source files
FILE_LIST := \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/AD/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/COMMON/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/CONTROL/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/DATA/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/DATA/LMT/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/DATA/LSD/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/DATA/LSDCHK/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/EVENT/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/FILEIO/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/FILEIO/lib_json/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/FILEIO/md5/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/FILEIO/minizip/*.c) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/FILEIO/zlib_1.2.3/*.c) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/GRAPHIC/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/GRAPHIC/OBJECT/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/MENU/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/NETWORK/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/APPJOY/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/APPMAIN/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/APPVKEY/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/BATTLE/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/GAMEOVER/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/MAP/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/MENU/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/NAMEINPUT/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/SAVE/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/SHOP/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SCENE/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/SOUND/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../../../../Classes/TRANSLATE/*.cpp)

# Filter out platform specific files not for Android and .mm files
FILE_LIST := $(filter-out %iOS.cpp, $(FILE_LIST))
FILE_LIST := $(filter-out %Win32.cpp, $(FILE_LIST))
FILE_LIST := $(filter-out %.mm, $(FILE_LIST))
FILE_LIST := $(filter-out $(LOCAL_PATH)/../../../../Classes/COMMON/MultiOutterForiOS.cpp, $(FILE_LIST))


# Add main.cpp
FILE_LIST += $(LOCAL_PATH)/main.cpp

# Add libiconv sources (simplified)
FILE_LIST += $(LOCAL_PATH)/libiconv-1.14/lib/iconv.c
FILE_LIST += $(LOCAL_PATH)/libiconv-1.14/srclib/localcharset.c
# Add other necessary .c files from libiconv/srclib if direct compilation is chosen for them
# For example:
# FILE_LIST += $(LOCAL_PATH)/libiconv-1.14/srclib/relocatable.c

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)


# Define include paths
LOCAL_C_INCLUDES := \
  $(LOCAL_PATH)/../../../../Classes \
  $(LOCAL_PATH)/../../../../Classes/AD \
  $(LOCAL_PATH)/../../../../Classes/COMMON \
  $(LOCAL_PATH)/../../../../Classes/CONTROL \
  $(LOCAL_PATH)/../../../../Classes/DATA \
  $(LOCAL_PATH)/../../../../Classes/DATA/GLB \
  $(LOCAL_PATH)/../../../../Classes/DATA/LDB \
  $(LOCAL_PATH)/../../../../Classes/DATA/LMT \
  $(LOCAL_PATH)/../../../../Classes/DATA/LMU \
  $(LOCAL_PATH)/../../../../Classes/DATA/LSD \
  $(LOCAL_PATH)/../../../../Classes/DATA/LSDCHK \
  $(LOCAL_PATH)/../../../../Classes/EVENT \
  $(LOCAL_PATH)/../../../../Classes/FILEIO \
  $(LOCAL_PATH)/../../../../Classes/FILEIO/lib_json \
  $(LOCAL_PATH)/../../../../Classes/FILEIO/md5 \
  $(LOCAL_PATH)/../../../../Classes/FILEIO/minizip \
  $(LOCAL_PATH)/../../../../Classes/FILEIO/zlib_1.2.3 \
  $(LOCAL_PATH)/../../../../Classes/GRAPHIC \
  $(LOCAL_PATH)/../../../../Classes/GRAPHIC/OBJECT \
  $(LOCAL_PATH)/../../../../Classes/MENU \
  $(LOCAL_PATH)/../../../../Classes/NETWORK \
  $(LOCAL_PATH)/../../../../Classes/SCENE \
  $(LOCAL_PATH)/../../../../Classes/SCENE/APPJOY \
  $(LOCAL_PATH)/../../../../Classes/SCENE/APPMAIN \
  $(LOCAL_PATH)/../../../../Classes/SCENE/APPVKEY \
  $(LOCAL_PATH)/../../../../Classes/SCENE/BATTLE \
  $(LOCAL_PATH)/../../../../Classes/SCENE/GAMEOVER \
  $(LOCAL_PATH)/../../../../Classes/SCENE/MAP \
  $(LOCAL_PATH)/../../../../Classes/SCENE/MENU \
  $(LOCAL_PATH)/../../../../Classes/SCENE/NAMEINPUT \
  $(LOCAL_PATH)/../../../../Classes/SCENE/SAVE \
  $(LOCAL_PATH)/../../../../Classes/SCENE/SHOP \
  $(LOCAL_PATH)/../../../../Classes/SOUND \
  $(LOCAL_PATH)/../../../../Classes/TRANSLATE \
  $(LOCAL_PATH)/libiconv-1.14/include \
  $(LOCAL_PATH)/libiconv-1.14/lib \
  $(LOCAL_PATH)/libiconv-1.14/srclib


# Add Cocos2d-x engine includes by referencing COCOS2DX_ROOT
# This path will be relative to where the Android.mk is (proj.android/app/jni)
# Assuming cocos2d and external are now at the project root.
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../cocos2d
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../cocos2d/cocos
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../cocos2d/cocos/audio
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../cocos2d/cocos/editor-support
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../cocos2d/extensions
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../cocos2d/external
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../cocos2d/external/chipmunk/include/chipmunk
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../cocos2d/external/json/include


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
# If using Box2D or other extensions, add them here:
# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static

include $(BUILD_SHARED_LIBRARY)

# Import Cocos2d-x modules
# The path is relative to this Android.mk file's location (proj.android/app/jni)
# COCOS2DX_ROOT should point to the root of the cocos2d-x engine files (e.g., ../../../../cocos2d)
$(call import-add-path, $(LOCAL_PATH)/../../../../)
$(call import-add-path, $(LOCAL_PATH)/../../../../cocos2d)
$(call import-add-path, $(LOCAL_PATH)/../../../../cocos2d/cocos)
$(call import-add-path, $(LOCAL_PATH)/../../../../cocos2d/external)


$(call import-module, cocos)
$(call import-module, cocos/audio/android) # For SimpleAudioEngine
# If using extensions:
# $(call import-module, extensions)
# If using Box2D:
# $(call import-module, external/Box2D)
# If using Chipmunk:
# $(call import-module, external/chipmunk)


# For libiconv, if building as a separate static library (alternative to direct source inclusion)
# include $(CLEAR_VARS)
# LOCAL_MODULE    := libiconv
# LOCAL_SRC_FILES := $(LOCAL_PATH)/libiconv-1.14/lib/iconv.c \
#                    $(LOCAL_PATH)/libiconv-1.14/srclib/localcharset.c \
#                    $(LOCAL_PATH)/libiconv-1.14/srclib/relocatable.c
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/libiconv-1.14/include \
#                     $(LOCAL_PATH)/libiconv-1.14/lib \
#                     $(LOCAL_PATH)/libiconv-1.14/srclib
# LOCAL_CFLAGS := -Wno-multichar -DBUILDING_LIBICONV -DLIBDIR="\"c\"" -DNO_XMALLOC -Dset_relocation_prefix=libiconv_set_relocation_prefix -Drelocate=libiconv_relocate
# include $(BUILD_STATIC_LIBRARY)
# And then add LOCAL_STATIC_LIBRARIES += libiconv to the main module.
