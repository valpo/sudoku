aux_source_directory(. SRC_LIST)
add_definitions(-std=c++11 -g -Wall -O0)
add_executable(classic_sudoku ${SRC_LIST})


