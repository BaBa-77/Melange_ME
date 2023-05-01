#ifndef  AEEGL_H
#define  AEEGL_H
/*=================================================================================
FILE:       AEEGL.h

SERVICES:   OpenGL-ES and EGL graphics interfaces.
  
DESCRIPTION: This file should be included in all BREW OpenGL-ES 1.0 applications. Both
             the BREW interface for IGL and IEGL are defined in this file. 
			 Note: ALL standard OpenGL-ES 1.0 and EGL APIs are available, this file
			 is provide as a standard Brew header file. Please refer to the 
			 OpenGL-ES 1.0 BREW sample application as a reference on how to use the
			 standard OpenGL-ES and EGL APIs.
                          

        Copyright (c) 2003,2004,2005 QUALCOMM Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
=================================================================================*/

/*-------------------------------------------------------------------------------*
 *                      I N C L U D E   F I L E S                                *
 *-------------------------------------------------------------------------------*/
#include <AEE.h>

#include <GLES/gl.h>
#include <GLES/egl.h>


/*-------------------------------------------------------------------------------*
 *                            M A C R O S                                        *
 *-------------------------------------------------------------------------------*/

#define AEECLSID_GL    	   0x01014bc3   //| OpenGLES 1.0 Common-Lite spec

#define AEECLSID_EGL    0x01014bc4	//| EGL 1.0 spec 


/*-------------------------------------------------------------------------------*
 *                      E N U M   T Y P E S                                      *
 *-------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------*
 *                      S T R U C T   T Y P E S                                  *
 *-------------------------------------------------------------------------------*/
typedef struct _OPENGLES  IGL;
typedef struct _EGL       IEGL;



/*===============================================================================*
 *                                                                               *
 *                OpenGLES I N T E R F A C E                                     *
 *                                                                               *
 *===============================================================================*/
AEEINTERFACE (IGL)
{
	INHERIT_IQueryInterface (IGL);
	
	void  (*glActiveTexture) (GLenum texture);
	void  (*glAlphaFuncx) (GLenum func, GLclampx ref);
	void  (*glBindTexture) (GLenum target, GLuint texture);
	void  (*glBlendFunc) (GLenum sfactor, GLenum dfactor);
	void  (*glClear) (GLbitfield mask);
	void  (*glClearColorx) (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
	void  (*glClearDepthx) (GLclampx depth);
	void  (*glClearStencil) (GLint s);
	void  (*glClientActiveTexture) (GLenum texture);
	void  (*glColor4x) (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
	void  (*glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	void  (*glColorPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void  (*glCompressedTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
	void  (*glCompressedTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
	void  (*glCopyTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
	void  (*glCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	void  (*glCullFace) (GLenum mode);
	void  (*glDeleteTextures) (GLsizei n, const GLuint *textures);
	void  (*glDepthFunc) (GLenum func);
	void  (*glDepthMask) (GLboolean flag);
	void  (*glDepthRangex) (GLclampx zNear, GLclampx zFar);
	void  (*glDisable) (GLenum cap);
	void  (*glDisableClientState) (GLenum array);
	void  (*glDrawArrays) (GLenum mode, GLint first, GLsizei count);
	void  (*glDrawElements) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
	void  (*glEnable) (GLenum cap);
	void  (*glEnableClientState) (GLenum array);
	void  (*glFinish) (void);
	void  (*glFlush) (void);
	void  (*glFogx) (GLenum pname, GLfixed param);
	void  (*glFogxv) (GLenum pname, const GLfixed *params);
	void  (*glFrontFace) (GLenum mode);
	void  (*glFrustumx) (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
	void  (*glGenTextures) (GLsizei n, GLuint *textures);
	GLenum  (*glGetError) ();
	void  (*glGetIntegerv) (GLenum pname, GLint *params);
	const GLubyte *  (*glGetString) (GLenum name);
	void  (*glHint) (GLenum target, GLenum mode);
	void  (*glLightModelx) (GLenum pname, GLfixed param);
	void  (*glLightModelxv) (GLenum pname, const GLfixed *params);
	void  (*glLightx) (GLenum light, GLenum pname, GLfixed param);
	void  (*glLightxv) (GLenum light, GLenum pname, const GLfixed *params);
	void  (*glLineWidthx) (GLfixed width);
	void  (*glLoadIdentity) (void);
	void  (*glLoadMatrixx) (const GLfixed *m);
	void  (*glLogicOp) (GLenum opcode);
	void  (*glMaterialx) (GLenum face, GLenum pname, GLfixed param);
	void  (*glMaterialxv) (GLenum face, GLenum pname, const GLfixed *params);
	void  (*glMatrixMode) (GLenum mode);
	void  (*glMultMatrixx) (const GLfixed *m);	
	void  (*glMultiTexCoord4x) (GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);	
	void  (*glNormal3x) (GLfixed nx, GLfixed ny, GLfixed nz);
	void  (*glNormalPointer) (GLenum type, GLsizei stride, const GLvoid *pointer);	
	void  (*glOrthox) (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
	void  (*glPixelStorei) (GLenum pname, GLint param);	
	void  (*glPointSizex) (GLfixed size);	
	void  (*glPolygonOffsetx) (GLfixed factor, GLfixed units);
	void  (*glPopMatrix) (void);
	void  (*glPushMatrix) (void);
	void  (*glReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);	
	void  (*glRotatex) (GLfixed angle, GLfixed x, GLfixed y, GLfixed z);	
	void  (*glSampleCoveragex) (GLclampx value, GLboolean invert);	
	void  (*glScalex) (GLfixed x, GLfixed y, GLfixed z);
	void  (*glScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
	void  (*glShadeModel) (GLenum mode);
	void  (*glStencilFunc) (GLenum func, GLint ref, GLuint mask);
	void  (*glStencilMask) (GLuint mask);
	void  (*glStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
	void  (*glTexCoordPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);	
	void  (*glTexEnvx) (GLenum target, GLenum pname, GLfixed param);
	void  (*glTexEnvxv) (GLenum target, GLenum pname, const GLfixed *params);
	void  (*glTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);	
	void  (*glTexParameterx) (GLenum target, GLenum pname, GLfixed param);
	void  (*glTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
	void  (*glTranslatex) (GLfixed x, GLfixed y, GLfixed z);
	void  (*glVertexPointer) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	void  (*glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
};

/*-------------------------------------------------------------------------------*
 *                      A C C E S S   M A C R O S                                *
 *-------------------------------------------------------------------------------*/
#define  IGL_AddRef(p)                   AEEGETPVTBL(p,IGL)->AddRef(p)
#define  IGL_Release(p)                  AEEGETPVTBL(p,IGL)->Release(p)
#define  IGL_QueryInterface(p,a,b)       AEEGETPVTBL(p,IGL)->QueryInterface(p,a,b)

#define  IGL_glActiveTexture(p,a)							AEEGETPVTBL(p,IGL)->glActiveTexture(a)
#define  IGL_glAlphaFuncx(p,a,b)							AEEGETPVTBL(p,IGL)->glAlphaFuncx(a,b)
#define  IGL_glBindTexture(p,a,b)							AEEGETPVTBL(p,IGL)->glBindTexture(a,b)
#define  IGL_glBlendFunc(p,a,b)								AEEGETPVTBL(p,IGL)->glBlendFunc(a,b)
#define  IGL_glClear(p,a)									AEEGETPVTBL(p,IGL)->glClear(a)
#define  IGL_glClearColorx(p,a,b,c,d)						AEEGETPVTBL(p,IGL)->glClearColorx(a,b,c,d)
#define  IGL_glClearDepthx(p,a)								AEEGETPVTBL(p,IGL)->glClearDepthx(a)
#define  IGL_glClearStencil(p,a)							AEEGETPVTBL(p,IGL)->glClearStencil(a)
#define  IGL_glClientActiveTexture(p,a)						AEEGETPVTBL(p,IGL)->glClientActiveTexture(a)
#define  IGL_glColor4x(p,a,b,c,d)							AEEGETPVTBL(p,IGL)->glColor4x(a,b,c,d)
#define  IGL_glColorMask(p,a,b,c,d)							AEEGETPVTBL(p,IGL)->glColorMask(a,b,c,d)
#define  IGL_glColorPointer(p, a,b,c,d)						AEEGETPVTBL(p,IGL)->glColorPointer(a,b,c,d)
#define  IGL_glCompressedTexImage2D(p,a,b,c,d,e,f,g,h)		AEEGETPVTBL(p,IGL)->glCompressedTexImage2D(a,b,c,d,e,f,g,h)
#define  IGL_glCompressedTexSubImage2D(p,a,b,c,d,e,f,g,h,i)	AEEGETPVTBL(p,IGL)->glCompressedTexSubImage2D(a,b,c,d,e,f,g,h,i)
#define  IGL_glCopyTexImage2D(p,a,b,c,d,e,f,g,h)			AEEGETPVTBL(p,IGL)->glCopyTexImage2D(a,b,c,d,e,f,g,h)
#define  IGL_glCopyTexSubImage2D(p,a,b,c,d,e,f,g,h)			AEEGETPVTBL(p,IGL)->glCopyTexSubImage2D(a,b,c,d,e,f,g,h)
#define  IGL_glCullFace(p,a)								AEEGETPVTBL(p,IGL)->glCullFace(a)
#define  IGL_glDeleteTextures(p,a,b)						AEEGETPVTBL(p,IGL)->glDeleteTextures(a,b)
#define  IGL_glDepthFunc(p,a)								AEEGETPVTBL(p,IGL)->glDepthFunc(a)
#define  IGL_glDepthMask(p,a)								AEEGETPVTBL(p,IGL)->glDepthMask(a)
#define  IGL_glDepthRangex(p,a,b)							AEEGETPVTBL(p,IGL)->glDepthRangex(a,b)
#define  IGL_glDisable(p,a)									AEEGETPVTBL(p,IGL)->glDisable(a)
#define  IGL_glDisableClientState(p,a)						AEEGETPVTBL(p,IGL)->glDisableClientState(a)
#define  IGL_glDrawArrays(p,a,b,c)							AEEGETPVTBL(p,IGL)->glDrawArrays(a,b,c)
#define  IGL_glDrawElements(p,a,b,c,d)						AEEGETPVTBL(p,IGL)->glDrawElements(a,b,c,d)
#define  IGL_glEnable(p,a)									AEEGETPVTBL(p,IGL)->glEnable(a)
#define  IGL_glEnableClientState(p,a)						AEEGETPVTBL(p,IGL)->glEnableClientState(a)
#define  IGL_glFinish(p)									AEEGETPVTBL(p,IGL)->glFinish()
#define  IGL_glFlush(p)										AEEGETPVTBL(p,IGL)->glFlush()	
#define  IGL_glFogx(p,a,b)									AEEGETPVTBL(p,IGL)->glFogx(a,b)
#define  IGL_glFogxv(p,a,b)									AEEGETPVTBL(p,IGL)->glFogxv(a,b)
#define  IGL_glFrontFace(p,a)								AEEGETPVTBL(p,IGL)->glFrontFace(a)
#define  IGL_glFrustumx(p,a,b,c,d,e,f)						AEEGETPVTBL(p,IGL)->glFrustumx(a,b,c,d,e,f)
#define  IGL_glGenTextures(p,a,b)							AEEGETPVTBL(p,IGL)->glGenTextures(a,b)
#define  IGL_glGetError(p)									AEEGETPVTBL(p,IGL)->glGetError()	
#define  IGL_glGetIntegerv(p,a,b)							AEEGETPVTBL(p,IGL)->glGetIntegerv(a,b)	
#define  IGL_glGetString(p,a)								AEEGETPVTBL(p,IGL)->glGetString(a)
#define  IGL_glHint(p,a,b)									AEEGETPVTBL(p,IGL)->glHint(a,b)
#define  IGL_glLightModelx(p,a,b)							AEEGETPVTBL(p,IGL)->glLightModelx(a,b)
#define  IGL_glLightModelxv(p,a,b)							AEEGETPVTBL(p,IGL)->glLightModelxv(a,b)
#define  IGL_glLightx(p,a,b,c)								AEEGETPVTBL(p,IGL)->glLightx(a,b,c)
#define  IGL_glLightxv(p,a,b,c)								AEEGETPVTBL(p,IGL)->glLightxv(a,b,c)	
#define  IGL_glLineWidthx(p,a)								AEEGETPVTBL(p,IGL)->glLineWidthx(a)
#define  IGL_glLoadIdentity(p)								AEEGETPVTBL(p,IGL)->glLoadIdentity()
#define  IGL_glLoadMatrixx(p,a)								AEEGETPVTBL(p,IGL)->glLoadMatrixx(a)
#define  IGL_glLogicOp(p,a)									AEEGETPVTBL(p,IGL)->glLogicOp(a)
#define  IGL_glMaterialx(p,a,b,c)							AEEGETPVTBL(p,IGL)->glMaterialx(a,b,c)
#define  IGL_glMaterialxv(p,a,b,c)							AEEGETPVTBL(p,IGL)->glMaterialxv(a,b,c)	
#define  IGL_glMatrixMode(p,a)								AEEGETPVTBL(p,IGL)->glMatrixMode(a)
#define  IGL_glMultMatrixx(p,a)								AEEGETPVTBL(p,IGL)->glMultMatrixx(a)
#define  IGL_glMultiTexCoord4x(p,a,b,c,d,e)					AEEGETPVTBL(p,IGL)->glMultiTexCoord4x(a,b,c,d,e)
#define  IGL_glNormal3x(p,a,b,c)							AEEGETPVTBL(p,IGL)->glNormal3x(a,b,c)
#define  IGL_glNormalPointer(p,a,b,c)						AEEGETPVTBL(p,IGL)->glNormalPointer(a,b,c)	
#define  IGL_glOrthox(p,a,b,c,d,e,f)						AEEGETPVTBL(p,IGL)->glOrthox(a,b,c,d,e,f)
#define  IGL_glPixelStorei(p,a,b)							AEEGETPVTBL(p,IGL)->glPixelStorei(a,b)
#define  IGL_glPointSizex(p,a)								AEEGETPVTBL(p,IGL)->glPointSizex(a)	
#define  IGL_glPolygonOffsetx(p,a,b)						AEEGETPVTBL(p,IGL)->glPolygonOffsetx(a,b)
#define  IGL_glPopMatrix(p)									AEEGETPVTBL(p,IGL)->glPopMatrix()
#define  IGL_glPushMatrix(p)								AEEGETPVTBL(p,IGL)->glPushMatrix()
#define  IGL_glReadPixels(p,a,b,c,d,e,f,g)					AEEGETPVTBL(p,IGL)->glReadPixels(a,b,c,d,e,f,g)	
#define  IGL_glRotatex(p,a,b,c,d) 							AEEGETPVTBL(p,IGL)->glRotatex(a,b,c,d) 	
#define  IGL_glSampleCoveragex(p,a,b)						AEEGETPVTBL(p,IGL)->glSampleCoveragex(a,b)
#define  IGL_glScalex(p,a,b,c)								AEEGETPVTBL(p,IGL)->glScalex(a,b,c)
#define  IGL_glScissor(p,a,b,c,d)							AEEGETPVTBL(p,IGL)->glScissor(a,b,c,d)	
#define  IGL_glShadeModel(p,a)								AEEGETPVTBL(p,IGL)->glShadeModel(a)
#define  IGL_glStencilFunc(p,a,b,c)							AEEGETPVTBL(p,IGL)->glStencilFunc(a,b,c)		
#define  IGL_glStencilMask(p,a)								AEEGETPVTBL(p,IGL)->glStencilMask(a)
#define  IGL_glStencilOp(p,a,b,c)							AEEGETPVTBL(p,IGL)->glStencilOp(a,b,c)
#define  IGL_glTexCoordPointer(p,a,b,c,d)					AEEGETPVTBL(p,IGL)->glTexCoordPointer(a,b,c,d)
#define  IGL_glTexEnvx(p,a,b,c)								AEEGETPVTBL(p,IGL)->glTexEnvx(a,b,c)	
#define  IGL_glTexEnvxv(p,a,b,c)							AEEGETPVTBL(p,IGL)->glTexEnvxv(a,b,c)	
#define  IGL_glTexImage2D(p,a,b,c,d,e,f,g,h,i)				AEEGETPVTBL(p,IGL)->glTexImage2D(a,b,c,d,e,f,g,h,i)	
#define  IGL_glTexParameterx(p,a,b,c) 						AEEGETPVTBL(p,IGL)->glTexParameterx(a,b,c)
#define  IGL_glTexSubImage2D(p,a,b,c,d,e,f,g,h,i)			AEEGETPVTBL(p,IGL)->glTexSubImage2D(a,b,c,d,e,f,g,h,i)
#define  IGL_glTranslatex(p,a,b,c)							AEEGETPVTBL(p,IGL)->glTranslatex(a,b,c)
#define  IGL_glVertexPointer(p,a,b,c,d)						AEEGETPVTBL(p,IGL)->glVertexPointer(a,b,c,d)	
#define  IGL_glViewport(p,a,b,c,d)	                        AEEGETPVTBL(p,IGL)->glViewport(a,b,c,d)	


/*===============================================================================*
 *                                                                               *
 *                EGL I N T E R F A C E                                         *
 *                                                                               *
 *===============================================================================*/
AEEINTERFACE(IEGL)
{
	
	INHERIT_IQueryInterface(IEGL);

	  EGLint   (*eglGetError) (void);
  
	  EGLDisplay   (*eglGetDisplay) (NativeDisplayType display);
	  EGLBoolean   (*eglInitialize) (EGLDisplay dpy, EGLint *major, EGLint *minor);
	  EGLBoolean   (*eglTerminate) (EGLDisplay dpy);
	  const char *   (*eglQueryString) (EGLDisplay dpy, EGLint name);
	  void (*   (*eglGetProcAddress) (const char *procname))();
	  
	  EGLBoolean   (*eglGetConfigs) (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config);
	  EGLBoolean   (*eglChooseConfig) (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
	  EGLBoolean   (*eglGetConfigAttrib) (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value);
	  
	  EGLSurface   (*eglCreateWindowSurface) (EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list);
	  EGLSurface   (*eglCreatePixmapSurface) (EGLDisplay dpy, EGLConfig config, NativePixmapType pixmap, const EGLint *attrib_list);
	  EGLSurface   (*eglCreatePbufferSurface) (EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
	  EGLBoolean   (*eglDestroySurface) (EGLDisplay dpy, EGLSurface surface);
	  EGLBoolean   (*eglQuerySurface) (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
	  
	  EGLContext   (*eglCreateContext) (EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list);
	  EGLBoolean   (*eglDestroyContext) (EGLDisplay dpy, EGLContext ctx);
	  EGLBoolean   (*eglMakeCurrent) (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
	  EGLContext   (*eglGetCurrentContext) (void);
	  EGLSurface   (*eglGetCurrentSurface) (EGLint readdraw);
	  EGLDisplay   (*eglGetCurrentDisplay) (void);
	  EGLBoolean   (*eglQueryContext) (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value);
	  
	  EGLBoolean   (*eglWaitGL) (void);
	  EGLBoolean   (*eglWaitNative) (EGLint engine);
	  EGLBoolean   (*eglSwapBuffers) (EGLDisplay dpy, EGLSurface draw);
	  EGLBoolean   (*eglCopyBuffers) (EGLDisplay dpy, EGLSurface surface, NativePixmapType target);	  

};

/*-------------------------------------------------------------------------------*
 *                      A C C E S S   M A C R O S                                *
 *-------------------------------------------------------------------------------*/
#define  IEGL_AddRef(p)                   AEEGETPVTBL(p,IEGL)->AddRef(p)
#define  IEGL_Release(p)                  AEEGETPVTBL(p,IEGL)->Release(p)
#define  IEGL_QueryInterface(p,a,b)       AEEGETPVTBL(p,IEGL)->QueryInterface(p,a,b)


#define  IEGL_eglGetError(p)                    AEEGETPVTBL(p,IEGL)->eglGetError()   
#define  IEGL_eglGetDisplay(p,a)                AEEGETPVTBL(p,IEGL)->eglGetDisplay(a)  
#define  IEGL_eglInitialize(p,a,b,c)            AEEGETPVTBL(p,IEGL)->eglInitialize(a,b,c)  
#define  IEGL_eglTerminate(p,a)                 AEEGETPVTBL(p,IEGL)->eglTerminate(a)  
#define  IEGL_eglQueryString(p,a,b)             AEEGETPVTBL(p,IEGL)->eglQueryString(a,b)  
#define  IEGL_eglGetProcAddress(p,a)            AEEGETPVTBL(p,IEGL)->eglGetProcAddress(a)
#define  IEGL_eglGetConfigs(p,a,b,c,d)          AEEGETPVTBL(p,IEGL)->eglGetConfigs(a,b,c,d)  
#define  IEGL_eglChooseConfig(p,a,b,c,d,e)      AEEGETPVTBL(p,IEGL)->eglChooseConfig(a,b,c,d,e)  
#define  IEGL_eglGetConfigAttrib(p,a,b,c,d)     AEEGETPVTBL(p,IEGL)->eglGetConfigAttrib(a,b,c,d)  	  
#define  IEGL_eglCreateWindowSurface(p,a,b,c,d) AEEGETPVTBL(p,IEGL)->eglCreateWindowSurface(a,b,c,d) 
#define  IEGL_eglCreatePixmapSurface(p,a,b,c,d) AEEGETPVTBL(p,IEGL)->eglCreatePixmapSurface(a,b,c,d) 
#define  IEGL_eglCreatePbufferSurface(p,a,b,c)  AEEGETPVTBL(p,IEGL)->eglCreatePbufferSurface(a,b,c)  
#define  IEGL_eglDestroySurface(p,a,b)          AEEGETPVTBL(p,IEGL)->eglDestroySurface(a,b)          
#define  IEGL_eglQuerySurface(p,a,b,c,d)        AEEGETPVTBL(p,IEGL)->eglQuerySurface(a,b,c,d)       	  
#define  IEGL_eglCreateContext(p,a,b,c,d)       AEEGETPVTBL(p,IEGL)->eglCreateContext(a,b,c,d)   
#define  IEGL_eglDestroyContext(p,a,b)          AEEGETPVTBL(p,IEGL)->eglDestroyContext(a,b)      
#define  IEGL_eglMakeCurrent(p,a,b,c,d)         AEEGETPVTBL(p,IEGL)->eglMakeCurrent(a,b,c,d)     
#define  IEGL_eglGetCurrentContext(p)           AEEGETPVTBL(p,IEGL)->eglGetCurrentContext()       
#define  IEGL_eglGetCurrentSurface(p,a)         AEEGETPVTBL(p,IEGL)->eglGetCurrentSurface(a)     
#define  IEGL_eglGetCurrentDisplay(p)           AEEGETPVTBL(p,IEGL)->eglGetCurrentDisplay()       
#define  IEGL_eglQueryContext(p,a,b,c,d)        AEEGETPVTBL(p,IEGL)->eglQueryContext(a,b,c,d)    	  
#define  IEGL_eglWaitGL(p)                      AEEGETPVTBL(p,IEGL)->eglWaitGL() 
#define  IEGL_eglWaitNative(p,a)                AEEGETPVTBL(p,IEGL)->eglWaitNative(a)  
#define  IEGL_eglSwapBuffers(p,a,b)             AEEGETPVTBL(p,IEGL)->eglSwapBuffers(a,b) 
#define  IEGL_eglCopyBuffers(p,a,b,c)           AEEGETPVTBL(p,IEGL)->eglCopyBuffers(a,b,c) 


#endif	// AEEGL_H

/*=====================================================================
  DATA STRUCTURE DOCUMENTATION
=======================================================================

GLenum

Description:
    GLenum is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLboolean

Description:
    GLboolean is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLbitfield

Description:
    GLbitfield is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLbyte

Description:
    GLbyte is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLshort

Description:
    GLshort is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLint

Description:
    GLint is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLsizei

Description:
    GLsizei is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLubyte

Description:
    GLubyte is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLushort

Description:
    GLushort is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLuint

Description:
    GLuint is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLfloat

Description:
    GLfloat is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLclampf

Description:
    GLclampf is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLvoid

Description:
    GLvoid is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLintptrARB

Description:
    GLintptrARB is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLsizeiptrARB

Description:
    GLsizeiptrARB is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLfixed

Description:
    GLfixed is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
GLclampx

Description:
    GLclampx is an OpenGLES defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See gl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
NativeDisplayType

Description:
    NativeDisplayType is the native display type for EGL on BREW.  This
    is defined to be an IDisplay pointer.

Definition:
    typedef IDisplay* NativeDisplayType;

See Also:
    IDisplay~
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
NativeWindowType

Description:
    NativeWindowType is the native window type for EGL on BREW.  This
    is defined to be an IDIB pointer.

Definition:
    typedef IDIB* NativeWindowType;

See Also:
    IDIB~
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
NativePixmapType

Description:
    NativePixmapType is the native pixmap type for EGL on BREW.  This
    is defined to be an IDIB pointer.

Definition:
    typedef IDIB* NativePixmapType;

See Also:
    IDIB~
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
EGLBoolean

Description:
    EGLBoolean is an EGL defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See egl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
EGLint

Description:
    EGLint is an EGL defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See egl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
EGLDisplay

Description:
    EGLDisplay is an EGL defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See egl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
EGLConfig

Description:
    EGLConfig is an EGL defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See egl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
EGLSurface

Description:
    EGLSurface is an EGL defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See egl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
EGLContext

Description:
    EGLContext is an EGL defined type.  See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
    for details.

Definition:
    See egl.h on the Khronos website.

See Also:
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
=======================================================================
  INTERFACES   DOCUMENTATION
=======================================================================

IGL Interface

Description:

   The IGL interface provides access to the OpenGL ES implementation on BREW.
   IGL wraps the standard OpenGL ES functions with their IGL BREW equivalents
   as well as provides the standard interface functions.

   For OpenGL ES documentation, see the OpenGL ES documentation available
   on the Khronos website at 
===a href="http://www.khronos.org">www.khronos.org.
===/a>

   The following header files are required for this interface:~
   AEE.h~
   GLES/gl.h~
   GLES/egl.h


========================================================================
IGL_AddRef()
This function is inherited from IBASE_AddRef().
========================================================================

========================================================================
IGL_Release()
This function is inherited from IBASE_Release().
========================================================================

========================================================================
IGL_QueryInterface()

Description: 
   This function queries the IGL interface
   
Prototype:
   int IGL_QueryInterface( IGL *pMe, AEECLSID clsid, void **ppNew )

Parameters:
   pMe : Pointer to IGL instance
   clsid : Class id
   ppNew : Address of pointer to be set
                     
Return Value: 
   SUCCESS, on success~
   EUNSUPPORTED, otherwise

========================================================================

==========================================================================
IGL_glActiveTexture()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glActiveTexture.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glActiveTexture( IGL *pIGL, GLenum texture );

Parameters:
	pIGL : A pointer to the IGL interface
	texture : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glAlphaFuncx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glAlphaFuncx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glAlphaFuncx( IGL *pIGL, GLenum func, GLclampx ref );

Parameters:
	pIGL : A pointer to the IGL interface
	func : See Khronos documentation for details
	ref : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glBindTexture()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glBindTexture.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glBindTexture( IGL *pIGL, GLenum target, GLuint texture );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	texture : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glBlendFunc()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glBlendFunc.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glBlendFunc( IGL *pIGL, GLenum sfactor, GLenum dfactor );

Parameters:
	pIGL : A pointer to the IGL interface
	sfactor : See Khronos documentation for details
	dfactor : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glClear()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glClear.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glClear( IGL *pIGL, GLbitfield mask );

Parameters:
	pIGL : A pointer to the IGL interface
	mask : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glClearColorx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glClearColorx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glClearColorx( IGL *pIGL, GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha );

Parameters:
	pIGL : A pointer to the IGL interface
	red : See Khronos documentation for details
	green : See Khronos documentation for details
	blue : See Khronos documentation for details
	alpha : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glClearDepthx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glClearDepthx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glClearDepthx( IGL *pIGL, GLclampx depth );

Parameters:
	pIGL : A pointer to the IGL interface
	depth : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glClearStencil()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glClearStencil.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glClearStencil( IGL *pIGL, GLint s );

Parameters:
	pIGL : A pointer to the IGL interface
	s : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glClientActiveTexture()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glClientActiveTexture.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glClientActiveTexture( IGL *pIGL, GLenum texture );

Parameters:
	pIGL : A pointer to the IGL interface
	texture : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glColor4x()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glColor4x.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glColor4x( IGL *pIGL, GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha );

Parameters:
	pIGL : A pointer to the IGL interface
	red : See Khronos documentation for details
	green : See Khronos documentation for details
	blue : See Khronos documentation for details
	alpha : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glColorMask()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glColorMask.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glColorMask( IGL *pIGL, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );

Parameters:
	pIGL : A pointer to the IGL interface
	red : See Khronos documentation for details
	green : See Khronos documentation for details
	blue : See Khronos documentation for details
	alpha : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glColorPointer()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glColorPointer.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glColorPointer( IGL *pIGL, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );

Parameters:
	pIGL : A pointer to the IGL interface
	size : See Khronos documentation for details
	type : See Khronos documentation for details
	stride : See Khronos documentation for details
	pointer : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glCompressedTexImage2D()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glCompressedTexImage2D.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glCompressedTexImage2D( IGL *pIGL, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	level : See Khronos documentation for details
	internalformat : See Khronos documentation for details
	width : See Khronos documentation for details
	height : See Khronos documentation for details
	border : See Khronos documentation for details
	imageSize : See Khronos documentation for details
	data : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glCompressedTexSubImage2D()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glCompressedTexSubImage2D.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glCompressedTexSubImage2D( IGL *pIGL, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	level : See Khronos documentation for details
	xoffset : See Khronos documentation for details
	yoffset : See Khronos documentation for details
	width : See Khronos documentation for details
	height : See Khronos documentation for details
	format : See Khronos documentation for details
	imageSize : See Khronos documentation for details
	data : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glCopyTexImage2D()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glCopyTexImage2D.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glCopyTexImage2D( IGL *pIGL, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	level : See Khronos documentation for details
	internalformat : See Khronos documentation for details
	x : See Khronos documentation for details
	y : See Khronos documentation for details
	width : See Khronos documentation for details
	height : See Khronos documentation for details
	border : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glCopyTexSubImage2D()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glCopyTexSubImage2D.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glCopyTexSubImage2D( IGL *pIGL, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	level : See Khronos documentation for details
	xoffset : See Khronos documentation for details
	yoffset : See Khronos documentation for details
	x : See Khronos documentation for details
	y : See Khronos documentation for details
	width : See Khronos documentation for details
	height : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glCullFace()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glCullFace.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glCullFace( IGL *pIGL, GLenum mode );

Parameters:
	pIGL : A pointer to the IGL interface
	mode : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glDeleteTextures()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glDeleteTextures.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glDeleteTextures( IGL *pIGL, GLsizei n, const GLuint *textures );

Parameters:
	pIGL : A pointer to the IGL interface
	n : See Khronos documentation for details
	textures : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glDepthFunc()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glDepthFunc.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glDepthFunc( IGL *pIGL, GLenum func );

Parameters:
	pIGL : A pointer to the IGL interface
	func : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glDepthMask()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glDepthMask.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glDepthMask( IGL *pIGL, GLboolean flag );

Parameters:
	pIGL : A pointer to the IGL interface
	flag : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glDepthRangex()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glDepthRangex.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glDepthRangex( IGL *pIGL, GLclampx zNear, GLclampx zFar );

Parameters:
	pIGL : A pointer to the IGL interface
	zNear : See Khronos documentation for details
	zFar : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glDisable()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glDisable.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glDisable( IGL *pIGL, GLenum cap );

Parameters:
	pIGL : A pointer to the IGL interface
	cap : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glDisableClientState()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glDisableClientState.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glDisableClientState( IGL *pIGL, GLenum array );

Parameters:
	pIGL : A pointer to the IGL interface
	array : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glDrawArrays()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glDrawArrays.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glDrawArrays( IGL *pIGL, GLenum mode, GLint first, GLsizei count );

Parameters:
	pIGL : A pointer to the IGL interface
	mode : See Khronos documentation for details
	first : See Khronos documentation for details
	count : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glDrawElements()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glDrawElements.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glDrawElements( IGL *pIGL, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices );

Parameters:
	pIGL : A pointer to the IGL interface
	mode : See Khronos documentation for details
	count : See Khronos documentation for details
	type : See Khronos documentation for details
	indices : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glEnable()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glEnable.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glEnable( IGL *pIGL, GLenum cap );

Parameters:
	pIGL : A pointer to the IGL interface
	cap : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glEnableClientState()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glEnableClientState.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glEnableClientState( IGL *pIGL, GLenum array );

Parameters:
	pIGL : A pointer to the IGL interface
	array : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glFinish()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glFinish.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glFinish( IGL *pIGL );

Parameters:
	pIGL : A pointer to the IGL interface

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glFlush()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glFlush.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glFlush( IGL *pIGL );

Parameters:
	pIGL : A pointer to the IGL interface

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glFogx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glFogx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glFogx( IGL *pIGL, GLenum pname, GLfixed param );

Parameters:
	pIGL : A pointer to the IGL interface
	pname : See Khronos documentation for details
	param : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glFogxv()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glFogxv.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glFogxv( IGL *pIGL, GLenum pname, const GLfixed *params );

Parameters:
	pIGL : A pointer to the IGL interface
	pname : See Khronos documentation for details
	params : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glFrontFace()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glFrontFace.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glFrontFace( IGL *pIGL, GLenum mode );

Parameters:
	pIGL : A pointer to the IGL interface
	mode : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glFrustumx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glFrustumx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glFrustumx( IGL *pIGL, GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar );

Parameters:
	pIGL : A pointer to the IGL interface
	left : See Khronos documentation for details
	right : See Khronos documentation for details
	bottom : See Khronos documentation for details
	top : See Khronos documentation for details
	zNear : See Khronos documentation for details
	zFar : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glGenTextures()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glGenTextures.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glGenTextures( IGL *pIGL, GLsizei n, GLuint *textures );

Parameters:
	pIGL : A pointer to the IGL interface
	n : See Khronos documentation for details
	textures : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glGetError()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glGetError.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
GLenum IGL_glGetError( IGL *pIGL );

Parameters:
	pIGL : A pointer to the IGL interface

Return Value:
	GLenum : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glGetIntegerv()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glGetIntegerv.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glGetIntegerv( IGL *pIGL, GLenum pname, GLint *params );

Parameters:
	pIGL : A pointer to the IGL interface
	pname : See Khronos documentation for details
	params : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glGetString()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glGetString.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
const GLubyte * IGL_glGetString( IGL *pIGL, GLenum name );

Parameters:
	pIGL : A pointer to the IGL interface
	name : See Khronos documentation for details

Return Value:
	const GLubyte * : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glHint()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glHint.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glHint( IGL *pIGL, GLenum target, GLenum mode );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	mode : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glLightModelx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glLightModelx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glLightModelx( IGL *pIGL, GLenum pname, GLfixed param );

Parameters:
	pIGL : A pointer to the IGL interface
	pname : See Khronos documentation for details
	param : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glLightModelxv()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glLightModelxv.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glLightModelxv( IGL *pIGL, GLenum pname, const GLfixed *params );

Parameters:
	pIGL : A pointer to the IGL interface
	pname : See Khronos documentation for details
	params : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glLightx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glLightx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glLightx( IGL *pIGL, GLenum light, GLenum pname, GLfixed param );

Parameters:
	pIGL : A pointer to the IGL interface
	light : See Khronos documentation for details
	pname : See Khronos documentation for details
	param : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glLightxv()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glLightxv.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glLightxv( IGL *pIGL, GLenum light, GLenum pname, const GLfixed *params );

Parameters:
	pIGL : A pointer to the IGL interface
	light : See Khronos documentation for details
	pname : See Khronos documentation for details
	params : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glLineWidthx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glLineWidthx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glLineWidthx( IGL *pIGL, GLfixed width );

Parameters:
	pIGL : A pointer to the IGL interface
	width : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glLoadIdentity()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glLoadIdentity.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glLoadIdentity( IGL *pIGL );

Parameters:
	pIGL : A pointer to the IGL interface

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glLoadMatrixx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glLoadMatrixx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glLoadMatrixx( IGL *pIGL, const GLfixed *m );

Parameters:
	pIGL : A pointer to the IGL interface
	m : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glLogicOp()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glLogicOp.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glLogicOp( IGL *pIGL, GLenum opcode );

Parameters:
	pIGL : A pointer to the IGL interface
	opcode : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glMaterialx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glMaterialx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glMaterialx( IGL *pIGL, GLenum face, GLenum pname, GLfixed param );

Parameters:
	pIGL : A pointer to the IGL interface
	face : See Khronos documentation for details
	pname : See Khronos documentation for details
	param : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glMaterialxv()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glMaterialxv.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glMaterialxv( IGL *pIGL, GLenum face, GLenum pname, const GLfixed *params );

Parameters:
	pIGL : A pointer to the IGL interface
	face : See Khronos documentation for details
	pname : See Khronos documentation for details
	params : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glMatrixMode()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glMatrixMode.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glMatrixMode( IGL *pIGL, GLenum mode );

Parameters:
	pIGL : A pointer to the IGL interface
	mode : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glMultMatrixx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glMultMatrixx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glMultMatrixx( IGL *pIGL, const GLfixed *m );

Parameters:
	pIGL : A pointer to the IGL interface
	m : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glMultiTexCoord4x()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glMultiTexCoord4x.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glMultiTexCoord4x( IGL *pIGL, GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	s : See Khronos documentation for details
	t : See Khronos documentation for details
	r : See Khronos documentation for details
	q : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glNormal3x()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glNormal3x.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glNormal3x( IGL *pIGL, GLfixed nx, GLfixed ny, GLfixed nz );

Parameters:
	pIGL : A pointer to the IGL interface
	nx : See Khronos documentation for details
	ny : See Khronos documentation for details
	nz : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glNormalPointer()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glNormalPointer.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glNormalPointer( IGL *pIGL, GLenum type, GLsizei stride, const GLvoid *pointer );

Parameters:
	pIGL : A pointer to the IGL interface
	type : See Khronos documentation for details
	stride : See Khronos documentation for details
	pointer : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glOrthox()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glOrthox.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glOrthox( IGL *pIGL, GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar );

Parameters:
	pIGL : A pointer to the IGL interface
	left : See Khronos documentation for details
	right : See Khronos documentation for details
	bottom : See Khronos documentation for details
	top : See Khronos documentation for details
	zNear : See Khronos documentation for details
	zFar : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glPixelStorei()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glPixelStorei.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glPixelStorei( IGL *pIGL, GLenum pname, GLint param );

Parameters:
	pIGL : A pointer to the IGL interface
	pname : See Khronos documentation for details
	param : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glPointSizex()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glPointSizex.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glPointSizex( IGL *pIGL, GLfixed size );

Parameters:
	pIGL : A pointer to the IGL interface
	size : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glPolygonOffsetx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glPolygonOffsetx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glPolygonOffsetx( IGL *pIGL, GLfixed factor, GLfixed units );

Parameters:
	pIGL : A pointer to the IGL interface
	factor : See Khronos documentation for details
	units : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glPopMatrix()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glPopMatrix.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glPopMatrix( IGL *pIGL );

Parameters:
	pIGL : A pointer to the IGL interface

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glPushMatrix()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glPushMatrix.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glPushMatrix( IGL *pIGL );

Parameters:
	pIGL : A pointer to the IGL interface

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glReadPixels()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glReadPixels.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glReadPixels( IGL *pIGL, GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels );

Parameters:
	pIGL : A pointer to the IGL interface
	x : See Khronos documentation for details
	y : See Khronos documentation for details
	width : See Khronos documentation for details
	height : See Khronos documentation for details
	format : See Khronos documentation for details
	type : See Khronos documentation for details
	pixels : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glRotatex()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glRotatex.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glRotatex( IGL *pIGL, GLfixed angle, GLfixed x, GLfixed y, GLfixed z );

Parameters:
	pIGL : A pointer to the IGL interface
	angle : See Khronos documentation for details
	x : See Khronos documentation for details
	y : See Khronos documentation for details
	z : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glSampleCoveragex()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glSampleCoveragex.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glSampleCoveragex( IGL *pIGL, GLclampx value, GLboolean invert );

Parameters:
	pIGL : A pointer to the IGL interface
	value : See Khronos documentation for details
	invert : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glScalex()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glScalex.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glScalex( IGL *pIGL, GLfixed x, GLfixed y, GLfixed z );

Parameters:
	pIGL : A pointer to the IGL interface
	x : See Khronos documentation for details
	y : See Khronos documentation for details
	z : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glScissor()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glScissor.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glScissor( IGL *pIGL, GLint x, GLint y, GLsizei width, GLsizei height );

Parameters:
	pIGL : A pointer to the IGL interface
	x : See Khronos documentation for details
	y : See Khronos documentation for details
	width : See Khronos documentation for details
	height : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glShadeModel()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glShadeModel.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glShadeModel( IGL *pIGL, GLenum mode );

Parameters:
	pIGL : A pointer to the IGL interface
	mode : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glStencilFunc()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glStencilFunc.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glStencilFunc( IGL *pIGL, GLenum func, GLint ref, GLuint mask );

Parameters:
	pIGL : A pointer to the IGL interface
	func : See Khronos documentation for details
	ref : See Khronos documentation for details
	mask : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glStencilMask()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glStencilMask.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glStencilMask( IGL *pIGL, GLuint mask );

Parameters:
	pIGL : A pointer to the IGL interface
	mask : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glStencilOp()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glStencilOp.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glStencilOp( IGL *pIGL, GLenum fail, GLenum zfail, GLenum zpass );

Parameters:
	pIGL : A pointer to the IGL interface
	fail : See Khronos documentation for details
	zfail : See Khronos documentation for details
	zpass : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glTexCoordPointer()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glTexCoordPointer.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glTexCoordPointer( IGL *pIGL, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );

Parameters:
	pIGL : A pointer to the IGL interface
	size : See Khronos documentation for details
	type : See Khronos documentation for details
	stride : See Khronos documentation for details
	pointer : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glTexEnvx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glTexEnvx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glTexEnvx( IGL *pIGL, GLenum target, GLenum pname, GLfixed param );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	pname : See Khronos documentation for details
	param : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glTexEnvxv()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glTexEnvxv.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glTexEnvxv( IGL *pIGL, GLenum target, GLenum pname, const GLfixed *params );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	pname : See Khronos documentation for details
	params : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glTexImage2D()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glTexImage2D.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glTexImage2D( IGL *pIGL, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	level : See Khronos documentation for details
	internalformat : See Khronos documentation for details
	width : See Khronos documentation for details
	height : See Khronos documentation for details
	border : See Khronos documentation for details
	format : See Khronos documentation for details
	type : See Khronos documentation for details
	pixels : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glTexParameterx()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glTexParameterx.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glTexParameterx( IGL *pIGL, GLenum target, GLenum pname, GLfixed param );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	pname : See Khronos documentation for details
	param : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glTexSubImage2D()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glTexSubImage2D.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glTexSubImage2D( IGL *pIGL, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels );

Parameters:
	pIGL : A pointer to the IGL interface
	target : See Khronos documentation for details
	level : See Khronos documentation for details
	xoffset : See Khronos documentation for details
	yoffset : See Khronos documentation for details
	width : See Khronos documentation for details
	height : See Khronos documentation for details
	format : See Khronos documentation for details
	type : See Khronos documentation for details
	pixels : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glTranslatex()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glTranslatex.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glTranslatex( IGL *pIGL, GLfixed x, GLfixed y, GLfixed z );

Parameters:
	pIGL : A pointer to the IGL interface
	x : See Khronos documentation for details
	y : See Khronos documentation for details
	z : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glVertexPointer()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glVertexPointer.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glVertexPointer( IGL *pIGL, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer );

Parameters:
	pIGL : A pointer to the IGL interface
	size : See Khronos documentation for details
	type : See Khronos documentation for details
	stride : See Khronos documentation for details
	pointer : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IGL_glViewport()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function glViewport.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard gl functions.

Prototype:
void IGL_glViewport( IGL *pIGL, GLint x, GLint y, GLsizei width, GLsizei height );

Parameters:
	pIGL : A pointer to the IGL interface
	x : See Khronos documentation for details
	y : See Khronos documentation for details
	width : See Khronos documentation for details
	height : See Khronos documentation for details

Return Value:
	void

Side Effects:
	None

Version:
	1.0

See Also:
	IGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

=======================================================================
  INTERFACES   DOCUMENTATION
=======================================================================

IEGL Interface

Description:

   The IEGL interface provides access to the EGL implementation on BREW.
   IEGL wraps the standard EGL functions with their IEGL BREW equivalents
   as well as provides the standard interface functions.

   For OpenGL ES documentation, see the OpenGL ES documentation available
   on the Khronos website at 
===a href="http://www.khronos.org">www.khronos.org.
===/a>

   The following header files are required for this interface:~
   AEE.h~
   GLES/gl.h~
   GLES/egl.h


========================================================================
IEGL_AddRef()
This function is inherited from IBASE_AddRef().
========================================================================

========================================================================
IEGL_Release()
This function is inherited from IBASE_Release().
========================================================================

========================================================================
IEGL_QueryInterface()

Description: 
   This function queries the IEGL interface
   
Prototype:
   int IEGL_QueryInterface( IEGL *pMe, AEECLSID clsid, void **ppNew )

Parameters:
   pMe : Pointer to IEGL instance
   clsid : Class id
   ppNew : Address of pointer to be set
                     
Return Value: 
   SUCCESS, on success~
   EUNSUPPORTED, otherwise

========================================================================



==========================================================================
IEGL_eglGetError()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglGetError.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLint IEGL_eglGetError( IEGL *pIEGL );

Parameters:
	pIEGL : A pointer to the IEGL interface

Return Value:
	EGLint : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglGetDisplay()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglGetDisplay.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLDisplay IEGL_eglGetDisplay( IEGL *pIEGL, NativeDisplayType display );

Parameters:
	pIEGL : A pointer to the IEGL interface
	display : See Khronos documentation for details

Return Value:
	EGLDisplay : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglInitialize()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglInitialize.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglInitialize( IEGL *pIEGL, EGLDisplay dpy, EGLint *major, EGLint *minor );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	major : See Khronos documentation for details
	minor : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglTerminate()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglTerminate.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglTerminate( IEGL *pIEGL, EGLDisplay dpy );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglQueryString()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglQueryString.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
const char * IEGL_eglQueryString( IEGL *pIEGL, EGLDisplay dpy, EGLint name );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	name : See Khronos documentation for details

Return Value:
	const char * : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglGetProcAddress()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglGetProcAddress.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
void (* IEGL_eglGetProcAddress( IEGL *pIEGL, const char *procname ))();

Parameters:
	pIEGL : A pointer to the IEGL interface
	procname : See Khronos documentation for details

Return Value:
	void (*fnPtr)() : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglGetConfigs()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglGetConfigs.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglGetConfigs( IEGL *pIEGL, EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	configs : See Khronos documentation for details
	config_size : See Khronos documentation for details
	num_config : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglChooseConfig()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglChooseConfig.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglChooseConfig( IEGL *pIEGL, EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	attrib_list : See Khronos documentation for details
	configs : See Khronos documentation for details
	config_size : See Khronos documentation for details
	num_config : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglGetConfigAttrib()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglGetConfigAttrib.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglGetConfigAttrib( IEGL *pIEGL, EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	config : See Khronos documentation for details
	attribute : See Khronos documentation for details
	value : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglCreateWindowSurface()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglCreateWindowSurface.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLSurface IEGL_eglCreateWindowSurface( IEGL *pIEGL, EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	config : See Khronos documentation for details
	window : See Khronos documentation for details
	attrib_list : See Khronos documentation for details

Return Value:
	EGLSurface : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglCreatePixmapSurface()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglCreatePixmapSurface.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLSurface IEGL_eglCreatePixmapSurface( IEGL *pIEGL, EGLDisplay dpy, EGLConfig config, NativePixmapType pixmap, const EGLint *attrib_list );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	config : See Khronos documentation for details
	pixmap : See Khronos documentation for details
	attrib_list : See Khronos documentation for details

Return Value:
	EGLSurface : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglCreatePbufferSurface()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglCreatePbufferSurface.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLSurface IEGL_eglCreatePbufferSurface( IEGL *pIEGL, EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	config : See Khronos documentation for details
	attrib_list : See Khronos documentation for details

Return Value:
	EGLSurface : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglDestroySurface()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglDestroySurface.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglDestroySurface( IEGL *pIEGL, EGLDisplay dpy, EGLSurface surface );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	surface : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglQuerySurface()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglQuerySurface.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglQuerySurface( IEGL *pIEGL, EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	surface : See Khronos documentation for details
	attribute : See Khronos documentation for details
	value : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglCreateContext()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglCreateContext.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLContext IEGL_eglCreateContext( IEGL *pIEGL, EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	config : See Khronos documentation for details
	share_list : See Khronos documentation for details
	attrib_list : See Khronos documentation for details

Return Value:
	EGLContext : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglDestroyContext()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglDestroyContext.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglDestroyContext( IEGL *pIEGL, EGLDisplay dpy, EGLContext ctx );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	ctx : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglMakeCurrent()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglMakeCurrent.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglMakeCurrent( IEGL *pIEGL, EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	draw : See Khronos documentation for details
	read : See Khronos documentation for details
	ctx : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglGetCurrentContext()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglGetCurrentContext.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLContext IEGL_eglGetCurrentContext( IEGL *pIEGL );

Parameters:
	pIEGL : A pointer to the IEGL interface

Return Value:
	EGLContext : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglGetCurrentSurface()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglGetCurrentSurface.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLSurface IEGL_eglGetCurrentSurface( IEGL *pIEGL, EGLint readdraw );

Parameters:
	pIEGL : A pointer to the IEGL interface
	readdraw : See Khronos documentation for details

Return Value:
	EGLSurface : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglGetCurrentDisplay()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglGetCurrentDisplay.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLDisplay IEGL_eglGetCurrentDisplay( IEGL *pIEGL );

Parameters:
	pIEGL : A pointer to the IEGL interface

Return Value:
	EGLDisplay : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglQueryContext()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglQueryContext.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglQueryContext( IEGL *pIEGL, EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	ctx : See Khronos documentation for details
	attribute : See Khronos documentation for details
	value : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglWaitGL()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglWaitGL.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglWaitGL( IEGL *pIEGL );

Parameters:
	pIEGL : A pointer to the IEGL interface

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglWaitNative()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglWaitNative.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglWaitNative( IEGL *pIEGL, EGLint engine );

Parameters:
	pIEGL : A pointer to the IEGL interface
	engine : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglSwapBuffers()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglSwapBuffers.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglSwapBuffers( IEGL *pIEGL, EGLDisplay dpy, EGLSurface draw );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	draw : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>

==========================================================================
IEGL_eglCopyBuffers()

Description:
	This function is the BREW wrapper for the standard OpenGLES 1.0
	function eglCopyBuffers.
	See the Khronos website at
===a href="http://www.khronos.org">www.khronos.org
===/a>
	 for documentation on the standard egl functions.

Prototype:
EGLBoolean IEGL_eglCopyBuffers( IEGL *pIEGL, EGLDisplay dpy, EGLSurface surface, NativePixmapType target );

Parameters:
	pIEGL : A pointer to the IEGL interface
	dpy : See Khronos documentation for details
	surface : See Khronos documentation for details
	target : See Khronos documentation for details

Return Value:
	EGLBoolean : See Khronos documentation for details

Side Effects:
	None

Version:
	1.0

See Also:
	IEGL Interface
===a href="http://www.khronos.org">www.khronos.org
===/a>


=============================================================================
*/
