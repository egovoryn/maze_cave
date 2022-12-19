#include "cave.h"

namespace s21 {

void Cave::ParseFile(std::ifstream &file_) {
  std::string read_string_;
  std::stringstream ss_;
  std::vector<unsigned> row_;
  unsigned x_;

  std::getline(file_, read_string_);

  width_ = std::atof(read_string_.substr(0).c_str());
  height_ = std::atof(read_string_.substr(2).c_str());

  std::vector<unsigned> border_(width_ + 2, 1);

  cave_.push_back(border_);

  while (!file_.eof()) {
    std::getline(file_, read_string_);
    if (!read_string_.empty()) {
      ss_ << read_string_;

      row_.push_back(1);
      while (ss_ >> x_) {
        row_.push_back(x_);
      }
      row_.push_back(1);
      cave_.push_back(row_);

      ss_.clear();
      row_.clear();
    }
  }

  if (height_ != cave_.size() - 1) throw(true);
  cave_.push_back(border_);
  file_.close();
}

void Cave::CaveLogic() {
  unsigned count;
  std::vector<std::vector<unsigned>> tmp_(height_ + 2,
                                          std::vector<unsigned>(width_ + 2));

  for (unsigned i = 0; i < height_ + 2; i++) {
    for (unsigned j = 0; j < width_ + 2; j++) {
      if (i == 0 || i - height_ - 1 == 0 || j == 0 || j - width_ - 1 == 0) {
        tmp_[i][j] = 1;
      } else {
        count = cave_[i - 1][j - 1] + cave_[i - 1][j] + cave_[i - 1][j + 1] +
                cave_[i][j - 1] + cave_[i][j + 1] + cave_[i + 1][j - 1] +
                cave_[i + 1][j] + cave_[i + 1][j + 1];

        if (count < death_limit_ && cave_[i][j] == 1)
          tmp_[i][j] = 0;
        else if (count > birth_limit_ && cave_[i][j] == 0)
          tmp_[i][j] = 1;
        else
          tmp_[i][j] = cave_[i][j];
      }
    }
  }

  SetCave(tmp_);
}

void Cave::ClearCaveAll() {
  cave_.clear();
  width_ = 0;
  height_ = 0;
  birth_limit_ = 0;
  death_limit_ = 0;
}

}  //   namespace s21
