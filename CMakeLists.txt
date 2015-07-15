project(sudoku)
cmake_minimum_required(VERSION 2.8)

# needed for clang-3.6 on ubuntu 14.04
include_directories(/usr/include/c++/4.9 /usr/include/x86_64-linux-gnu/c++/4.9)

add_subdirectory(classic)
add_subdirectory(constexpr)
# add_subdirectory(qt)

