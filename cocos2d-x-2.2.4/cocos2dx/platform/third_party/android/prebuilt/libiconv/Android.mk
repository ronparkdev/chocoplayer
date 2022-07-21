#
# libiconv android.mk
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libiconv
LOCAL_CFLAGS := \
                -DHAVE_CONFIG_H -DBUILDING_LIBICONV -DBUILDING_DLL -DENABLE_RELOCATABLE=1 -DIN_LIBRARY -DNO_XMALLOC \
                -DLIBDIR=\"\" \
                -Dset_relocation_prefix=libiconv_set_relocation_prefix \
                -Drelocate=libiconv_relocate

LOCAL_C_INCLUDES := \
                    $(LOCAL_PATH)/include \
                    $(LOCAL_PATH)/libcharset \
                    $(LOCAL_PATH)/libcharset/include \

LOCAL_SRC_FILES := \
                    lib/iconv.c \
                    libcharset/lib/localcharset.c \
                    lib/relocatable.c
LOCAL_ARM_MODE := arm

ifeq ($(ICNV_OPTION), shared)
    include $(BUILD_SHARED_LIBRARY)
else
    include $(BUILD_STATIC_LIBRARY)
endif