cmake_minimum_required(VERSION 3.14)
set (CMAKE_C_COMPILER "/opt/homebrew/bin/gcc-12")
set (CMAKE_CXX_COMPILER "/opt/homebrew/bin/g++-12")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(dbtrain)

include_directories(src)

add_subdirectory(src)
