project(sudoku)
cmake_minimum_required(VERSION 2.8)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # this is clang
add_definitions(-std=c++1y -stdlib=libc++ -g -Wall -O0 -fconstexpr-steps=2147483647)
set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") # this is gcc
add_definitions(-std=c++1y -g -Wall -O0)
set(CMAKE_EXE_LINKER_FLAGS "")
endif()

add_subdirectory(classic)
add_subdirectory(constexpr)
add_subdirectory(constexpr_arr)
add_subdirectory(qt)

