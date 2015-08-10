#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QQueue>
#include <QTimer>
#include <QElapsedTimer>
#include "ui_mainwindow.h"
#include "sudoku.h"
#include "buffer.h"
#include <vector>

class SudokuLabel;
class Solver;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
  Q_OBJECT
  enum class State { IDLE, SOLVER_PLAYING, SOLVER_PAUSING };
  State state = State::IDLE;
  Sudoku sudoku, startGame;
  Buffer buffer;
  QTimer *viewRefresh;
  bool solverRunning;
  std::vector<SudokuLabel*> labelVec; // speed up access to labels
  QElapsedTimer *bencher = 0;
  Solver *solver = 0;
public:
  explicit MainWindow(QWidget *parent = 0);
  void showSudoku(const Sudoku& s);
public slots:
  void initSudoku();
private slots:
  void solverFinished();
  void refreshView(); // gets item from buffer and displays it
  void on_generateButton_clicked();
  void on_playButton_clicked();
  void on_resetButton_clicked();
  void on_nextButton_clicked();
};

#endif // MAINWINDOW_H
