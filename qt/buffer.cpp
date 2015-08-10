#include <QMutexLocker>
#include <QDebug>
#include "buffer.h"

Buffer::Buffer(QObject *parent) : QObject(parent), pcount(0), gcount(0), empties(0), fulls(0)
{

}

Buffer::~Buffer()
{
  qDebug("empty: %d, full %d",empties,fulls);
}



void Buffer::put(const Sudoku &s)
{
  QMutexLocker locker(&lock);
  while (buffer.size() >= MAX_SIZE) {
    ++fulls;
    //qDebug() << "full";
    waitNotFull.wait(&lock);
  }
  buffer.enqueue(s);
  pcount++;
  waitNotEmpty.wakeAll();
  emit added();
}

Sudoku Buffer::get()
{
  QMutexLocker locker(&lock);
  int waitcount = 0;
  while (waitcount < 10 && buffer.size() == 0) {
    ++waitcount;
    ++empties;
    //qDebug() << "empty";
    waitNotEmpty.wait(&lock, 100);
  }
  if (buffer.isEmpty()) { throw std::logic_error("cannot get from empty buffer"); }
  auto res = buffer.dequeue();
  ++gcount;
  waitNotFull.wakeAll();
  return res;
}

bool Buffer::empty() const
{
  return buffer.isEmpty();
}

void Buffer::clear()
{
  QMutexLocker locker(&lock);
  buffer.clear();
}

