#ifndef SRC_CAVE_H_
#define SRC_CAVE_H_

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace s21 {
class Cave {
 public:
  std::vector<std::vector<unsigned>> &GetCave() { return cave_; }
  unsigned GetWidth() { return width_; }
  unsigned GetHeight() { return height_; }

  void SetBirthLimit(unsigned x) { birth_limit_ = x; }
  void SetDeathLimit(unsigned x) { death_limit_ = x; }
  void SetCave(std::vector<std::vector<unsigned>> cave) {
    std::swap(cave_, cave);
  }

  void ParseFile(std::ifstream &file_);
  void CaveLogic();
  void ClearCaveAll();

 private:
  std::vector<std::vector<unsigned>> cave_;

  unsigned width_{0};   //  first
  unsigned height_{0};  //  second

  unsigned birth_limit_{4};
  unsigned death_limit_{3};
};
}  //  namespace s21

#endif  //  SRC_CAVE_H_
