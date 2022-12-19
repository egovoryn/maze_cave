#include <gtest/gtest.h>
#include <iostream>
#include "Maze/maze.h"
#include "Cave/cave.h"

TEST(maze_case, generate_test) {
  s21::Maze maze_;
  maze_.SetHeight(7);
  maze_.SetWidth(3);

  ASSERT_EQ(maze_.GetHeight(), 7);
  ASSERT_EQ(maze_.GetWidth(), 3);
}

TEST(maze_case, open_test) {
  s21::Maze maze_;
  std::ifstream file_("new_maze.txt");
  maze_.ParseFile(file_);

  ASSERT_EQ(maze_.GetHeight(), 5);
  ASSERT_EQ(maze_.GetWidth(), 5);
}

TEST(maze_case, find_test) {
  std::vector<std::pair<int, int>> test_walker_ = {
      {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
      {1, 4}, {2, 4}, {3, 4}, {4, 4}, {3, 3}};
  s21::Maze maze_;
  std::ifstream file_("new_maze.txt");
  maze_.ParseFile(file_);

  maze_.ExitFromMaze(std::pair<int, int>(0, 0), std::pair<int, int>(3, 3));

  std::vector<std::pair<int, int>> way_walker_ = maze_.GetPossWays();

  for (unsigned i = 0; i < way_walker_.size(); i++) {
    ASSERT_EQ(way_walker_[i].second, test_walker_[i].first);
    ASSERT_EQ(way_walker_[i].first, test_walker_[i].second);
  }
}

TEST(generateMaze, one_step_test) {
  s21::Maze test;
  test.SetHeight(5);
  test.SetWidth(4);
  test.GenerateMaze();
  test.GetWidth();
  ASSERT_EQ(test.GetWidth(), 4);

  test.ClearAllMaze();
  ASSERT_EQ(test.GetHeight(), 0);
  ASSERT_EQ(test.GetWidth(), 0);
}

TEST(all_live, all_live_test) {
  s21::Cave test;
  std::ifstream file_("new_cave.txt");
  test.ParseFile(file_);

  ASSERT_EQ(test.GetWidth(), 4);
  ASSERT_EQ(test.GetHeight(), 4);

  for (unsigned i = 1; i < test.GetHeight() + 1; i++) {
    for (unsigned j = 1; j < test.GetWidth() + 1; j++) {
      ASSERT_EQ(test.GetCave()[i][j], 0);
    }
  }

	test.CaveLogic();

  for (unsigned i = 1; i < test.GetHeight() + 1; i += 3) {
    for (unsigned j = 1; j < test.GetWidth() + 1; j += 3) {
      ASSERT_EQ(test.GetCave()[i][j], 1);
    }
  }

  test.ClearCaveAll();
  ASSERT_EQ(test.GetHeight(), 0);
  ASSERT_EQ(test.GetWidth(), 0);
}

int main() {
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
