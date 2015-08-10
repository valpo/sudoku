# sudoku
Compile time sudoku. Just an example for compile time evaluation, used for demonstration purposes for our 
local c++ user group and for my colleguages at work. Tested with clang only (as my older gcc was not able
to compile the constexpr stuff...)

## classic
This is a quick shot implementation of a sudoku solver, used at starting point for the compile time variant. 

## constexpr
This is the compile time variant. Works for me with clang 3.6, needs C++14 (constexpr using "if" and loops). 

## qt
This is just a graphical frontend to demonstrate the problem and visualize how backtracking works. 

See also [my blog](http://www.valpo.de/?p=190)
