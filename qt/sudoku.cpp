#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <cassert>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <iterator>
#include "sudoku.h"
#include <QDateTime>

using namespace std;


void Sudoku::generate()
{
  // empty data, create some random numbers, try to solve, clear many fields
  while (true) {
    typedef std::minstd_rand G;
    G g;
    g.seed(QDateTime::currentMSecsSinceEpoch());
    typedef std::uniform_int_distribution<> D;
    D d(0, data.size());
    unsigned fillCount = dimension()*dimension()/5;
    std::fill(data.begin(),data.end(),0);
    while (std::count_if(data.cbegin(),data.cend(),[](unsigned i){ return i>0; }) < fillCount) {
      size_t idx;
      do {
        idx = d(g);
      } while (data[idx] != 0);
      unsigned val;
      do {
        val = d(g)/dimension();
        data[idx] = val;
      } while (!valid(*this));
    }
    Sudoku solved = ::solve(*this);
    if (::valid(solved) && ::finished(solved)) {
      this->data = solved.data;
      fillCount = 2*dimension()*dimension()/3;
      while (fillCount) {
        unsigned x = d(g)/dimension();
        unsigned y = d(g)/dimension();
        if ((*this)(x,y)) {
          (*this)(x,y) = 0;
          --fillCount;
        }
      }
      break;
    }
    cout << "sudoku not solvabled, trying again" << endl;
  }
  cout << *this << endl;
}

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
