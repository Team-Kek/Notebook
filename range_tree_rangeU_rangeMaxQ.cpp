#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

//-RECIPE-START-//
template <typename T>
struct MaxRangeTreeRangeUpdate {
	vector<T> node_vals, aux_vals;
	int N;
	int ceil_pow_2(int v) { // Nearest power of two upper bound.
		int p; for (p = 1; p < v; p *= 2) {} return p;
	}
	// Range tree from [0, sz). So may need coordinate compression on input.
	// Initializes all elements to "base".
	MaxRangeTreeRangeUpdate(int sz, T base) : N(ceil_pow_2(sz)) {
		// The maximum value of any element contained within the bounds of a node.
		node_vals.resize(2*N-1, base);
		// The maximum of any range update that encompassed the bounds of the node.
		aux_vals.resize(2*N-1, base);
	}
	// Could change to min if needed, or something similar.
	T combine(T a, T b) {
		return max(a, b);
	}
	// l = query left, r = query right
	// nid = node id, nl = node left, nr = node right
	T query(int l, int r, int nid, int nl, int nr) {
		if (l == nl && r == nr)
			return node_vals[nid];
		int mid = (nl+nr)/2;
		T result = aux_vals[nid];
		if (l <= mid)
			result = combine(result, query(l, min(r, mid), 2*nid+1, nl, mid));
		if (r > mid)
			result = combine(result, query(max(l, mid+1), r, 2*nid+2, mid+1, nr));
		return result;
	}
	// The maximum value of any element (elements are ranges/segments, not points)
	// touching the bounds from l to r inclusive.
	T query(int l, int r) { return query(l, r, 0, 0, N-1); }
	// l = update left, r = update right, value = update value.
	// nid = node id, nl = node left, nr = node right
	void update(int l, int r, T value, int nid, int nl, int nr) {
		if (l == nl && r == nr) {
			node_vals[nid] = combine(value, node_vals[nid]);
			aux_vals[nid] = combine(value, aux_vals[nid]);
			return;
		}
		int mid = (nl+nr)/2;
		if (l <= mid)
			update(l, min(r, mid), value, 2*nid+1, nl, mid);
		if (r > mid)
			update(max(l, mid+1), r, value, 2*nid+2, mid+1, nr);
		node_vals[nid] = combine(node_vals[nid], combine(node_vals[2*nid+1], node_vals[2*nid+2]));
	}
	// There is an element from l to r inclusive with value = "value".
	void update(int l, int r, T value) { update(l, r, value, 0, 0, N-1); }
	void print() {
		for (int i : node_vals)
			cout << i << " ";
		cout << endl;
		for (int i : aux_vals)
			cout << i << " ";
		cout << endl;
	}
};
//-RECIPE-END-//

#include <random>

int main() {
	// MaxRangeTreeRangeUpdate<int> rrt(4, 0);
	// rrt.update(0, 0, 1);
	// cout << rrt.query(1, 2) << endl;
	// rrt.print();
	// return 0;
	default_random_engine re;
	int tests = 1000, sz = 1000, updates = 50000;
	for (int test = 0; test < tests; ++test) {
		vector<int> arr(sz);
		MaxRangeTreeRangeUpdate<int> rt(sz, 0);
		for (int q = 0; q < updates; ++q) {
			int t = re()%2, r = re()%sz, l = re()%(r+1);
			if (t) {
				int r1 = rt.query(l, r), r2 = 0;
				for (int i = l; i <= r; ++i)
					r2 = max(r2, arr[i]);
				if (r1 != r2) {
					cout << r1 << " " << r2 << endl;
					cout << l << " " << r << endl;
					for (int i : arr)
						cout << i << " ";
					cout << endl;
					rt.print();
					break;
				}
			} else {
				int val = re()%sz;
				rt.update(l, r, val);
				for (int i = l; i <= r; ++i)
					arr[i] = max(arr[i], val);
			}
		}
	}
	return 0;
}
