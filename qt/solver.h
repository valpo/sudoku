#ifndef SOLVER_H
#define SOLVER_H

#include <QThread>
#include "sudoku.h"
#include "buffer.h"

class Solver : public QThread
{
  Q_OBJECT
  Buffer *buffer;
  Sudoku sudoku;
public:
  explicit Solver(Buffer* buf, Sudoku s, QObject *parent = 0);
  Sudoku solve(Sudoku s);
  void run() override;
signals:
  void ready();
public slots:
};

#endif // SOLVER_H
