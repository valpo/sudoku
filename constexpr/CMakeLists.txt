aux_source_directory(. SRC_LIST)
#  -ftemplate-backtrace-limit=0
add_executable(constexpr_sudoku ${SRC_LIST})
#target_link_libraries(constexpr_sudoku c++ c++abi)


