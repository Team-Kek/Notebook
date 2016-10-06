#include <bits/stdc++.h>

using namespace std;

//-RECIPE-START-//
template <typename T>
struct RangeTree {
	int N; vector<T> node_vals;
	T base_value;
	int ceil_pow_2(int v) { // Nearest power of two upper bound.
		int p; for (p = 1; p < v; p *= 2) {} return p;
	}
	RangeTree(int sz, T base) : base_value(base), N(ceil_pow_2(sz)) {
		node_vals.resize(2*N-1, base_value);
	}
	T combine(T a, T b) { return max(a, b); } // Could be min or +.
	T query(int l, int r, int nid, int nl, int nr) {
		if (l == nl && r == nr)
			return node_vals[nid];
		int mid = (nl+nr)/2; T result = base_value;
		if (l <= mid)
			result = combine(result, query(l, min(r, mid), 2*nid+1, nl, mid));
		if (r > mid)
			result = combine(result, query(max(l, mid+1), r, 2*nid+2, mid+1, nr));
		return result;
	}
	T query(int l, int r) { return query(l, r, 0, 0, N-1); }
	void update(int index, T value, int nid, int nl, int nr) {
		if (nl == nr) { node_vals[nid] = value; return; }
		int mid = (nl+nr)/2;
		if (index <= mid) 	update(index, value, 2*nid+1, nl, mid);
		else				update(index, value, 2*nid+2, mid+1, nr);
		node_vals[nid] = combine(node_vals[2*nid+1], node_vals[2*nid+2]);
	}
	void update(int index, T value) { update(index, value, 0, 0, N-1); }
	// Optional
	void print() {
		for (int i : node_vals)
			cout << i << " ";
		cout << endl;
	}
};
//-RECIPE-END-//

int main() {
	default_random_engine re;
	int sz = 111111;
	RangeTree<int> rt(sz, 0);
	vector<int> vr(sz);
	for (int i = 0; i < 1e5; ++i) {
		int t = re()%2;
		if (t == 0) {
			int r = re()%sz, l = re()%(r+1);
			int res = -1e9, resrt = rt.query(l, r);
			for (int j = l; j <= r; ++j)
				res = max(res, vr[j]);	
			if (res != resrt) {
				cout << "oops " << l << " " << r << " " << res << " " << resrt << endl;
				for (int i : vr)
					cout << i << " ";
				cout << endl;
				rt.print();
				break;
			}
		} else {
			int i = re()%sz, v = re()%(int)1e6;
			vr[i] = v;
			rt.update(i, v);
		}
	}
	
