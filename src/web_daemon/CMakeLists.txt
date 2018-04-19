cmake_minimum_required(VERSION 2.8)
project(web_daemon)

find_library(CORE_SERVICES CoreServices)
find_library(APP_SERVICES ApplicationServices)
find_library(CARBON Carbon)
find_library(APPKIT AppKit)
find_library(SYSCONFIG SystemConfiguration)
find_library(SEQURITY Security)

IF(WIN32)
	set(LIBS Qt5Core Qt5Gui Qt5Widgets Qt5Network Qt5Multimedia Qt5WebKit Qt5WebKitWidgets Imm32 Mincore Shlwapi winmm Ws2_32 version)
ELSE(WIN32)
    IF(APPLE)
        add_definitions(-DQT_MAC_USE_COCOA)
        set(LIBS QtWebKit QtGui QtNetwork QtCore z ${CORE_SERVICES} ${APP_SERVICES} ${CARBON} ${APPKIT} ${SYSCONFIG} ${SEQURITY})
    ELSE(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        set(LIBS Qt5WebKit Qt5WebKitWidgets Qt5WebView Qt5Gui Qt5Widgets Qt5Network Qt5Core X11 SM ICE glib-2.0 Xrandr Xrender Xcursor Xfixes Xext Xinerama Xi freetype pthread dl fontconfig crypto ssl)
    ENDIF(APPLE)
ENDIF(WIN32)

set(SOURCE_FILES main.cpp
                 WebApp.h
                 WebApp.cpp
				 WebPage.h
				 WebPage.cpp
                 WebView.h
                 WebView.cpp)

list(APPEND ALL_SOURCE_FILES ${SOURCE_FILES})
source_group("src" FILES ${SOURCE_FILES})

add_executable(web_daemon ${ALL_SOURCE_FILES})
target_link_libraries(web_daemon common ${LIBS})

add_custom_command(TARGET web_daemon 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:web_daemon> ${WORKING_DIRECTORY})
