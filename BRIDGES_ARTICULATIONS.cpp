#include <bits/stdc++.h>
using namespace std;

//-RECIPE-START-//
// Finds articulation points and bridges.
// Tested on UVa 315.
struct BridgesAndArtics {
	vector<int> time, es, children;
	vector<vector<int>> adj; // no duplicate edges +  undirected graphs only
	set<int> artics; set<pair<int, int>> bridges;
	int t, root;
	int dfs(int at, int p) { // returns earliest node seen by children
		if (es[at] != -1) return es[at]; // don't recompute earliest.
		time[at] = t++; es[at] = at; children[at] = 0; // init arrays
		for (int c : adj[at]) if (c != p) { // iterate through non-parent links
			if (time[c] == -1) ++children[at];
			int e = time[c]==-1 ? dfs(c, at) : c; // get child earliest
			if (time[e] < time[es[at]]) es[at] = e; // update earliest seen
		}
		if (at != root) { // find those tasty articulations
			if (es[at] == at) {
				if (p != root) artics.insert(p);
				if (children[at] != 0) artics.insert(at);
				bridges.insert(make_pair(p, at));
			} else if (es[at] == p && p != root) artics.insert(p);
		}
		return es[at];
	}
	void compute_sets(const vector<vector<int>> &a) { // computes artics and bridges
		const int N = a.size();
		adj = a; t = 0; root = 0;
		time.assign(N, -1); es = time; children = time;
		artics.clear(); bridges.clear();
		dfs(root, root); // explore dfs tree and fill tables
		if (children[root] >= 2) artics.insert(root); // check root
	}
};
//-RECIPE-END-//

struct BruteForce {
	vector<set<int>> adj;
	vector<vector<int>> al;
	vector<bool> vis;
	void dfs(int at) {
		vis[at] = true;
		for (int c : adj[at]) if (!vis[c]) dfs(c);
	}
	bool connected() {
		vis.assign(adj.size(), false);
		vis[0] = true;
		dfs(0);
		for (bool b : vis) if (!b) return false;
		return true;
	}
	BruteForce(vector<vector<int>> al) : al(al) {
		adj.assign(al.size(), set<int>());
		for (int i = 0; i < al.size(); ++i) {
			for (int j : al[i]) {
				adj[i].insert(j);
			}
		}
	}
	set<pair<int, int>> bridges() {
		set<pair<int, int>> bridge_list;
		for (int i = 0; i < al.size(); ++i) {
			for (int j : al[i]) if (i < j) {
				adj[i].erase(j);
				adj[j].erase(i);
				if (!connected()) bridge_list.insert(make_pair(i, j));
				adj[i].insert(j);
				adj[j].insert(i);
			}
		}
		return bridge_list;
	}
};

void test() {
	int TC = 800000;
	int V = 50;
	double density = 0.05;
	default_random_engine re;
	uniform_real_distribution<double> rd(0.0, 1.0);
	for (int tc = 0; tc < TC; ++tc) {
		vector<vector<int>> adj(V);
		for (int i = 0; i < V; ++i) {
			for (int j = i+1; j < V; ++j) {
				if (rd(re) < density) {
					adj[i].push_back(j);
					adj[j].push_back(i);
				}
			}
		}
		BruteForce bf(adj);
		if (!bf.connected()) {
			cout << "Not connected" << endl;
			continue;
		}
		auto bfb = bf.bridges();
		BridgesAndArtics ba;
		ba.compute_sets(adj);
		set<pair<int, int>> ans;
		for (auto p : ba.bridges) {
			if (p.first > p.second)
				swap(p.first, p.second);
			ans.insert(p);
		}
		for (auto p : bfb) cout << "(" << p.first << "," << p.second << ") ";
		cout << endl;
		assert(bfb == ans);
	}
	cout << "All tests passed" << endl;
}

int main() {
	test();
}
