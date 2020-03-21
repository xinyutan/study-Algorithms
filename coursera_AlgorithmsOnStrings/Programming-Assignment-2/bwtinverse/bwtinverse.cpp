#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::map;

typedef map<pair<char, int>, pair<char, int>>  FrontBackMap;

int l2i(char letter) {
  switch (letter) {
    case '$':
      return 0; break;
    case 'A':
      return 1; break;
    case 'C':
      return 2; break;
    case 'T':
      return 3; break;
    case 'G':
      return 4; break;
    default:
      return -1;
  }

}

FrontBackMap build_map(const string& fnt, const string& bck) {
  int fnt_index[5] = {0, 0, 0, 0, 0}, bck_index[5] = {0, 0, 0, 0, 0};
  FrontBackMap fbmap;
  for (int i = 0; i < fnt.size(); ++i) {
    char f = fnt[i], b = bck[i];
    int fi = fnt_index[l2i(f)], bi = bck_index[l2i(b)];
    fbmap[pair<char, int>(f, fi)] = pair<char, int>(b, bi);
    fnt_index[l2i(f)] += 1;
    bck_index[l2i(b)] += 1;
  }
  return fbmap;
}

string InverseBWT(const string& bwt) {
  string text = "";
  string frontletters = bwt;
  std::sort(frontletters.begin(), frontletters.end());
  auto fbmap = build_map(frontletters, bwt);
  pair<char, int> f('$', 0);
  for (int i = 0; i < bwt.size(); ++i) {
    text += f.first;
    f = fbmap[f];
  }
  std:reverse(text.begin(), text.end());
  return text;
}


int main() {
  string bwt;
  cin >> bwt;
  cout << InverseBWT(bwt) << endl;
  return 0;
}
