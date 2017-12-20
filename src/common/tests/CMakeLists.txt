cmake_minimum_required(VERSION 3.2)
project(test_common)

add_executable(test_common main.cpp
                           test_common.h
                           test_pipe.cpp
                           test_process.cpp
                           test_shared_memory.cpp)

target_link_libraries(test_common common)
