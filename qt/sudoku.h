#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>
#include <iostream>

class Sudoku
{
private:
  std::vector<unsigned> data;
  std::vector<unsigned>::size_type dim;
public:
  Sudoku(size_t d) : dim{d} { data.resize(dim*dim,0); }
  unsigned& operator()(size_t row, size_t col) { return data[row*dim+col]; }
  const unsigned& operator()(size_t row, size_t col) const { return data.at(row*dim+col); }
  size_t dimension() const { return dim; }
  void generate();
};
std::ostream& operator<<(std::ostream& os, const Sudoku& s);
Sudoku solve(Sudoku s);
std::pair<size_t, size_t> next(const Sudoku& s);
bool finished(const Sudoku& s);
bool valid(const Sudoku& s);

#endif // SUDOKU_H

