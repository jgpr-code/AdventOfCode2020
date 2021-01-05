#include <algorithm>
#include <iostream>
#include <regex>

using namespace std;
using ll = long long;

// slightly modified find_first_of that just returns the size of the string on
// failure
size_t find_first_of(const string& expr, const string& search, size_t from) {
  size_t pos = expr.find_first_of(search, from);
  if (pos == string::npos) pos = expr.length();
  return pos;
}
// range based substr, to is exclusive
string substr(const string& str, size_t from, size_t to) {
  return str.substr(from, to - from);
}

ll evalLeftToRightNoParentheses(const string& expr) {
  size_t pos = find_first_of(expr, "+*", 0);
  ll acc = stoll(substr(expr, 0, pos));
  while (pos < expr.size()) {
    size_t next_pos = find_first_of(expr, "+*", pos + 1);
    ll operand = stoll(substr(expr, pos + 1, next_pos));
    switch (expr[pos]) {
      case '+':
        acc += operand;
        break;
      case '*':
        acc *= operand;
        break;
    }
    pos = next_pos;
  }
  return acc;
}

ll evalExpr(const string& expression) {
  string expr = expression;  // copy to modify it

  // Regex explanation:
  // r(  \(  (  [^()]+  )  \)  )r
  // r( and )r just delimit the raw string literal and are not part of the regex
  // itself
  // \( and \) this is used to match the literal chars '(' and ')'
  // ( and ) this is to capture anything between the actual parentheses (see
  // previous line)
  // [^()]+ match non empty sequences that are not allowed to contain
  // parentheses
  regex innermostParensExpr(R"r(\(([^()]+)\))r", regex_constants::extended);

  smatch matches;

  while (regex_search(expr, matches, innermostParensExpr)) {
    expr.replace(matches[0].first, matches[0].second,
                 to_string(evalLeftToRightNoParentheses(matches[1].str())));
  }
  return evalLeftToRightNoParentheses(expr);
}

int main() {
  string expr("10+32*6+21*4");
  cout << "eval " << expr << " = " << evalLeftToRightNoParentheses(expr)
       << endl;

  ll exprSum = 0;
  for (string line; getline(cin, line);) {
    // discard whitespace
    line.erase(
        remove_if(line.begin(), line.end(), [](char c) { return isspace(c); }),
        line.end());
    exprSum += evalExpr(line);
    cout << "line: " << line << " -> ";
    cout << evalExpr(line) << endl;
  }

  cout << "Part 1: " << exprSum << endl;
}
