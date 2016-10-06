#include <iostream>
#include <cstdlib>
#include <cstdint>

using namespace std;

typedef int64_t nd_t;

nd_t gcd(nd_t a, nd_t b) {
	if (b == 0) return a;
	else return gcd(b, a%b);
}

inline nd_t abso(nd_t arg) {
	return arg < 0 ? -arg : arg;
}

struct rational {
	nd_t num, den;
	rational() : num(0), den(1) {}
	rational(nd_t num) : num(num), den(1) {}
	rational(nd_t num, nd_t den) : num(num), den(den) {
		if (den == 0) throw "Division by zero";
		if (num == 0) den = 1;
		if (den < 0) {
			num *= -1;
			den *= -1;
		}
		reduce();
	}
	rational(const rational& cpy) : rational(cpy.num, cpy.den) {}
	void reduce() {
		nd_t div = abso(gcd(num, den));
		num /= div;
		den /= div;
		if (den < 0) {
			num *= -1;
			den *= -1;
		}
	}
	rational operator+(const rational& rhs) const {
		nd_t n, d, div = gcd(den, rhs.den);
		n = num*(rhs.den/div)+rhs.num*(den/div);
		d = den/div*rhs.den;	
		return rational(n, d);
	}
	rational operator-() const {
		return rational(-num, den);
	}
	rational operator-(const rational& rhs) const {
		return (*this)+(-rhs);
	}
	rational operator*(const rational& rhs) const {
		if (num == 0 || rhs.num == 0) return rational(0,1);
		nd_t n, d, diva = gcd(num, rhs.den), divb = gcd(rhs.num, den);
		n = (num/diva)*(rhs.num/divb);
		d = (den/divb)*(rhs.den/diva);
		return rational(n, d);
	}
	rational operator/(const rational& rhs) const {
		return (*this)*rational(rhs.den, rhs.num);
	}
	bool operator<(const rational& rhs) const {
		nd_t div = gcd(den, rhs.den);
		return rhs.den/div*num < den/div*rhs.num;
	}
	bool operator==(const rational& rhs) const {
		return num==rhs.num && den==rhs.den;
	}
	friend ostream& operator<<(ostream& os, const rational& rhs) {
		rational out(rhs);
		out.reduce();
		os << out.num << '/' << out.den;
		return os;
	}
};

// Convert double to rational
rational stern_brocot(double x) {
	const double eps = 0.000001;

	nd_t mn = 1, md = 1; // Current node
	nd_t ln = 0, ld = 1; // Left ancestor
	nd_t rn = 1, rd = 0; // Right ancestor
	
	while (mn < md * (x - eps) || md * (x + eps) < mn) {
		if (mn < md * x) { // Go right
			ln = mn;	ld = md;
			mn += rn;	md += rd;
		} else if (md * x < mn) { // Go left
			rn = mn;	rd = md;
			mn += ln;	md += ld;
		} else break;
	}

	return rational(mn, md);
}

int main() {
	cout << "Enter double to convert to rational:" << endl;
	double x;
	cin >> x;
	cout << stern_brocot(x) << endl;
	cout << "Enter numerator and denominator for A:" << endl;
	nd_t An, Ad;
	cin >> An >> Ad;
	rational A(An, Ad);
	cout << "A = " << A << endl;
	cout << "Enter numerator and denominator for B:" << endl;
	nd_t Bn, Bd;
	cin >> Bn >> Bd;
	rational B(Bn, Bd);
	cout << "B = " << B << endl;
	cout << "A + B = " << A + B << endl;
	cout << "A - B = " << A - B << endl;
	cout << "A * B = " << A * B << endl;
	cout << "A / B = " << A / B << endl;
	return 0;
}
