#include "breGfx.h"

#include <android/native_window.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <assert.h>
#include <jni.h>
#include <android/native_window_jni.h>
#include <AEE_OEM.h>
#include "OEMDisplayDev.h"
#include "breConfig.h"

ANativeWindow *gNativeWindow = NULL;
static EGLDisplay gEglDpy = NULL;
static EGLSurface gEglSurface = NULL;
static EGLContext gEglCx = NULL;
static EGLConfig gEglConfig = NULL;
static const EGLint gEglAttribs[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_TRANSPARENT_TYPE, EGL_NONE,
        EGL_NONE
};

void breGfxInitDisplay() {
    gEglDpy = eglGetDisplay(NULL);
    assert(gEglDpy != EGL_NO_DISPLAY);
    assert(eglInitialize(gEglDpy, NULL, NULL));

    assert(eglBindAPI(EGL_OPENGL_ES_API));

    EGLint numConfig;
    assert(eglChooseConfig(gEglDpy, gEglAttribs, &gEglConfig, 1, &numConfig));
    assert(numConfig > 0);

    EGLint cxAttribs[] = {
            0x3098, 1,
            EGL_NONE
    };

    gEglCx = eglCreateContext(gEglDpy, gEglConfig, EGL_NO_CONTEXT, cxAttribs);
    assert(gEglCx);
}

void breGfxInitSurface(ANativeWindow *win) {
    ANativeWindow_acquire(win);
    gNativeWindow = win;

    gEglSurface = eglCreateWindowSurface(gEglDpy, gEglConfig, gNativeWindow, NULL);
    assert(gEglSurface);
}

void breGfxAcqCx() {
    assert(eglMakeCurrent(gEglDpy, gEglSurface, gEglSurface, gEglCx));
}

void breGfxRelCx() {
    eglMakeCurrent(gEglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_SURFACE);
    extern void CEGL_restoreContext();
    CEGL_restoreContext();
}

void breGfxSwap() {
    eglSwapBuffers(gEglDpy, gEglSurface);
}

void breGfxDestroySurface() {
    breGfxRelCx();

    eglDestroySurface(gEglDpy, gEglSurface);

    ANativeWindow_release(gNativeWindow);
    gNativeWindow = NULL;
}

int breGfxIsInitialized() {
    return gEglCx != NULL;
}

int breOemDpyUpdate();

JNIEXPORT void JNICALL
Java_io_github_usernameak_brewemulator_EmulatorSurfaceView_nSurfaceCreated(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jobject surface) {
    if (!gEglCx) {
        breGfxInitDisplay();
    }
    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    breGfxInitSurface(nativeWindow);
    ANativeWindow_release(nativeWindow);
    if (AEE_IsInitialized()) {
        breOemDpyUpdate();
    }
}

JNIEXPORT void JNICALL
Java_io_github_usernameak_brewemulator_EmulatorSurfaceView_nSurfaceChanged(JNIEnv *env,
                                                                           jobject thiz) {
    if (AEE_IsInitialized()) {
        breOemDpyUpdate();
    }
}

JNIEXPORT void JNICALL
Java_io_github_usernameak_brewemulator_EmulatorSurfaceView_nSurfaceDestroyed(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jobject surface) {
    breGfxDestroySurface();
}
