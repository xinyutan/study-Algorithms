#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int const Letters =    4;
int const NA      =   -1;

struct Node
{
	int next [Letters];
	bool patternEnd;

	Node ()
	{
		fill (next, next + Letters, NA);
		patternEnd = false;
	}
};

int letterToIndex (char letter)
{
	switch (letter)
	{
		case 'A': return 0; break;
		case 'C': return 1; break;
		case 'G': return 2; break;
		case 'T': return 3; break;
		default: assert (false); return -1;
	}
}

vector<Node> build_trie(vector<string>& patterns) {
  int tot_node = 1;
  vector<Node> trie(1);
  for (auto& p: patterns) {
    int curr_node = 0;
    for (auto& c: p) {
      if (trie[curr_node].next[letterToIndex(c)] == NA) {
        trie[curr_node].next[letterToIndex(c)] = tot_node;
        trie.push_back(Node());
        curr_node = tot_node;
        tot_node++;
      } else {
        curr_node = trie[curr_node].next[letterToIndex(c)];
      }
    }
    trie[curr_node].patternEnd = true;
  }
  return trie;
}

bool is_match(string& text, int start, vector<Node>& trie) {
  int curr_node = 0;
  for (int i = start; i < text.size(); ++i) {
    char curr_l = text[i];
    if (trie[curr_node].patternEnd)
      return true;
    if (trie[curr_node].next[letterToIndex(curr_l)] == NA)
      return false;
    curr_node = trie[curr_node].next[letterToIndex(curr_l)];
  }
  return trie[curr_node].patternEnd;
}


vector <int> solve (string& text, int n, vector <string>& patterns)
{
	vector <int> result;

	// write your code here
  vector<Node> t = build_trie(patterns);

  for(int i = 0; i < text.size(); ++i) {
    if (is_match(text, i, t))
        result.push_back(i);
  }
	return result;
}

int main (void)
{
	string t;
	cin >> t;

	int n;
	cin >> n;

	vector <string> patterns (n);
	for (int i = 0; i < n; i++)
	{
		cin >> patterns[i];
	}

	vector <int> ans;
	ans = solve (t, n, patterns);

	for (int i = 0; i < (int) ans.size (); i++)
	{
		cout << ans[i];
		if (i + 1 < (int) ans.size ())
		{
			cout << " ";
		}
		else
		{
			cout << endl;
		}
	}

	return 0;
}
