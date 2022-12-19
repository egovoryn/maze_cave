#include "widget.h"

#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget),
      path(QSize(500, 500), QImage::Format_ARGB32),
      image(QSize(500, 500), QImage::Format_ARGB32) {}

Widget::~Widget() { delete ui; }

void Widget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  QRect dirtyRect = event->rect();
  painter.drawImage(dirtyRect, image, dirtyRect);
  painter.drawImage(dirtyRect, path, dirtyRect);
}

void Widget::DrawMaze() {
  ClearImage(image);
  ClearImage(path);

  if (!file_name_.empty() && !GetCaveTab()) {
    QPainter painter;
    painter.begin(&image);

    QPen pen;
    pen.setColor(Qt::white);
    pen.setWidth(1);
    painter.setPen(pen);

    unsigned width = maze.GetWidth();
    unsigned height = maze.GetHeight();

    int h = 500 % height;
    height_pixel_ = (500 - h) / height - 0.02;
    int w = 500 % width;
    width_pixel_ = (500 - w) / width - 0.02;

    if (height_pixel_ < width_pixel_) {
      width_pixel_ = height_pixel_;
    } else {
      height_pixel_ = width_pixel_;
    }

    painter.drawLine(0, 0, 0, height_pixel_ * height);
    painter.drawLine(0, 0, width_pixel_ * width, 0);
    painter.drawLine(0, height_pixel_ * height, width_pixel_ * width,
                     height_pixel_ * height);
    painter.drawLine(width_pixel_ * width, 0, width_pixel_ * width,
                     height_pixel_ * height);

    for (unsigned i = 0; i < height; i++) {
      for (unsigned j = 0; j < width; j++) {
        if (maze.GetMaze()[i][j].right_wall_) {
          painter.drawLine((j + 1) * width_pixel_, i * height_pixel_,
                           (j + 1) * width_pixel_, (i + 1) * height_pixel_);
        }
        if (maze.GetMaze()[i][j].down_wall_) {
          painter.drawLine(j * width_pixel_, (i + 1) * height_pixel_,
                           (j + 1) * width_pixel_, (i + 1) * height_pixel_);
        }
      }
    }

    painter.end();
  }
}

void Widget::DrawCave() {
  ClearImage(image);
  ClearImage(path);

  if (!file_name_.empty() && GetCaveTab()) {
    QPainter painter;
    painter.begin(&image);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white);
    painter.setBrush(brush);

    QPen pen;
    pen.setStyle(Qt::NoPen);
    painter.setPen(pen);

    height_pixel_ = 500 / cave.GetHeight();
    width_pixel_ = 500 / cave.GetWidth();

    for (unsigned i = 1; i < cave.GetHeight() + 1; i++) {
      for (unsigned j = 1; j < cave.GetWidth() + 1; j++) {
        if (cave.GetCave()[i][j])
          painter.drawRect(width_pixel_ * (j - 1), height_pixel_ * (i - 1),
                           width_pixel_, height_pixel_);
      }
    }

    painter.end();
  }
}

void Widget::mousePressEvent(QMouseEvent *event) {
  Q_UNUSED(event);

  ClearImage(path);
  if (event->button() == Qt::LeftButton) {
    QPoint start_point_ = event->pos();
    start_pos_.second = start_point_.x() / width_pixel_;
    start_pos_.first = start_point_.y() / height_pixel_;
    left_click_ = true;

  } else if (event->button() == Qt::RightButton) {
    QPoint goal_point = event->pos();
    end_pos_.second = goal_point.x() / width_pixel_;
    end_pos_.first = goal_point.y() / height_pixel_;
    right_click_ = true;
  }

  if (left_click_ && right_click_) {
    DrawExitMaze();
    left_click_ = false;
    right_click_ = false;
  }
}

void Widget::DrawExitMaze() {
  QPainter painter;
  painter.begin(&path);

  maze.ExitFromMaze(start_pos_, end_pos_);

  QPen pen;

  pen.setColor(Qt::red);
  painter.setPen(pen);

  int b_wid = 0, b_heig = 0, a_wid = 0, a_heig = 0;
  bool turn = 0;
  float half = 0.5;

  while (!maze.GetPossWays().empty() &&
         maze.GetPossWays().back() != start_pos_) {
    if (turn == 0) {
      a_wid = maze.GetPossWays().back().second;
      a_heig = maze.GetPossWays().back().first;
    }
    maze.GetPossWays().pop_back();

    b_wid = maze.GetPossWays().back().second;
    b_heig = maze.GetPossWays().back().first;

    if (maze.GetMaze()[a_heig][a_wid].path_ -
                maze.GetMaze()[b_heig][b_wid].path_ ==
            1 ||
        maze.GetMaze()[b_heig][b_wid].path_ == -1) {
      turn = 0;
      painter.drawLine(a_wid * width_pixel_ + (width_pixel_ * half),
                       a_heig * height_pixel_ + (height_pixel_ * half),
                       b_wid * width_pixel_ + (width_pixel_ * half),
                       b_heig * height_pixel_ + (height_pixel_ * half));
    } else {
      turn = 1;
    }
  }

  update();
  painter.end();
}

void Widget::ClearImage(QImage &image) {
  image.fill(QColor(0, 0, 0, 0));
  update();
}
