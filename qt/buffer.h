#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include "sudoku.h"

class Buffer : public QObject
{
  Q_OBJECT
  QQueue<Sudoku> buffer;
  QMutex lock;
  QWaitCondition waitNotEmpty;
  QWaitCondition waitNotFull;
  const QQueue<Sudoku>::size_type MAX_SIZE = 10000;
  int pcount, gcount;
  unsigned empties, fulls;
public:
  explicit Buffer(QObject *parent = 0);
  ~Buffer();
  void put(const Sudoku& s);
  Sudoku get();
  bool empty() const;

signals:
  void added(); //  emitted if item added
public slots:
};

#endif // BUFFER_H
