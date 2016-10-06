#include <vector>
#include <set>
#include <iostream>

using namespace std;

//-RECIPE-START-//
// KAHN'S ALGORITHM FOR TOPOLOGICAL SORT.
// Tested on HackerRank Kingdom Connectivity and UVa 11686
// O(E lg V)
vector<int> kahns(const vector<vector<int>> &adj_lists) {
	// Construct our own adj list so we don't delete stuff out of the input.
	// Note, using a set here also elegantly deals with duplicate edges.
	// Duplicate edges break the algorithm.
	vector<set<int>> als(adj_lists.size());
	for (int i = 0; i < adj_lists.size(); ++i) {
		for (int c : adj_lists[i]) {
			als[i].insert(c);
		}
	}
	vector<int> output; // the topo sort order
	set<int> S; // Root vertices.
	for (int i = 0; i < als.size(); ++i) {
		S.insert(i); // Add everything to the set.
	}
	vector<int> incoming(als.size()); // Count of incoming edges.
	// Remove stuff from the set if there is an edge to it.
	for (const auto &al : als) {
		for (int c : al) {
			++incoming[c];
			S.erase(c);
		}
	}
	// Now S contains only nodes with no incoming edges.
	while (!S.empty()) {
		int n = *(S.begin());
		S.erase(n);
		output.push_back(n);
		// Examine children, removing as we go.
		while (als[n].size() > 0) {
			// Process child.
			int c = *(als[n].begin());
			als[n].erase(c);
			--incoming[c];
			// If now a root, add to set.
			if (incoming[c] == 0) {
				S.insert(c);
			}
		}
	}

	// return empty order if no topo sort exists.
	// No sort exists when there are still edges in the graph.
	for (const auto &al : als) {
		if (al.size() > 0)
			return vector<int>();
	}
	return output;
}
//-RECIPE-END-//

int main() {
	while (true) {
		int N, M;
		cin >> N >> M;
		if (N == 0 && M == 0) break;
		vector<vector<int>> als(N);
		for (int i = 0; i < M; ++i) {
			int a, b;
			cin >> a >> b;
			--a; --b;
			als[a].push_back(b);
		}
		auto res = kahns(als);
		if (res.size() == 0)
			cout << "IMPOSSIBLE" << endl;
		else {
			for (int i : res)
				cout << i+1 << endl;
		}
	}
	return 0;
}
