#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <fstream>

#include "widget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void TimerSlot();

  void on_limit_death_valueChanged(int arg1);

  void on_limit_birth_valueChanged(int arg1);

  void on_open_file_clicked();

  void on_next_step_clicked();

  void on_auto_mode_clicked();

  void on_for_auto_timer_valueChanged(double arg1);

  void on_tabWidget_currentChanged(int index);

  void on_generate_maze_clicked();

 private:
  Ui::MainWindow *ui;
  QTimer *timer_;
  int time_;
  bool auto_ = 0;

  void DefaultSettings();
};
#endif  // MAINWINDOW_H
