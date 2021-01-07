#include <iostream>
#include <map>
#include <regex>
#include <vector>

using namespace std;
using ll = long long;

class Tile {
  ll id;
  // Borders are canonalized by rotating to top and then reading left to right
  enum border_id { TOP = 0, BOTTOM = 1, LEFT = 2, RIGHT = 3 };
  vector<string> borders;
  vector<string> lines;  // full tile content

  void Clear() {
    id = -1;
    borders.clear();
    lines.clear();
  }

  void InitBorders() {
    if (lines.size() <= 0) return;
    int rows = static_cast<int>(lines.size());
    int cols = static_cast<int>(lines[0].size());
    borders.assign(4, string());
    // top -> left to right
    for (int c = 0; c < cols; ++c) {
      borders[TOP].push_back(lines[0][c]);
    }
    // bottom -> right to left
    for (int c = cols - 1; c >= 0; --c) {
      borders[BOTTOM].push_back(lines[rows - 1][c]);
    }
    // left -> bottom to top
    for (int r = rows - 1; r >= 0; --r) {
      borders[LEFT].push_back(lines[r][0]);
    }
    // right -> top to bottom
    for (int r = 0; r < rows; ++r) {
      borders[RIGHT].push_back(lines[r][cols - 1]);
    }
  }

 public:
  Tile() : id(-1) {}

  ll GetId() const { return id; }
  vector<string> GetBorders() const { return borders; }

  friend istream& operator>>(istream& is, Tile& t) {
    static int call = 0;
    ++call;
    cout << "called istream >> " << call << endl;
    t.Clear();
    string line;
    getline(is, line);
    regex re("Tile (\\d+):");
    smatch matches;
    if (!regex_match(line, matches, re)) {
      cerr << "wrong tile header!" << endl;
    }
    t.id = stoll(matches[1].str());
    while (getline(is, line)) {
      if (line == "") break;
      cout << line << endl;
      t.lines.push_back(line);
    }
    t.InitBorders();
    return is;
  }

  friend ostream& operator<<(ostream& os, const Tile& t) {
    os << "Id " << t.id << endl;
    os << "Lines" << endl;
    for (string l : t.lines) {
      os << l << endl;
    }
    os << "Borders" << endl;
    for (string b : t.borders) {
      os << b << endl;
    }
    return os;
  }
};

class TileImage {
  vector<Tile> image;
  map<string, int> border_count;

 public:
  void AddTile(Tile t) {
    image.push_back(t);
    for (string border : t.GetBorders()) {
      border_count.try_emplace(border, 0);
      border_count[border]++;
    }
  }

  bool IsBorderTile(const Tile& t) {
    for (string b : t.GetBorders()) {
    }
    return true;
  }

  map<string, int> GetBorderCount() const { return border_count; }

  vector<Tile> GetBorderTiles() const {
    vector<Tile> ret;
    return ret;
  }
};

int main() {
  // assemble 12x12 tiles image
  TileImage image;
  int read_tiles = 0;
  while (true) {
    Tile t;
    cin >> t;
    image.AddTile(t);
    ++read_tiles;
    if (!cin) break;
  }
  cout << "Read " << read_tiles << " tiles" << endl;
  auto b_count = image.GetBorderCount();
  int total_borders = 0;
  int one_time_borders = 0;
  int two_time_borders = 0;
  int other_borders = 0;
  for (auto p : b_count) {
    cout << p.first << " = " << p.second << endl;
    total_borders += p.second;
    if (p.second == 1)
      ++one_time_borders;
    else if (p.second == 2)
      ++two_time_borders;
    else
      ++other_borders;
  }
  cout << total_borders << " = " << one_time_borders << "x1 + "
       << two_time_borders << "x2 + " << other_borders << endl;
  ll factor = 1;
  for (auto t : image.GetBorderTiles()) {
    factor *= t.GetId();
  }
  cout << "Part 1: " << factor << endl;
}
