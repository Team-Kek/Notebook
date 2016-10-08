#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  

using namespace std;

//-RECIPE-START-//

const int verts = 20;
const int inf = 9999999;
int instance_verts;

int dp[(1 << verts) + 1][verts];
int adj_mat[verts][verts];

int tsp(int at, int bitset) {
	
	if (dp[bitset][at])
		return dp[bitset][at];
	// base case is that all vertices are visited
	if (bitset == (1 << instance_verts) - 1)
		return adj_mat[at][0];

	int min = inf;

	// try all possible next vertices
	for (int i = 0; i < instance_verts; ++i) {
		// if already visited, skip
		if ((bitset & (1 << i)) > 0)
			continue;
		// cost of visiting vertex i next
		int cost = tsp(i, bitset | (1 << i)) + adj_mat[at][i];
		if (cost < min)
			min = cost;
	}

	return dp[bitset][at] = min;
}

//-RECIPE-END-//

int setup_rand() {
	for (int i = 0; i < instance_verts; ++i)
		for (int j = 0; j < instance_verts; ++j)
			adj_mat[i][j] = inf;
	for (int i = 0; i < instance_verts; ++i)
		for (int j = i+1; j < instance_verts; ++j) {
			adj_mat[i][j] = rand() % 100 + 1;
			adj_mat[j][i] = adj_mat[i][j];
		}
}


int main() {
	instance_verts = 20;
	srand (time(NULL));
	setup_rand();
	cout << tsp(0, 1) << endl;
	return 0;
}
