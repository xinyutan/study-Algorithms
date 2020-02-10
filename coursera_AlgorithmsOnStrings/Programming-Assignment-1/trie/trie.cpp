#include <string>
#include <iostream>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::string;

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(vector<string> & patterns) {
  trie t(1);
  int tot_node = 1;
  for (const auto& p: patterns) {
    int curr_node = 0;
    for (int i = 0; i < p.size(); ++i) {
      if (t[curr_node].find(p[i]) == t[curr_node].end()) {
        // current_node doesn't contain the p[i]
        t[curr_node][p[i]] = tot_node;
        t.push_back({});
        curr_node = tot_node;
        tot_node++;
      }
      else
        curr_node = t[curr_node][p[i]];
    }
  }
  return t;
}

int main() {
  size_t n;
  std::cin >> n;
  vector<string> patterns;
  for (size_t i = 0; i < n; i++) {
    string s;
    std::cin >> s;
    patterns.push_back(s);
  }

  trie t = build_trie(patterns);
  for (size_t i = 0; i < t.size(); ++i) {
    for (const auto & j : t[i]) {
      std::cout << i << "->" << j.second << ":" << j.first << "\n";
    }
  }

  return 0;
}
