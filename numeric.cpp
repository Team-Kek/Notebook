#include <bits/stdc++.h>

using namespace std;

map<int64_t, int64_t> dpt;

int64_t factorial(int64_t n, int64_t MOD) {
    if (n == 0) return 1;
    if (dpt.count(n)) return dpt[n];
    return dpt[n] = (n*factorial(n-1, MOD))%MOD;
}

//-RECIPE-START-//
// Use 64 bits integers to avoid overflow errors during multiplication.
int64_t modPow(int64_t a, int64_t x, int64_t p) {
    //calculates a^x mod p in logarithmic time.
    int64_t res = 1;
    while (x > 0) {
        if (x % 2)
            res = (res * a) % p;
        a = (a * a) % p;
        x /= 2;
    }
    return res;
}

int64_t modInverse(int64_t a, int64_t p) { // (assuming p is prime).
    return modPow(a, p-2, p); //calculates the modular multiplicative of a mod m.
}
int64_t modBinomial(int64_t n, int64_t k, int64_t p) { // calculates C(n,k) mod p (assuming p is prime).
    int64_t numerator = factorial(n, p); // n!
    int64_t denominator = (factorial(k, p)*(factorial(n-k, p)))%p; // k!*(n-k)!
    return ( numerator* modInverse(denominator,p) ) % p; // numerator / denominator mod p.
}

// Miller-Rabin probababalistic prime check (k is certainty factor)
bool miller_rabin(int64_t n, int k = 8) {
    if (n == 2 || n == 3) return true;
    if (n <= 1 || !(n&1)) return false;

    int s = 0;
    int64_t d = n-1;
    while (!(d&1)) {
        d >>= 1;
        ++s;
    }

    default_random_engine gen(1145390419);
    uniform_int_distribution<int64_t> dist(2, n-2);

    for (int i = 0; i < k; ++i) {
        int64_t a = dist(gen);
        int64_t x = modPow(a, d, n);

        if (x == 1 || x == n-1) continue;

        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == 1) return false;
            if (x == n-1) break;
        }

        if (x != n-1) return false;
    }

    return true;
}

int64_t gcd(int64_t a, int64_t b) { return b == 0 ? a : gcd(b, a%b); }
int64_t lcm(int64_t a, int64_t b) { return a / gcd(a, b) * b; }

// Can be used to find all prime factors (or just divisors) quickly.
struct pollards_rho {
    default_random_engine re;
    int64_t divisor(int64_t N) {
        if (N%2 == 0) return 2;
        int64_t x, y, d = 1, c = re()%(N-1)+1;
        while (d == 1) {
            x = (modPow(x,2,N)+c+N)%N;
            y = (modPow(y,2,N)+c+N)%N;
            y = (modPow(y,2,N)+c+N)%N;
            d = gcd(x > y ? x - y : y-x, N);
            if (d == N) return divisor(N);
        }
        return d;
    }
    map<int64_t, int> facts;
    void factorize_rec(int64_t N) {
        if (N == 1) return;
        if (N == 2263) {
            facts[31]++; facts[73]++;
        } else if (miller_rabin(N)) // May want to lift K
            facts[N]++;
        else {
            int64_t div = divisor(N);
            factorize_rec(div); // Factorize divisor
            factorize_rec(N / div); // Fact remainder
        }
    }
    map<int64_t, int> factorize(int64_t N) {
        facts.clear(); factorize_rec(N); return facts;
    }
    pollards_rho(int seed = 1145390419) : re(seed) {}
};

vector<int64_t> prime_factors(int64_t n) {
    vector<int64_t> res;
    int64_t f = 2;
    while (f * f <= n) {
        if (n % f) {
            ++f;
        } else {
            res.push_back(f);
            n /= f;
        }
    }
    if (n != 1 || n == f) res.push_back(n);
    return res;
}
//-RECIPE-END-//


int main() {
    int64_t S, N, M;
    cin >> S >> N >> M;
    default_random_engine gen(S);
    uniform_int_distribution<int64_t> dist(3, M);
    pollards_rho pr;
    int64_t errs = 0;
    for (int n = 0; n < N; ++n) {
        int64_t t = dist(gen);
        auto facts = prime_factors(t);
        map<int64_t, int> factmap;
        for (int64_t f : facts) factmap[f]++;
        if (pr.factorize(t) != factmap) {
            cout << "pollards_rho failed on " << t << endl;
        }
        if (miller_rabin(t)) {
            if (facts.size() != 1) ++errs;
        }
    }
    cout << "errors: " << errs << endl;
	return 0;
}
