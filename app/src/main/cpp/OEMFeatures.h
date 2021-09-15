
#ifndef BREWEMULATOR_OEMFEATURES_H
#define BREWEMULATOR_OEMFEATURES_H

#if !defined(OEMTEXT)
#define OEMTEXT             // Use OEMText.c
#endif

#if !defined(OEMSTRINGFUNCS)
#define OEMSTRINGFUNCS             // Use OEMStringFuncs.c
#endif

#if !defined(OEMOBJECTMGR)
#define OEMOBJECTMGR             // Use OEMObjectMgr.c
#endif

#if !defined(OEMMODTABLE)
#define OEMMODTABLE
#endif

#if !defined(OEMDISP)
#define OEMDISP
#endif

#define AEEMENU_LIB     1
#define AEECONTROLS_LIB 1


#define FEATURE_BREW_SECURITY       // turn on hashes, ciphers and random numbers
#define FEATURE_BREW_RSA            // Turn on RSA; needed for sig verify, SSL & IX509Chain
#if defined(FEATURE_BREW_SECURITY)
#define FEATURE_BREW_DES3DES     // define this to turn on DES and 3DES in BREW
#endif
#define FEATURE_BREW_FONTS
#define FEATURE_BREW_SCALE
#define FEATURE_JPEG_DECODER
#define FEATURE_BREW_PNG_DECODE
#define FEATURE_BREW_TAPI
#define FEATURE_BREW_SEED
#define FEATURE_GRAPHICS
#define FEATURE_UNZIP
// #define FEATURE_BREW_MULTIMEDIA
#define FEATURE_BREW_BITMAPDEV
#define FEATURE_BREW_SOUND
#define FEATURE_TOPVISIBLE
#define FEATURE_BREW_RAMCACHE
#define FEATURE_BREW_AEEAPPLETCTL
#define FEATURE_BREW_SIGNAL
#define FEATURE_BREW_AEEAPPHISTORY
#define FEATURE_BREW_LICENSE
#define FEATURE_BREW_PROVISION
#define FEATURE_BREW_AEETHREAD
#define FEATURE_BCI_DECODE
#define FEATURE_APP_MANAGER

#define FEATURE_DYNAMIC_OEM_HEAP

#endif //BREWEMULATOR_OEMFEATURES_H
