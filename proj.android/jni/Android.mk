LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := \
  -Wno-write-strings \
  -Wno-multichar \
  -DANDROID \
  -Wno-format-security \
  -DLIBDIR=\"c\" \
  -DBUILDING_LIBICONV \
  -DIN_LIBRARY 
  
LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

FILE_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/AD/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/COMMON/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/CONTROL/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/DATA/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/DATA/LSD/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/DATA/LSDOLD/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/DATA/LSDCHK/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/DATA/LMT/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/DATA/LMU/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/DATA/LDB/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/EVENT/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/FILEIO/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/FILEIO/zlib_1.2.3/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/FILEIO/minizip/*.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/FILEIO/md5/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/FILEIO/lib_json/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/GRAPHIC/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/GRAPHIC/OBJECT/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/MENU/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/NETWORK/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/APPMAIN/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/APPVKEY/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/APPJOY/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/BATTLE/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/GAMEOVER/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/MAP/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/MENU/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/SAVE/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/SHOP/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/TELEPORT/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/TITLE/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SCENE/NAMEINPUT/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/SOUND/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/TRANSLATE/*.cpp)

LOCAL_SRC_FILES :=  \
	libiconv-1.14/libcharset/lib/localcharset.c \
	libiconv-1.14/lib/iconv.c \
	libiconv-1.14/lib/relocatable.c	\
	$(FILE_LIST:$(LOCAL_PATH)/%=%)	\
	hellocpp/main.cpp				

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../Classes	\
	$(LOCAL_PATH)/../../Classes/AD \
	$(LOCAL_PATH)/../../Classes/COMMON \
	$(LOCAL_PATH)/../../Classes/CONTROL	\
	$(LOCAL_PATH)/../../Classes/DATA \
	$(LOCAL_PATH)/../../Classes/DATA/GLB \
	$(LOCAL_PATH)/../../Classes/DATA/LMU \
	$(LOCAL_PATH)/../../Classes/DATA/LMT \
	$(LOCAL_PATH)/../../Classes/DATA/LSD \
	$(LOCAL_PATH)/../../Classes/DATA/LSDOLD \
	$(LOCAL_PATH)/../../Classes/DATA/LSDCHK \
	$(LOCAL_PATH)/../../Classes/EVENT \
	$(LOCAL_PATH)/../../Classes/FILEIO \
	$(LOCAL_PATH)/../../Classes/FILEIO/zlib_1.2.3 \
	$(LOCAL_PATH)/../../Classes/FILEIO/minizip \
	$(LOCAL_PATH)/../../Classes/FILEIO/md5 \
	$(LOCAL_PATH)/../../Classes/FILEIO/lib_json \
	$(LOCAL_PATH)/../../Classes/GRAPHIC \
	$(LOCAL_PATH)/../../Classes/GRAPHIC/OBJECT \
	$(LOCAL_PATH)/../../Classes/MENU \
	$(LOCAL_PATH)/../../Classes/NETWORK \
	$(LOCAL_PATH)/../../Classes/SCENE \
	$(LOCAL_PATH)/../../Classes/SCENE/APPMAIN \
	$(LOCAL_PATH)/../../Classes/SCENE/APPVKEY \
	$(LOCAL_PATH)/../../Classes/SCENE/APPJOY \
	$(LOCAL_PATH)/../../Classes/SCENE/BATTLE \
	$(LOCAL_PATH)/../../Classes/SCENE/GAMEOVER \
	$(LOCAL_PATH)/../../Classes/SCENE/MAP \
	$(LOCAL_PATH)/../../Classes/SCENE/MENU \
	$(LOCAL_PATH)/../../Classes/SCENE/SAVE \
	$(LOCAL_PATH)/../../Classes/SCENE/SHOP \
	$(LOCAL_PATH)/../../Classes/SCENE/TELEPORT \
	$(LOCAL_PATH)/../../Classes/SCENE/TITLE \
	$(LOCAL_PATH)/../../Classes/SCENE/NAMEINPUT \
	$(LOCAL_PATH)/../../Classes/SOUND \
	$(LOCAL_PATH)/../../Classes/TRANSLATE \
	$(LOCAL_PATH)/libiconv-1.14/include \
	$(LOCAL_PATH)/libiconv-1.14/libcharset \
	$(LOCAL_PATH)/libiconv-1.14/lib \
	$(LOCAL_PATH)/libiconv-1.14/libcharset/include \
	$(LOCAL_PATH)/libiconv-1.14/srclib
  
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += openssl
LOCAL_WHOLE_STATIC_LIBRARIES += libiconv

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)

#$(call import-module,external/Box2D)
#$(call import-module,external/chipmunk)
#LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
#LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
