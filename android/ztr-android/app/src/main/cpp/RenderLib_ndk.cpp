//
// See LICENSE.txt for this sample’s licensing information.
//
// RenderLib_ndk.cpp
// ZOZO Technologies Cross Platform Renderer Example
//

#define APPNAME "com.zozo.ztr_android"

#import "ztr_platform_abstraction_layer.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <assert.h>
#include <jni.h>
#include <string>
#include <android/log.h>
#include <EGL/egl.h>

static JavaVM* javaVm;
static AAssetManager* asset_manager;

static ztr_platform_api_t g_platform;
static ztr_hid_t hid;

PLATFORM_OPEN_FILE(openFile)
{
    assert (fileName != NULL);
    ztr_file_t result = {};

    AAsset *asset = AAssetManager_open (asset_manager, fileName, AASSET_MODE_STREAMING);
    assert (asset != NULL);
    if (asset != NULL)
    {
        result.data = (void *) AAsset_getBuffer (asset);
        result.dataSize = AAsset_getLength (asset);
    }

    return (result);
}

extern "C" JNIEXPORT void JNICALL
Java_com_zozo_ztr_1android_RenderLib_init(JNIEnv* env, void *reserved, jobject assetManager)
{
    env->GetJavaVM(&javaVm);
    asset_manager = AAssetManager_fromJava(env, assetManager);
    g_platform.openFile = openFile;

    ztrInit (&g_platform);
}

extern "C" JNIEXPORT void JNICALL
Java_com_zozo_ztr_1android_RenderLib_draw(JNIEnv* env, jobject obj, jint mouseDown, jint mouseDownUp, jint x, jint y)
{
    hid.mouseDown = static_cast<int> (mouseDown);
    hid.mouseTransition = static_cast<int> (mouseDownUp);
    hid.mouseX = static_cast<int> (x);
    hid.mouseY = static_cast<int> (-y);

    ztrDraw (0, hid);

    hid.mouseTransition = 0;
    hid.doubleTap = 0;
    hid.pinchZoomTransition = 0;
}

extern "C" JNIEXPORT void JNICALL
Java_com_zozo_ztr_1android_RenderLib_resize(JNIEnv* env, jobject obj, jint width, jint height)
{
    ztrResize (&g_platform, width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_zozo_ztr_1android_RenderLib_free(JNIEnv* env, jobject obj)
{
    ztrFree ();
}
