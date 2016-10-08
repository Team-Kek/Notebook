#include <bits/stdc++.h>

using namespace std;

//-RECIPE-START-//
// Finds and euler path or cycle.
// Is a path/cycle that visits every edge.
// Undirected graph has eulerian cycle iff every vertex has even degree.
// Undirected graph has eulerian path iff 0 or 2 vertiex have odd degree.
// Directed has cycle iff in degree = out degree for every vertex + is SCC.
// Directed has path iff <= 1 vertex has (in degree)-(out degree) = 1
// and <= 1 vertex has out-in = 1, and the others have in=out.
struct EulerianGraph {
	vector<int> path; // Will be in reverse order.
	// adj will not all be 0 if the graph has > 1 component with > 1 vertex
	// and thus no cycle/path.
	vector<vector<int>> adj; // Could be vector<multiset<>> for sparse graphs
	void hierholtz(int at = 0) {
		// Iterate through children and remove/mark undirected edge.
		for (int c = 0; c < adj[at].size(); ++c) if (adj[at][c]) {
			--adj[at][c]; 
			//--adj[c][at]; uncomment this for undirected graph
			hierholtz(c);
		}
		path.push_back(at);
	}
};
//-RECIPE-END-//

// Implementation of codeforces "Tanya and Password"
int main() {
	int N; cin >> N;
	map<pair<char, char>, int> M;
	vector<pair<char, char>> pairs;
	string chars;
	for (char c = 'a'; c <= 'z'; ++c) {
		chars.push_back(c);
		chars.push_back(toupper(c));
	}
	for (char c = '0'; c <= '9'; ++c) {
		chars.push_back(c);
	}
	for (char c : chars) {
	       for (char c2 : chars) {
		       M[make_pair(c, c2)] = pairs.size();
		       pairs.push_back(make_pair(c, c2));
	       }
	}	       
	EulerianGraph eg;
	eg.adj.assign(pairs.size(), vector<int>(pairs.size()));
	vector<int> in(pairs.size()), out(pairs.size());
	for (int i = 0; i < N; ++i) {
		string str; cin >> str;
		int fst = M[make_pair(str[0], str[1])];
		int snd = M[make_pair(str[1], str[2])];
		eg.adj[fst][snd]++;
		in[snd]++; out[fst]++;
	}
	bool all_eq = true, sink = false, borked = false;
	int src = -1, first_eq = -1;
	for (int i = 0; i < pairs.size(); ++i) {
		if (in[i] == 0 && out[i] == 0) continue;
		if (in[i] != out[i]) {
			all_eq = false;
			if (in[i]-out[i] == 1 && !sink) {
				sink = true;
			} else if (out[i]-in[i] == 1 && src == -1) {
				src = i;
			} else borked = true;
		} else if (first_eq == -1) {
			first_eq = i;
		}
	}
	if (all_eq == false && (src == -1 || sink == false))
		borked = true;
	if (borked == false) {
		if (all_eq) {
			eg.hierholtz(first_eq);
		} else {
			eg.hierholtz(src);
		}
	}
	if (borked == true || eg.path.size() != N+1) {
		cout << "NO" << endl;
	} else {
		cout << "YES" << endl;
		if (all_eq) {
			cout << (char)pairs[first_eq].first;
		} else {
			cout << (char)pairs[src].first;
		}
		reverse(eg.path.begin(), eg.path.end());
		for (int i : eg.path)
			cout << (char)pairs[i].second;
		cout << endl;
	}
}
