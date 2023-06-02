#ifndef __AEEEGLTYPES_H_
#define __AEEEGLTYPES_H_
typedef int             AEEEGLint;
typedef unsigned int    AEEEGLBoolean;
typedef void *          AEEEGLConfig;
typedef void *          AEEEGLContext;
typedef void *          AEEEGLDisplay;
typedef void *          AEEEGLSurface;

typedef AEEEGLint AEEEGLfixed;        // s15.16 fixed point format

typedef struct
{
  AEEEGLint     x;
  AEEEGLint     y;
  AEEEGLint     width;
  AEEEGLint     height;
} AEEEGLSurfaceScaleRect;

typedef struct
{
  AEEEGLfixed   MinXScaleFactor;
  AEEEGLfixed   MaxXScaleFactor;
  AEEEGLfixed   MinYScaleFactor;
  AEEEGLfixed   MaxYScaleFactor;
  AEEEGLint     MinSrcWidth;
  AEEEGLint     MaxSrcWidth;
  AEEEGLint     MinSrcHeight;
  AEEEGLint     MaxSrcHeight;
  AEEEGLint     MinDstWidth;
  AEEEGLint     MaxDstWidth;
  AEEEGLint     MinDstHeight;
  AEEEGLint     MaxDstHeight;
} AEEEGLSurfaceScaleCaps;

typedef struct
{
  AEEEGLint     max_overlay;
  AEEEGLint     max_underlay;
  AEEEGLint     max_surface_per_layer;
  AEEEGLint     max_total_surfaces;
  AEEEGLBoolean pbuffer_support;
  AEEEGLBoolean pixmap_support;
} AEEEGLCompositeSurfaceCaps;

#endif  // __AEEEGLTYPES_H_
