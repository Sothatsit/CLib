cmake_minimum_required(VERSION 3.6)
project(CLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c")

set(SOURCE_FILES
        src/numbers.h
        src/numbers.c
        src/string.h
        src/string.c
        src/test.c)

add_executable(CLib ${SOURCE_FILES})