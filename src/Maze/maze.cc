#include "maze.h"

namespace s21 {

void Maze::ParseFile(std::ifstream &file_) {
	std::string read_string_;
	std::stringstream ss_;
	std::vector<Node> row_;
	bool x_;
	bool is_first_block = true;
    unsigned i = 0;

	std::getline(file_, read_string_);

	height_ = std::atof(read_string_.substr(0).c_str());
	width_ = std::atof(read_string_.substr(2).c_str());

	while (!file_.eof()) {
		std::getline(file_, read_string_);

		if (!read_string_.empty()) {
		ss_ << read_string_;

		if (is_first_block == true) {
			while (ss_ >> x_) {
                Node node_;
                if (x_ == 1) {
                    node_.right_wall_ = true;
                } else {
                    node_.right_wall_ = false;
                }
                    row_.push_back(node_);
			}
			maze_.push_back(row_);
		} else {
			unsigned j = 0;
			while (ss_ >> x_) {
                if (x_ == 1) {
                    maze_[i][j].down_wall_ = true;
                } else {
                    maze_[i][j].down_wall_ = false;
                }
                    j++;
			}
            i++;
		}
            ss_.clear();
            row_.clear();
		} else {
            is_first_block = 0;
		}
	}

    if (i != height_) throw(true);

	file_.close();
	LeftTopWalls();
}

void Maze::LeftTopWalls() {
  for (unsigned i = 0; i < height_; i++) {
    for (unsigned j = 0; j < width_; j++) {
      if (i >= maze_.size() || j >= maze_[i].size()) throw(true);

      if (i == 0) maze_[i][j].top_wall_ = true;
      if (j == 0) maze_[i][j].left_wall_ = true;

      if ((maze_[i][j].down_wall_) == 1 && i + 1 < height_) {
        maze_[i + 1][j].top_wall_ = true;
      }
      if (maze_[i][j].right_wall_ == 1 && j + 1 < width_) {
        maze_[i][j + 1].left_wall_ = true;
      }
    }
  }
}

void Maze::ClearPath() {
  for (unsigned i = 0; i < height_; i++) {
    for (unsigned j = 0; j < width_; j++) {
      maze_[i][j].path_ = 0;
    }
  }
}

void Maze::ExitFromMaze(std::pair<int, int> start, std::pair<int, int> end) {
  possible_ways_.clear();
  ClearPath();

  possible_ways_.push_back({start.first, start.second});
  int local_path = 1;
  bool deadlock = 0;

  while (!possible_ways_.empty() && possible_ways_.back() != end) {
    int y = possible_ways_.back().first;
    int x = possible_ways_.back().second;

    deadlock = 1;

    if (maze_[y][x].right_wall_ == 0 && x + 1 < (int)width_ &&
        maze_[y][x + 1].path_ == 0) {
      maze_[y][x + 1].path_ = maze_[y][x].path_ + 1;
      possible_ways_.push_back({y, x + 1});
      deadlock = 0;
    }
    if (maze_[y][x].left_wall_ == 0 && x - 1 >= 0 &&
        maze_[y][x - 1].path_ == 0) {
      maze_[y][x - 1].path_ += maze_[y][x].path_ + 1;
      possible_ways_.push_back({y, x - 1});
      deadlock = 0;
    }
    if (maze_[y][x].top_wall_ == 0 && y - 1 >= 0 &&
        maze_[y - 1][x].path_ == 0) {
      maze_[y - 1][x].path_ += maze_[y][x].path_ + 1;
      possible_ways_.push_back({y - 1, x});
      deadlock = 0;
    }
    if (maze_[y][x].down_wall_ == 0 && y + 1 < (int)height_ &&
        maze_[y + 1][x].path_ == 0) {
      maze_[y + 1][x].path_ += maze_[y][x].path_ + 1;
      possible_ways_.push_back({y + 1, x});
      deadlock = 0;
    }
    if (deadlock) possible_ways_.pop_back();
    local_path++;
    maze_[start.first][start.second].path_ = -1;
  }
}

void Maze::GenerateMaze() {
  std::vector<int> line_(width_);
  int counter = 1;

  maze_.resize(height_);
  for (unsigned i = 0; i < height_; i++) {
    maze_[i].resize(width_);
  }

  for (unsigned j = 0; j < height_ - 1; j++) {
    SetUniqueSet(line_, counter);
    AddVertical(line_, j);
    AddHorizontal(line_, j);
    CheckHorizontal(line_, j);
    PreparateNewLine(line_, j);
  }
  SetUniqueSet(line_, counter);
  AddVertical(line_, height_ - 1);
  CheckEndLine(line_);
  SaveMaze();
}

void Maze::SaveMaze() {
  std::fstream save;
  save.open(save_path_, std::ios::out);

  save << height_ << " " << width_ << std::endl;

  for (unsigned i = 0; i < height_; i++) {
    for (unsigned j = 0; j < width_; j++) {
      save << maze_[i][j].right_wall_ << " ";
    }
    save << std::endl;
  }

  save << std::endl;

  for (unsigned i = 0; i < height_; i++) {
    for (unsigned j = 0; j < width_; j++) {
      save << maze_[i][j].down_wall_ << " ";
    }
    save << std::endl;
  }
}

void Maze::CheckEndLine(std::vector<int> &line_) {
  for (unsigned i = 0; i < width_ - 1; i++) {
    if (line_[i] != line_[i + 1]) {
      maze_[height_ - 1][i].right_wall_ = false;
      MergeSet(i, line_);
    }
    maze_[height_ - 1][i].down_wall_ = true;
  }
  maze_[height_ - 1][width_ - 1].down_wall_ = true;
}

void Maze::PreparateNewLine(std::vector<int> &line_, int j) {
  for (unsigned i = 0; i < width_; i++) {
    if (maze_[j][i].down_wall_) {
      line_[i] = 0;
    }
  }
}

void Maze::CheckHorizontal(std::vector<int> &line_, int j) {
  for (unsigned i = 0; i < width_; i++) {
    if (CalculateHorizontal(line_, j, i) == 0) {
      maze_[j][i].down_wall_ = false;
    }
  }
}

int Maze::CalculateHorizontal(std::vector<int> line_, int j, int index) {
  int countHorizontalWalls = 0;
  int element = line_[index];
  for (unsigned i = 0; i < width_; i++) {
    if (line_[i] == element && maze_[j][i].down_wall_ == false) {
      countHorizontalWalls++;
    }
  }
  return countHorizontalWalls;
}

void Maze::AddHorizontal(std::vector<int> &line_, int j) {
  for (unsigned i = 0; i < width_; i++) {
    if (CalculateUniqueSet(line_, i) == 1) {
      maze_[j][i].down_wall_ = false;
    } else {
      bool choise = rand() % 2;
      maze_[j][i].down_wall_ = choise;
    }
  }
}

int Maze::CalculateUniqueSet(std::vector<int> line_, int index) {
  int countUniqSet = 0;
  for (unsigned i = 0; i < line_.size(); i++) {
    if (line_[i] == line_[index]) {
      countUniqSet++;
    }
  }
  return countUniqSet;
}

void Maze::SetUniqueSet(std::vector<int> &line_, int &counter) {
  for (unsigned i = 0; i < width_; i++) {
    if (line_[i] == 0) {
      line_[i] = counter;
      counter++;
    }
  }
}

void Maze::AddVertical(std::vector<int> &line_, int row) {
  for (unsigned i = 0; i < width_ - 1; i++) {
    bool choise = rand() % 2;
    if (choise || line_[i] == line_[i + 1]) {
      maze_[row][i].right_wall_ = true;
    } else {
      MergeSet(i, line_);
    }
  }
  maze_[row][width_ - 1].right_wall_ = true;
}

void Maze::MergeSet(int index, std::vector<int> &line_) {
  int mutableSet = line_[index + 1];
  int element = line_[index];
  for (unsigned j = 0; j < line_.size(); j++) {
    if (line_[j] == mutableSet) {
      line_[j] = element;
    }
  }
}

void Maze::ClearAllMaze() {
  if (width_ && height_) {
    for (unsigned i = 0; i < maze_.size(); i++) {
      for (unsigned j = 0; j < maze_[i].size(); j++) {
        maze_[i][j].down_wall_ = 0;
        maze_[i][j].left_wall_ = 0;
        maze_[i][j].right_wall_ = 0;
        maze_[i][j].top_wall_ = 0;

        maze_[i][j].path_ = 0;
      }
    }
    width_ = 0;
    height_ = 0;
    if (!maze_.empty()) maze_.clear();
  }
}

}  // namespace s21