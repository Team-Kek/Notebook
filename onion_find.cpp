
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef vector<long long int> VI;
typedef vector<VI> VVI;
typedef vector<bool> VB;
typedef long long int LLI;

//-RECIPE-START-//

VI uf_parents; // my parent (myself if I'm a root) or -1 if I'm not in the UF
VI uf_ranks;   // the rank of me (as a root)
int uf_count;  // number of partitions
 
// Resets the uf to allow it to hold items with id's 0 .. n-1
void new_uf(int n) {
	uf_parents = VI(n, -1);
	uf_ranks   = VI(n, 0);
	uf_count   = 0; 
}
    
// Adds an item (if it's not already in) to the UF in it's own partition
void uf_add(int id) {
	if (uf_parents[id] == -1) {
		uf_parents[id] = id;
		uf_count++;
	}
}

// Returns the id of the parent of a partition
int uf_find(int id) {
	if (uf_parents[id] >= 0)
		return (uf_parents[id] = (id == uf_parents[id]) ?
				id : uf_find(uf_parents[id]));
	return -1;
}

// unions two sets
void uf_union(int a, int b) {
	a = uf_find(a);
	b = uf_find(b);
	if (a < 0 || b < 0 || a==b) return;
	
	if (uf_ranks[a] < uf_ranks[b]) uf_parents[a] = b;
	else uf_parents[b] = a;

	if (uf_ranks[a] == uf_ranks[b]) uf_ranks[a]++;
	uf_count--;
}
	
//-RECIPE-END-//

/*
Each problem starts with two ints, the number of nodes and the number of unions
to perform.
	(Two zeros signifies the end of the input.)
For each union, the id (zero indexed) of two nodes is given.
Print out the number of partitions left after all unions are performed.
*/
int main() {
	int N, M;
	while (cin >> N >> M, N) {
		new_uf(N);
		FOR(i, N) uf_add(i);
		FOR(i, M) {
			int a, b;
			cin >> a >> b;
			uf_union(a, b);
		}
		cout << uf_count << endl;
	}
}
