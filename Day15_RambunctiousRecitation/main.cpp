#include <iostream>
#include <sstream>
#include <unordered_map>

using namespace std;
using two_elem_history = unordered_map<int, pair<int, int>>;

void update_history(two_elem_history& history, int spoken, int idx) {
  if (history.count(spoken) == 0) {
    history.emplace(spoken, make_pair(idx, -1));
  } else {
    auto old = history[spoken];
    history[spoken] = make_pair(idx, old.first);
  }
}

int next_spoken(const two_elem_history& history, int spoken) {
  auto history_p = history.at(spoken);
  if (history_p.second == -1) {
    return 0;
  }
  return history_p.first - history_p.second;
}

int main() {
  int last_spoken = -1;
  two_elem_history spoken_history;
  string line;
  getline(cin, line);
  cout << "line: " << line << endl;
  istringstream iss(line);
  int idx = 1;
  for (string num; getline(iss, num, ','); ++idx) {
    cout << "num: " << num << endl;
    last_spoken = stoi(num);
    cout << "init: " << last_spoken << endl;
    update_history(spoken_history, last_spoken, idx);
  }
  for (; idx <= 2020; ++idx) {
    cout << "last_spoken: " << idx << " -> " << last_spoken << endl;
    last_spoken = next_spoken(spoken_history, last_spoken);
    cout << "next_spoken: " << last_spoken << endl;
    update_history(spoken_history, last_spoken, idx);
  }
  cout << "Part 1: " << last_spoken << endl;
}
