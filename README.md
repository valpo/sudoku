# sudoku
Compile time sudoku. Just an example for compile time evaluation, used for demonstration purposes for our 
local c++ user group and for my colleguages at work. Tested with clang only (as my older gcc was not able
to compile the constexpr stuff...)

## classic
This is a quick shot implementation of a sudoku solver, used at starting point for the compile time variant. 

## constexpr
This is the compile time variant. Works for me with clang 3.6, needs C++14 (constexpr using "if" and loops). 

## constexpr_arr
This is a modification of the constexpr example using builtin array instead of std::array. We had a 
longer discussion in our local c++ user group about the fact, that c++14 removed the implicit const 
from constexpr methods. This is the result of our search for a use case. And it simplifies the 
implemention by removing the template-gymnastics used in the constexpr example to "modify" values
in a std::array. Courtesy goes to [Arne Metz](http://arne-mertz.de/category/cpp/new-features/)

## qt
This is just a graphical frontend to demonstrate the problem and visualize how backtracking works. 

See also [my blog](http://www.valpo.de/?p=190)

# Compiler
Examples tested with clang (3.6 and current version from svn) and gcc 5.2.1. Clang needs around 5 minutes to 
compile one constexpr example and uses around 100MB of memory. gcc needs around 388GB of memory for the 
constexpr example, so do not forget to add some memory before compiling. 
