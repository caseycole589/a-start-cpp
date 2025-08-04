#include <algorithm>
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
using std::sort;
using std::string;
using std::vector;

// directional deltas for testing
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

enum class State { kEmpty, kObstacle, kClosed, kPath, kStart, kFinish };

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
  case State::kPath:
    return "🚗   ";
  case State::kStart:
    return "🚦   ";
  case State::kFinish:
    return "🏁   ";
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
  // f1(g+h) > f2(g+h)
  return node1[2] + node1[3] > node2[2] + node2[3];
}

void CellSort(vector<vector<int>> *v) { sort(v->begin(), v->end(), Compare); }

// distance
int Heuristic(const int x1, const int y1, const int x2, const int y2) {
  int result = std::abs(x2 - x1) + std::abs(y2 - y1);
  return result;
}

bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
  if ((x >= 0 && x < grid.size()) && (y >= 0 && y < grid[0].size()))
    return grid[x][y] == State::kEmpty;
  return false;
}
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openList,
               vector<vector<State>> &grid) {
  vector<int> node{x, y, g, h};
  openList.push_back(node);
  grid[x][y] = State::kClosed;
}

void ExpandNeighbors(const vector<int> &currentNode, int goal[2],
                     vector<vector<int>> &open, vector<vector<State>> &grid) {
  int x = currentNode[0];
  int y = currentNode[1];
  int g = currentNode[2];
  for (int i = 0; i < 4; i++) {
    int x2 = x + delta[i][0];
    int y2 = y + delta[i][1];

    if (CheckValidCell(x2, y2, grid)) {
      int g2 = g + 1;
      int h2 = Heuristic(x2, y2, goal[0], goal[1]);
      AddToOpen(x2, y2, g2, h2, open, grid);
    }
  }
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

  while (!open.empty()) {
    // sort the open list in a descending order to have the node with the lowest
    // h value at the end.
    CellSort(&open);
    auto currentNode = open.back();
    open.pop_back();
    int x = currentNode[0];
    int y = currentNode[1];
    grid[x][y] = State::kPath;

    // if we reached the goal return the grid
    if (x == goal[0] && y == goal[1]) {
      grid[init[0]][init[1]] = State::kStart;
      grid[goal[0]][goal[1]] = State::kFinish;
      return grid;
    }

    // If we're not done, expand search to current node's neighbors.
    ExpandNeighbors(currentNode, goal, open, grid);
  }
  cout << "No path found!" << "\n";
  return vector<vector<State>>{};
}

int main() {
  int init[2] = {0, 0};
  int goal[2] = {4, 5};

  auto grid = ReadBoardFile("1.board");
  auto solution = SearchBoard(grid, init, goal);
  PrintBoard(solution);

  cout << "\n";
}
