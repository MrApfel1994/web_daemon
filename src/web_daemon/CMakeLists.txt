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
        set(LIBS QtWebKit QtWebKitWidgets QtGui QtNetwork QtCore z ${CORE_SERVICES} ${APP_SERVICES} ${CARBON} ${APPKIT} ${SYSCONFIG} ${SEQURITY})
    ELSE(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        #set(LIBS Qt5WebKit Qt5WebKitWidgets Qt5WebView Qt5Gui Qt5Widgets Qt5Network Qt5Core X11 SM ICE glib-2.0 Xrandr Xrender Xcursor Xfixes Xext Xinerama Xi freetype pthread dl fontconfig crypto ssl)
        
        set(LIBS -Wl,--start-group gstlibav avfilter avformat avcodec avutil gstaudioresample gstencoding gsttypefindfunctions gstwavenc gstalaw gstaudiotestsrc gstlevel gstpulseaudio gstudp gstwavparse gstalpha gstauparse gstflv gstrawparse gstvideo4linux2 gstximagesink gstalphacolor gstautodetect gstflxdec gstmulaw gstreplaygain  gstvideobox gstximagesrc gstalsa gstavi gstrtp gstvideoconvert gstxvimagesink gstcoreelements gstmultipart gstrtpmanager gstvideocrop gsty4menc gstapp gstcoretracers gstgoom gstnavigationtest gstvideofilter gstaudioconvert gstcutter gsticydemux gstopengl gstshapewipe gstvideomixer gstaudiofx gstid3demux gstoss4 gstsmpte gstvideorate gstaudiomixer gstdeinterlace gstimagefreeze gstossaudio gstspectrum gstvideoscale gstaudioparsers gstdtmf gstinterleave gstpbtypes gstsubparse gstvideotestsrc gstaudiorate gsteffectv gstisomp4 gstplayback gsttcp gstvolume gstallocators-1.0 gstbase-1.0 gstfft-1.0 gstpbutils-1.0 gstrtp-1.0 gsttag-1.0 gstapp-1.0 gstreamer-1.0 gstvideo-1.0 gstaudio-1.0 gstcontroller-1.0 gstnet-1.0 gstriff-1.0 Qt5WebKitWidgets Qt5WebKit WebCore JavaScriptCore woff2 WTF ANGLESupport icuuc icui18n icudata png z jpeg gstreamer-1.0 gstapp-1.0 gstbase-1.0 gstfft-1.0 gstvideo-1.0 gstaudio-1.0 gsttag-1.0 gstpbutils-1.0 gmodule-2.0 gobject-2.0 hyphen brotli xml2 sqlite3 bmalloc Qt5Widgets Qt5ThemeSupport Qt5Gui Qt5Network Qt5Positioning Qt5EventDispatcherSupport Qt5LinuxAccessibilitySupport Qt5DBus Qt5AccessibilitySupport Qt5ServiceSupport Qt5FontDatabaseSupport Qt5Core qxcb Qt5XcbQpa xcb-sync xcb X11-xcb xcb-static gio-2.0 GL harfbuzz pcre2-16 X11 SM ICE glib-2.0 Xrandr Xrender Xcursor Xfixes Xext Xinerama Xi freetype pthread dl fontconfig crypto ssl EGL wayland-client wayland-egl asound pulse pulsecommon-11.1 dbus-1 swresample)
    ENDIF(APPLE)
ENDIF(WIN32)

set(SOURCE_FILES main.cpp
                 gst_plugins.cpp
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
