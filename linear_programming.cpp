#include <bits/stdc++.h>

using namespace std;
typedef long double ld;

typedef vector<int> vi;
typedef ld el_t;
typedef vector<el_t> ve;
typedef vector<ve> vve;

const el_t EPS = 1e-10;

template <typename T> T tabs(T val) { return val < T(0) ? -val : val; }

// Gauss-Jordan elimination with full pivoting.
//
// Uses:
//   (1) solving systems of linear equations (AX=B)
//   (2) inverting matrices (AX=I)
//   (3) computing determinants of square matrices
//
// Running time: O(n^3)
//
// INPUT:    a[][] = an nxn matrix
//           b[][] = an nxm matrix
//
// OUTPUT:   X      = an nxm matrix (stored in b[][])
//           A^{-1} = an nxn matrix (stored in a[][])
//           returns determinant of a[][] (0 implies insoluble)
el_t gauss_jordan(vve &a, vve &b) {
	const int n = a.size();
	const int m = b[0].size();
	vi irow(n), icol(n), ipiv(n);
	el_t det = 1;

	for (int i = 0; i < n; ++i) {
		int pj = -1, pk = -1;
		for (int j = 0; j < n; ++j) if (!ipiv[j])
			for (int k = 0; k < n; ++k) if (!ipiv[k])
				if (pj == -1 || tabs(a[pj][pk]) < tabs(a[j][k])) { pj = j; pk = k; }
		assert(EPS < tabs(a[pj][pk]));
		++ipiv[pk];
		swap(a[pj], a[pk]);
		swap(b[pj], b[pk]);
		if (pj != pk) det = -det;
		irow[i] = pj;
		icol[i] = pk;

		el_t c = a[pk][pk];
		det *= a[pk][pk];
		if (det == 0) return det;
		a[pk][pk] = 1;
		for (int p = 0; p < n; ++p) a[pk][p] /= c;
		for (int p = 0; p < m; ++p) b[pk][p] /= c;
		for (int p = 0; p < n; ++p) if (p != pk) {
			c = a[p][pk];
			a[p][pk] = 0;
			for (int q = 0; q < n; ++q) a[p][q] -= a[pk][q] * c;
			for (int q = 0; q < m; ++q) b[p][q] -= b[pk][q] * c;
		}
	}

	for (int p = n-1; p >= 0; p--) if (irow[p] != icol[p])
		for (int k = 0; k < n; k++) swap(a[k][irow[p]], a[k][icol[p]]);

	return det;
}

// Reduced row echelon form via Gauss-Jordan elimination 
// with partial pivoting.  This can be used for computing
// the rank of a matrix.
//
// Running time: O(n^3)
//
// INPUT:    a[][] = an nxm matrix
//
// OUTPUT:   rref[][] = an nxm matrix (stored in a[][])
//           returns rank of a[][]
int rref(vve &a) {
	const int n = a.size();
	const int m = a[0].size();
	int r = 0;
	for (int c = 0; c < m && r < n; ++c) {
		int j = r;
		for (int i = r + 1; i < n; i++)
			if (tabs(a[j][c]) < tabs(a[i][c])) j = i;
		if (tabs(a[j][c]) < EPS) continue;
		swap(a[j], a[r]);

		el_t s = a[r][c];
		for (int j = 0; j < m; j++) a[r][j] /= s;
		for (int i = 0; i < n; i++) if (i != r) {
			s = a[i][c];
			for (int j = 0; j < m; j++) a[i][j] -= s * a[r][j];
		}
		r++;
	}
	return r;
}

// Two-phase simplex algorithm for solving linear programs of the form
//
//     maximize     c^T x
//     subject to   Ax <= b
//                  x >= 0
//
// INPUT: A -- an m x n matrix
//        b -- an m-dimensional vector
//        c -- an n-dimensional vector
//        x -- a vector where the optimal solution will be stored
//
// OUTPUT: value of the optimal solution (infinity if unbounded
//         above, nan if infeasible)
//
// To use this code, create an SimplexSolver object with A, b, and c as
// arguments.  Then, call Solve(x).
struct SimplexSolver {
	int m, n;
	vi B, N;
	vve D;
	SimplexSolver(const vve &A, const ve &b, const ve &c) : m(b.size()), n(c.size()), N(n + 1), B(m), D(m + 2, ve(n + 2)) {
		for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) D[i][j] = A[i][j];
		for (int i = 0; i < m; ++i) { B[i] = n + i; D[i][n] = -1; D[i][n + 1] = b[i]; }
		for (int j = 0; j < n; ++j) { N[j] = j; D[m][j] = -c[j]; }
		N[n] = -1; D[m + 1][n] = 1;
	}
	void pivot(int r, int s) {
		for (int i = 0; i < m + 2; ++i) if (i != r)
			for (int j = 0; j < n + 2; ++j) if (j != s)
				D[i][j] -= D[r][j] * D[i][s] / D[r][s];
		for (int j = 0; j < n + 2; ++j) if (j != s) D[r][j] /= D[r][s];
		for (int i = 0; i < m + 2; ++i) if (i != r) D[i][s] /= -D[r][s];
		D[r][s] = el_t(1) / D[r][s];
		swap(B[r], N[s]);
	}
	bool simplex(int phase) {
		int x = phase == 1 ? m + 1 : m;
		while (true) {
			int s = -1;
			for (int j = 0; j <= n; ++j) {
				if (phase == 2 && N[j] == -1) continue;
				if (s == -1 || D[x][j] < D[x][s] || D[x][j] == D[x][s] && N[j] < N[s]) s = j;
			}
			if (-EPS < D[x][s]) return true;
			int r = -1;
			for (int i = 0; i < m; ++i) {
				if (D[i][s] < EPS) continue;
				if (r == -1 || D[i][n + 1] / D[i][s] < D[r][n + 1] / D[r][s] || (D[i][n + 1] / D[i][s]) == (D[r][n + 1] / D[r][s]) && B[i] < B[r]) r = i;
			}
			if (r == -1) return false;
			pivot(r, s);
		}
	}
	el_t solve(ve &x) {
		int r = 0;
		for (int i = 1; i < m; ++i) if (D[i][n + 1] < D[r][n + 1]) r = i;
		if (D[r][n + 1] < -EPS) {
			pivot(r, n);
			if (!simplex(1) || D[m + 1][n + 1] < -EPS) return -numeric_limits<el_t>::infinity();
			for (int i = 0; i < m; ++i) if (B[i] == -1) {
				int s = -1;
				for (int j = 0; j <= n; ++j)
					if (s == -1 || D[i][j] < D[i][s] || D[i][j] == D[i][s] && N[j] < N[s]) s = j;
				pivot(i, s);
			}
		}
		if (!simplex(2)) return numeric_limits<el_t>::infinity();
		x = ve(n);
		for (int i = 0; i < m; ++i) if (B[i] < n) x[B[i]] = D[i][n + 1];
		return D[m][n + 1];
	}
};
