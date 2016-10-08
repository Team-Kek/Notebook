#include <bits/stdc++.h>
using namespace std;

struct SCC {
	stack<int> stk;
	vector<vector<int>> adj_list;
	vector<int> parent;
	bool fill_stk;
	void dfs(int at, int colour) {
		parent[at] = colour;
		for (int child : adj_list[at])
			if (parent[child] == -1)
				dfs(child, colour);
		if (fill_stk) stk.push(at);
	}
	void fill_parents() { // THIS WILL MUTATE adj_list!!!!!
		const int N = adj_list.size();
		parent = vector<int>(N, -1);
		fill_stk = true;
		for (int i = 0; i < N; ++i) // Get dfs order on stack.
			if (parent[i] == -1) dfs(i, i);
		vector<vector<int>> transposed(N);
		for (int i = 0; i < N; ++i) // Transpose graph.
			for (int j : adj_list[i])
				transposed[j].push_back(i);
		adj_list = transposed;
		parent = vector<int>(N, -1);
		fill_stk = false;
		while (!stk.empty()) { // Find SCCs.
			int i = stk.top(); stk.pop();
			if (parent[i] == -1)
				dfs(i, i);
		}

	}
};

//-RECIPE-START-//
struct Var { bool neg; int id; };
// Takes a CNF two sat forumla with N variables
// and finds an assignment of variables that satisfies it.
// O(N lg N + M) where N is variables and M is clauses in CNF.
// Assumes variables have ids 0..N-1
vector<bool> two_sat(const vector<pair<Var, Var>> &cnf, int N) {
	SCC scc; // The notebook SCC implementation!
	scc.adj_list.assign(2*N, vector<int>()); // Build implication graph
	for (auto &p : cnf) {
		int i = p.first.id;
		int j = p.second.id;
		scc.adj_list[p.first.neg ? i : i+N].push_back(p.second.neg ? j+N : j);
		scc.adj_list[p.second.neg ? j : j+N].push_back(p.first.neg ? i+N : i);
	}
	scc.fill_parents();
	map<int, set<int>> sccs; // Can be sped up easily
	for (int i = 0; i < scc.parent.size(); ++i) {
		int negation = i < N ? i+N : i-N; // Check if in scc with negation
		if (sccs[scc.parent[i]].count(negation)) return {};
		sccs[scc.parent[i]].insert(i);
	}
	scc.fill_stk = true;
	scc.parent.assign(2*N, -1);
	for (int i = 0; i < 2*N; ++i) if (scc.parent[i] == -1) 
		scc.dfs(i, i);
	vector<bool> assignments(N); // Deduce assignments from stack order
	while (!scc.stk.empty()) {
		int top = scc.stk.top(); scc.stk.pop();
		if (top >= N) assignments[top-N] = true;
		else assignments[top] = false;
	}
	return assignments;
}
//-RECIPE-END-//


// Implementation of codesforces http://codeforces.com/problemset/problem/228/E
int main() {
	int N, M;
	cin >> N >> M;
	vector<pair<Var, Var>> variables;
	int vars = N;
	for (int i = 0; i < M; ++i) {
		int u, v, c;
		cin >> u >> v >> c;
		--u; --v;
		if (u > v) swap(u,v);
		if (c) {
			variables.push_back({{0,u},{1,v}});
			variables.push_back({{0,v},{1,u}});
			variables.push_back({{1,u},{0,v}});
			variables.push_back({{1,v},{0,u}});
		} else {
			variables.push_back({{1,u},{1,v}});
			variables.push_back({{1,v},{1,u}});
			variables.push_back({{0,u},{0,v}});
			variables.push_back({{0,v},{0,u}});
		}
	}
	auto ans = two_sat(variables,vars);
	if (ans.size() == 0) {
		cout << "Impossible" << endl;
		return 0;
	}
	vector<int> things;
	for (int i = 0; i < N; ++i) {
		if (ans[i]) things.push_back(i+1);
	}
	cout << things.size() << endl;
	for (int i : things) cout << i << " ";
	cout << endl;
}
