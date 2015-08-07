#include "solver.h"

Solver::Solver(Buffer* buf, Sudoku s, QObject *parent) : QThread(parent), buffer{buf}, sudoku{s}
{

}

Sudoku Solver::solve(Sudoku s)
{
  buffer->put(s);
  if (!valid(s)) return s;
  //cout << s << endl;
  try {
    std::pair<size_t, size_t> n = next(s);
    for(unsigned i=1;i<=s.dimension();++i) {
      s(n.first,n.second) = i;
      Sudoku res = this->solve(s);
      if (::finished(res) && valid(res)) return res;
    }
  }
  catch (std::logic_error) {
  }

  return s;

}

void Solver::run()
{
  qDebug("solve started");
  auto s = solve(sudoku);
  std::cout << s << std::endl;
  buffer->put(s);
  qDebug("solve finished");
}
