//#include <tuple>
//#include <type_traits>
#include <utility>
#include <array>
#include <limits>

#include <iostream>

using std::cout;
using std::endl;

constexpr int sqrt_impl(int i, int val)
{
  return i*i == val ? i : sqrt_impl(i-1,val);
}
constexpr int sqrt(int i)
{
  return sqrt_impl(i,i);
}

// sudoku (== matrix) class
template<typename T, size_t N>
class SudokuT
{
private:
public:
  T data[N*N];
  constexpr unsigned operator()(size_t row, size_t col) const {
    return row*N+col >= N*N ? 0
                            : data[row*N+col];
  }
  constexpr SudokuT<T,N> replace(size_t row, size_t col, T val) {
    data[row*N+col] = val;
    return *this;
  }
  constexpr size_t dimension() const { return N; }
};

using Sudoku = SudokuT<unsigned, 9ul>;
using Cell = std::pair<size_t, size_t>;

std::ostream& operator<<(std::ostream& os, const Sudoku& s) {
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

std::ostream& operator<<(std::ostream& os, const Cell& c)
{
  os << '(' << c.first << ',' << c.second << ')';
  return os;
}

constexpr bool rowValid(const Sudoku& s, size_t row, size_t pos = 0)
{
  if (pos > s.dimension()) return true;
  else if (s(row,pos) == 0) return rowValid(s,row,pos+1);
  else {
    for(size_t i=pos+1;i<s.dimension();++i)
      if (s(row,i) == s(row,pos)) return false;
    return rowValid(s,row,pos+1);
  }
}
constexpr bool colValid(const Sudoku& s, size_t col, size_t pos = 0)
{
  if (pos > s.dimension()) return true;
  else if (s(pos,col) == 0) return colValid(s,col,pos+1);
  else {
    for(size_t i=pos+1;i<s.dimension();++i)
      if (s(i,col) == s(pos,col)) return false;
    return colValid(s,col,pos+1);
  }
}
// return the value at offset in quadrant(x,y)
constexpr unsigned qVal(const Sudoku& s, size_t x,size_t y, size_t offset = 0)
{
  size_t qsize = sqrt(s.dimension());
  return s(qsize*x + offset/qsize, qsize*y + offset % qsize);
}

constexpr bool quadrantValid(const Sudoku& s, size_t x, size_t y, size_t pos = 0)
{
  if (pos > s.dimension()) return true;
  else if (qVal(s, x,y,pos) == 0) return quadrantValid(s, x, y, pos+1);
  else {
    for(size_t i=pos+1;i<s.dimension();++i)
      if (qVal(s, x,y,i) == qVal(s, x,y,pos)) return false;
    return quadrantValid(s,x,y,pos+1);
  }
}

constexpr bool valid(const Sudoku& s)
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

constexpr bool finished(const Sudoku& s)
{
  for(size_t i=0; i<s.dimension(); ++i)
    for(size_t j=0; j<s.dimension(); ++j)
      if (s(i,j) == 0) return false;
  return true;
}

constexpr Cell next(const Sudoku& s)
{
  for(size_t i=0; i<s.dimension(); ++i)
    for(size_t j=0; j<s.dimension(); ++j)
      if (s(i,j) == 0) return {i,j};
  return {std::numeric_limits<Cell::first_type>::max(),std::numeric_limits<Cell::second_type>::max()};
}

constexpr Sudoku solve(const Sudoku& s)
{
  if (!valid(s)) return s;
  else {
    const Cell n{next(s)};
    if (n.first == std::numeric_limits<Cell::first_type>::max()) return s;
    for(unsigned i=1;i<=s.dimension();++i) {
      const auto ns = Sudoku(s).replace(n.first,n.second,i);
      const auto res = solve(ns);
      if (finished(res) && valid(res)) return res;
    }
    return s;
  }
}

int main()
{
  constexpr Sudoku sudoku{
    {
        9, 5, 0, 0, 2, 0, 0, 0, 0,
        0, 7, 8, 5, 0, 0, 0, 0, 0,
        0, 6, 0, 4, 0, 0, 0, 0, 0,
        0, 9, 0, 7, 3, 0, 0, 5, 0,
        8, 0, 0, 0, 0, 0, 0, 0, 2,
        0, 1, 0, 0, 8, 5, 0, 6, 0,
        0, 0, 0, 0, 0, 6, 0, 8, 0,
        0, 0, 0, 0, 0, 8, 7, 9, 0,
        0, 0, 0, 0, 7, 0, 0, 3, 1,
      }
  };
  cout << sudoku << endl;
  constexpr Sudoku res = solve(sudoku);
  cout << res << endl;
  return valid(res) && finished(res) ? 0 : 1;
}
