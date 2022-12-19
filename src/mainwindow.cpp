#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::TimerSlot() {
  if (!ui->area->GetCave().GetCave().empty()) {
    ui->area->GetCave().CaveLogic();
    ui->area->DrawCave();
  }
}

void MainWindow::on_limit_death_valueChanged(int arg1) {
  ui->area->GetCave().SetDeathLimit(arg1);
}

void MainWindow::on_limit_birth_valueChanged(int arg1) {
  ui->area->GetCave().SetBirthLimit(arg1);
}

void MainWindow::on_open_file_clicked() {
  QString filename = QFileDialog::getOpenFileName(
      this, tr("Open File"), "../../../../src/Files/.", "*.txt");
  ui->area->SetFileName(filename.toStdString());
  if (!filename.isEmpty()) {
      bool err = 0;
          std::ifstream file_(filename.toStdString());
          if (ui->area->GetCaveTab() == 1) {
            ui->area->GetCave().GetCave().clear();
            try {
                ui->area->GetCave().ParseFile(file_);
            } catch (bool e) {
                ui->area->GetCave().GetCave().clear();
                DefaultSettings();
                err = e;
               std::cerr << "Error of Cave\n";
            }
            if (!err) ui->area->DrawCave();
          } else {
            ui->area->GetMaze().ClearAllMaze();
            try {
                ui->area->GetMaze().ParseFile(file_);
            } catch (bool e) {
                ui->area->GetMaze().ClearAllMaze();
                err = e;
                DefaultSettings();
               std::cerr << "Error of Maze\n";
            }
            if (!err) ui->area->DrawMaze();
          }
  }
}

void MainWindow::on_next_step_clicked() {
  if (!ui->area->GetCave().GetCave().empty()) {
    ui->area->GetCave().CaveLogic();

    ui->area->GetCave().SetBirthLimit(ui->limit_birth->value());
    ui->area->GetCave().SetDeathLimit(ui->limit_death->value());

    ui->area->DrawCave();
  }
}

void MainWindow::on_auto_mode_clicked() {
  if (!auto_) {
    auto_ = 1;
    ui->auto_mode->setStyleSheet(
        "color: black;"
        "background-color: white;");
    time_ = 0;
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(TimerSlot()));
    timer_->start(ui->for_auto_timer->value());
  } else {
    auto_ = 0;
    if (timer_->isActive()) timer_->stop();
    ui->auto_mode->setStyleSheet(
        "color: white;"
        "background-color: grey;"
        "border-style: outset;"
        "border-width: 0.5px;"
        "border-color: white;");
  }
}

void MainWindow::on_for_auto_timer_valueChanged(double arg1) {
  if (auto_) {
    timer_->stop();
    timer_->start(arg1);
  }
}

void MainWindow::on_tabWidget_currentChanged(int index) {
  DefaultSettings();
  ui->area->SetCaveTab(index);
}

void MainWindow::DefaultSettings() {
    ui->area->ClearImage(ui->area->GetImage());
    ui->area->ClearImage(ui->area->GetPath());
    ui->area->SetFileName("");

    if (ui->area->GetCaveTab() && auto_ == 1) {
        timer_->stop();
    }

    auto_ = 0;
    ui->auto_mode->setStyleSheet(
        "color: white;"
        "background-color: grey;"
        "border-style: outset;"
        "border-width: 0.5px;"
        "border-color: white;");

    ui->for_auto_timer->setValue(3000);
    ui->limit_birth->setValue(4);
    ui->limit_death->setValue(3);
    ui->height_maze->setValue(10);
    ui->width_maze->setValue(10);
}

void MainWindow::on_generate_maze_clicked() {
  if (!ui->area->GetMaze().GetMaze().empty())
    ui->area->GetMaze().ClearAllMaze();

  srand(time(0));
  ui->area->GetMaze().SetHeight(ui->height_maze->value());
  ui->area->GetMaze().SetWidth(ui->width_maze->value());

  QString save_filename = QDir::homePath() + "/new_maze_1.txt";

  ui->area->SetFileName(save_filename.toStdString());
  ui->area->GetMaze().SetSavePath(save_filename.toStdString());

  std::ifstream file_(save_filename.toStdString());

  ui->area->GetMaze().GenerateMaze();

  ui->area->GetMaze().ParseFile(file_);
  ui->area->DrawMaze();
}
