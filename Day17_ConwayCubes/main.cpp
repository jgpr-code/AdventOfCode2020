#include <cassert>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ConwayCubeLayer {
 public:
  static const char inactive_cube;
  static const char active_cube;

 private:
  int layer_z;
  deque<deque<char>> grid;

 public:
  ConwayCubeLayer(int z, int rows, int cols)
      : layer_z(z),
        grid(deque<deque<char>>(rows, deque<char>(cols, inactive_cube))) {}

  int z() const { return layer_z; }
  int rows() const { return grid.size(); }
  int cols() const {
    if (grid.size() <= 0) return 0;
    return grid.front().size();
  }

  bool isInsideGrid(int row, int col) const {
    return row >= 0 && row < rows() && col >= 0 && col < cols();
  }

  char at(int row, int col) const {
    assert(isInsideGrid(row, col));
    return grid[row][col];
  }

  void assignAt(int row, int col, char c) {
    assert(isInsideGrid(row, col));
    grid[row][col] = c;
  }

  vector<char> get8Neighborhood(int row, int col) const {
    if (!isInsideGrid(row, col)) return vector<char>();
    vector<int> dr = {-1, -1, -1, 0, 0, 1, 1, 1};
    vector<int> dc = {-1, 0, 1, -1, 1, -1, 0, 1};
    vector<char> ret;
    for (size_t i = 0; i < dr.size(); ++i) {
      int drow = row + dr[i];
      int dcol = col + dc[i];
      if (isInsideGrid(drow, dcol)) ret.push_back(grid[drow][dcol]);
    }
    return ret;
  }

  vector<char> get9Neighborhood(int row, int col) const {
    vector<char> ret = get8Neighborhood(row, col);
    if (!isInsideGrid(row, col)) return vector<char>();
    ret.push_back(grid[row][col]);
    return ret;
  }

  // let r = rows(), c = cols() before this call
  // afterwards rows() == r + 2, cols() == c + 2
  // grid is extended with empty_cells in all 4 directions (left, right, bottom,
  // top)!
  void grow() {
    // extend bottom, top
    grid.emplace_front(cols(), inactive_cube);
    grid.emplace_back(cols(), inactive_cube);
    // extend left, right
    for (auto& row : grid) {
      row.push_front(inactive_cube);
      row.push_back(inactive_cube);
    }
  }

  int countActive() const {
    int count = 0;
    for (int row = 0; row < rows(); ++row) {
      for (int col = 0; col < cols(); ++col) {
        if (grid[row][col] == ConwayCubeLayer::active_cube) ++count;
      }
    }
    return count;
  }

  friend ostream& operator<<(ostream& os, const ConwayCubeLayer& layer) {
    os << "z=" << layer.layer_z << endl;
    for (size_t row = 0; row < layer.grid.size(); ++row) {
      for (size_t col = 0; col < layer.grid.front().size(); ++col) {
        os << layer.grid[row][col];
      }
      os << endl;
    }
    return os;
  }

  friend istream& operator>>(istream& is, ConwayCubeLayer& layer) {
    string line;
    for (int row = 0; getline(is, line) && row < layer.rows(); ++row) {
      assert(line.length() == layer.cols());
      for (int col = 0; col < layer.cols(); ++col) {
        layer.grid[row][col] = line[col];
      }
    }
    return is;
  }
};
const char ConwayCubeLayer::inactive_cube = '.';
const char ConwayCubeLayer::active_cube = '#';

struct ConwayCubes {
  deque<ConwayCubeLayer> layers;

  bool isInsideLayers(int z) const { return z >= 0 && z < layers.size(); }

  vector<char> get26Neighborhood(int z, int row, int col) const {
    if (!isInsideLayers(z) || !layers[z].isInsideGrid(row, col))
      return vector<char>();
    vector<char> ret;
    for (int dz = -1; dz <= 1; ++dz) {
      if (!isInsideLayers(z + dz)) continue;
      const auto& layer = layers[z + dz];
      vector<char> neighborhood = (dz == 0) ? layer.get8Neighborhood(row, col)
                                            : layer.get9Neighborhood(row, col);
      ret.insert(ret.end(), neighborhood.begin(), neighborhood.end());
    }
    return ret;
  }

  int countActive26Neighborhood(int z, int row, int col) const {
    vector<char> neighborhood = get26Neighborhood(z, row, col);
    int sum = 0;
    for (char c : neighborhood) {
      if (c == ConwayCubeLayer::active_cube) ++sum;
    }
    return sum;
  }

  char determineNextCubeValue(int z, int row, int col) const {
    int active_neighbors = countActive26Neighborhood(z, row, col);
    if (layers[z].at(row, col) == ConwayCubeLayer::active_cube) {
      if (active_neighbors == 2 || active_neighbors == 3)
        return ConwayCubeLayer::active_cube;
      else
        return ConwayCubeLayer::inactive_cube;
    } else {
      if (active_neighbors == 3)
        return ConwayCubeLayer::active_cube;
      else
        return ConwayCubeLayer::inactive_cube;
    }
  }

  // extend all layers by -1,+1 in x,y,z
  void growLayers() {
    if (layers.size() <= 0) return;
    const auto& bottom_layer = layers.front();
    const auto& top_layer = layers.back();
    layers.emplace_front(bottom_layer.z() - 1, bottom_layer.rows(),
                         bottom_layer.cols());
    layers.emplace_back(top_layer.z() + 1, top_layer.rows(), top_layer.cols());
    for (auto& layer : layers) {
      layer.grow();
    }
  }

  void simulate(int steps) {
    while (steps > 0) {
      cout << "steps left: " << steps << endl;
      cout << *this;
      cout << "<<<" << endl << endl;
      --steps;
      growLayers();
      ConwayCubes old_cubes = *this;
      cout << old_cubes << endl << endl;
      for (size_t layer_idx = 0; layer_idx < layers.size(); ++layer_idx) {
        auto& layer = layers[layer_idx];
        for (int row = 0; row < layer.rows(); ++row) {
          for (int col = 0; col < layer.cols(); ++col) {
            layer.assignAt(
                row, col,
                old_cubes.determineNextCubeValue(layer_idx, row, col));
          }
        }
      }
    }
  }

  int countActive() const {
    int count = 0;
    for (const auto& layer : layers) {
      count += layer.countActive();
    }
    return count;
  }

  friend ostream& operator<<(ostream& os, const ConwayCubes& cubes) {
    for (size_t i = 0; i < cubes.layers.size(); ++i) {
      if (i != 0) os << endl;
      os << cubes.layers[i];
    }
    return os;
  }
};

template <typename T>
struct TestArgs {
  T actual;
  T expected;
};

void assert_same_strings(TestArgs<string> args) {
  if (args.actual != args.expected) {
    cout << "<<< EXPECTED >>>" << endl;
    cout << ">>>" << args.expected << "<<<" << endl;
    cout << "<<< BUT WAS >>>" << endl;
    cout << ">>>" << args.actual << "<<<" << endl;
    assert(false);
  }
}

void test_ConwayCubeLayer_iostream_validInput_expectedOutput() {
  string input = string(R"(
.#.
..#
###
)")
                     .substr(1);

  string expected("z=0\n" + input);

  ConwayCubeLayer l(0, 3, 3);
  stringstream is(input);
  is >> l;
  stringstream os;
  os << l;
  assert_same_strings({.actual = os.str(), .expected = expected});
}

void test_ConwayCubeLayer_grow_3x3_5x5() {
  string input = string(R"(
.#.
..#
###
)")
                     .substr(1);  // ignore leading '\n'

  string expected = string(R"(
z=0
.....
..#..
...#.
.###.
.....
)")
                        .substr(1);  // ignore leading '\n'

  ConwayCubeLayer l(0, 3, 3);
  stringstream is(input);
  is >> l;
  l.grow();

  assert(l.rows() == 5);
  assert(l.cols() == 5);
  stringstream os;
  os << l;
  assert_same_strings({.actual = os.str(), .expected = expected});
}

void test_ConwayCubes_simulate_aocTest3Steps_expectedOutput() {
  string input = string(R"(
.#.
..#
###
)")
                     .substr(1);  // ignore leading '\n'

  string expected = string(R"(
z=-3
.........
.........
.........
.........
.........
.........
.........
.........
.........

z=-2
.........
.........
.........
.........
...##....
...###...
.........
.........
.........

z=-1
.........
.........
...#.....
....#....
.#.......
......##.
..#...#..
...#.#...
....#....

z=0
.........
.........
....#....
.........
.#.......
.........
......##.
..##.#...
....#....

z=1
.........
.........
...#.....
....#....
.#.......
......##.
..#...#..
...#.#...
....#....

z=2
.........
.........
.........
.........
...##....
...###...
.........
.........
.........

z=3
.........
.........
.........
.........
.........
.........
.........
.........
.........
)")
                        .substr(1);  // ignore leading '\n'

  stringstream is(input);
  ConwayCubeLayer initial_layer(0, 3, 3);
  is >> initial_layer;
  ConwayCubes cubes;
  cubes.layers.push_back(initial_layer);
  cubes.simulate(3);

  stringstream os;
  os << cubes;
  assert_same_strings({.actual = os.str(), .expected = expected});
}

void test_ConwayCubes_simulate_aocTest6Steps_correctAmountActive() {
  string input = string(R"(
.#.
..#
###
)")
                     .substr(1);  // ignore leading '\n'
  stringstream is(input);
  ConwayCubeLayer initial_layer(0, 3, 3);
  is >> initial_layer;
  ConwayCubes cubes;
  cubes.layers.push_back(initial_layer);
  cubes.simulate(6);
  int active = cubes.countActive();
  assert(active == 112);
}

void run_all_tests() {
  test_ConwayCubeLayer_iostream_validInput_expectedOutput();
  test_ConwayCubeLayer_grow_3x3_5x5();
  test_ConwayCubes_simulate_aocTest3Steps_expectedOutput();
  test_ConwayCubes_simulate_aocTest6Steps_correctAmountActive();
  cout << "Tests passed without assertions" << endl;
}

int main() {
  run_all_tests();
  ConwayCubes cubes;
  ConwayCubeLayer initial_layer(0, 8, 8);
  cin >> initial_layer;
  cubes.layers.push_back(initial_layer);
  cubes.simulate(6);
  cout << "Part 1: " << cubes.countActive() << endl;
}
