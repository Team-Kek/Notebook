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
// malePrefs[m][p] holds the woman that is in man m's preference position p
// returns a females to males array
// this is male optimal
VI marriages(VVI malePrefs, VVI femalePrefs) {
	int n = malePrefs.size();
	
	VI fm(n, -1); // which male each female is "engaged" to
	VI prefs(n, -1); // which prefs the given male is at
	queue<int> lonelyMales;
	for (int i = 0; i < n; i++) {
		lonelyMales.push(i);
	}
	
	while ( ! lonelyMales.empty()) {
		int k = lonelyMales.front();
		lonelyMales.pop();
		
		int w = malePrefs[k][ prefs[k]++ ];
		
		if (fm[w] == -1) fm[w] = k; // free female
		else for(int i = 0; i < femalePrefs[w].size(); i++) {
			if (femalePrefs[w][i] == fm[w]) { // w likes her man better
				lonelyMales.push(k);
				break; 
			}
			if (femalePrefs[w][i] == k) { // w likes k better
				lonelyMales.push(fm[w]);
				fm[w] = k;
				break;
			}
		}
	}
	
	return fm;
}

//-RECIPE-END-//

int main() {
	int N;
	while (cin >> N, N) {

		VVI mp(N, VI(N));
		FOR(i, N)
			FOR(j, N) {
				cin >> mp[i][j];
				mp[i][j]--;
			}

		VVI fp(N, VI(N));
		FOR(i, N)
			FOR(j, N) {
				cin >> fp[i][j];
				fp[i][j]--;
			}

		VI ans = marriages(mp, fp);
		FOREACH(it, ans)
			cout << (*it + 1) << ' ';
		cout << endl;
	}
}
