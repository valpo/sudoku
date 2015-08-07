#include <QtGui>
#include "sudokulabel.h"

FontCache SudokuLabel::cache(SudokuLabel::minFontSize);

SudokuLabel::SudokuLabel(unsigned max, QWidget *parent) : QWidget(parent), maxVal(max), font("Arial")
{
  setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
  font.setPointSize(minFontSize);
}

QSize SudokuLabel::sizeHint() const
{
  QFontMetrics fm(font);
  int w = fm.width(QString::number(value));
  int h = fm.height();
  return QSize(w,h);
}

void SudokuLabel::setValue(unsigned v)
{
  if (v != value) {
    value = v;
    //update();
  }
}

void SudokuLabel::paintEvent(QPaintEvent *event)
{
  QPixmap* pix = cache.pix(font.pointSize(), value);
  QRect cr = contentsRect();
  if (!pix) {
    pix = new QPixmap(cr.width(),cr.height());
    QPainter painter(pix);
    painter.eraseRect(cr);
    if (value > 0) {
      painter.setFont(font);
      painter.drawText(cr,Qt::AlignCenter|Qt::AlignVCenter,QString::number(value));
    }
    cache.add(font.pointSize(),value,pix);
  }
  QPainter painter(this);
  painter.drawPixmap(cr,*pix);
}

void SudokuLabel::resizeEvent(QResizeEvent *event)
{
  auto size = event->size();
  if (size != sizeForFont) {
    for(int pi = 1;pi;++pi) {
      if (pi > 50) {
        //
      }
      font.setPointSize(pi);
      QFontMetrics fm(font);
      int w = fm.maxWidth();  //width(QString::number(value));
      int h = fm.height();
      if (w < size.width() && h < size.height()) {
      }
      else {
        font.setPointSize(pi-1);
        sizeForFont = size;
        break;
      }
    }
  }
  QWidget::resizeEvent(event);
}

