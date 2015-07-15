aux_source_directory(. SRC_LIST)
add_definitions(-std=c++1z -stdlib=libc++ -g -Wall -O0 -fconstexpr-steps=2147483647)
#  -ftemplate-backtrace-limit=0
#add_definitions(-std=c++1y)
add_executable(constexpr_sudoku ${SRC_LIST})
#target_link_libraries(constexpr_sudoku c++ c++abi)


