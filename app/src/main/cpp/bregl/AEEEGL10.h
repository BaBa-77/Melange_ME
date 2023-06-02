#ifndef AEEEGL10_H
#define AEEEGL10_H
/*=============================================================================

FILE:         AEEEGL10.h

SERVICES:     IEGL10 interface

DESCRIPTION:  IEGL10 is an interface for accessing Q3Dimension functionality

===============================================================================
        Copyright © 2006 QUALCOMM Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
=============================================================================*/

#include "AEE.h"
#include "AEEDisp.h"
#include "AEEEGLTypes.h"

/* interface */
#define AEEIID_EGL10    	0x0103d8ed

#define INHERIT_IEGL10(iname) \
   INHERIT_IQueryInterface(iname); \
   int (*GetError) (iname *pMe, AEEEGLint *ret); \
   int (*GetDisplay) (iname *pMe, IQueryInterface *display, AEEEGLDisplay *ret); \
   int (*Initialize) (iname *pMe, AEEEGLDisplay dpy, AEEEGLint *major, AEEEGLint *minor, AEEEGLBoolean *ret); \
   int (*Terminate) (iname *pMe, AEEEGLDisplay dpy, AEEEGLBoolean *ret); \
   int (*QueryString) (iname *pMe, AEEEGLDisplay dpy, AEEEGLint name, const char **ret); \
   int (*GetConfigs)  (iname *pMe, AEEEGLDisplay dpy, AEEEGLConfig *configs, AEEEGLint config_size, AEEEGLint *num_config, AEEEGLBoolean *ret); \
   int (*ChooseConfig) (iname *pMe, AEEEGLDisplay dpy, const AEEEGLint *attrib_list, AEEEGLConfig *configs, AEEEGLint config_size, AEEEGLint *num_config, AEEEGLBoolean *ret); \
   int (*GetConfigAttrib) (iname *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, AEEEGLint attribute, AEEEGLint *value, AEEEGLBoolean *ret); \
   int (*CreateWindowSurface) (iname *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, IQueryInterface *window, const AEEEGLint *attrib_list, AEEEGLSurface *ret); \
   int (*CreatePixmapSurface) (iname *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, IQueryInterface *pixmap, const AEEEGLint *attrib_list, AEEEGLSurface *ret); \
   int (*CreatePbufferSurface) (iname *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, const AEEEGLint *attrib_list, AEEEGLSurface *ret); \
   int (*DestroySurface) (iname *pMe, AEEEGLDisplay dpy, AEEEGLSurface surface, AEEEGLBoolean *ret); \
   int (*QuerySurface) (iname *pMe, AEEEGLDisplay dpy, AEEEGLSurface surface, AEEEGLint attribute, AEEEGLint *value, AEEEGLBoolean *ret); \
   int (*CreateContext) (iname *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, AEEEGLContext share_list, const AEEEGLint *attrib_list, AEEEGLContext *ret); \
   int (*DestroyContext) (iname *pMe, AEEEGLDisplay dpy, AEEEGLContext ctx, AEEEGLBoolean *ret); \
   int (*MakeCurrent) (iname *pMe, AEEEGLDisplay dpy, AEEEGLSurface draw, AEEEGLSurface read, AEEEGLContext ctx, AEEEGLBoolean *ret); \
   int (*GetCurrentContext) (iname *pMe, AEEEGLContext *ret); \
   int (*GetCurrentSurface) (iname *pMe, AEEEGLint readdraw, AEEEGLSurface *ret); \
   int (*GetCurrentDisplay) (iname *pMe, AEEEGLDisplay *ret); \
   int (*QueryContext) (iname *pMe, AEEEGLDisplay dpy, AEEEGLContext ctx, AEEEGLint attribute, AEEEGLint *value, AEEEGLBoolean *ret); \
   int (*WaitGL) (iname *pMe, AEEEGLBoolean *ret); \
   int (*WaitNative) (iname *pMe, AEEEGLint engine, AEEEGLBoolean *ret); \
   int (*SwapBuffers) (iname *pMe, AEEEGLDisplay dpy, AEEEGLSurface draw, AEEEGLBoolean *ret); \
   int (*CopyBuffers) (iname *pMe, AEEEGLDisplay dpy, AEEEGLSurface surface, IQueryInterface *target, AEEEGLBoolean *ret)

// declare the actual interface
AEEINTERFACE_DEFINE(IEGL10);

static __inline uint32 IEGL10_AddRef(IEGL10 *pMe)
{
   return AEEGETPVTBL(pMe,IEGL10)->AddRef(pMe);
}

static __inline uint32 IEGL10_Release(IEGL10 *pMe)
{
   return AEEGETPVTBL(pMe,IEGL10)->Release(pMe);
}

static __inline int IEGL10_QueryInterface(IEGL10 *pMe, AEECLSID cls, void **ppo)
{
   return AEEGETPVTBL(pMe,IEGL10)->QueryInterface(pMe, cls, ppo);
}

static __inline int IEGL10_GetError(IEGL10 *pMe, AEEEGLint *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->GetError(pMe,ret);
}

static __inline int IEGL10_GetDisplay(IEGL10 *pMe, IQueryInterface *display, AEEEGLDisplay *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->GetDisplay(pMe,display,ret);
}

static __inline int IEGL10_Initialize(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLint *major, AEEEGLint *minor, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->Initialize(pMe,dpy,major,minor,ret);
}

static __inline int IEGL10_Terminate(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->Terminate(pMe,dpy,ret);
}

static __inline int IEGL10_QueryString(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLint name, const char **ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->QueryString(pMe,dpy,name,ret);
}

static __inline int IEGL10_GetConfigs(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig *configs, AEEEGLint config_size, AEEEGLint *num_config, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->GetConfigs(pMe,dpy,configs,config_size,num_config,ret);
}

static __inline int IEGL10_ChooseConfig(IEGL10 *pMe, AEEEGLDisplay dpy, const AEEEGLint *attrib_list, AEEEGLConfig *configs, AEEEGLint config_size, AEEEGLint *num_config, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->ChooseConfig(pMe,dpy,attrib_list,configs,config_size,num_config,ret);
}

static __inline int IEGL10_GetConfigAttrib(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, AEEEGLint attribute, AEEEGLint *value, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->GetConfigAttrib(pMe,dpy,config,attribute,value,ret);
}

static __inline int IEGL10_CreateWindowSurface(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, IQueryInterface * window, const AEEEGLint *attrib_list, AEEEGLSurface *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->CreateWindowSurface(pMe,dpy,config,window,attrib_list,ret);
}

static __inline int IEGL10_CreatePixmapSurface(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, IQueryInterface * pixmap, const AEEEGLint *attrib_list, AEEEGLSurface *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->CreatePixmapSurface(pMe,dpy,config,pixmap,attrib_list,ret);
}

static __inline int IEGL10_CreatePbufferSurface(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, const AEEEGLint *attrib_list, AEEEGLSurface *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->CreatePbufferSurface(pMe,dpy,config,attrib_list,ret);
}

static __inline int IEGL10_DestroySurface(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLSurface surface, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->DestroySurface(pMe,dpy,surface,ret);
}

static __inline int IEGL10_QuerySurface(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLSurface surface, AEEEGLint attribute, AEEEGLint *value, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->QuerySurface(pMe,dpy,surface,attribute,value,ret);
}

static __inline int IEGL10_CreateContext(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLConfig config, AEEEGLContext share_list, const AEEEGLint *attrib_list, AEEEGLContext *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->CreateContext(pMe,dpy,config,share_list,attrib_list,ret);
}

static __inline int IEGL10_DestroyContext(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLContext ctx, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->DestroyContext(pMe,dpy,ctx,ret);
}

static __inline int IEGL10_MakeCurrent(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLSurface draw, AEEEGLSurface read, AEEEGLContext ctx, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->MakeCurrent(pMe,dpy,draw,read,ctx,ret);
}

static __inline int IEGL10_GetCurrentContext(IEGL10 *pMe, AEEEGLContext *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->GetCurrentContext(pMe,ret);
}

static __inline int IEGL10_GetCurrentSurface(IEGL10 *pMe, AEEEGLint readdraw, AEEEGLSurface *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->GetCurrentSurface(pMe,readdraw,ret);
}

static __inline int IEGL10_GetCurrentDisplay(IEGL10 *pMe, AEEEGLDisplay *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->GetCurrentDisplay(pMe,ret);
}

static __inline int IEGL10_QueryContext(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLContext ctx, AEEEGLint attribute, AEEEGLint *value, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->QueryContext(pMe,dpy,ctx,attribute,value,ret);
}

static __inline int IEGL10_WaitGL(IEGL10 *pMe, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->WaitGL(pMe,ret);
}

static __inline int IEGL10_WaitNative(IEGL10 *pMe, AEEEGLint engine, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->WaitNative(pMe,engine,ret);
}

static __inline int IEGL10_SwapBuffers(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLSurface draw, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->SwapBuffers(pMe,dpy,draw,ret);
}

static __inline int IEGL10_CopyBuffers(IEGL10 *pMe, AEEEGLDisplay dpy, AEEEGLSurface surface, IQueryInterface * target, AEEEGLBoolean *ret)
{
   return AEEGETPVTBL(pMe,IEGL10)->CopyBuffers(pMe,dpy,surface,target,ret);
}


#endif /* AEEEGL10_H */
