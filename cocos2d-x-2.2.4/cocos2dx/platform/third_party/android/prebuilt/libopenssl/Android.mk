LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libopenssl
LOCAL_MODULE_FILENAME := libopenssl
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libssl.a
LOCAL_EXPORT_C_INCLUDES := \
		$(LOCAL_PATH)/include

include $(PREBUILT_STATIC_LIBRARY)
