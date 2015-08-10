#include <iostream>
#include <QDebug>
#include "sudokulabel.h"
#include "mainwindow.h"
#include "solver.h"

using namespace std;

static unsigned SudokuIn[9][9] = {
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

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), sudoku(9), startGame(9)
{
  setupUi(this);
  initSudoku();
  viewRefresh = new QTimer(this);
  connect(viewRefresh, SIGNAL(timeout()),this,SLOT(refreshView()));
}

void MainWindow::showSudoku(const Sudoku &s)
{
#if 0
  if (!bencher) {
    bencher = new QElapsedTimer;
  }
  else {
    qDebug()<< "showSudoku bencher " << bencher->nsecsElapsed()/(1000*1000);
  }
#endif
  QWidget *w = stackedWidget->currentWidget();
  QGridLayout *l = qobject_cast<QGridLayout*>(w->layout());
  if (!l) return;
  Q_ASSERT( (l->rowCount() == l->columnCount()) && (l->columnCount() == s.dimension()));
  if (labelVec.empty()) {
    labelVec.reserve(s.dimension()*s.dimension());
    for(size_t x=0;x<s.dimension();++x)
      for(size_t y=0;y<s.dimension();++y) {
        SudokuLabel *label = qobject_cast<SudokuLabel*>(l->itemAtPosition(x,y)->widget()); // itemAtPosition is expensive
        labelVec.push_back(label);
      }
  }
  for(size_t x=0;x<s.dimension();++x)
    for(size_t y=0;y<s.dimension();++y) {
      //SudokuLabel *label = qobject_cast<SudokuLabel*>(l->itemAtPosition(x,y)->widget());
      SudokuLabel *label = labelVec.at(x*s.dimension()+y);
      label->setValue(s(x,y));
    }
  update();
#if 0
  repaint();
  bencher->start();
#endif
}

void MainWindow::initSudoku()
{
  const auto size = spinBox->value();
  sudoku = Sudoku(size*size);
  sudoku.generate();
  QWidget *w = new QWidget;
  QGridLayout *l = new QGridLayout(w);
  for(int x=0;x<size*size;++x)
    for(int y=0;y<size*size;++y) {
      auto label = new SudokuLabel(y);
      l->addWidget(label, x, y);
    }
  auto idx = stackedWidget->addWidget(w);
  stackedWidget->setCurrentIndex(idx);
  labelVec.clear();

  showSudoku(sudoku);
}

void MainWindow::solverFinished()
{
  qDebug() << "solver finished\n";
  solverRunning = false;
}

void MainWindow::refreshView()
{
  if (!solverRunning && buffer.empty()) {
    viewRefresh->stop();
    qDebug() << "play finished\n";
    return;
  }
  try {
    Sudoku s = buffer.get();
    showSudoku(s);
  }
  catch (std::logic_error&) {
  }
}

void MainWindow::on_generateButton_clicked()
{
  qDebug("generate sudoku");
  viewRefresh->stop();
  while (solver) {
    bool res = solver->wait();
    if (res) {
      solver->deleteLater();
      solver = nullptr;
    }
  }
  buffer.clear();
  state = State::IDLE;
  sudoku.generate();
  startGame = sudoku;
  showSudoku(sudoku);
  qDebug("generate sudoku finished");
}

void MainWindow::on_playButton_clicked()
{
  if (state == State::SOLVER_PAUSING or state == State::IDLE) {
    if (!solver) {
      startGame = sudoku;
      solver = new Solver(&buffer, sudoku, this);
      connect(solver, SIGNAL(finished()), this, SLOT(solverFinished()));
      solver->start();
      solverRunning = true;
    }
    viewRefresh->start(0);
    playButton->setText("Stop");
    state = State::SOLVER_PLAYING;
  }
  else if (state == State::SOLVER_PLAYING) {
    viewRefresh->stop();
    playButton->setText("Play");
    state = State::SOLVER_PAUSING;
  }
}

void MainWindow::on_resetButton_clicked()
{
  viewRefresh->stop();
  while (solver) {
    bool res = solver->wait();
    if (res) {
      solver->deleteLater();
      solver = nullptr;
    }
  }
  buffer.clear();
  sudoku = startGame;
  showSudoku(sudoku);
  state = State::IDLE;
  playButton->setText("Play");
}

void MainWindow::on_nextButton_clicked()
{

  viewRefresh->stop();
  if (!solver) {
    startGame = sudoku;
    solver = new Solver(&buffer, sudoku, this);
    connect(solver, SIGNAL(finished()), this, SLOT(solverFinished()));
    solver->start();
    solverRunning = true;
  }
  state = State::SOLVER_PAUSING;
  playButton->setText("Play");
  refreshView();
}
