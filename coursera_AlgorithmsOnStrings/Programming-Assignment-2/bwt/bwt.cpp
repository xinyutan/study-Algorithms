#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

string BWT(const string& text) {
  string result = "";
  int n = text.size();
  vector<string> matrix;
  for (int i = 0; i < n; ++i)
    matrix.push_back(text.substr(i, n - i) + text.substr(0, i));

  std::sort(matrix.begin(), matrix.end());
  for (int i = 0; i < n; ++i)
    result += matrix[i][n-1];

  return result;
}

int main() {
  string text;
  cin >> text;
  cout << BWT(text) << endl;
  return 0;
}
