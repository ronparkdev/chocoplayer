LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := openssl
LOCAL_MODULE_FILENAME := openssl

LOCAL_SRC_FILES := \
		libs/$(TARGET_ARCH_ABI)/libcrypto.a \
		libs/$(TARGET_ARCH_ABI)/libssl.a \

LOCAL_EXPORT_C_INCLUDES := \
		$(LOCAL_PATH)/include

include $(PREBUILT_STATIC_LIBRARY)
