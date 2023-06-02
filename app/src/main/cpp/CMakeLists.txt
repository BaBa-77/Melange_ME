cmake_minimum_required(VERSION 3.13)

project("brewemulator")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O0")

add_library(brew STATIC IMPORTED)
set_property(TARGET brew PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/brew.fixed.lib)

add_library(aee STATIC IMPORTED)
set_property(TARGET aee PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aee.fixed.lib)

add_library(aeedialog STATIC IMPORTED)
set_property(TARGET aeedialog PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeedialog.fixed.lib)

add_library(aeepdiparsers STATIC IMPORTED)
set_property(TARGET aeepdiparsers PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeepdiparsers.fixed.lib)

add_library(aeevcalstore STATIC IMPORTED)
set_property(TARGET aeevcalstore PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeevcalstore.fixed.lib)

add_library(aeefont STATIC IMPORTED)
set_property(TARGET aeefont PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeefont.fixed.lib)

add_library(aeesecurity STATIC IMPORTED)
set_property(TARGET aeesecurity PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeesecurity.fixed.lib)

add_library(aeersa STATIC IMPORTED)
set_property(TARGET aeersa PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeersa.fixed.lib)

add_library(aeeweb STATIC IMPORTED)
set_property(TARGET aeeweb PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeeweb.fixed.lib)

add_library(aeenet STATIC IMPORTED)
set_property(TARGET aeenet PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeenet.fixed.lib)

add_library(aeecontrols STATIC IMPORTED)
set_property(TARGET aeecontrols PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeecontrols.fixed.lib)

add_library(aeemenu STATIC IMPORTED)
set_property(TARGET aeemenu PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeemenu.fixed.lib)

add_library(aeeijg STATIC IMPORTED)
set_property(TARGET aeeijg PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeeijg.fixed.lib)

add_library(aeepng STATIC IMPORTED)
set_property(TARGET aeepng PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeepng.fixed.lib)

add_library(bpng STATIC IMPORTED)
set_property(TARGET bpng PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/png.fixed.lib)

add_library(aeeseed STATIC IMPORTED)
set_property(TARGET aeeseed PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeeseed.fixed.lib)

add_library(aeemm STATIC IMPORTED)
set_property(TARGET aeemm PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeemm.fixed.lib)

add_library(aeetapi STATIC IMPORTED)
set_property(TARGET aeetapi PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeetapi.fixed.lib)

add_library(aeeunzip STATIC IMPORTED)
set_property(TARGET aeeunzip PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeeunzip.fixed.lib)

add_library(aeebci STATIC IMPORTED)
set_property(TARGET aeebci PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeebci.fixed.lib)

add_library(aeedb STATIC IMPORTED)
set_property(TARGET aeedb PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeedb.fixed.lib)

add_library(brewz STATIC IMPORTED)
set_property(TARGET brewz PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/z.fixed.lib)

add_library(aeeaddrbk STATIC IMPORTED)
set_property(TARGET aeeaddrbk PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/brewrt/aeeaddrbk.fixed.lib)

add_library(brewemu SHARED
        breABI.c

        bre/AEENet.c
        bre/AEEMediaUtil.c
        bre/OEMRan.c
        bre/OEMOS.c
        bre/OEMMutex.c
        bre/OEMHeap.c
        bre/OEMAppFuncs.c
        bre/OEMFSPath.cpp
        bre/OEMFS.cpp
        bre/OEMSound.c
        bre/md5c.c
        bre/OEMMD5.c
        bre/OEMDebug.c
        bre/OEMBN.c
        bre/atof.c
        bre/DummyMIFs.c
        bre/Shims.c
        bre/OEMMemCacheArch.c
        bre/OEMDisplayDevImpl.h
        bre/OEMDisplayDevImpl.c
        bre/time_jul.c
        bre/OEMCriticalSection.c
        bre/OEMTAPI.c
        bre/OEMSysClock.c
        bre/ARMLegacyABI.S
        bre/OEMAddrBook.c
        bre/OEMMediaCMidi.cpp
        bre/OEMAnnunciatorControl.c

        bregl/AEEGL.h
        bregl/bregl.c

        bre2/breConfig.c
        bre2/breConfig.h
        bre2/toml.c
        bre2/toml.h
        bre2/breMockSignature.c
        bre2/breStartup.c
        bre2/breMainLoop.c
        bre2/breGfx.c
        bre2/brewemu.c
        bre2/brewemu.h
        bre2/stb_image_write.c
        bre2/stb_image_write.h
        bre2/breScreenshot.c
        bre2/breScreenshot.h

        brekddi/brekddi.c

        brefont/brefont_jp.c
        brefont/msx_kanji.h

        brewappmgr/BrewAppMgr.c
        brewappmgr/BrewAppMgr.h
        brewappmgr/BrewAppMgrUtils.c
        brewappmgr/BrewAppMgrUtils.h
        brewappmgr/BrewAppMgrSettings.h
        brewappmgr/MobileShop.c
        brewappmgr/MobileShop.h
        brewappmgr/brewappmgr_mif.c
        brewappmgr/oemcfg_txt.c
        brewappmgr/brewappmgrcommon_bar.c
        brewappmgr/brewappmgrln_bar.c
        brewappmgr/brewappmgrls_bar.c

        AEEAppGen.c
        AEEModGen.c
        basic11.h
        basic11b.h
        basic14.h
        OEMAppFrame.c
        OEMAppx5.c
        OEMAuth.c
        OEMBitmap_common.c
        OEMBitmap_generic.h
        OEMBitmap_priv.h
        OEMBitmap16.c
        OEMBitmap24.c
        OEMBitmapFuncs_priv.h
        OEMCallHistory.c
        OEMConstFiles.c
        OEMDB.c
        OEMDefaults.c
        OEMDeviceNotifier.c
        OEMDisp.c
        OEMFlip.c
        OEMFont.c
        OEMIMM.c
        OEMIMUI.c
        oemitextctl.c
        OEMKeysMapping.c
        OEMLogger.c
        OEMLoggerBTIL.c
        OEMLoggerFile.c
        OEMMCFMIF_10888.c
        OEMMCFMIF_10889.c
        OEMMCFMIF_10890.c
        OEMMCFMIF_10891.c
        OEMMCFMIF_10892.c
        OEMMCFMIF_16225.c
        OEMMCFMIF_18067.c
        OEMMCFMIF_19240.c
        OEMMCFMIF_19917.c
        OEMMCFMIF_19918.c
        OEMMCFMIF_19919.c
        OEMMCFMIF_19920.c
        OEMMCFMIF_19922.c
        OEMMCFMIF_19923.c
        OEMMCFMIF_19924.c
        OEMMedia.c
        OEMMediaFormats.c
        OEMMemCache.c
        OEMModTable.c
        OEMNativeNotifier.c
        OEMObjectMgr.c
        OEMPrivacy.c
        OEMRegistry.c
        OEMResArbiter.c
        OEMRinger.c
        OEMStringFuncs.c
        OEMText.c
        OEMTransform_generic.h
        OEMUSBHMSMIF_26108.c
        OEMUSBHMSMIF_26109.c

        mmapi/MMPlayer.cpp
        mmapi/MMPlayer.h
        mmapi/MMStream.cpp
        mmapi/MMStream.h
        mmapi/MMCMXLoader.cpp
        mmapi/MMCMXLoader.h
        )

set_property(SOURCE bre/ARMLegacyABI.S PROPERTY LANGUAGE C)

add_definitions(-DAEE_STATIC -DBREW_STATIC_APP)

target_include_directories(brewemu PRIVATE
        "$ENV{BREWPK}\\..\\inc"
        "$ENV{BREWPK}\\inc"
        "$ENV{BREWPK}\\..\\sdk\\inc"
        "${CMAKE_CURRENT_SOURCE_DIR}/glad")
target_compile_options(brewemu PRIVATE -mfpu=vfpv3)
target_link_options(brewemu PRIVATE -Wl,-z,notext -Wl,-z,muldefs -Wl,-error-limit=0)

find_library(log-lib log)
find_library(android-lib android)
find_package(ZLIB)

target_link_libraries(brewemu
        brew
        aee
        aeebci
        aeedialog
        aeepdiparsers
        aeevcalstore
        aeefont
        aeesecurity
        aeersa
        aeeweb
        aeenet
        aeecontrols
        aeemenu
        aeeijg
        aeepng
        aeeseed
        aeemm
        aeetapi
        aeeunzip
        aeedb
        aeeaddrbk
        brewz
        bpng
        z
        egl
        GLESv1_CM
        ${log-lib}
        ${android-lib}
        )