CXX = g++
STD = -std=c++17
CFLAG = -Wall -Wextra -Werror -pedantic
TEST_FILES = Cave/cave.cc Maze/maze.cc test.cc
GTEST = -lgtest

all: build test

build: clean
	mkdir build
	cd ./build/ && qmake6 ../ && make
	cd ./build/ && rm -rf *.o *.cpp *.h Makefile *.stash
	open build

install: build
	mkdir $(HOME)/Desktop/MazeApp/
	cp -rf build/* $(HOME)/Desktop/MazeApp/

uninstall:
	rm -rf $(HOME)/Desktop/MazeApp

dvi:
	open maze.html

dist:
	@rm -rf backup_maze
	@mkdir backup_maze
	@cp Makefile Maze/* Cave/* *.h *.cpp *.ui *.pro backup_maze/
	@tar -cvzf $(HOME)/Desktop/backup_maze.tgz backup_maze/
	@rm -rf backup_maze/

test: clean
	$(CXX) $(STD) $(CFLAG) -o test $(TEST_FILES) $(GTEST)
	./test

gcov_report: clean
	$(CXX) $(STD) $(CFLAG) -o test $(TEST_FILES) $(GTEST) --coverage
	./test || true
	lcov -t "test" -o test.info --no-external -c -d .
	genhtml -o report test.info
	open report/index.html

clean: uninstall
	rm -rf build
	rm -rf *.o  *.gc* report *.a test.info \
	build *.out *.pro.user *.pro.user.* \
	*.dSYM test

rebuild: clean all

leaks: test
	CK_FORK=no leaks --atExit -- ./test
