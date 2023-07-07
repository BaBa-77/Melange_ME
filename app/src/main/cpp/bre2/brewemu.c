#include "brewemu.h"

#include <AEE_OEMDispatch.h>
#include <OEMNotify.h>
#include <AEEConfig.h>
#include <AEEModTable.h>
#include <BREWVersion.h>
#undef EALREADY

#include "breConfig.h"
#include <jni.h>
#include <sys/stat.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <android/keycodes.h>
#include <AEE_OEM.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const char *gExternalFilesDir = NULL;

static void mkdirs(const char *dir) {
    char tmp[PATH_MAX];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);
    if(tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
        if(*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    mkdir(tmp, S_IRWXU);
}

static void acquireExternalFilesDir(JNIEnv *env, jobject thiz) {
    jclass environment_clazz = (*env)->FindClass(env, "android/os/Environment");
    jclass file_clazz = (*env)->FindClass(env, "java/io/File");
    jmethodID getExternalStorageDir = (*env)->GetStaticMethodID(env, environment_clazz, "getExternalStorageDirectory",
                                                     "()Ljava/io/File;");
    jobject externalFilesDir = (*env)->CallStaticObjectMethod(env, environment_clazz, getExternalStorageDir);

    jmethodID getAbsolutePath = (*env)->GetMethodID(env, file_clazz, "getAbsolutePath",
                                                    "()Ljava/lang/String;");
    jobject absPath = (jstring) (*env)->CallObjectMethod(env, externalFilesDir, getAbsolutePath);

    const char *externalFilesDirStr = (*env)->GetStringUTFChars(env, absPath, NULL);
    char *brewDataDir = malloc(PATH_MAX);
    strcpy(brewDataDir, externalFilesDirStr);
    strcpy(brewDataDir + strlen(externalFilesDirStr), "/MelangeBREW/");
    gExternalFilesDir = brewDataDir;

    mkdirs(gExternalFilesDir);
}

void AEEREGISTRY_EnableDebugMsg(int enable);
void AEENOTIFY_EnableDebugMsg(int enable);
void AEEMIFPROP_EnableDebugMsg(int enable);
void AEEMIF_EnableDebugMsg(int enable);
void AEELICENSE_EnableDebugMsg(int enable);
void AEEPRELOAD_EnableDebugMsg(int enable);
void AEEFASTLOAD_EnableDebugMsg(int enable);
void AEEMOD_EnableDebugMsg(int enable);
void AEESERVICE_EnableDebugMsg(int enable);
void AEEDISPATCH_EnableDebugMsg(int enable);
void AEEALARMS_EnableDebugMsg(int enable);
void AEEEVENT_EnableDebugMsg(int enable);
void AEEPRIV_EnableDebugMsg(int enable);
void AEEKEY_EnableDebugMsg(int enable);
void AEEAPP_EnableDebugMsg(int enable);
void AEEDBGKEY_EnableDebugMsg(int enable);
void AEEFREEMEM_EnableDebugMsg(int enable);
void AEESTACK_EnableDebugMsg(int enable);

static AEECallback gCBStartLauncherApp;

#include "breStartup.h"
#include <assert.h>
#include <stdbool.h>
#include <AEE_OEMEvent.h>
#include "breGfx.h"
#include "breHooks.h"
#include "breScreenshot.h"

void breMainStart();
void breMainTerminate();

JNIEXPORT void JNICALL
Java_io_github_usernameak_brewemulator_MainActivity_brewEmuJNIStartup(JNIEnv *env, jobject thiz) {
    breSetupHooks();

    acquireExternalFilesDir(env, thiz);

    breInitConfig();

    int keypadMode;
    breGetConfigEntry(BRE_CFGE_KEYPAD, &keypadMode);
    if(keypadMode == 0) {
        (*env)->CallVoidMethod(env, thiz, (*env)->GetMethodID(env, (*env)->FindClass(env, "io/github/usernameak/brewemulator/MainActivity"), "hideKeypad", "()V"));
    }

    int useDbgLog = 0;
    breGetConfigEntry(BRE_CFGE_DEBUG_LOG, &useDbgLog);
    if(useDbgLog) {
        AEEREGISTRY_EnableDebugMsg(1);
        AEENOTIFY_EnableDebugMsg(1);
        AEEMIFPROP_EnableDebugMsg(1);
        AEEMIF_EnableDebugMsg(1);
        AEELICENSE_EnableDebugMsg(1);
        AEEPRELOAD_EnableDebugMsg(1);
        AEEFASTLOAD_EnableDebugMsg(1);
        AEEMOD_EnableDebugMsg(1);
        AEESERVICE_EnableDebugMsg(1);
        AEEDISPATCH_EnableDebugMsg(1);
        AEEALARMS_EnableDebugMsg(1);
        AEEEVENT_EnableDebugMsg(1);
        AEEPRIV_EnableDebugMsg(1);
        AEEKEY_EnableDebugMsg(1);
        AEEAPP_EnableDebugMsg(1);
        AEEDBGKEY_EnableDebugMsg(1);
        AEEFREEMEM_EnableDebugMsg(1);
        AEESTACK_EnableDebugMsg(1);
    }

    int width, height;
    breGetConfigEntry(BRE_CFGE_DISP_WIDTH, &width);
    breGetConfigEntry(BRE_CFGE_DISP_HEIGHT, &height);
    bool landscape = width > height;

    (*env)->CallVoidMethod(env, thiz, (*env)->GetMethodID(env, (*env)->FindClass(env, "io/github/usernameak/brewemulator/MainActivity"), "setUseLandscapeOrientation", "(Z)V"), landscape);

    breMainStart();
}

AVKType translateKeycode(jint keyCode) {
    AVKType avk = AVK_UNDEFINED;
    switch(keyCode) {
        case AKEYCODE_DPAD_DOWN: avk = AVK_DOWN; break;
        case AKEYCODE_DPAD_UP: avk = AVK_UP; break;
        case AKEYCODE_DPAD_LEFT: avk = AVK_LEFT; break;
        case AKEYCODE_DPAD_RIGHT: avk = AVK_RIGHT; break;

        case AKEYCODE_DPAD_CENTER:
        case AKEYCODE_ENTER:
            avk = AVK_SELECT; break;

        case AKEYCODE_CLEAR:
        case AKEYCODE_ESCAPE:
        case AKEYCODE_C:
        case AKEYCODE_DEL:
            avk = AVK_CLR; break;

        case AKEYCODE_MENU:
        case AKEYCODE_W:
            avk = AVK_SOFT1; break;

        case AKEYCODE_BACK:
        case AKEYCODE_E:
            avk = AVK_SOFT2; break;

        case AKEYCODE_X:
        case AKEYCODE_ENDCALL:
            avk = AVK_END; break;

        case AKEYCODE_Z:
        case AKEYCODE_CALL:
            avk = AVK_SEND; break;

        case AKEYCODE_0:
        case AKEYCODE_NUMPAD_0:
            avk = AVK_0; break;

        case AKEYCODE_1:
        case AKEYCODE_NUMPAD_1:
            avk = AVK_1; break;

        case AKEYCODE_2:
        case AKEYCODE_NUMPAD_2:
            avk = AVK_2; break;

        case AKEYCODE_3:
        case AKEYCODE_NUMPAD_3:
            avk = AVK_3; break;

        case AKEYCODE_4:
        case AKEYCODE_NUMPAD_4:
            avk = AVK_4; break;

        case AKEYCODE_5:
        case AKEYCODE_NUMPAD_5:
            avk = AVK_5; break;

        case AKEYCODE_6:
        case AKEYCODE_NUMPAD_6:
            avk = AVK_6; break;

        case AKEYCODE_7:
        case AKEYCODE_NUMPAD_7:
            avk = AVK_7; break;

        case AKEYCODE_8:
        case AKEYCODE_NUMPAD_8:
            avk = AVK_8; break;

        case AKEYCODE_9:
        case AKEYCODE_NUMPAD_9:
            avk = AVK_9; break;

        case AKEYCODE_STAR:
        case AKEYCODE_S:
        case AKEYCODE_NUMPAD_MULTIPLY:
            avk = AVK_STAR; break;

        case AKEYCODE_POUND:
        case AKEYCODE_D:
        case AKEYCODE_NUMPAD_DIVIDE:
            avk = AVK_POUND; break;
    }

    return avk;
}

JNIEXPORT jboolean JNICALL
Java_io_github_usernameak_brewemulator_MainActivity_brewEmuKeyUp(JNIEnv *env, jobject thiz, jint avk) {
    /*AVKType avk = translateKeycode(keyCode);*/
    if(avk == AVK_UNDEFINED) {
        return false;
    }
    if(avk == AVK_LAST + 1) {
        return true;
    }
    AEE_KeyRelease(avk);
    return true;
}

JNIEXPORT jboolean JNICALL
Java_io_github_usernameak_brewemulator_MainActivity_brewEmuKeyDown(JNIEnv *env, jobject thiz, jint avk) {
    /*AVKType avk = translateKeycode(keyCode);*/
    if(avk == AVK_UNDEFINED) {
        return false;
    }
    if(avk == AVK_LAST + 1) {
        breScreenshot();
        return true;
    }
    AEE_Key(avk);
    AEE_KeyPress(avk);
    return true;
}

JNIEXPORT void JNICALL
Java_io_github_usernameak_brewemulator_MainActivity_brewEmuJNIShutdown(JNIEnv *env, jobject thiz) {
    breMainTerminate();
}