cmake_minimum_required(VERSION 3.17)

project(AppUI)

set(APPUI_SOURCES
        AppUI.c
)

if (APPUI_BUILD_STATIC_MODULE)
    add_library(AppUI STATIC ${APPUI_SOURCES} AppUI_mif.c)
    target_include_directories(AppUI PRIVATE
            "$ENV{BREWPK}\\..\\inc"
            "$ENV{BREWPK}\\inc"
            "$ENV{BREWPK}\\..\\sdk\\inc")
    target_compile_definitions(AppUI PRIVATE -DAEE_STATIC -DBREW_STATIC_APP)
else()
    add_brew_module(AppUI
            ${BREW_PLATFORM_SDK_ROOT}/src/AEEModGen.c
            ${BREW_PLATFORM_SDK_ROOT}/src/AEEAppGen.c

            ${APPUI_SOURCES}
            )
endif()
