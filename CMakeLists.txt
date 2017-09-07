# Dummy file so CLion knows which files are source files.

cmake_minimum_required(VERSION 3.6)
project(CLib)

file(GLOB_RECURSE SOURCE_FILES *.h *.c)
add_executable(CLib ${SOURCE_FILES})