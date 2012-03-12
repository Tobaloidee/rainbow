#!/bin/bash
BUILD_DIR=`pwd`
HEADER="# This file was generated by $0"
NDK_BUILD="$HOME/bin/android-ndk/ndk-build"
PROJECT="$(cd -P "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TARGET=rainbow

echo -n "Generating jni/Android.mk..."

# Gather Rainbow source files
cd $PROJECT
for c in `find src -name 'Platform' -prune -o -name '*.cpp'`; do
	SRC_FILES="$SRC_FILES $c"
done

# Include libraries
for lib in Box2D Lua libpng; do
	SRC_FILES="$SRC_FILES \\"$'\n'
	for c in `find lib/$lib -name '*.c' -and ! -name 'lua.c' -and ! -name 'luac.c' -or -name '*.cpp'`; do
		SRC_FILES="$SRC_FILES $c"
	done
done

cd $BUILD_DIR
mkdir -p jni
echo "\
$HEADER

#LOCAL_PATH := \$(call my-dir)
LOCAL_PATH := $PROJECT

include \$(CLEAR_VARS)

LOCAL_MODULE := $TARGET
LOCAL_SRC_FILES := $SRC_FILES

#LOCAL_CPP_EXTENSION := .c .cpp
#LOCAL_CPP_FEATURES := exceptions  # Required by Lua
LOCAL_C_INCLUDES := $PROJECT/src $PROJECT/lib $PROJECT/lib/FreeType/include $PROJECT/lib/libpng $PROJECT/lib/Lua
LOCAL_CFLAGS := -finline-functions -ftree-vectorize

LOCAL_LDLIBS := -lGLESv2 -ldl -llog -lz

include \$(BUILD_SHARED_LIBRARY)" \
> jni/Android.mk
echo " done"

echo -n "Generating jni/Application.mk..."
echo "\
$HEADER

APP_PLATFORM := android-9
APP_ABI := armeabi-v7a  # all
APP_STL := stlport_static  # Required by Box2D" \
> jni/Application.mk
echo " done"

$NDK_BUILD -i