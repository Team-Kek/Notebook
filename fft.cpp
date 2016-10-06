#include <bits/stdc++.h>

using namespace std;
//-RECIPE-START-//
typedef long double ld;
struct cpx {
	ld r, i;
	cpx() : r(0), i(0) {}
	cpx(ld r) : r(r), i(0) {}
	cpx(ld r, ld i) : r(r), i(i) {}
	ld modsq(void) const {
		return r * r + i * i;
	}
	cpx bar(void) const {
		return cpx(r, -i);
	}
	cpx operator+(const cpx& rhs) const {
		return cpx(r + rhs.r, i + rhs.i);
	}
	cpx operator*(const cpx& rhs) const {
		return cpx(r * rhs.r - i * rhs.i, r * rhs.i + i * rhs.r);
	}
	cpx operator/(const cpx& rhs) const {
		cpx d = *this * rhs.bar();
		return cpx(d.r / rhs.modsq(), d.i / rhs.modsq());
	}
};

cpx EXP(ld theta) {
	return cpx(cos(theta), sin(theta));
}

const ld PI = 3.141592653589793238L;

// in:     input array
// out:    output array
// step:   {SET TO 1} (used internally)
// size:   length of the input/output {MUST BE A POWER OF 2}
// dir:    either plus or minus one (direction of the FFT)
// RESULT: out[k] = \sum_{j=0}^{size - 1} in[j] * exp(dir * 2pi * i * j * k / size)
void fft(vector<cpx>::iterator in, vector<cpx>::iterator out, int size, int dir = 1, int step = 1) {
	if (size < 1) return;
	if (size == 1) { *out = *in; return; }
	fft(in, out, size / 2, dir, step * 2);
	fft(in + step, out + size / 2, size / 2, dir, step * 2);
	for (int i = 0; i < size / 2; ++i) {
		cpx even = out[i];
		cpx odd = out[i + size / 2];
		out[i] = even + EXP(dir * 2 * PI * i / size) * odd;
		out[i + size / 2] = even + EXP(dir * 2 * PI * (i + size / 2) / size) * odd;
	}
}
//-RECIPE-END-//

vector<int> num_fac;
void sieve(int N) {
	num_fac = vector<int>(N+1, 1);
	for (int i = 2; i <= N; ++i) {
		for (int j = i; j < N+1; j += i) {
			++num_fac[j];
		}
	}
	num_fac[0] = 0;
}

int main() {
	int F = 500000;
	++F;
	sieve(F);
	int s = 1;
	while ((s <<= 1) < F);
	s <<= 1;
	vector<cpx> dt(s);
	for (int i = 1; i < num_fac.size(); ++i)
		dt[i] = cpx(num_fac[i]);
	vector<cpx> df(s);
	fft(dt.begin(), df.begin(), s);	// Fourier
	for (int i = 0; i < s; ++i)
		df[i] = df[i] * df[i];	// Convolution
	fft(df.begin(), dt.begin(), s, -1);	// Inverse
	vector<int64_t> res;
	for (int i = 0; i < F; ++i)
		res.push_back((int64_t) round(dt[i].r/s));
	int N;
	cin >> N;
	for (int n = 0; n < N; ++n) {
		int alo, ahi;
		cin >> alo >> ahi;
		int best = ahi;
		for (int a = alo; a < ahi; ++a) {
			if (res[best] < res[a]) best = a;
		}
		cout << best << " " << res[best] << endl;
	}
}
