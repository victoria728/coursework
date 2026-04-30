cmake_minimum_required(VERSION 3.21)
project(untitled C)

set(CMAKE_C_STANDARD 99)

add_executable(untitled main.c
        io.c
        io.h
        waveform.h waveform.c)

if(UNIX)
    target_link_libraries(untitled m)
endif()

