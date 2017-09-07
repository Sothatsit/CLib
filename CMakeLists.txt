cmake_minimum_required(VERSION 3.6)
project(CLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c")

set(SOURCE_FILES
        src/numbers.h
        src/numbers.c
        src/string.h
        src/string.c
        test/test.h
        test/test.c
        test/testNumbers.h
        test/testNumbers.c
        test/testString.h
        test/testString.c
        test/testStringBuilder.h
        test/testStringBuilder.c)

add_executable(CLib ${SOURCE_FILES})