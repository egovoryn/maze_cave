#ifndef WIDGET_H
#define WIDGET_H

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "Cave/cave.h"
#include "Maze/maze.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget {
  Q_OBJECT

 public:
  explicit Widget(QWidget *parent = nullptr);
  ~Widget();

  void SetFileName(std::string str) { file_name_ = str; }
  s21::Cave &GetCave() { return cave; }
  s21::Maze &GetMaze() { return maze; }
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *event) override;
  void ExitPath();

  void DrawCave();

  void DrawMaze();
  void DrawExitMaze();

  bool GetCaveTab() { return is_cave_tab; }
  void SetCaveTab(bool x) { is_cave_tab = x; }

  void ClearImage(QImage &image);
  QImage &GetImage() { return image; }
  QImage &GetPath() { return path; }

 private:
  Ui::Widget *ui;
  s21::Cave cave;
  s21::Maze maze;
  std::string file_name_;

  QImage path;
  QImage image;

  bool left_click_;
  bool right_click_;

  std::pair<int, int> start_pos_;
  std::pair<int, int> end_pos_;

  //    QPainter painter;

  double height_pixel_;
  double width_pixel_;

  bool is_cave_tab = 0;  // 0 – maze, 1 – cave
};

#endif  // WIDGET_H
