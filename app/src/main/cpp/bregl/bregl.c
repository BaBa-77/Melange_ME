#include <OEMHeap.h>
#include <AEE_OEM.h>
#include <AEEModTable.h>
#include "AEEGL.h"

struct breEGLSurface {
    EGLSurface pRealSurface;
    EGLContext pLastContext;
    IDisplay *pDisplay;
    IDIB *pCopyBuffer;
    IDIB *pDIB;
};

static EGLDisplay g_currentDisplay = EGL_NO_DISPLAY;
static struct breEGLSurface *g_currentReadSurface = NULL;
static struct breEGLSurface *g_currentDrawSurface = NULL;
static unsigned char *g_extensionString = NULL;

struct breEGLProcAddressMapEntry {
    const char *name;

    void (*proc)();
};

static struct breEGLProcAddressMapEntry g_eglProcAddressMap[];

OBJECT(CEGL) {
    AEEVTBL(IEGL) *pvt;
    uint32 m_nRefs;
};

static uint32 CEGL_AddRef(IEGL *po) {
    CEGL *pMe = (CEGL *) po;

    return (++(pMe->m_nRefs));
}

static uint32 CEGL_Release(IEGL *po) {
    CEGL *pMe = (CEGL *) po;
    if (pMe->m_nRefs) {
        if (--pMe->m_nRefs == 0) {
            sys_free(po);
        }
    }
    return pMe->m_nRefs;
}

static int CEGL_QueryInterface(IEGL *po, AEEIID cls, void **ppo) {
    switch (cls) {
        case AEECLSID_QUERYINTERFACE:
        case AEECLSID_EGL:
            *ppo = (void *) po;
            CEGL_AddRef(po);
            return SUCCESS;
        default:
            *ppo = NULL;
            return ECLASSNOTSUPPORT;
    }
}

EGLDisplay CEGL_eglGetDisplay(EGLNativeDisplayType display_id) {
    return eglGetDisplay(NULL);
}

EGLBoolean CEGL_eglInitialize(EGLDisplay dpy, EGLint *major, EGLint *minor) {
    return EGL_TRUE;
}

EGLBoolean CEGL_eglTerminate(EGLDisplay dpy) {
    return EGL_TRUE;
}

EGLSurface CEGL_eglCreateWindowSurface(EGLDisplay dpy, EGLConfig config, NativeWindowType window,
                                       const EGLint *attrib_list) {
    IDIB *dib = (IDIB *) window;
    IDIB *pCopyBuffer = NULL;

    IShell *pShell = AEE_GetShell();
    IDisplay *pDisplay = NULL;
    if (SUCCESS != ISHELL_CreateInstance(pShell, AEECLSID_DISPLAY, (void **) &pDisplay)) {
        return EGL_NO_SURFACE;
    }
    if (SUCCESS != IDisplay_CreateDIBitmap(pDisplay, &pCopyBuffer, 32, dib->cx, dib->cy)) {
        IDisplay_Release(pDisplay);
        return EGL_NO_SURFACE;
    }

    pCopyBuffer->nColorScheme = IDIB_COLORSCHEME_888;
    IDIB_FlushPalette(pCopyBuffer);

    EGLint pbAttribList[] = {
            EGL_WIDTH, (EGLint) dib->cx,
            EGL_HEIGHT, (EGLint) dib->cy,
            EGL_NONE, EGL_NONE
    };

    // well well, we're gonna create a pbuffer and then handle that on our own
    EGLSurface realSurface = eglCreatePbufferSurface(dpy, config, pbAttribList);
    if (!realSurface) {
        IDIB_Release(pCopyBuffer);
        IDisplay_Release(pDisplay);
        return EGL_NO_SURFACE;
    }

    struct breEGLSurface *surface = sys_malloc(sizeof(struct breEGLSurface));
    if (!surface) {
        // we ran out of memory
        eglDestroySurface(dpy, realSurface);
        IDIB_Release(pCopyBuffer);
        IDisplay_Release(pDisplay);
        return EGL_NO_SURFACE;
    }

    surface->pRealSurface = realSurface;
    surface->pDIB = dib;
    surface->pDisplay = pDisplay;
    surface->pLastContext = NULL;
    surface->pCopyBuffer = pCopyBuffer;

    return surface;
}

EGLSurface CEGL_eglCreatePixmapSurface(EGLDisplay dpy, EGLConfig config, NativePixmapType pixmap,
                                       const EGLint *attrib_list) {
    // for BREW, these are the same
    return CEGL_eglCreateWindowSurface(dpy, config, (NativeWindowType) pixmap, attrib_list);
}

EGLSurface
CEGL_eglCreatePbufferSurface(EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list) {
    // we need to handle this one as well for the sake of completeness
    EGLSurface realSurface = eglCreatePbufferSurface(dpy, config, attrib_list);
    if (!realSurface) {
        return EGL_NO_SURFACE;
    }
    struct breEGLSurface *surface = sys_malloc(sizeof(struct breEGLSurface));
    if (!surface) {
        // we ran out of memory
        eglDestroySurface(dpy, realSurface);
        return EGL_NO_SURFACE;
    }

    surface->pRealSurface = realSurface;
    surface->pDIB = NULL;
    surface->pDisplay = NULL;
    surface->pLastContext = NULL;
    surface->pCopyBuffer = NULL;

    return surface;
}

EGLBoolean CEGL_eglDestroySurface(EGLDisplay dpy, EGLSurface surface) {
    struct breEGLSurface *surfaceData = (struct breEGLSurface *) surface;
    EGLBoolean status = eglDestroySurface(dpy, surfaceData->pRealSurface);
    IDIB_Release(surfaceData->pCopyBuffer);
    IDisplay_Release(surfaceData->pDisplay);
    sys_free(surfaceData);
    return status;
}

EGLBoolean
CEGL_eglQuerySurface(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value) {
    struct breEGLSurface *surfaceData = (struct breEGLSurface *) surface;
    if (attribute == EGL_LARGEST_PBUFFER && !surfaceData->pDIB) {
        return EGL_TRUE; // necessary to follow the spec.
    }
    return eglQuerySurface(dpy, surfaceData->pRealSurface, attribute, value);
}

EGLBoolean CEGL_eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) {
    struct breEGLSurface *drawSurfaceData = (struct breEGLSurface *) draw;
    struct breEGLSurface *readSurfaceData = (struct breEGLSurface *) read;

    if (eglMakeCurrent(
            dpy,
            drawSurfaceData ? drawSurfaceData->pRealSurface : NULL,
            readSurfaceData ? readSurfaceData->pRealSurface : NULL,
            ctx)) {
        g_currentDisplay = dpy;

        if (drawSurfaceData) drawSurfaceData->pLastContext = ctx;
        if (readSurfaceData) readSurfaceData->pLastContext = ctx;

        g_currentDrawSurface = drawSurfaceData;
        g_currentReadSurface = readSurfaceData;

        return EGL_TRUE;
    }

    return EGL_FALSE;
}

EGLSurface CEGL_eglGetCurrentSurface(EGLint readdraw) {
    if (readdraw == EGL_READ) {
        return g_currentReadSurface;
    } else if (readdraw == EGL_DRAW) {
        return g_currentDrawSurface;
    } else {
        return EGL_NO_SURFACE;
    }
}

EGLBoolean CEGL_eglCopyBuffers(EGLDisplay dpy, EGLSurface surface, NativePixmapType target) {
    IDIB *pDIB = (IDIB *) target;
    if (!pDIB) return EGL_TRUE; // ignore in this case

    struct breEGLSurface *surfaceData = (struct breEGLSurface *) surface;
    eglMakeCurrent(dpy, surfaceData->pRealSurface, surfaceData->pRealSurface,
                   surfaceData->pLastContext);

    glReadPixels(0, 0, pDIB->cx, pDIB->cy, GL_RGBA, GL_UNSIGNED_BYTE,
                 surfaceData->pCopyBuffer->pBmp);

    int cx = surfaceData->pCopyBuffer->cx;
    int cy = surfaceData->pCopyBuffer->cy;
    byte *pBmp = surfaceData->pCopyBuffer->pBmp;
    for (int y = 0; y < cy / 2; y++) {
        for (int x = 0; x < cx; x++) {
            size_t srcOffset = (y * cx + x) * 4;
            size_t dstOffset = ((cy - y - 1) * cx + x) * 4;
            byte tmp0 = pBmp[srcOffset + 0];
            byte tmp1 = pBmp[srcOffset + 1];
            byte tmp2 = pBmp[srcOffset + 2];
            byte tmp3 = pBmp[srcOffset + 3];
            pBmp[srcOffset + 0] = pBmp[dstOffset + 2];
            pBmp[srcOffset + 1] = pBmp[dstOffset + 1];
            pBmp[srcOffset + 2] = pBmp[dstOffset + 0];
            pBmp[srcOffset + 3] = pBmp[dstOffset + 3];
            pBmp[dstOffset + 0] = tmp2;
            pBmp[dstOffset + 1] = tmp1;
            pBmp[dstOffset + 2] = tmp0;
            pBmp[dstOffset + 3] = tmp3;
        }
    }

    IBitmap_BltIn(IDIB_TO_IBITMAP(pDIB), 0, 0, pDIB->cx, pDIB->cy,
                  IDIB_TO_IBITMAP(surfaceData->pCopyBuffer), 0, 0, AEE_RO_COPY);

    eglMakeCurrent(dpy,
                   g_currentDrawSurface ? g_currentDrawSurface->pRealSurface : NULL,
                   g_currentReadSurface ? g_currentReadSurface->pRealSurface : NULL,
                   g_currentDrawSurface ? g_currentDrawSurface->pLastContext : NULL);

    IBitmapDev *pBitmapDev = NULL;

    if (SUCCESS ==
        IBitmap_QueryInterface(IDIB_TO_IBITMAP(pDIB), AEEIID_BITMAPDEV, (void **) &pBitmapDev)) {
        IBitmapDev_Update(pBitmapDev);
        IBitmapDev_Release(pBitmapDev);
    }

    return EGL_TRUE;
}

EGLBoolean CEGL_eglSwapBuffers(EGLDisplay dpy, EGLSurface draw) {
    struct breEGLSurface *surfaceData = (struct breEGLSurface *) draw;
    if (!surfaceData) {
        return EGL_FALSE;
    }
    return CEGL_eglCopyBuffers(dpy, draw, (NativePixmapType) surfaceData->pDIB);
}

void (*CEGL_eglGetProcAddress(const char *procname))() {
    struct breEGLProcAddressMapEntry *entry = g_eglProcAddressMap;
    while (entry->name) {
        if (strcmp(procname, entry->name) == 0) {
            return entry->proc;
        }

        entry++;
    }
    return eglGetProcAddress(procname);
}

void CEGL_restoreContext() {
    EGLContext ctx = EGL_NO_CONTEXT;
    if (g_currentDrawSurface) {
        ctx = g_currentDrawSurface->pLastContext;
    } else if (g_currentReadSurface) {
        ctx = g_currentReadSurface->pLastContext;
    }

    eglMakeCurrent(g_currentDisplay,
                   g_currentDrawSurface ? g_currentDrawSurface->pRealSurface : NULL,
                   g_currentReadSurface ? g_currentReadSurface->pRealSurface : NULL,
                   ctx);
}

static const VTBL(IEGL) gsCEGLFuncs = {
        CEGL_AddRef,
        CEGL_Release,
        CEGL_QueryInterface,
        eglGetError,
        CEGL_eglGetDisplay,
        CEGL_eglInitialize,
        CEGL_eglTerminate,
        eglQueryString,
        CEGL_eglGetProcAddress,
        eglGetConfigs,
        eglChooseConfig,
        eglGetConfigAttrib,
        CEGL_eglCreateWindowSurface,
        CEGL_eglCreatePixmapSurface,
        CEGL_eglCreatePbufferSurface,
        CEGL_eglDestroySurface,
        CEGL_eglQuerySurface,
        eglCreateContext,
        eglDestroyContext,
        CEGL_eglMakeCurrent,
        eglGetCurrentContext,
        CEGL_eglGetCurrentSurface,
        eglGetCurrentDisplay,
        eglQueryContext,
        eglWaitGL,
        eglWaitNative,
        CEGL_eglSwapBuffers,
        CEGL_eglCopyBuffers,
};

int CEGL_New(IShell *ps, AEECLSID ClsId, void **ppObj) {
    CEGL *pNew;

    *ppObj = NULL;

    if (ClsId == AEECLSID_EGL) {
        pNew = (CEGL *) AEE_NewClassEx((IBaseVtbl *) &gsCEGLFuncs,
                                       sizeof(CEGL), TRUE);
        if (!pNew) {
            return ENOMEMORY;
        } else {
            pNew->m_nRefs = 1;

            *ppObj = pNew;
            return AEE_SUCCESS;
        }
    }

    return EUNSUPPORTED;
}


// === //

OBJECT(CGL) {
    AEEVTBL(IGL) *pvt;
    uint32 m_nRefs;
};


static uint32 CGL_AddRef(IGL *po) {
    CGL *pMe = (CGL *) po;

    return (++(pMe->m_nRefs));
}

static uint32 CGL_Release(IGL *po) {
    CGL *pMe = (CGL *) po;
    if (pMe->m_nRefs) {
        if (--pMe->m_nRefs == 0) {
            sys_free(po);
        }
    }
    return pMe->m_nRefs;
}

static int CGL_QueryInterface(IGL *po, AEEIID cls, void **ppo) {
    switch (cls) {
        case AEECLSID_QUERYINTERFACE:
        case AEECLSID_GL:
            *ppo = (void *) po;
            CGL_AddRef(po);
            return SUCCESS;
        default:
            *ppo = NULL;
            return ECLASSNOTSUPPORT;
    }
}

static const GLubyte *CGL_glGetString(GLenum name) {
    if (name == GL_EXTENSIONS) {
        if (g_extensionString) {
            return g_extensionString;
        }
        GLubyte *originalExtensionString = glGetString(GL_EXTENSIONS);
        size_t totalChars = strlen((const char *) originalExtensionString) + 1 + 200;
        g_extensionString = malloc(sizeof(GLubyte) * totalChars);
        strcpy(g_extensionString, (const char *) originalExtensionString);
        strncat(g_extensionString, " GL_ATI_texture_compression_atitc ", totalChars);
        return g_extensionString;
    }
    return glGetString(name);
}

static void
CGL_glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width,
                           GLsizei height, GLint border, GLsizei imageSize,
                           const void *data) {
    glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

static void
CGL_glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset,
                              GLsizei width,
                              GLsizei height, GLenum format, GLsizei imageSize, const void *data) {
    glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize,
                              data);
}

static const VTBL(IGL) gsCGLFuncs = {
        CGL_AddRef,
        CGL_Release,
        CGL_QueryInterface,
        glActiveTexture,
        glAlphaFuncx,
        glBindTexture,
        glBlendFunc,
        glClear,
        glClearColorx,
        glClearDepthx,
        glClearStencil,
        glClientActiveTexture,
        glColor4x,
        glColorMask,
        glColorPointer,
        CGL_glCompressedTexImage2D,
        CGL_glCompressedTexSubImage2D,
        glCopyTexImage2D,
        glCopyTexSubImage2D,
        glCullFace,
        glDeleteTextures,
        glDepthFunc,
        glDepthMask,
        glDepthRangex,
        glDisable,
        glDisableClientState,
        glDrawArrays,
        glDrawElements,
        glEnable,
        glEnableClientState,
        glFinish,
        glFlush,
        glFogx,
        glFogxv,
        glFrontFace,
        glFrustumx,
        glGenTextures,
        glGetError,
        glGetIntegerv,
        CGL_glGetString,
        glHint,
        glLightModelx,
        glLightModelxv,
        glLightx,
        glLightxv,
        glLineWidthx,
        glLoadIdentity,
        glLoadMatrixx,
        glLogicOp,
        glMaterialx,
        glMaterialxv,
        glMatrixMode,
        glMultMatrixx,
        glMultiTexCoord4x,
        glNormal3x,
        glNormalPointer,
        glOrthox,
        glPixelStorei,
        glPointSizex,
        glPolygonOffsetx,
        glPopMatrix,
        glPushMatrix,
        glReadPixels,
        glRotatex,
        glSampleCoveragex,
        glScalex,
        glScissor,
        glShadeModel,
        glStencilFunc,
        glStencilMask,
        glStencilOp,
        glTexCoordPointer,
        glTexEnvx,
        glTexEnvxv,
        glTexImage2D,
        glTexParameterx,
        glTexSubImage2D,
        glTranslatex,
        glVertexPointer,
        glViewport
};

int CGL_New(IShell *ps, AEECLSID ClsId, void **ppObj) {
    CGL *pNew;

    *ppObj = NULL;

    if (ClsId == AEECLSID_GL) {
        pNew = (CGL *) AEE_NewClassEx((IBaseVtbl *) &gsCGLFuncs,
                                      sizeof(CGL), TRUE);
        if (!pNew) {
            return ENOMEMORY;
        } else {
            pNew->m_nRefs = 1;

            *ppObj = pNew;
            return AEE_SUCCESS;
        }
    }

    return EUNSUPPORTED;
}

// === //

static struct breEGLProcAddressMapEntry g_eglProcAddressMap[] = {
        {"eglGetProcAddress",         (void (*)()) CEGL_eglGetProcAddress},
        {"eglGetDisplay",             (void (*)()) CEGL_eglGetDisplay},
        {"eglInitialize",             (void (*)()) CEGL_eglInitialize},
        {"eglTerminate",              (void (*)()) CEGL_eglTerminate},
        {"eglCreateWindowSurface",    (void (*)()) CEGL_eglCreateWindowSurface},
        {"eglCreatePixmapSurface",    (void (*)()) CEGL_eglCreatePixmapSurface},
        {"eglCreatePbufferSurface",   (void (*)()) CEGL_eglCreatePbufferSurface},
        {"eglDestroySurface",         (void (*)()) CEGL_eglDestroySurface},
        {"eglQuerySurface",           (void (*)()) CEGL_eglQuerySurface},
        {"eglMakeCurrent",            (void (*)()) CEGL_eglMakeCurrent},
        {"eglGetCurrentSurface",      (void (*)()) CEGL_eglGetCurrentSurface},
        {"eglCopyBuffers",            (void (*)()) CEGL_eglCopyBuffers},
        {"eglSwapBuffers",            (void (*)()) CEGL_eglSwapBuffers},
        {"glGetString",               (void (*)()) CGL_glGetString},
        {"glCompressedTexImage2D",    (void (*)()) CGL_glCompressedTexImage2D},
        {"glCompressedTexSubImage2D", (void (*)()) CGL_glCompressedTexSubImage2D},
        {NULL, NULL}
};

// === //

const AEEStaticClass gAEEGLClasses[] = {
        {AEECLSID_GL,  ASCF_UPGRADE, 0, NULL, CGL_New},
        {AEECLSID_EGL, ASCF_UPGRADE, 0, NULL, CEGL_New},
        {0, 0,                       0, NULL, NULL}
};
