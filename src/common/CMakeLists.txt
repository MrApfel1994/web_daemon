cmake_minimum_required(VERSION 2.8.4)
project(common)

IF(WIN32)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(LIBS )
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(LIBS )
    endif()
ELSE(WIN32)
    IF(APPLE)
        set(LIBS )
    ELSE(APPLE)
        set(LIBS rt)
    ENDIF(APPLE)
ENDIF(WIN32)

set(SOURCE_FILES MsgDef.h
                 PipeClient.h
                 PipeServer.h
                 Process.h
                 SharedMemory.h)

set(WIN32_SOURCE_FILES PipeClient_win32.cpp
                       PipeServer_win32.cpp
                       Process_win32.cpp
                       SharedMemory_win32.cpp)

set(UNIX_SOURCE_FILES PipeClient_unix.cpp
                      PipeServer_unix.cpp
                      Process_unix.cpp
                      SharedMemory_unix.cpp)

list(APPEND ALL_SOURCE_FILES ${SOURCE_FILES})
source_group("src" FILES ${SOURCE_FILES})

IF(WIN32)
    list(APPEND ALL_SOURCE_FILES ${WIN32_SOURCE_FILES})
    source_group("src" FILES ${WIN32_SOURCE_FILES})
ELSE(WIN32)
    set(CMAKE_CXX_STANDARD 11)

    list(APPEND ALL_SOURCE_FILES ${UNIX_SOURCE_FILES})
    source_group("src" FILES ${UNIX_SOURCE_FILES})
ENDIF(WIN32)

add_library(common STATIC ${ALL_SOURCE_FILES})
target_link_libraries(common ${LIBS})

add_subdirectory(tests)
