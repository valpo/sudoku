aux_source_directory(. SRC_LIST)
add_definitions(-std=c++1z)
#  -ftemplate-backtrace-limit=0
#add_definitions(-std=c++1y)
add_executable(constexpr_sudoku ${SRC_LIST})
#target_link_libraries(constexpr_sudoku c++ c++abi)


