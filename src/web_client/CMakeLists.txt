cmake_minimum_required(VERSION 2.8)
project(web_client)

IF(WIN32)
    IF(SDL_STATIC)
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            set(LIBS :libSDL2main.a :libSDL2.a :libSDL2_mixer.a winmm imm32 version)
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            set(LIBS SDL2main.lib SDL2.lib winmm imm32 version)
        endif()
    ELSE(SDL_STATIC)
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            set(LIBS opengl32 SDL2 SDL2_mixer)
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            set(LIBS opengl32 SDL2 SDL2_mixer)
        endif()
    ENDIF(SDL_STATIC)
ELSE(WIN32)
    IF(APPLE)
        set(LIBS SDL2)
    ELSE(APPLE)
        set(LIBS X11 SDL2 dl pthread GL glut c)
    ENDIF(APPLE)
ENDIF(WIN32)

set(SOURCE_FILES main.cpp
                 ClientApp.h
                 ClientApp.cpp)

list(APPEND ALL_SOURCE_FILES ${SOURCE_FILES})
#set_source_files_properties(${SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("src" FILES ${SOURCE_FILES})

add_executable(web_client ${ALL_SOURCE_FILES})
target_link_libraries(web_client common ${LIBS})

add_dependencies(web_client web_daemon)

add_custom_command(TARGET web_client
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:web_client> ${WORKING_DIRECTORY})
