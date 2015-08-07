#ifndef SUDOKULABEL_H
#define SUDOKULABEL_H

#include <QWidget>
#include <set>
#include <QDebug>
#include <QCache>

struct CacheKey {
  int fontSize;
  unsigned value;
  CacheKey(int fs, unsigned val) : fontSize{fs}, value{val} {}
};
inline bool operator<(const CacheKey& l, const CacheKey& r)
{
  if (l.fontSize < r.fontSize) return true;
  else if (l.fontSize == r.fontSize && l.value < r.value) return true;
  else return false;
}
inline uint qHash(const CacheKey &key)
{
  return qHash(key.fontSize) ^ qHash(key.value);
}
inline bool operator==(const CacheKey &l, const CacheKey& r)
{
  return l.fontSize == r.fontSize && l.value == r.value;
}

struct FontCache {
  int fontSize;
  QCache<CacheKey, QPixmap> images;
  FontCache(int fs = 0) : fontSize(fs) {}
  void reset(int fs) {
    if (fs != fontSize) {
      images.clear();
      fontSize = fs;
    }
  }
  QPixmap* pix(int fs, unsigned val) {
    //qDebug() << "get pix " << fs << val;
    try {
      return images.object({fs,val});
    }
    catch (std::out_of_range) {
      return nullptr;
    }
  }
  void add(int fs,unsigned value, QPixmap* pix) {
    //qDebug() << "add pix " << fs << value;
    images.insert({fs,value}, pix);
  }
};

class SudokuLabel : public QWidget
{
  Q_OBJECT
  const unsigned maxVal;
  static const int minFontSize = 9;
  unsigned value = 0;
  static FontCache cache;
  QFont font;
  QSize sizeForFont; // font is set for this size
public:
  explicit SudokuLabel(unsigned max, QWidget *parent = 0);
  QSize sizeHint() const override;
  void setValue(unsigned v);
protected:
  void paintEvent(QPaintEvent * event) override;
  void resizeEvent(QResizeEvent *event) override;
signals:

public slots:
};

#endif // SUDOKULABEL_H
