cmake_minimum_required(VERSION 2.8)
project(web_daemon)

IF(WIN32)
	set(LIBS QtCore QtGui QtNetwork QtWebKit Imm32 Mincore Shlwapi winmm Ws2_32)
ELSE(WIN32)
    set(LIBS QtWebKit QtGui QtNetwork QtCore X11 SM ICE glib-2.0 Xrandr Xrender Xcursor Xfixes Xext Xinerama Xi freetype pthread dl)
ENDIF(WIN32)

set(SOURCE_FILES    main.cpp
                    WebApp.h
                    WebApp.cpp
                    WebView.h
                    WebView.cpp)

list(APPEND ALL_SOURCE_FILES ${SOURCE_FILES})
source_group("src" FILES ${SOURCE_FILES})

add_executable(web_daemon ${ALL_SOURCE_FILES})
target_link_libraries(web_daemon common ${LIBS})

add_custom_command(TARGET web_daemon 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:web_daemon> ${WORKING_DIRECTORY})