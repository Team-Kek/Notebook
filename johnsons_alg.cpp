#include <bits/stdc++.h>

using namespace std;


// Good for sparse graphs.
// 0.8 seconds for V = 2000, E = 5000. Floy-warshall takes almost 8 seconds.
// Tested on:
// https://www.hackerrank.com/challenges/floyd-city-of-blinding-lights
//-RECIPE-START-//
// An implementation of Johnson's algorithm for APSP. O(VE + VE lg E)
typedef int weight_t;
const weight_t WEIGHTINF = numeric_limits<weight_t>::max()/3;
struct Johnson {
    vector<vector<int>> parents; // Shortest path parents tree
    vector<vector<weight_t>> SP; // Shortest path table
    void run(vector<vector<pair<weight_t, int>>> als) {
        const int N = als.size();
        vector<pair<weight_t, int>> special_adj(N); // Construct special node.
        for (int i = 0; i < N; ++i) special_adj[i] = {0, i};
        als.push_back(special_adj);
        vector<weight_t> P(N+1, 0); // Run bellman ford to get potentials
        for (int i = 0; i < N; ++i) // Loop |vertices|-1 times.
            for (int u = 0; u < N+1; ++u) for (auto &e : als[u]) { // Loop through all edges
                P[e.second] = min(P[e.second], e.first + P[u]); 
            }
        for (int u = 0; u < N+1; ++u) // Crash on negative cycle and reweight graph.
            for (auto &e : als[u]) {
                assert(P[e.second] <= e.first + P[u]);
                e.first += P[u]-P[e.second];
            }
        als.pop_back(); // Remove special node after reweighting is done.
        parents.assign(N, vector<int>(N, -1));
        SP.assign(N, vector<weight_t>(N, WEIGHTINF));
        for (int S = 0; S < N; ++S) { // Run Dijkstra's from each node.
            set<tuple<weight_t, int, int>> pq = {make_tuple(0, S, -1)};
            vector<bool> vis(N);
            SP[S][S] = 0;
            while (!pq.empty()) {
                auto top = *pq.begin(); pq.erase(pq.begin());
                weight_t d = get<0>(top); int v = get<1>(top), p = get<2>(top);
                if (vis[v] || d > SP[S][v]) continue;
                vis[v] = true; parents[S][v] = p;
                for (auto &e : als[v]) if (!vis[e.second] && e.first+d < SP[S][e.second]) {
                    SP[S][e.second] = e.first+d ;
                    pq.insert(make_tuple(e.first+d, e.second, v));
                }
            }
            for (int v = 0; v < N; ++v) if (parents[S][v] != -1) SP[S][v] += P[v]-P[S];
        }
    }
};
//-RECIPE-END-//

pair<weight_t, vector<vector<weight_t>>> floyd(vector<vector<weight_t>> G) {
    const int N = G.size();
    for (int k = 0; k < N; ++k) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (G[i][k] == WEIGHTINF || G[k][j] == WEIGHTINF)
                    continue;
                if (G[i][k] + G[k][j] < G[i][j]) {
                    G[i][j] = G[i][k] + G[k][j];
                }
            }
        }
    }
    for (int i = 0; i < N; ++i) if (G[i][i] != 0)
        return {-WEIGHTINF, G};
    weight_t sum = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (G[i][j] != WEIGHTINF) sum += G[i][j];
        }
    } 
    return {sum, G};
}

int main() {
    default_random_engine re;
    int tests = 10000, MAXV = 10;
    uniform_int_distribution<weight_t> dist(-2, 10);
    Johnson john;
    for (int t = 1; t <= tests; ++t) {
        int V =  re()%MAXV+1, E = re()%(V*V-V+1);
        vector<vector<pair<weight_t, int>>> als(V);
        vector<vector<weight_t>> G(V, vector<weight_t>(V, WEIGHTINF));
        for (int i = 0; i < V; ++i)
            G[i][i] = 0;
        vector<pair<int, int>> edges;
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) if (i != j) {
                edges.push_back(make_pair(i, j));
            }
        }
        shuffle(edges.begin(), edges.end(), re);
        for (int i = 0; i < E; ++i) {
            weight_t w = dist(re);
            als[edges[i].first].push_back({w, edges[i].second});
            G[edges[i].first][edges[i].second] = w;
        }
        auto fres = floyd(G);
        if (fres.first == -WEIGHTINF) {
            cout << "Ignoring negative cycle" << endl;
            --t;
            continue;
        }
        john.run(als);
        vector<vector<int>> parent = john.parents;
        weight_t jres = 0;
        for (int s = 0; s < V; ++s) {
            for (int t = 0; t < V; ++t) {
                // cout << s << " " << t << endl;
                cout << (john.SP[s][t] == WEIGHTINF ? "inf" : to_string(john.SP[s][t])) << "\t";
                for (int v = t; parent[s][v] != -1; v = parent[s][v]) {
                    // cout << "(" <<parent[s][v] << " " << v << " " << G[parent[s][v]][v] << ") ";
                    jres += G[parent[s][v]][v];
                }
            }
            cout << endl;
        }
        if (fres.first != jres || fres.second != john.SP) {
            cout << "Something went wrong: " << fres.first << " " << jres << endl;
            return 0;
        }
        cout << "Passed test #" << t << endl;
    }
    cout << "All tests passed" << endl;
}
