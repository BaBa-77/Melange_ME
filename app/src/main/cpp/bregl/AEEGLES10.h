#ifndef AEEGLES10_H
#define AEEGLES10_H
/*=============================================================================

FILE:         AEEGLES10.h

SERVICES:     GLES10 interface

DESCRIPTION:  GLES10 is an interface for accessing Q3Dimension functionality

===============================================================================
        Copyright © 2006 QUALCOMM Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
=============================================================================*/

#include "AEE.h"
#include "AEEGLESTypes.h"


/* interface */
#define AEEIID_GLES10    	0x0103d8dd


#define INHERIT_IGLES10(iname) \
   INHERIT_IQueryInterface(iname); \
   int (*AlphaFunc) (iname *pMe, AEEGLenum func, AEEGLclampf ref); \
   int (*ClearColor) (iname *pMe, AEEGLclampf red, AEEGLclampf green, AEEGLclampf blue, AEEGLclampf alpha); \
   int (*ClearDepthf) (iname *pMe, AEEGLclampf depth); \
   int (*Color4f) (iname *pMe, AEEGLfloat red, AEEGLfloat green, AEEGLfloat blue, AEEGLfloat alpha); \
   int (*DepthRangef) (iname *pMe, AEEGLclampf zNear, AEEGLclampf zFar); \
   int (*Fogf) (iname *pMe, AEEGLenum pname, AEEGLfloat param); \
   int (*Fogfv) (iname *pMe, AEEGLenum pname, const AEEGLfloat *params); \
   int (*Frustumf) (iname *pMe, AEEGLfloat left, AEEGLfloat right, AEEGLfloat bottom, AEEGLfloat top, AEEGLfloat zNear, AEEGLfloat zFar); \
   int (*LightModelf) (iname *pMe, AEEGLenum pname, AEEGLfloat param); \
   int (*LightModelfv) (iname *pMe, AEEGLenum pname, const AEEGLfloat *params); \
   int (*Lightf) (iname *pMe, AEEGLenum light, AEEGLenum pname, AEEGLfloat param); \
   int (*Lightfv) (iname *pMe, AEEGLenum light, AEEGLenum pname, const AEEGLfloat *params); \
   int (*LineWidth) (iname *pMe, AEEGLfloat width); \
   int (*LoadMatrixf) (iname *pMe, const AEEGLfloat *m); \
   int (*Materialf) (iname *pMe, AEEGLenum face, AEEGLenum pname, AEEGLfloat param); \
   int (*Materialfv) (iname *pMe, AEEGLenum face, AEEGLenum pname, const AEEGLfloat *params); \
   int (*MultMatrixf) (iname *pMe, const AEEGLfloat *m); \
   int (*MultiTexCoord4f) (iname *pMe, AEEGLenum target, AEEGLfloat s, AEEGLfloat t, AEEGLfloat r, AEEGLfloat q); \
   int (*Normal3f) (iname *pMe, AEEGLfloat nx, AEEGLfloat ny, AEEGLfloat nz); \
   int (*Orthof) (iname *pMe, AEEGLfloat left, AEEGLfloat right, AEEGLfloat bottom, AEEGLfloat top, AEEGLfloat zNear, AEEGLfloat zFar); \
   int (*PointSize) (iname *pMe, AEEGLfloat size); \
   int (*PolygonOffset) (iname *pMe, AEEGLfloat factor, AEEGLfloat units); \
   int (*Rotatef) (iname *pMe, AEEGLfloat angle, AEEGLfloat x, AEEGLfloat y, AEEGLfloat z); \
   int (*Scalef) (iname *pMe, AEEGLfloat x, AEEGLfloat y, AEEGLfloat z); \
   int (*TexEnvf) (iname *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfloat param); \
   int (*TexEnvfv) (iname *pMe, AEEGLenum target, AEEGLenum pname, const AEEGLfloat *params); \
   int (*TexParameterf) (iname *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfloat param); \
   int (*Translatef) (iname *pMe, AEEGLfloat x, AEEGLfloat y, AEEGLfloat z); \
   int (*ActiveTexture) (iname *pMe, AEEGLenum texture); \
   int (*AlphaFuncx) (iname *pMe, AEEGLenum func, AEEGLclampx ref); \
   int (*BindTexture) (iname *pMe, AEEGLenum target, AEEGLuint texture); \
   int (*BlendFunc) (iname *pMe, AEEGLenum sfactor, AEEGLenum dfactor); \
   int (*Clear) (iname *pMe, AEEGLbitfield mask); \
   int (*ClearColorx) (iname *pMe, AEEGLclampx red, AEEGLclampx green, AEEGLclampx blue, AEEGLclampx alpha); \
   int (*ClearDepthx) (iname *pMe, AEEGLclampx depth); \
   int (*ClearStencil) (iname *pMe, AEEGLint s); \
   int (*ClientActiveTexture) (iname *pMe, AEEGLenum texture); \
   int (*Color4x) (iname *pMe, AEEGLfixed red, AEEGLfixed green, AEEGLfixed blue, AEEGLfixed alpha); \
   int (*ColorMask) (iname *pMe, AEEGLboolean red, AEEGLboolean green, AEEGLboolean blue, AEEGLboolean alpha); \
   int (*ColorPointer) (iname *pMe, AEEGLint size, AEEGLenum type, AEEGLsizei stride, const AEEGLvoid *pointer); \
   int (*CompressedTexImage2D) (iname *pMe, AEEGLenum target, AEEGLint level, AEEGLenum internalformat, AEEGLsizei width, AEEGLsizei height, AEEGLint border, AEEGLsizei imageSize, const AEEGLvoid *data); \
   int (*CompressedTexSubImage2D) (iname *pMe, AEEGLenum target, AEEGLint level, AEEGLint xoffset, AEEGLint yoffset, AEEGLsizei width, AEEGLsizei height, AEEGLenum format, AEEGLsizei imageSize, const AEEGLvoid *data); \
   int (*CopyTexImage2D) (iname *pMe, AEEGLenum target, AEEGLint level, AEEGLenum internalformat, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height, AEEGLint border); \
   int (*CopyTexSubImage2D) (iname *pMe, AEEGLenum target, AEEGLint level, AEEGLint xoffset, AEEGLint yoffset, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height); \
   int (*CullFace) (iname *pMe, AEEGLenum mode); \
   int (*DeleteTextures) (iname *pMe, AEEGLsizei n, const AEEGLuint *textures); \
   int (*DepthFunc) (iname *pMe, AEEGLenum func); \
   int (*DepthMask) (iname *pMe, AEEGLboolean flag); \
   int (*DepthRangex) (iname *pMe, AEEGLclampx zNear, AEEGLclampx zFar); \
   int (*Disable) (iname *pMe, AEEGLenum cap); \
   int (*DisableClientState) (iname *pMe, AEEGLenum array); \
   int (*DrawArrays) (iname *pMe, AEEGLenum mode, AEEGLint first, AEEGLsizei count); \
   int (*DrawElements) (iname *pMe, AEEGLenum mode, AEEGLsizei count, AEEGLenum type, const AEEGLvoid *indices); \
   int (*Enable) (iname *pMe, AEEGLenum cap); \
   int (*EnableClientState) (iname *pMe, AEEGLenum array); \
   int (*Finish) (iname *pMe); \
   int (*Flush) (iname *pMe); \
   int (*Fogx) (iname *pMe, AEEGLenum pname, AEEGLfixed param); \
   int (*Fogxv) (iname *pMe, AEEGLenum pname, const AEEGLfixed *params); \
   int (*FrontFace) (iname *pMe, AEEGLenum mode); \
   int (*Frustumx) (iname *pMe, AEEGLfixed left, AEEGLfixed right, AEEGLfixed bottom, AEEGLfixed top, AEEGLfixed zNear, AEEGLfixed zFar); \
   int (*GenTextures) (iname *pMe, AEEGLsizei n, AEEGLuint *textures); \
   int (*GetError) (iname *pMe, AEEGLenum *ret); \
   int (*GetIntegerv) (iname *pMe, AEEGLenum pname, AEEGLint *params); \
   int (*GetString) (iname *pMe, AEEGLenum name, AEEGLubyte const **ret); \
   int (*Hint) (iname *pMe, AEEGLenum target, AEEGLenum mode); \
   int (*LightModelx) (iname *pMe, AEEGLenum pname, AEEGLfixed param); \
   int (*LightModelxv) (iname *pMe, AEEGLenum pname, const AEEGLfixed *params); \
   int (*Lightx) (iname *pMe, AEEGLenum light, AEEGLenum pname, AEEGLfixed param); \
   int (*Lightxv) (iname *pMe, AEEGLenum light, AEEGLenum pname, const AEEGLfixed *params); \
   int (*LineWidthx) (iname *pMe, AEEGLfixed width); \
   int (*LoadIdentity) (iname *pMe); \
   int (*LoadMatrixx) (iname *pMe, const AEEGLfixed *m); \
   int (*LogicOp) (iname *pMe, AEEGLenum opcode); \
   int (*Materialx) (iname *pMe, AEEGLenum face, AEEGLenum pname, AEEGLfixed param); \
   int (*Materialxv) (iname *pMe, AEEGLenum face, AEEGLenum pname, const AEEGLfixed *params); \
   int (*MatrixMode) (iname *pMe, AEEGLenum mode); \
   int (*MultMatrixx) (iname *pMe, const AEEGLfixed *m); \
   int (*MultiTexCoord4x) (iname *pMe, AEEGLenum target, AEEGLfixed s, AEEGLfixed t, AEEGLfixed r, AEEGLfixed q); \
   int (*Normal3x) (iname *pMe, AEEGLfixed nx, AEEGLfixed ny, AEEGLfixed nz); \
   int (*NormalPointer) (iname *pMe, AEEGLenum type, AEEGLsizei stride, const AEEGLvoid *pointer); \
   int (*Orthox) (iname *pMe, AEEGLfixed left, AEEGLfixed right, AEEGLfixed bottom, AEEGLfixed top, AEEGLfixed zNear, AEEGLfixed zFar); \
   int (*PixelStorei) (iname *pMe, AEEGLenum pname, AEEGLint param); \
   int (*PointSizex) (iname *pMe, AEEGLfixed size); \
   int (*PolygonOffsetx) (iname *pMe, AEEGLfixed factor, AEEGLfixed units); \
   int (*PopMatrix) (iname *pMe); \
   int (*PushMatrix) (iname *pMe); \
   int (*ReadPixels) (iname *pMe, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height, AEEGLenum format, AEEGLenum type, AEEGLvoid *pixels); \
   int (*Rotatex) (iname *pMe, AEEGLfixed angle, AEEGLfixed x, AEEGLfixed y, AEEGLfixed z); \
   int (*SampleCoverage) (iname *pMe, AEEGLclampf value, AEEGLboolean invert); \
   int (*SampleCoveragex) (iname *pMe, AEEGLclampx value, AEEGLboolean invert); \
   int (*Scalex) (iname *pMe, AEEGLfixed x, AEEGLfixed y, AEEGLfixed z); \
   int (*Scissor) (iname *pMe, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height); \
   int (*ShadeModel) (iname *pMe, AEEGLenum mode); \
   int (*StencilFunc) (iname *pMe, AEEGLenum func, AEEGLint ref, AEEGLuint mask); \
   int (*StencilMask) (iname *pMe, AEEGLuint mask); \
   int (*StencilOp) (iname *pMe, AEEGLenum fail, AEEGLenum zfail, AEEGLenum zpass); \
   int (*TexCoordPointer) (iname *pMe, AEEGLint size, AEEGLenum type, AEEGLsizei stride, const AEEGLvoid *pointer); \
   int (*TexEnvx) (iname *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfixed param); \
   int (*TexEnvxv) (iname *pMe, AEEGLenum target, AEEGLenum pname, const AEEGLfixed *params); \
   int (*TexImage2D) (iname *pMe, AEEGLenum target, AEEGLint level, AEEGLint internalformat, AEEGLsizei width, AEEGLsizei height, AEEGLint border, AEEGLenum format, AEEGLenum type, const AEEGLvoid *pixels); \
   int (*TexParameterx) (iname *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfixed param); \
   int (*TexSubImage2D) (iname *pMe, AEEGLenum target, AEEGLint level, AEEGLint xoffset, AEEGLint yoffset, AEEGLsizei width, AEEGLsizei height, AEEGLenum format, AEEGLenum type, const AEEGLvoid *pixels); \
   int (*Translatex) (iname *pMe, AEEGLfixed x, AEEGLfixed y, AEEGLfixed z); \
   int (*VertexPointer) (iname *pMe, AEEGLint size, AEEGLenum type, AEEGLsizei stride, const AEEGLvoid *pointer); \
   int (*Viewport) (iname *pMe, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height)


// declare the actual interface
AEEINTERFACE_DEFINE(IGLES10);

static __inline uint32 IGLES10_AddRef(IGLES10 *pMe)
{
   return AEEGETPVTBL(pMe,IGLES10)->AddRef(pMe);
}

static __inline uint32 IGLES10_Release(IGLES10 *pMe)
{
   return AEEGETPVTBL(pMe,IGLES10)->Release(pMe);
}

static __inline int IGLES10_QueryInterface(IGLES10 *pMe, AEECLSID cls, void **ppo)
{
   return AEEGETPVTBL(pMe,IGLES10)->QueryInterface(pMe, cls, ppo);
}

static __inline int IGLES10_AlphaFunc(IGLES10 *pMe, AEEGLenum func, AEEGLclampf ref)
{
   return AEEGETPVTBL(pMe,IGLES10)->AlphaFunc(pMe, func, ref);
}

static __inline int IGLES10_ClearColor(IGLES10 *pMe, AEEGLclampf red, AEEGLclampf green, AEEGLclampf blue, AEEGLclampf alpha)
{
   return AEEGETPVTBL(pMe,IGLES10)->ClearColor(pMe, red, green, blue, alpha);
}

static __inline int IGLES10_ClearDepthf(IGLES10 *pMe, AEEGLclampf depth)
{
   return AEEGETPVTBL(pMe,IGLES10)->ClearDepthf(pMe, depth);
}

static __inline int IGLES10_Color4f(IGLES10 *pMe, AEEGLfloat red, AEEGLfloat green, AEEGLfloat blue, AEEGLfloat alpha)
{
   return AEEGETPVTBL(pMe,IGLES10)->Color4f(pMe, red, green, blue, alpha);
}

static __inline int IGLES10_DepthRangef(IGLES10 *pMe, AEEGLclampf zNear, AEEGLclampf zFar)
{
   return AEEGETPVTBL(pMe,IGLES10)->DepthRangef(pMe, zNear, zFar);
}

static __inline int IGLES10_Fogf(IGLES10 *pMe, AEEGLenum pname, AEEGLfloat param)
{
   return AEEGETPVTBL(pMe,IGLES10)->Fogf(pMe, pname, param);
}

static __inline int IGLES10_Fogfv(IGLES10 *pMe, AEEGLenum pname, const AEEGLfloat *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->Fogfv(pMe, pname, params);
}

static __inline int IGLES10_Frustumf(IGLES10 *pMe, AEEGLfloat left, AEEGLfloat right, AEEGLfloat bottom, AEEGLfloat top, AEEGLfloat zNear, AEEGLfloat zFar)
{
   return AEEGETPVTBL(pMe,IGLES10)->Frustumf(pMe, left, right, bottom, top, zNear, zFar);
}

static __inline int IGLES10_LightModelf(IGLES10 *pMe, AEEGLenum pname, AEEGLfloat param)
{
   return AEEGETPVTBL(pMe,IGLES10)->LightModelf(pMe, pname, param);
}

static __inline int IGLES10_LightModelfv(IGLES10 *pMe, AEEGLenum pname, const AEEGLfloat *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->LightModelfv(pMe, pname, params);
}

static __inline int IGLES10_Lightf(IGLES10 *pMe, AEEGLenum light, AEEGLenum pname, AEEGLfloat param)
{
   return AEEGETPVTBL(pMe,IGLES10)->Lightf(pMe, light, pname, param);
}

static __inline int IGLES10_Lightfv(IGLES10 *pMe, AEEGLenum light, AEEGLenum pname, const AEEGLfloat *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->Lightfv(pMe, light, pname, params);
}

static __inline int IGLES10_LineWidth(IGLES10 *pMe, AEEGLfloat width)
{
   return AEEGETPVTBL(pMe,IGLES10)->LineWidth(pMe, width);
}

static __inline int IGLES10_LoadMatrixf(IGLES10 *pMe, const AEEGLfloat *m)
{
   return AEEGETPVTBL(pMe,IGLES10)->LoadMatrixf(pMe, m);
}

static __inline int IGLES10_Materialf(IGLES10 *pMe, AEEGLenum face, AEEGLenum pname, AEEGLfloat param)
{
   return AEEGETPVTBL(pMe,IGLES10)->Materialf(pMe, face, pname, param);
}

static __inline int IGLES10_Materialfv(IGLES10 *pMe, AEEGLenum face, AEEGLenum pname, const AEEGLfloat *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->Materialfv(pMe, face, pname, params);
}

static __inline int IGLES10_MultMatrixf(IGLES10 *pMe, const AEEGLfloat *m)
{
   return AEEGETPVTBL(pMe,IGLES10)->MultMatrixf(pMe, m);
}

static __inline int IGLES10_MultiTexCoord4f(IGLES10 *pMe, AEEGLenum target, AEEGLfloat s, AEEGLfloat t, AEEGLfloat r, AEEGLfloat q)
{
   return AEEGETPVTBL(pMe,IGLES10)->MultiTexCoord4f(pMe, target, s, t, r, q);
}

static __inline int IGLES10_Normal3f(IGLES10 *pMe, AEEGLfloat nx, AEEGLfloat ny, AEEGLfloat nz)
{
   return AEEGETPVTBL(pMe,IGLES10)->Normal3f(pMe, nx, ny, nz);
}

static __inline int IGLES10_Orthof(IGLES10 *pMe, AEEGLfloat left, AEEGLfloat right, AEEGLfloat bottom, AEEGLfloat top, AEEGLfloat zNear, AEEGLfloat zFar)
{
   return AEEGETPVTBL(pMe,IGLES10)->Orthof(pMe, left, right, bottom, top, zNear, zFar);
}

static __inline int IGLES10_PointSize(IGLES10 *pMe, AEEGLfloat size)
{
   return AEEGETPVTBL(pMe,IGLES10)->PointSize(pMe, size);
}

static __inline int IGLES10_PolygonOffset(IGLES10 *pMe, AEEGLfloat factor, AEEGLfloat units)
{
   return AEEGETPVTBL(pMe,IGLES10)->PolygonOffset(pMe, factor, units);
}

static __inline int IGLES10_Rotatef(IGLES10 *pMe, AEEGLfloat angle, AEEGLfloat x, AEEGLfloat y, AEEGLfloat z)
{
   return AEEGETPVTBL(pMe,IGLES10)->Rotatef(pMe, angle, x, y, z);
}

static __inline int IGLES10_Scalef(IGLES10 *pMe, AEEGLfloat x, AEEGLfloat y, AEEGLfloat z)
{
   return AEEGETPVTBL(pMe,IGLES10)->Scalef(pMe, x, y, z);
}

static __inline int IGLES10_TexEnvf(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfloat param)
{
   return AEEGETPVTBL(pMe,IGLES10)->TexEnvf(pMe, target, pname, param);
}

static __inline int IGLES10_TexEnvfv(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, const AEEGLfloat *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->TexEnvfv(pMe, target, pname, params);
}

static __inline int IGLES10_TexParameterf(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfloat param)
{
   return AEEGETPVTBL(pMe,IGLES10)->TexParameterf(pMe, target, pname, param);
}

static __inline int IGLES10_Translatef(IGLES10 *pMe, AEEGLfloat x, AEEGLfloat y, AEEGLfloat z)
{
   return AEEGETPVTBL(pMe,IGLES10)->Translatef(pMe, x, y, z);
}

static __inline int IGLES10_ActiveTexture(IGLES10 *pMe, AEEGLenum texture)
{
   return AEEGETPVTBL(pMe,IGLES10)->ActiveTexture(pMe, texture);
}

static __inline int IGLES10_AlphaFuncx(IGLES10 *pMe, AEEGLenum func, AEEGLclampx ref)
{
   return AEEGETPVTBL(pMe,IGLES10)->AlphaFuncx(pMe, func, ref);
}

static __inline int IGLES10_BindTexture(IGLES10 *pMe, AEEGLenum target, AEEGLuint texture)
{
   return AEEGETPVTBL(pMe,IGLES10)->BindTexture(pMe, target, texture);
}

static __inline int IGLES10_BlendFunc(IGLES10 *pMe, AEEGLenum sfactor, AEEGLenum dfactor)
{
   return AEEGETPVTBL(pMe,IGLES10)->BlendFunc(pMe, sfactor, dfactor);
}

static __inline int IGLES10_Clear(IGLES10 *pMe, AEEGLbitfield mask)
{
   return AEEGETPVTBL(pMe,IGLES10)->Clear(pMe, mask);
}

static __inline int IGLES10_ClearColorx(IGLES10 *pMe, AEEGLclampx red, AEEGLclampx green, AEEGLclampx blue, AEEGLclampx alpha)
{
   return AEEGETPVTBL(pMe,IGLES10)->ClearColorx(pMe, red, green, blue, alpha);
}

static __inline int IGLES10_ClearDepthx(IGLES10 *pMe, AEEGLclampx depth)
{
   return AEEGETPVTBL(pMe,IGLES10)->ClearDepthx(pMe, depth);
}

static __inline int IGLES10_ClearStencil(IGLES10 *pMe, AEEGLint s)
{
   return AEEGETPVTBL(pMe,IGLES10)->ClearStencil(pMe, s);
}

static __inline int IGLES10_ClientActiveTexture(IGLES10 *pMe, AEEGLenum texture)
{
   return AEEGETPVTBL(pMe,IGLES10)->ClientActiveTexture(pMe, texture);
}

static __inline int IGLES10_Color4x(IGLES10 *pMe, AEEGLfixed red, AEEGLfixed green, AEEGLfixed blue, AEEGLfixed alpha)
{
   return AEEGETPVTBL(pMe,IGLES10)->Color4x(pMe, red, green, blue, alpha);
}

static __inline int IGLES10_ColorMask(IGLES10 *pMe, AEEGLboolean red, AEEGLboolean green, AEEGLboolean blue, AEEGLboolean alpha)
{
   return AEEGETPVTBL(pMe,IGLES10)->ColorMask(pMe, red, green, blue, alpha);
}

static __inline int IGLES10_ColorPointer(IGLES10 *pMe, AEEGLint size, AEEGLenum type, AEEGLsizei stride, const AEEGLvoid *pointer)
{
   return AEEGETPVTBL(pMe,IGLES10)->ColorPointer(pMe, size, type, stride, pointer);
}

static __inline int IGLES10_CompressedTexImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLenum internalformat, AEEGLsizei width, AEEGLsizei height, AEEGLint border, AEEGLsizei imageSize, const AEEGLvoid *data)
{
   return AEEGETPVTBL(pMe,IGLES10)->CompressedTexImage2D(pMe, target, level, internalformat, width, height, border, imageSize, data);
}

static __inline int IGLES10_CompressedTexSubImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLint xoffset, AEEGLint yoffset, AEEGLsizei width, AEEGLsizei height, AEEGLenum format, AEEGLsizei imageSize, const AEEGLvoid *data)
{
   return AEEGETPVTBL(pMe,IGLES10)->CompressedTexSubImage2D(pMe, target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

static __inline int IGLES10_CopyTexImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLenum internalformat, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height, AEEGLint border)
{
   return AEEGETPVTBL(pMe,IGLES10)->CopyTexImage2D(pMe, target, level, internalformat, x, y, width, height, border);
}

static __inline int IGLES10_CopyTexSubImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLint xoffset, AEEGLint yoffset, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height)
{
   return AEEGETPVTBL(pMe,IGLES10)->CopyTexSubImage2D(pMe, target, level, xoffset, yoffset, x, y, width, height);
}

static __inline int IGLES10_CullFace(IGLES10 *pMe, AEEGLenum mode)
{
   return AEEGETPVTBL(pMe,IGLES10)->CullFace(pMe, mode);
}

static __inline int IGLES10_DeleteTextures(IGLES10 *pMe, AEEGLsizei n, const AEEGLuint *textures)
{
   return AEEGETPVTBL(pMe,IGLES10)->DeleteTextures(pMe, n, textures);
}

static __inline int IGLES10_DepthFunc(IGLES10 *pMe, AEEGLenum func)
{
   return AEEGETPVTBL(pMe,IGLES10)->DepthFunc(pMe, func);
}

static __inline int IGLES10_DepthMask(IGLES10 *pMe, AEEGLboolean flag)
{
   return AEEGETPVTBL(pMe,IGLES10)->DepthMask(pMe, flag);
}

static __inline int IGLES10_DepthRangex(IGLES10 *pMe, AEEGLclampx zNear, AEEGLclampx zFar)
{
   return AEEGETPVTBL(pMe,IGLES10)->DepthRangex(pMe, zNear, zFar);
}

static __inline int IGLES10_Disable(IGLES10 *pMe, AEEGLenum cap)
{
   return AEEGETPVTBL(pMe,IGLES10)->Disable(pMe, cap);
}

static __inline int IGLES10_DisableClientState(IGLES10 *pMe, AEEGLenum array)
{
   return AEEGETPVTBL(pMe,IGLES10)->DisableClientState(pMe, array);
}

static __inline int IGLES10_DrawArrays(IGLES10 *pMe, AEEGLenum mode, AEEGLint first, AEEGLsizei count)
{
   return AEEGETPVTBL(pMe,IGLES10)->DrawArrays(pMe, mode, first, count);
}

static __inline int IGLES10_DrawElements(IGLES10 *pMe, AEEGLenum mode, AEEGLsizei count, AEEGLenum type, const AEEGLvoid *indices)
{
   return AEEGETPVTBL(pMe,IGLES10)->DrawElements(pMe, mode, count, type, indices);
}

static __inline int IGLES10_Enable(IGLES10 *pMe, AEEGLenum cap)
{
   return AEEGETPVTBL(pMe,IGLES10)->Enable(pMe, cap);
}

static __inline int IGLES10_EnableClientState(IGLES10 *pMe, AEEGLenum array)
{
   return AEEGETPVTBL(pMe,IGLES10)->EnableClientState(pMe, array);
}

static __inline int IGLES10_Finish(IGLES10 *pMe)
{
   return AEEGETPVTBL(pMe,IGLES10)->Finish(pMe);
}

static __inline int IGLES10_Flush(IGLES10 *pMe)
{
   return AEEGETPVTBL(pMe,IGLES10)->Flush(pMe);
}

static __inline int IGLES10_Fogx(IGLES10 *pMe, AEEGLenum pname, AEEGLfixed param)
{
   return AEEGETPVTBL(pMe,IGLES10)->Fogx(pMe, pname, param);
}

static __inline int IGLES10_Fogxv(IGLES10 *pMe, AEEGLenum pname, const AEEGLfixed *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->Fogxv(pMe, pname, params);
}

static __inline int IGLES10_FrontFace(IGLES10 *pMe, AEEGLenum mode)
{
   return AEEGETPVTBL(pMe,IGLES10)->FrontFace(pMe, mode);
}

static __inline int IGLES10_Frustumx(IGLES10 *pMe, AEEGLfixed left, AEEGLfixed right, AEEGLfixed bottom, AEEGLfixed top, AEEGLfixed zNear, AEEGLfixed zFar)
{
   return AEEGETPVTBL(pMe,IGLES10)->Frustumx(pMe, left, right, bottom, top, zNear, zFar);
}

static __inline int IGLES10_GenTextures(IGLES10 *pMe, AEEGLsizei n, AEEGLuint *textures)
{
   return AEEGETPVTBL(pMe,IGLES10)->GenTextures(pMe, n, textures);
}

static __inline int IGLES10_GetError(IGLES10 *pMe, AEEGLenum *ret)
{
   return AEEGETPVTBL(pMe,IGLES10)->GetError(pMe, ret);
}

static __inline int IGLES10_GetIntegerv(IGLES10 *pMe, AEEGLenum pname, AEEGLint *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->GetIntegerv(pMe, pname, params);
}

static __inline int IGLES10_GetString(IGLES10 *pMe, AEEGLenum name, AEEGLubyte const **ret)
{
   return AEEGETPVTBL(pMe,IGLES10)->GetString(pMe, name, ret);
}

static __inline int IGLES10_Hint(IGLES10 *pMe, AEEGLenum target, AEEGLenum mode)
{
   return AEEGETPVTBL(pMe,IGLES10)->Hint(pMe, target, mode);
}

static __inline int IGLES10_LightModelx(IGLES10 *pMe, AEEGLenum pname, AEEGLfixed param)
{
   return AEEGETPVTBL(pMe,IGLES10)->LightModelx(pMe, pname, param);
}

static __inline int IGLES10_LightModelxv(IGLES10 *pMe, AEEGLenum pname, const AEEGLfixed *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->LightModelxv(pMe, pname, params);
}

static __inline int IGLES10_Lightx(IGLES10 *pMe, AEEGLenum light, AEEGLenum pname, AEEGLfixed param)
{
   return AEEGETPVTBL(pMe,IGLES10)->Lightx(pMe, light, pname, param);
}

static __inline int IGLES10_Lightxv(IGLES10 *pMe, AEEGLenum light, AEEGLenum pname, const AEEGLfixed *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->Lightxv(pMe, light, pname, params);
}

static __inline int IGLES10_LineWidthx(IGLES10 *pMe, AEEGLfixed width)
{
   return AEEGETPVTBL(pMe,IGLES10)->LineWidthx(pMe, width);
}

static __inline int IGLES10_LoadIdentity(IGLES10 *pMe)
{
   return AEEGETPVTBL(pMe,IGLES10)->LoadIdentity(pMe);
}

static __inline int IGLES10_LoadMatrixx(IGLES10 *pMe, const AEEGLfixed *m)
{
   return AEEGETPVTBL(pMe,IGLES10)->LoadMatrixx(pMe, m);
}

static __inline int IGLES10_LogicOp(IGLES10 *pMe, AEEGLenum opcode)
{
   return AEEGETPVTBL(pMe,IGLES10)->LogicOp(pMe, opcode);
}

static __inline int IGLES10_Materialx(IGLES10 *pMe, AEEGLenum face, AEEGLenum pname, AEEGLfixed param)
{
   return AEEGETPVTBL(pMe,IGLES10)->Materialx(pMe, face, pname, param);
}

static __inline int IGLES10_Materialxv(IGLES10 *pMe, AEEGLenum face, AEEGLenum pname, const AEEGLfixed *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->Materialxv(pMe, face, pname, params);
}

static __inline int IGLES10_MatrixMode(IGLES10 *pMe, AEEGLenum mode)
{
   return AEEGETPVTBL(pMe,IGLES10)->MatrixMode(pMe, mode);
}

static __inline int IGLES10_MultMatrixx(IGLES10 *pMe, const AEEGLfixed *m)
{
   return AEEGETPVTBL(pMe,IGLES10)->MultMatrixx(pMe, m);
}

static __inline int IGLES10_MultiTexCoord4x(IGLES10 *pMe, AEEGLenum target, AEEGLfixed s, AEEGLfixed t, AEEGLfixed r, AEEGLfixed q)
{
   return AEEGETPVTBL(pMe,IGLES10)->MultiTexCoord4x(pMe, target, s, t, r, q);
}

static __inline int IGLES10_Normal3x(IGLES10 *pMe, AEEGLfixed nx, AEEGLfixed ny, AEEGLfixed nz)
{
   return AEEGETPVTBL(pMe,IGLES10)->Normal3x(pMe, nx, ny, nz);
}

static __inline int IGLES10_NormalPointer(IGLES10 *pMe, AEEGLenum type, AEEGLsizei stride, const AEEGLvoid *pointer)
{
   return AEEGETPVTBL(pMe,IGLES10)->NormalPointer(pMe, type, stride, pointer);
}

static __inline int IGLES10_Orthox(IGLES10 *pMe, AEEGLfixed left, AEEGLfixed right, AEEGLfixed bottom, AEEGLfixed top, AEEGLfixed zNear, AEEGLfixed zFar)
{
   return AEEGETPVTBL(pMe,IGLES10)->Orthox(pMe, left, right, bottom, top, zNear, zFar);
}

static __inline int IGLES10_PixelStorei(IGLES10 *pMe, AEEGLenum pname, AEEGLint param)
{
   return AEEGETPVTBL(pMe,IGLES10)->PixelStorei(pMe, pname, param);
}

static __inline int IGLES10_PointSizex(IGLES10 *pMe, AEEGLfixed size)
{
   return AEEGETPVTBL(pMe,IGLES10)->PointSizex(pMe, size);
}

static __inline int IGLES10_PolygonOffsetx(IGLES10 *pMe, AEEGLfixed factor, AEEGLfixed units)
{
   return AEEGETPVTBL(pMe,IGLES10)->PolygonOffsetx(pMe, factor, units);
}

static __inline int IGLES10_PopMatrix(IGLES10 *pMe)
{
   return AEEGETPVTBL(pMe,IGLES10)->PopMatrix(pMe);
}

static __inline int IGLES10_PushMatrix(IGLES10 *pMe)
{
   return AEEGETPVTBL(pMe,IGLES10)->PushMatrix(pMe);
}

static __inline int IGLES10_ReadPixels(IGLES10 *pMe, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height, AEEGLenum format, AEEGLenum type, AEEGLvoid *pixels)
{
   return AEEGETPVTBL(pMe,IGLES10)->ReadPixels(pMe, x, y, width, height, format, type, pixels);
}

static __inline int IGLES10_Rotatex(IGLES10 *pMe, AEEGLfixed angle, AEEGLfixed x, AEEGLfixed y, AEEGLfixed z)
{
   return AEEGETPVTBL(pMe,IGLES10)->Rotatex(pMe, angle, x, y, z);
}

static __inline int IGLES10_SampleCoverage(IGLES10 *pMe, AEEGLclampf value, AEEGLboolean invert)
{
   return AEEGETPVTBL(pMe,IGLES10)->SampleCoverage(pMe, value, invert);
}

static __inline int IGLES10_SampleCoveragex(IGLES10 *pMe, AEEGLclampx value, AEEGLboolean invert)
{
   return AEEGETPVTBL(pMe,IGLES10)->SampleCoveragex(pMe, value, invert);
}

static __inline int IGLES10_Scalex(IGLES10 *pMe, AEEGLfixed x, AEEGLfixed y, AEEGLfixed z)
{
   return AEEGETPVTBL(pMe,IGLES10)->Scalex(pMe, x, y, z);
}

static __inline int IGLES10_Scissor(IGLES10 *pMe, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height)
{
   return AEEGETPVTBL(pMe,IGLES10)->Scissor(pMe, x, y, width, height);
}

static __inline int IGLES10_ShadeModel(IGLES10 *pMe, AEEGLenum mode)
{
   return AEEGETPVTBL(pMe,IGLES10)->ShadeModel(pMe, mode);
}

static __inline int IGLES10_StencilFunc(IGLES10 *pMe, AEEGLenum func, AEEGLint ref, AEEGLuint mask)
{
   return AEEGETPVTBL(pMe,IGLES10)->StencilFunc(pMe, func, ref, mask);
}

static __inline int IGLES10_StencilMask(IGLES10 *pMe, AEEGLuint mask)
{
   return AEEGETPVTBL(pMe,IGLES10)->StencilMask(pMe, mask);
}

static __inline int IGLES10_StencilOp(IGLES10 *pMe, AEEGLenum fail, AEEGLenum zfail, AEEGLenum zpass)
{
   return AEEGETPVTBL(pMe,IGLES10)->StencilOp(pMe, fail, zfail, zpass);
}

static __inline int IGLES10_TexCoordPointer(IGLES10 *pMe, AEEGLint size, AEEGLenum type, AEEGLsizei stride, const AEEGLvoid *pointer)
{
   return AEEGETPVTBL(pMe,IGLES10)->TexCoordPointer(pMe, size, type, stride, pointer);
}

static __inline int IGLES10_TexEnvx(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfixed param)
{
   return AEEGETPVTBL(pMe,IGLES10)->TexEnvx(pMe, target, pname, param);
}

static __inline int IGLES10_TexEnvxv(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, const AEEGLfixed *params)
{
   return AEEGETPVTBL(pMe,IGLES10)->TexEnvxv(pMe, target, pname,params);
}

static __inline int IGLES10_TexImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLint internalformat, AEEGLsizei width, AEEGLsizei height, AEEGLint border, AEEGLenum format, AEEGLenum type, const AEEGLvoid *pixels)
{
   return AEEGETPVTBL(pMe,IGLES10)->TexImage2D(pMe, target, level, internalformat, width, height, border, format, type, pixels);
}

static __inline int IGLES10_TexParameterx(IGLES10 *pMe, AEEGLenum target, AEEGLenum pname, AEEGLfixed param)
{
   return AEEGETPVTBL(pMe,IGLES10)->TexParameterx(pMe, target, pname, param);
}

static __inline int IGLES10_TexSubImage2D(IGLES10 *pMe, AEEGLenum target, AEEGLint level, AEEGLint xoffset, AEEGLint yoffset, AEEGLsizei width, AEEGLsizei height, AEEGLenum format, AEEGLenum type, const AEEGLvoid *pixels)
{
   return AEEGETPVTBL(pMe,IGLES10)->TexSubImage2D(pMe, target, level, xoffset, yoffset, width, height, format, type, pixels);
}

static __inline int IGLES10_Translatex(IGLES10 *pMe, AEEGLfixed x, AEEGLfixed y, AEEGLfixed z)
{
   return AEEGETPVTBL(pMe,IGLES10)->Translatex(pMe, x, y, z);
}

static __inline int IGLES10_VertexPointer(IGLES10 *pMe, AEEGLint size, AEEGLenum type, AEEGLsizei stride, const AEEGLvoid *pointer)
{
   return AEEGETPVTBL(pMe,IGLES10)->VertexPointer(pMe, size, type, stride, pointer);
}

static __inline int IGLES10_Viewport(IGLES10 *pMe, AEEGLint x, AEEGLint y, AEEGLsizei width, AEEGLsizei height)
{
   return AEEGETPVTBL(pMe,IGLES10)->Viewport(pMe, x, y, width, height);
}


#endif /* #ifndef AEEGLES10_H */
