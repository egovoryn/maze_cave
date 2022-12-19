#ifndef SRC_MAZE_H_
#define SRC_MAZE_H_

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace s21 {
class Maze {
  struct Node {
    bool right_wall_{0};
    bool down_wall_{0};
    bool left_wall_{0};
    bool top_wall_{0};

    int path_{0};
  };

 public:
  void ClearAllMaze();

  unsigned GetWidth() { return width_; }
  unsigned GetHeight() { return height_; }
  std::vector<std::pair<int, int>> &GetPossWays() { return possible_ways_; }
  std::vector<std::vector<Node>> &GetMaze() { return maze_; }

  void SetWidth(unsigned x) { width_ = x; }
  void SetHeight(unsigned x) { height_ = x; }
  void SetSavePath(std::string x) { save_path_ = x; }

  void ParseFile(std::ifstream &file_);
  void GenerateMaze();
  void ExitFromMaze(std::pair<int, int> start, std::pair<int, int> end);

 private:
  std::vector<std::vector<Node>> maze_;
  std::vector<std::pair<int, int>> possible_ways_;
  std::string save_path_;

  unsigned width_;   //  first
  unsigned height_;  //  second

  void LeftTopWalls();
  void SetUniqueSet(std::vector<int> &line_, int &counter);
  void AddVertical(std::vector<int> &line_, int j);
  void AddHorizontal(std::vector<int> &line_, int j);
  void CheckHorizontal(std::vector<int> &line_, int j);
  void PreparateNewLine(std::vector<int> &line_, int j);
  void AddEndLine(std::vector<int> &line_);
  void CheckEndLine(std::vector<int> &line_);
  int CalculateHorizontal(std::vector<int> line_, int j, int index);
  int CalculateUniqueSet(std::vector<int> line_, int index);
  void MergeSet(int index, std::vector<int> &line_);
  void SaveLine(std::vector<int> line_);
  void SaveMaze();
  void ClearPath();
};
}  //  namespace s21

#endif  //  SRC_MAZE_H_
