#include <iostream>
#include <vector>
#include <algorithm>
#include<queue>
using namespace std;

#define VAR(a,b) __typeof(b) a = b
#define FORAB(i,a,b) for(VAR(i,a); i != b; i++)
#define FOR(i,n) FORAB(i,0,n)
#define FOREACH(i,v) FORAB(i,(v).begin(),(v).end())
#define RFOREACH(i,v) FORAB(i,(v).rbegin(),(v).rend())
#define RFOR(i,n) for(VAR(i,n-1); i != -1; i--)
#define ALL(c) (c).begin(),(c).end()

typedef vector<long long int> VI;
typedef vector<VI> VVI;
typedef vector<bool> VB;
typedef long long int LLI;

//-RECIPE-START-//
typedef vector<VB> VVB;

bool bicolourable(VVB adj) { 
    int n = adj.size();
    VB visited(n);
    VI col(n, -1);
    
    for (int i = 0; i < n; i++) {
        if ( ! visited[i]) {
            queue<int> q;
            q.push(i);
            col[i] = 0;
            
            while ( ! q.empty() ) {
                int nd = q.front(); q.pop();
                int c = (col[nd] + 1) % 2;
                for (int j = 0; j < n; j++) {
                    if (adj[nd][j]) { // I have assumed adj[i][i] = false
                        if (col[j] != -1  &&  col[j] != c)
                            return false;
                        if ( ! visited[j] ) {
                            col[j] = c;
                            q.push(j);
                            visited[j] = true;
                        }
                    }
                }
            }
            
        }
    }
    return true;
}


//-RECIPE-END-//

int main() {
	int N, M;
	while (cin >> N >> M, N) {
		VVB g(N, VB(N, false));
		FOR(i, M) {
			int a, b; cin >> a >> b;
			g[a][b] = true;
			g[b][a] = true;
		}
		cout << (bicolourable(g) ? "yes" : "no" ) << endl;
	}
}
