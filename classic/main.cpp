#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <cassert>
#include <stdexcept>

using namespace std;

unsigned SudokuIn[9][9] = {
  { 9, 5, 0, 0, 2, 0, 0, 0, 0 },
  { 0, 7, 8, 5, 0, 0, 0, 0, 0 },
  { 0, 6, 0, 4, 0, 0, 0, 0, 0 },
  { 0, 9, 0, 7, 3, 0, 0, 5, 0 },
  { 8, 0, 0, 0, 0, 0, 0, 0, 2 },
  { 0, 1, 0, 0, 8, 5, 0, 6, 0 },
  { 0, 0, 0, 0, 0, 6, 0, 8, 0 },
  { 0, 0, 0, 0, 0, 8, 7, 9, 0 },
  { 0, 0, 0, 0, 7, 0, 0, 3, 1 }
};

class Sudoku
{
private:
  vector<unsigned> data;
  vector<unsigned>::size_type dim;
public:
  Sudoku(size_t d) : dim{d} { data.resize(dim*dim,0); }
  unsigned& operator()(size_t row, size_t col) { return data[row*dim+col]; }
  const unsigned& operator()(size_t row, size_t col) const { return data.at(row*dim+col); }
  size_t dimension() const { return dim; }
};

ostream& operator<<(ostream& os, const Sudoku& s) {
  os << "+------------------+\n";
  for(size_t i=0; i< s.dimension(); ++i) {
    os << "|";
    for(size_t j=0; j< s.dimension(); ++j) {
      os << s(i,j) << " ";
    }
    os << "|\n";
  }
  os << "+------------------+\n";
  return os;
}

bool rowValid(const Sudoku& s, size_t row)
{
  size_t count = 0;
  std::set<unsigned> vals;
  for(size_t i=0; i<s.dimension(); ++i) {
    const unsigned& val = s(row,i);
    if (val == 0) continue;
    vals.insert(val);
    ++count;
    if (count != vals.size()) return false;
  }
  return true;
}
bool colValid(const Sudoku& s, size_t col)
{
  size_t count = 0;
  std::set<unsigned> vals;
  for(size_t i=0; i<s.dimension(); ++i) {
    const unsigned& val = s(i,col);
    if (val == 0) continue;
    vals.insert(val);
    ++count;
    if (count != vals.size()) return false;
  }
  return true;
}
bool quadrantValid(const Sudoku& s, size_t x, size_t y)
{
  size_t qsize = sqrt(s.dimension());
  size_t count = 0;
  std::set<unsigned> vals;
  for(size_t i=0; i<qsize; ++i)
    for(size_t j=0; j<qsize; ++j) {
      const unsigned& val = s(qsize*x+i,qsize*y+j);
      if (val == 0) continue;
      vals.insert(val);
      ++count;
      if (count != vals.size()) return false;
    }
  return true;
}

bool valid(const Sudoku& s)
{
  for(size_t x=0;x<s.dimension();++x) {
    if (!rowValid(s,x)) return false;
    if (!colValid(s,x)) return false;
  }
  size_t qsize = sqrt(s.dimension());
  for(size_t i=0; i<qsize; ++i)
    for(size_t j=0; j<qsize; ++j)
      if (!quadrantValid(s,i,j)) return false;
  return true;
}

bool finished(const Sudoku& s)
{
  for(size_t i=0; i<s.dimension(); ++i)
    for(size_t j=0; j<s.dimension(); ++j)
      if (s(i,j) == 0) return false;
  return true;
}

pair<size_t, size_t> next(const Sudoku& s)
{
  for(size_t i=0; i<s.dimension(); ++i)
    for(size_t j=0; j<s.dimension(); ++j)
      if (s(i,j) == 0) return {i,j};
  throw logic_error("sudoku finished, no free places");
}

Sudoku solve(Sudoku s)
{
  return s;
  if (!valid(s)) return s;
  //cout << s << endl;
  try {
    pair<size_t, size_t> n = next(s);
    for(unsigned i=1;i<=s.dimension();++i) {
      s(n.first,n.second) = i;
      Sudoku res = solve(s);
      if (finished(res) && valid(res)) return res;
    }
  }
  catch (logic_error) {
  }

  return s;
}

int main()
{
  Sudoku s(9);
  for(size_t x=0;x<s.dimension();++x)
    for(size_t y=0;y<s.dimension();++y)
      s(x,y) = SudokuIn[x][y];
  cout << s << endl;
  Sudoku res = solve(s);
  cout << res << endl;
  return 0;
}

