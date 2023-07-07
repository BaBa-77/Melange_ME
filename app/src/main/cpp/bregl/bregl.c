#include <OEMHeap.h>
#include <AEE_OEM.h>
#include <AEEModTable.h>
#include "AEEGL.h"
#include "AEEEGL10.h"
#include "AEEGLES10.h"
#include "AEECLSID_QEGL.bid"
#include <EGL/egl.h>

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
static char *g_eglExtensionString = NULL;

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
    if (!surface) return EGL_FALSE;
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
    if (g_currentDisplay == EGL_NO_DISPLAY) return;

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

const char *CEGL_eglQueryString(EGLDisplay dpy, EGLint name) {
    if (name == EGL_EXTENSIONS) {
        if (g_eglExtensionString) {
            return g_eglExtensionString;
        }

        const char *stringToAppend = " EGL_QUALCOMM_COLOR_BUFFER EGL_QUALCOMM_get_color_buffer ";
        const char *originalExtensionString = eglQueryString(dpy, EGL_EXTENSIONS);

        size_t totalChars =
                strlen((const char *) originalExtensionString) + strlen(stringToAppend) + 1;
        g_eglExtensionString = malloc(sizeof(char) * totalChars);

        strcpy(g_eglExtensionString, (const char *) originalExtensionString);
        strncat(g_eglExtensionString, stringToAppend, totalChars);

        return g_eglExtensionString;
    }
    return eglQueryString(dpy, name);
}

void *CEGL_eglGetColorBufferQUALCOMM(void) {
    if (!g_currentDrawSurface) return NULL;

    return g_currentDrawSurface->pCopyBuffer->pBmp;
}

static const VTBL(IEGL) gsCEGLFuncs = {
        CEGL_AddRef,
        CEGL_Release,
        CEGL_QueryInterface,
        eglGetError,
        CEGL_eglGetDisplay,
        CEGL_eglInitialize,
        CEGL_eglTerminate,
        CEGL_eglQueryString,
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

OBJECT(CGLES10) {
    AEEVTBL(IGLES10) *pvt;
    IEGL10 *m_pQEGL;
};

static uint32 CGLES10_AddRef(IGLES10 *po) {
    return IEGL10_AddRef(po);
}

static uint32 CGLES10_Release(IGLES10 *po) {
    return IEGL10_Release(po);
}

static int CGLES10_QueryInterface(IGLES10 *po, AEEIID cls, void **ppo) {
    return IEGL10_QueryInterface(po, cls, ppo);
}

int CGLES10_AlphaFunc(IGLES10 *pMe, AEEGLenum func, AEEGLclampf ref) {
    glAlphaFunc(func, ref);
    return SUCCESS;
}

int CGLES10_ClearColor(IGLES10 *pMe, AEEGLclampf red, AEEGLclampf green, AEEGLclampf blue,
                       AEEGLclampf alpha) {
    glClearColor(red, green, blue, alpha);
    return SUCCESS;
}

int CGLES10_ClearDepthf(IGLES10 *pMe, AEEGLclampf depth) {
    glClearDepthf(depth);
    return SUCCESS;
}

int
CGLES10_Color4f(IGLES10 *pMe, AEEGLfloat red, AEEGLfloat green, AEEGLfloat blue, AEEGLfloat alpha) {
    glColor4f(red, green, blue, alpha);
    return SUCCESS;
}

int CGLES10_DepthRangef(IGLES10 *pMe, AEEGLclampf zNear, AEEGLclampf zFar) {
    glDepthRangef(zNear, zFar);
    return SUCCESS;
}

int CGLES10_Fogf(IGLES10 *pMe, AEEGLenum pname, AEEGLfloat param) {
    glFogf(pname, param);
    return SUCCESS;
}

int CGLES10_Fogfv(IGLES10 *pMe, AEEGLenum pname, const AEEGLfloat *params) {
    glFogfv(pname, params);
    return SUCCESS;
}

int
CGLES10_Frustumf(IGLES10 *pMe, AEEGLfloat left, AEEGLfloat right, AEEGLfloat bottom, AEEGLfloat top,
                 AEEGLfloat zNear, AEEGLfloat zFar) {
    glFrustumf(left, right, bottom, top, zNear, zFar);
    return SUCCESS;
}

int CGLES10_LightModelf(IGLES10 *pMe, AEEGLenum pname, AEEGLfloat param) {
    glLightModelf(pname, param);
    return SUCCESS;
}

int CGLES10_LightModelfv(IGLES10 *pMe, AEEGLenum pname, const AEEGLfloat *params) {
    glLightModelfv(pname, params);
    return SUCCESS;
}

int CGLES10_Lightf(IGLES10 *pMe, AEEGLenum light, AEEGLenum pname, AEEGLfloat param) {
    glLightf(light, pname, param);
    return SUCCESS;
}

int CGLES10_Lightfv(IGLES10 *pMe, AEEGLenum light, AEEGLenum pname, const AEEGLfloat *params) {
    glLightfv(light, pname, params);
    return SUCCESS;
}

int CGLES10_LineWidth(IGLES10 *pMe, AEEGLfloat width) {
    glLineWidth(width);
    return SUCCESS;
}

int CGLES10_LoadMatrixf(IGLES10 *pMe, const AEEGLfloat *m) {
    glLoadMatrixf(m);
    return SUCCESS;
}

int CGLES10_Materialf(IGLES10 *pMe, AEEGLenum face, AEEGLenum pname, AEEGLfloat param) {
    glMaterialf(face, pname, param);
    return SUCCESS;
}

int CGLES10_Materialfv(IGLES10 *pMe, AEEGLenum face, AEEGLenum pname, const AEEGLfloat *params) {
    glMaterialfv(face, pname, params);
    return SUCCESS;
}

int CGLES10_MultMatrixf(IGLES10 *pMe, const AEEGLfloat *m) {
    glMultMatrixf(m);
    return SUCCESS;
}

int
CGLES10_MultiTexCoord4f(IGLES10 *pMe, AEEGLenum target, AEEGLfloat s, AEEGLfloat t, AEEGLfloat r,
                        AEEGLfloat q) {
    glMultiTexCoord4f(target, s, t, r, q);
    return SUCCESS;
}

int CGLES10_Normal3f(IGLES10 *pMe, AEEGLfloat nx, AEEGLfloat ny, AEEGLfloat nz) {
    glNormal3f(nx, ny, nz);
    return SUCCESS;
}

int
CGLES10_Orthof(IGLES10 *pMe, AEEGLfloat left, AEEGLfloat right, AEEGLfloat bottom, AEEGLfloat top,
               AEEGLfloat zNear, AEEGLfloat zFar) {
    glOrthof(left, right, bottom, top, zNear, zFar);
    return SUCCESS;
}

int CGLES10_PointSize(IGLES10 *pMe, AEEGLfloat size) {
    glPointSize(size);
    return SUCCESS;
}

int CGLES10_PolygonOffset(IGLES10 *pMe, AEEGLfloat factor, AEEGLfloat units) {
    glPolygonOffset(factor, units);
    return SUCCESS;
}

int CGLES10_Rotatef(IGLES10 *pMe, AEEGLfloat angle, AEEGLfloat x, AEEGLfloat y, AEEGLfloat z) {
    glRotatef(angle, x, y, z);
    return SUCCESS;
}

int CGLES10_Scalef(IGLES10 *pMe, AEEGLfloat x, AEEGLfloat y, AEEGLfloat z) {
    glScalef(x, y, z);
    return SUCCESS;
}

int CGLES10_TexEnvf(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfloat param) {
    glTexEnvf(target, pname, param);
    return SUCCESS;
}

int CGLES10_TexEnvfv(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, const AEEGLfloat *params) {
    glTexEnvfv(target, pname, params);
    return SUCCESS;
}

int CGLES10_TexParameterf(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfloat param) {
    glTexParameterf(target, pname, param);
    return SUCCESS;
}

int CGLES10_Translatef(IGLES10 *pMe, AEEGLfloat x, AEEGLfloat y, AEEGLfloat z) {
    glTranslatef(x, y, z);
    return SUCCESS;
}

int CGLES10_ActiveTexture(IGLES10 *pMe, AEEGLenum texture) {
    glActiveTexture(texture);
    return SUCCESS;
}

int CGLES10_AlphaFuncx(IGLES10 *pMe, AEEGLenum func, AEEGLclampx ref) {
    glAlphaFuncx(func, ref);
    return SUCCESS;
}

int CGLES10_BindTexture(IGLES10 *pMe, AEEGLenum target, AEEGLuint texture) {
    glBindTexture(target, texture);
    return SUCCESS;
}

int CGLES10_BlendFunc(IGLES10 *pMe, AEEGLenum sfactor, AEEGLenum dfactor) {
    glBlendFunc(sfactor, dfactor);
    return SUCCESS;
}

int CGLES10_Clear(IGLES10 *pMe, AEEGLbitfield mask) {
    glClear(mask);
    return SUCCESS;
}

int CGLES10_ClearColorx(IGLES10 *pMe, AEEGLclampx red, AEEGLclampx green, AEEGLclampx blue,
                        AEEGLclampx alpha) {
    glClearColorx(red, green, blue, alpha);
    return SUCCESS;
}

int CGLES10_ClearDepthx(IGLES10 *pMe, AEEGLclampx depth) {
    glClearDepthx(depth);
    return SUCCESS;
}

int CGLES10_ClearStencil(IGLES10 *pMe, AEEGLint s) {
    glClearStencil(s);
    return SUCCESS;
}

int CGLES10_ClientActiveTexture(IGLES10 *pMe, AEEGLenum texture) {
    glClientActiveTexture(texture);
    return SUCCESS;
}

int
CGLES10_Color4x(IGLES10 *pMe, AEEGLfixed red, AEEGLfixed green, AEEGLfixed blue, AEEGLfixed alpha) {
    glColor4x(red, green, blue, alpha);
    return SUCCESS;
}

int CGLES10_ColorMask(IGLES10 *pMe, AEEGLboolean red, AEEGLboolean green, AEEGLboolean blue,
                      AEEGLboolean alpha) {
    glColorMask(red, green, blue, alpha);
    return SUCCESS;
}

int CGLES10_ColorPointer(IGLES10 *pMe, AEEGLint size, AEEGLenum type, AEEGLsizei stride,
                         const AEEGLvoid *pointer) {
    glColorPointer(size, type, stride, pointer);
    return SUCCESS;
}

int CGLES10_CompressedTexImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level,
                                 AEEGLenum internalformat, AEEGLsizei width, AEEGLsizei height,
                                 AEEGLint border, AEEGLsizei imageSize, const AEEGLvoid *data) {
    CGL_glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize,
                               data);
    return SUCCESS;
}

int
CGLES10_CompressedTexSubImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLint xoffset,
                                AEEGLint yoffset, AEEGLsizei width, AEEGLsizei height,
                                AEEGLenum format, AEEGLsizei imageSize, const AEEGLvoid *data) {
    CGL_glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize,
                                  data);
    return SUCCESS;
}

int CGLES10_CopyTexImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLenum internalformat,
                           AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height,
                           AEEGLint border) {
    glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
    return SUCCESS;
}

int CGLES10_CopyTexSubImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLint xoffset,
                              AEEGLint yoffset, AEEGLint x, AEEGLint y, AEEGLsizei width,
                              AEEGLsizei height) {
    glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    return SUCCESS;
}

int CGLES10_CullFace(IGLES10 *pMe, AEEGLenum mode) {
    glCullFace(mode);
    return SUCCESS;
}

int CGLES10_DeleteTextures(IGLES10 *pMe, AEEGLsizei n, const AEEGLuint *textures) {
    glDeleteTextures(n, textures);
    return SUCCESS;
}

int CGLES10_DepthFunc(IGLES10 *pMe, AEEGLenum func) {
    glDepthFunc(func);
    return SUCCESS;
}

int CGLES10_DepthMask(IGLES10 *pMe, AEEGLboolean flag) {
    glDepthMask(flag);
    return SUCCESS;
}

int CGLES10_DepthRangex(IGLES10 *pMe, AEEGLclampx zNear, AEEGLclampx zFar) {
    glDepthRangex(zNear, zFar);
    return SUCCESS;
}

int CGLES10_Disable(IGLES10 *pMe, AEEGLenum cap) {
    glDisable(cap);
    return SUCCESS;
}

int CGLES10_DisableClientState(IGLES10 *pMe, AEEGLenum array) {
    glDisableClientState(array);
    return SUCCESS;
}

int CGLES10_DrawArrays(IGLES10 *pMe, AEEGLenum mode, AEEGLint first, AEEGLsizei count) {
    glDrawArrays(mode, first, count);
    return SUCCESS;
}

int CGLES10_DrawElements(IGLES10 *pMe, AEEGLenum mode, AEEGLsizei count, AEEGLenum type,
                         const AEEGLvoid *indices) {
    glDrawElements(mode, count, type, indices);
    return SUCCESS;
}

int CGLES10_Enable(IGLES10 *pMe, AEEGLenum cap) {
    glEnable(cap);
    return SUCCESS;
}

int CGLES10_EnableClientState(IGLES10 *pMe, AEEGLenum array) {
    glEnableClientState(array);
    return SUCCESS;
}

int CGLES10_Finish(IGLES10 *pMe) {
    glFinish();
    return SUCCESS;
}

int CGLES10_Flush(IGLES10 *pMe) {
    glFlush();
    return SUCCESS;
}

int CGLES10_Fogx(IGLES10 *pMe, AEEGLenum pname, AEEGLfixed param) {
    glFogx(pname, param);
    return SUCCESS;
}

int CGLES10_Fogxv(IGLES10 *pMe, AEEGLenum pname, const AEEGLfixed *params) {
    glFogxv(pname, params);
    return SUCCESS;
}

int CGLES10_FrontFace(IGLES10 *pMe, AEEGLenum mode) {
    glFrontFace(mode);
    return SUCCESS;
}

int
CGLES10_Frustumx(IGLES10 *pMe, AEEGLfixed left, AEEGLfixed right, AEEGLfixed bottom, AEEGLfixed top,
                 AEEGLfixed zNear, AEEGLfixed zFar) {
    glFrustumx(left, right, bottom, top, zNear, zFar);
    return SUCCESS;
}

int CGLES10_GenTextures(IGLES10 *pMe, AEEGLsizei n, AEEGLuint *textures) {
    glGenTextures(n, textures);
    return SUCCESS;
}

int CGLES10_GetError(IGLES10 *pMe, AEEGLenum *ret) {
    *ret = glGetError();
    return SUCCESS;
}

int CGLES10_GetIntegerv(IGLES10 *pMe, AEEGLenum pname, AEEGLint *params) {
    glGetIntegerv(pname, params);
    return SUCCESS;
}

int CGLES10_GetString(IGLES10 *pMe, AEEGLenum name, AEEGLubyte const **ret) {
    *ret = CGL_glGetString(name);
    return SUCCESS;
}

int CGLES10_Hint(IGLES10 *pMe, AEEGLenum target, AEEGLenum mode) {
    glHint(target, mode);
    return SUCCESS;
}

int CGLES10_LightModelx(IGLES10 *pMe, AEEGLenum pname, AEEGLfixed param) {
    glLightModelx(pname, param);
    return SUCCESS;
}

int CGLES10_LightModelxv(IGLES10 *pMe, AEEGLenum pname, const AEEGLfixed *params) {
    glLightModelxv(pname, params);
    return SUCCESS;
}

int CGLES10_Lightx(IGLES10 *pMe, AEEGLenum light, AEEGLenum pname, AEEGLfixed param) {
    glLightx(light, pname, param);
    return SUCCESS;
}

int CGLES10_Lightxv(IGLES10 *pMe, AEEGLenum light, AEEGLenum pname, const AEEGLfixed *params) {
    glLightxv(light, pname, params);
    return SUCCESS;
}

int CGLES10_LineWidthx(IGLES10 *pMe, AEEGLfixed width) {
    glLineWidthx(width);
    return SUCCESS;
}

int CGLES10_LoadIdentity(IGLES10 *pMe) {
    glLoadIdentity();
    return SUCCESS;
}

int CGLES10_LoadMatrixx(IGLES10 *pMe, const AEEGLfixed *m) {
    glLoadMatrixx(m);
    return SUCCESS;
}

int CGLES10_LogicOp(IGLES10 *pMe, AEEGLenum opcode) {
    glLogicOp(opcode);
    return SUCCESS;
}

int CGLES10_Materialx(IGLES10 *pMe, AEEGLenum face, AEEGLenum pname, AEEGLfixed param) {
    glMaterialx(face, pname, param);
    return SUCCESS;
}

int CGLES10_Materialxv(IGLES10 *pMe, AEEGLenum face, AEEGLenum pname, const AEEGLfixed *params) {
    glMaterialxv(face, pname, params);
    return SUCCESS;
}

int CGLES10_MatrixMode(IGLES10 *pMe, AEEGLenum mode) {
    glMatrixMode(mode);
    return SUCCESS;
}

int CGLES10_MultMatrixx(IGLES10 *pMe, const AEEGLfixed *m) {
    glMultMatrixx(m);
    return SUCCESS;
}

int
CGLES10_MultiTexCoord4x(IGLES10 *pMe, AEEGLenum target, AEEGLfixed s, AEEGLfixed t, AEEGLfixed r,
                        AEEGLfixed q) {
    glMultiTexCoord4x(target, s, t, r, q);
    return SUCCESS;
}

int CGLES10_Normal3x(IGLES10 *pMe, AEEGLfixed nx, AEEGLfixed ny, AEEGLfixed nz) {
    glNormal3x(nx, ny, nz);
    return SUCCESS;
}

int
CGLES10_NormalPointer(IGLES10 *pMe, AEEGLenum type, AEEGLsizei stride, const AEEGLvoid *pointer) {
    glNormalPointer(type, stride, pointer);
    return SUCCESS;
}

int
CGLES10_Orthox(IGLES10 *pMe, AEEGLfixed left, AEEGLfixed right, AEEGLfixed bottom, AEEGLfixed top,
               AEEGLfixed zNear, AEEGLfixed zFar) {
    glOrthox(left, right, bottom, top, zNear, zFar);
    return SUCCESS;
}

int CGLES10_PixelStorei(IGLES10 *pMe, AEEGLenum pname, AEEGLint param) {
    glPixelStorei(pname, param);
    return SUCCESS;
}

int CGLES10_PointSizex(IGLES10 *pMe, AEEGLfixed size) {
    glPointSizex(size);
    return SUCCESS;
}

int CGLES10_PolygonOffsetx(IGLES10 *pMe, AEEGLfixed factor, AEEGLfixed units) {
    glPolygonOffsetx(factor, units);
    return SUCCESS;
}

int CGLES10_PopMatrix(IGLES10 *pMe) {
    glPopMatrix();

    return SUCCESS;
}

int CGLES10_PushMatrix(IGLES10 *pMe) {
    glPushMatrix();

    return SUCCESS;
}

int CGLES10_ReadPixels(IGLES10 *pMe, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height,
                       AEEGLenum format, AEEGLenum type, AEEGLvoid *pixels) {
    glReadPixels(x, y, width, height, format, type, pixels);
    return SUCCESS;
}

int CGLES10_Rotatex(IGLES10 *pMe, AEEGLfixed angle, AEEGLfixed x, AEEGLfixed y, AEEGLfixed z) {
    glRotatex(angle, x, y, z);
    return SUCCESS;
}

int CGLES10_SampleCoverage(IGLES10 *pMe, AEEGLclampf value, AEEGLboolean invert) {
    glSampleCoverage(value, invert);
    return SUCCESS;
}

int CGLES10_SampleCoveragex(IGLES10 *pMe, AEEGLclampx value, AEEGLboolean invert) {
    glSampleCoveragex(value, invert);
    return SUCCESS;
}

int CGLES10_Scalex(IGLES10 *pMe, AEEGLfixed x, AEEGLfixed y, AEEGLfixed z) {
    glScalex(x, y, z);
    return SUCCESS;
}

int CGLES10_Scissor(IGLES10 *pMe, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height) {
    glScissor(x, y, width, height);
    return SUCCESS;
}

int CGLES10_ShadeModel(IGLES10 *pMe, AEEGLenum mode) {
    glShadeModel(mode);
    return SUCCESS;
}

int CGLES10_StencilFunc(IGLES10 *pMe, AEEGLenum func, AEEGLint ref, AEEGLuint mask) {
    glStencilFunc(func, ref, mask);
    return SUCCESS;
}

int CGLES10_StencilMask(IGLES10 *pMe, AEEGLuint mask) {
    glStencilMask(mask);
    return SUCCESS;
}

int CGLES10_StencilOp(IGLES10 *pMe, AEEGLenum fail, AEEGLenum zfail, AEEGLenum zpass) {
    glStencilOp(fail, zfail, zpass);
    return SUCCESS;
}

int CGLES10_TexCoordPointer(IGLES10 *pMe, AEEGLint size, AEEGLenum type, AEEGLsizei stride,
                            const AEEGLvoid *pointer) {
    glTexCoordPointer(size, type, stride, pointer);
    return SUCCESS;
}

int CGLES10_TexEnvx(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfixed param) {
    glTexEnvx(target, pname, param);
    return SUCCESS;
}

int CGLES10_TexEnvxv(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, const AEEGLfixed *params) {
    glTexEnvxv(target, pname, params);
    return SUCCESS;
}

int CGLES10_TexImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLint internalformat,
                       AEEGLsizei width, AEEGLsizei height, AEEGLint border, AEEGLenum format,
                       AEEGLenum type, const AEEGLvoid *pixels) {
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
    return SUCCESS;
}

int CGLES10_TexParameterx(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfixed param) {
    glTexParameterx(target, pname, param);
    return SUCCESS;
}

int CGLES10_TexSubImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLint xoffset,
                          AEEGLint yoffset, AEEGLsizei width, AEEGLsizei height, AEEGLenum format,
                          AEEGLenum type, const AEEGLvoid *pixels) {
    glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
    return SUCCESS;
}

int CGLES10_Translatex(IGLES10 *pMe, AEEGLfixed x, AEEGLfixed y, AEEGLfixed z) {
    glTranslatex(x, y, z);
    return SUCCESS;
}

int CGLES10_VertexPointer(IGLES10 *pMe, AEEGLint size, AEEGLenum type, AEEGLsizei stride,
                          const AEEGLvoid *pointer) {
    glVertexPointer(size, type, stride, pointer);
    return SUCCESS;
}

int CGLES10_Viewport(IGLES10 *pMe, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height) {
    glViewport(x, y, width, height);
    return SUCCESS;
}

struct VTBL(IGLES10) gsCGLES10Funcs = {
        CGLES10_AddRef,
        CGLES10_Release,
        CGLES10_QueryInterface,
        CGLES10_AlphaFunc,
        CGLES10_ClearColor,
        CGLES10_ClearDepthf,
        CGLES10_Color4f,
        CGLES10_DepthRangef,
        CGLES10_Fogf,
        CGLES10_Fogfv,
        CGLES10_Frustumf,
        CGLES10_LightModelf,
        CGLES10_LightModelfv,
        CGLES10_Lightf,
        CGLES10_Lightfv,
        CGLES10_LineWidth,
        CGLES10_LoadMatrixf,
        CGLES10_Materialf,
        CGLES10_Materialfv,
        CGLES10_MultMatrixf,
        CGLES10_MultiTexCoord4f,
        CGLES10_Normal3f,
        CGLES10_Orthof,
        CGLES10_PointSize,
        CGLES10_PolygonOffset,
        CGLES10_Rotatef,
        CGLES10_Scalef,
        CGLES10_TexEnvf,
        CGLES10_TexEnvfv,
        CGLES10_TexParameterf,
        CGLES10_Translatef,
        CGLES10_ActiveTexture,
        CGLES10_AlphaFuncx,
        CGLES10_BindTexture,
        CGLES10_BlendFunc,
        CGLES10_Clear,
        CGLES10_ClearColorx,
        CGLES10_ClearDepthx,
        CGLES10_ClearStencil,
        CGLES10_ClientActiveTexture,
        CGLES10_Color4x,
        CGLES10_ColorMask,
        CGLES10_ColorPointer,
        CGLES10_CompressedTexImage2D,
        CGLES10_CompressedTexSubImage2D,
        CGLES10_CopyTexImage2D,
        CGLES10_CopyTexSubImage2D,
        CGLES10_CullFace,
        CGLES10_DeleteTextures,
        CGLES10_DepthFunc,
        CGLES10_DepthMask,
        CGLES10_DepthRangex,
        CGLES10_Disable,
        CGLES10_DisableClientState,
        CGLES10_DrawArrays,
        CGLES10_DrawElements,
        CGLES10_Enable,
        CGLES10_EnableClientState,
        CGLES10_Finish,
        CGLES10_Flush,
        CGLES10_Fogx,
        CGLES10_Fogxv,
        CGLES10_FrontFace,
        CGLES10_Frustumx,
        CGLES10_GenTextures,
        CGLES10_GetError,
        CGLES10_GetIntegerv,
        CGLES10_GetString,
        CGLES10_Hint,
        CGLES10_LightModelx,
        CGLES10_LightModelxv,
        CGLES10_Lightx,
        CGLES10_Lightxv,
        CGLES10_LineWidthx,
        CGLES10_LoadIdentity,
        CGLES10_LoadMatrixx,
        CGLES10_LogicOp,
        CGLES10_Materialx,
        CGLES10_Materialxv,
        CGLES10_MatrixMode,
        CGLES10_MultMatrixx,
        CGLES10_MultiTexCoord4x,
        CGLES10_Normal3x,
        CGLES10_NormalPointer,
        CGLES10_Orthox,
        CGLES10_PixelStorei,
        CGLES10_PointSizex,
        CGLES10_PolygonOffsetx,
        CGLES10_PopMatrix,
        CGLES10_PushMatrix,
        CGLES10_ReadPixels,
        CGLES10_Rotatex,
        CGLES10_SampleCoverage,
        CGLES10_SampleCoveragex,
        CGLES10_Scalex,
        CGLES10_Scissor,
        CGLES10_ShadeModel,
        CGLES10_StencilFunc,
        CGLES10_StencilMask,
        CGLES10_StencilOp,
        CGLES10_TexCoordPointer,
        CGLES10_TexEnvx,
        CGLES10_TexEnvxv,
        CGLES10_TexImage2D,
        CGLES10_TexParameterx,
        CGLES10_TexSubImage2D,
        CGLES10_Translatex,
        CGLES10_VertexPointer,
        CGLES10_Viewport,
};

// === //

OBJECT(CEGL10) {
    AEEVTBL(IEGL10) *pvt;
    uint32 m_nRefs;
    CGLES10 m_GLES10;
};

static uint32 CEGL10_AddRef(IEGL10 *po) {
    CEGL10 *pMe = (CEGL10 *) po;

    return (++(pMe->m_nRefs));
}

static uint32 CEGL10_Release(IEGL10 *po) {
    CEGL10 *pMe = (CEGL10 *) po;
    if (pMe->m_nRefs) {
        if (--pMe->m_nRefs == 0) {
            sys_free(po);
        }
    }
    return pMe->m_nRefs;
}

static int CEGL10_QueryInterface(IEGL10 *po, AEEIID cls, void **ppo) {
    switch (cls) {
        case AEECLSID_QUERYINTERFACE:
        case AEEIID_EGL10:
        case AEECLSID_QEGL:
            *ppo = (void *) po;
            CEGL10_AddRef(po);
            return SUCCESS;
        case AEEIID_GLES10:
            *ppo = &((CEGL10 *) po)->m_GLES10;
            CEGL10_AddRef(po);
            return SUCCESS;
        default:
            *ppo = NULL;
            return ECLASSNOTSUPPORT;
    }
}

int CEGL10_GetError(IEGL10 *pMe, AEEEGLint *ret) {
    *ret = eglGetError();
    return SUCCESS;
}

int CEGL10_GetDisplay(IEGL10 *pMe, IQueryInterface *display, AEEEGLDisplay *ret) {
    *ret = CEGL_eglGetDisplay(display);
    return SUCCESS;
}

int CEGL10_Initialize(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLint *major, AEEEGLint *minor,
                      AEEEGLBoolean *ret) {
    *ret = CEGL_eglInitialize(dpy, major, minor);
    return SUCCESS;
}

int CEGL10_Terminate(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLBoolean *ret) {
    *ret = CEGL_eglTerminate(dpy);
    return SUCCESS;

}

int CEGL10_QueryString(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLint name, const char **ret) {
    *ret = CEGL_eglQueryString(dpy, name);
    return SUCCESS;
}

int CEGL10_GetConfigs(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig *configs, AEEEGLint config_size,
                      AEEEGLint *num_config, AEEEGLBoolean *ret) {
    *ret = eglGetConfigs(dpy, configs, config_size, num_config);
    return SUCCESS;
}

int CEGL10_ChooseConfig(IEGL10 *pMe, AEEEGLDisplay dpy, const AEEEGLint *attrib_list,
                        AEEEGLConfig *configs, AEEEGLint config_size, AEEEGLint *num_config,
                        AEEEGLBoolean *ret) {
    *ret = eglChooseConfig(dpy, attrib_list, configs, config_size, num_config);
    return SUCCESS;
}

int CEGL10_GetConfigAttrib(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, AEEEGLint attribute,
                           AEEEGLint *value, AEEEGLBoolean *ret) {
    *ret = eglGetConfigAttrib(dpy, config, attribute, value);
    return SUCCESS;
}

int CEGL10_CreateWindowSurface(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig config,
                               IQueryInterface *window, const AEEEGLint *attrib_list,
                               AEEEGLSurface *ret) {
    *ret = CEGL_eglCreateWindowSurface(dpy, config, (NativeWindowType) window, attrib_list);
    return SUCCESS;
}

int CEGL10_CreatePixmapSurface(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig config,
                               IQueryInterface *pixmap, const AEEEGLint *attrib_list,
                               AEEEGLSurface *ret) {
    *ret = CEGL_eglCreatePixmapSurface(dpy, config, (NativePixmapType) pixmap, attrib_list);
    return SUCCESS;
}

int CEGL10_CreatePbufferSurface(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig config,
                                const AEEEGLint *attrib_list, AEEEGLSurface *ret) {
    *ret = CEGL_eglCreatePbufferSurface(dpy, config, attrib_list);
    return SUCCESS;
}

int
CEGL10_DestroySurface(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLSurface surface, AEEEGLBoolean *ret) {
    *ret = CEGL_eglDestroySurface(dpy, surface);
    return SUCCESS;
}

int CEGL10_QuerySurface(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLSurface surface, AEEEGLint attribute,
                        AEEEGLint *value, AEEEGLBoolean *ret) {
    *ret = CEGL_eglQuerySurface(dpy, surface, attribute, value);
    return SUCCESS;
}

int
CEGL10_CreateContext(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, AEEEGLContext share_list,
                     const AEEEGLint *attrib_list, AEEEGLContext *ret) {
    *ret = eglCreateContext(dpy, config, share_list, attrib_list);
    return SUCCESS;
}

int CEGL10_DestroyContext(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLContext ctx, AEEEGLBoolean *ret) {
    *ret = eglDestroyContext(dpy, ctx);
    return SUCCESS;
}

int CEGL10_MakeCurrent(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLSurface draw, AEEEGLSurface read,
                       AEEEGLContext ctx, AEEEGLBoolean *ret) {
    *ret = CEGL_eglMakeCurrent(dpy, draw, read, ctx);
    return SUCCESS;
}

int CEGL10_GetCurrentContext(IEGL10 *pMe, AEEEGLContext *ret) {
    *ret = eglGetCurrentContext();
    return SUCCESS;
}

int CEGL10_GetCurrentSurface(IEGL10 *pMe, AEEEGLint readdraw, AEEEGLSurface *ret) {
    *ret = CEGL_eglGetCurrentSurface(readdraw);
    return SUCCESS;
}

int CEGL10_GetCurrentDisplay(IEGL10 *pMe, AEEEGLDisplay *ret) {
    *ret = eglGetCurrentDisplay();
    return SUCCESS;
}

int CEGL10_QueryContext(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLContext ctx, AEEEGLint attribute,
                        AEEEGLint *value, AEEEGLBoolean *ret) {
    *ret = eglQueryContext(dpy, ctx, attribute, value);
    return SUCCESS;
}

int CEGL10_WaitGL(IEGL10 *pMe, AEEEGLBoolean *ret) {
    *ret = eglWaitGL();
    return SUCCESS;
}

int CEGL10_WaitNative(IEGL10 *pMe, AEEEGLint engine, AEEEGLBoolean *ret) {
    *ret = eglWaitNative(engine);
    return SUCCESS;
}

int CEGL10_SwapBuffers(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLSurface draw, AEEEGLBoolean *ret) {
    *ret = CEGL_eglSwapBuffers(dpy, draw);
    return SUCCESS;
}

int
CEGL10_CopyBuffers(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLSurface surface, IQueryInterface *target,
                   AEEEGLBoolean *ret) {
    *ret = CEGL_eglCopyBuffers(dpy, surface, (NativePixmapType) target);
    return SUCCESS;
}

static const VTBL(IEGL10) gsCEGL10Funcs = {
        CEGL10_AddRef,
        CEGL10_Release,
        CEGL10_QueryInterface,
        CEGL10_GetError,
        CEGL10_GetDisplay,
        CEGL10_Initialize,
        CEGL10_Terminate,
        CEGL10_QueryString,
        CEGL10_GetConfigs,
        CEGL10_ChooseConfig,
        CEGL10_GetConfigAttrib,
        CEGL10_CreateWindowSurface,
        CEGL10_CreatePixmapSurface,
        CEGL10_CreatePbufferSurface,
        CEGL10_DestroySurface,
        CEGL10_QuerySurface,
        CEGL10_CreateContext,
        CEGL10_DestroyContext,
        CEGL10_MakeCurrent,
        CEGL10_GetCurrentContext,
        CEGL10_GetCurrentSurface,
        CEGL10_GetCurrentDisplay,
        CEGL10_QueryContext,
        CEGL10_WaitGL,
        CEGL10_WaitNative,
        CEGL10_SwapBuffers,
        CEGL10_CopyBuffers
};

int CEGL10_New(IShell *ps, AEECLSID ClsId, void **ppObj) {
    CEGL10 *pNew;

    *ppObj = NULL;

    if (ClsId == AEECLSID_QEGL) {
        pNew = (CEGL10 *) AEE_NewClassEx((IBaseVtbl *) &gsCEGL10Funcs,
                                         sizeof(CEGL10), TRUE);
        if (!pNew) {
            return ENOMEMORY;
        } else {
            pNew->m_nRefs = 1;
            pNew->m_GLES10.pvt = &gsCGLES10Funcs;
            pNew->m_GLES10.m_pQEGL = (IEGL10 *) pNew;

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
        {"eglQueryString",            (void (*)()) CEGL_eglQueryString},
        // {"eglGetColorBufferQUALCOMM", (void (*)()) CEGL_eglGetColorBufferQUALCOMM},
        {"glGetString",               (void (*)()) CGL_glGetString},
        {"glCompressedTexImage2D",    (void (*)()) CGL_glCompressedTexImage2D},
        {"glCompressedTexSubImage2D", (void (*)()) CGL_glCompressedTexSubImage2D},
        {NULL, NULL}
};

// === //

const AEEStaticClass gAEEGLClasses[] = {
        {AEECLSID_GL,   ASCF_UPGRADE, 0, NULL, CGL_New},
        {AEECLSID_EGL,  ASCF_UPGRADE, 0, NULL, CEGL_New},
        {AEECLSID_QEGL, ASCF_UPGRADE, 0, NULL, CEGL10_New},
        {0, 0,                        0, NULL, NULL}
};
