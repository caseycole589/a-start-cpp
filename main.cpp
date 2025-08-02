#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::abs;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;

enum class State { kEmpty, kObstacle, kClosed };

vector<State> ParseLine(const string &line) {
  istringstream sline(line);
  int n;
  char c;
  vector<State> row;
  while (sline >> n >> c && c == ',') {
    if (n == 0) {
      row.push_back(State::kEmpty);
    } else {
      row.push_back(State::kObstacle);
    }
  }

  return row;
}

vector<vector<State>> ReadBoardFile(const string &path) {
  ifstream myfile(path);
  vector<vector<State>> board;
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

// just for printing out the board
string CellString(State cell) {
  switch (cell) {
  case State::kObstacle:
    return "⛰️   ";
  default:
    return "0   ";
  }
}

// for visualizing
void PrintBoard(const vector<vector<State>> &board) {
  for (auto row : board) {
    for (auto j : row) {
      cout << CellString(j);
    }
    cout << "\n";
  }
}

// we want to compare f = g+h for these two nodes
bool Compare(vector<int> &node1, vector<int> &node2) {
  // f1 g+h > f2 g+h
  return node1[2] + node1[3] > node2[2] + node2[3];
}

// distance
int Heuristic(const int x1, const int y1, const int x2, const int y2) {
  int result = std::abs(x2 - x1) + std::abs(y2 - y1);
  return result;
}

void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openList,
               vector<vector<State>> &grid) {
  vector<int> node{x, y, g, h};
  openList.push_back(node);
  grid[x][y] = State::kClosed;
}

vector<vector<State>> SearchBoard(vector<vector<State>> &grid, int init[2],
                                  int goal[2]) {
  vector<vector<int>> open{};

  int x = init[0];
  int y = init[1];
  // initial cost
  int g = 0;
  int h = Heuristic(0, 0, goal[0], goal[1]);
  AddToOpen(x, y, g, h, open, grid);
  return vector<vector<State>>{};
}

int main() {
  int init[2] = {0, 0};
  int goal[2] = {4, 5};

  auto board = ReadBoardFile("1.board");
  auto solution = SearchBoard(board, init, goal);
  int an = Heuristic(1, 2, 3, 4);
  PrintBoard(board);

  cout << "\n";
}
