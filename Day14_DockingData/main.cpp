#include <bitset>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;
using ll = long long;

class DockerProgram {
  static const size_t bitLength = 36;
  string internal_mask;
  map<ll, string> internal_mem;

  char masked_value(char mask, char value) {
    if (mask == 'X') {
      return value;
    } else {
      return mask;
    }
  }

public:
  static string lltobitstr(ll value) {
    bitset<bitLength> bitrepresentation(value);
    return bitrepresentation.to_string();
  }
  void mask(const string &new_mask) { internal_mask = new_mask; }
  void mem(ll address, const string &value) {
    if (value.size() != bitLength) {
      cerr << "Value must be exactly " << bitLength
           << " chars long! Doing nothing now." << endl;
      return;
    }
    internal_mem.try_emplace(address, string(bitLength, '0'));
    for (size_t i = 0; i < bitLength; ++i) {
      internal_mem[address][i] = masked_value(internal_mask[i], value[i]);
    }
  }
  ll mem_sum() const {
    ll sum = 0;
    for (const auto &kvp : internal_mem) {
      sum += stoll(kvp.second, 0, 2);
    }
    return sum;
  }
};

int main() {
  DockerProgram prog;
  for (string line; getline(cin, line);) {
    if (line.find("-") != string::npos) {
      cerr << "We have negative numbers!" << endl;
    }
    istringstream linestream(line);
    string lhs;
    string eqsign;
    string rhs;
    getline(linestream, lhs, ' ');
    getline(linestream, eqsign, ' ');
    getline(linestream, rhs, ' ');
    if (eqsign != "=") {
      cerr << "A line was not in the correct format!" << endl;
    }
    cout << "lhs:" << lhs << " rhs:" << rhs << endl;
    istringstream lhsstream(lhs);
    string operation;
    getline(lhsstream, operation, '[');
    if (operation == "mask") {
      prog.mask(rhs);
    } else if (operation == "mem") {
      string address;
      getline(lhsstream, address, ']');
      cout << "rhs to bit: " << DockerProgram::lltobitstr(stoll(rhs)) << endl;
      prog.mem(stoll(address), DockerProgram::lltobitstr(stoll(rhs)));
    }
  }
  cout << "Part 1: " << prog.mem_sum() << endl;
}
