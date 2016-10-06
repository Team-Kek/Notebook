#include <limits>
#include <cmath>
#include <iostream>

#include "CompGeom.cpp"

using namespace std;

typedef long double coord_t;
const long double EPS = 1e-7;

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }
template <typename T> T tabs(T val) { return val < T(0) ? -val : val; }
template <typename T> bool eq(T lhs, T rhs, T eps) { return tabs(lhs - rhs) <= EPS * max(tabs(lhs), tabs(rhs)); }

struct Point {
	coord_t x, y;
	Point() : x(0), y(0) {}
	Point(coord_t x, coord_t y) : x(x), y(y) {}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
	Point operator*(const coord_t rhs) const {
		return Point(x * rhs, y * rhs);
	}
	Point operator/(const coord_t rhs) const {
		return Point(x / rhs, y / rhs);
	}
	coord_t operator*(const Point& rhs) const {
		return (x * rhs.x) + (y * rhs.y);
	}
	coord_t cross(const Point& rhs) const {
		return (x * rhs.y) - (y * rhs.x);
	}
	Point norm() {
		return Point(-y, x);
	}
	coord_t sqmag() {
		return x*x + y*y;
	}
	coord_t mag() {
		return sqrt(sqmag());
	}
	Point unit() {
		coord_t m = mag();
		return Point(x/m, y/m);
	}
	bool operator<(const Point& rhs) const {
		if (x == rhs.x) return y < rhs.y;
		else return x < rhs.x;
	}
	bool operator>(const Point& rhs) const {
		if (x == rhs.x) return y > rhs.y;
		else return x > rhs.x;
	}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return x != rhs.x || y != rhs.y;
	}
	friend ostream& operator<<(ostream& os, const Point& rhs) {
		os << "(" << rhs.x << ", " << rhs.y << ")";
		return os;
	}
};

struct Line {
	Point s, e;
	Line() : s(), e() {}
	Line(Point s, Point e) : s(s), e(e) {}
	Point vel() {
		return e - s;
	}
	friend ostream& operator<<(ostream& os, const Line& rhs) {
		os << rhs.s << " - " << rhs.e;
		return os;
	}
	bool contains(Point pt) {
		if (s == e) return pt == s;
		Point proj = Point((pt - s) * vel(), (pt - s) * vel().norm()) / vel().sqmag();
		if (0 <= proj.x && proj.x <= 1) proj.x = 0;
		return proj.mag() <= EPS;
	}
	pair<bool, Line> intersection(Line rhs) {
		Line lhs = *this;
		Point lv = lhs.vel(), rv = rhs.vel();
		coord_t den = lv.cross(rv);
		Point num = (lv * rhs.s.cross(rhs.e) - rv * lhs.s.cross(lhs.e)) * sgn(den);
		den *= sgn(den);
		if (den == 0) {	// Parallel or Point
			if (lv.cross(rhs.s-lhs.s) == 0 && rv.cross(lhs.s-rhs.s) == 0) {	// Collinear or Point
				if (lhs.s > lhs.e) swap(lhs.s, lhs.e);
				if (rhs.s > rhs.e) swap(rhs.s, rhs.e);
				Line result(max(lhs.s, rhs.s), min(lhs.e, rhs.e));
				if (result.s > result.e) {
					return pair<bool, Line>(false, Line());
				} else {
					return pair<bool, Line>(true, result);
				}
			} else {
				return pair<bool, Line>(false, Line());
			}
		} else {
			Point inter = num / den;
			return pair<bool, Line>(lhs.contains(inter) && rhs.contains(inter), Line(inter, inter));
		}
	}
};

int main() {
	coord_t eps = 0;
	coord_t lwr = -4, upr = 4;
	for (coord_t lsx = lwr; lsx <= upr; ++lsx) {
		cout << "lsx = " << lsx << endl;
		for (coord_t lsy = lwr; lsy <= upr; ++lsy) {
			cout << "lsy = " << lsy << endl;
			for (coord_t lex = lwr; lex <= upr; ++lex) {
				for (coord_t ley = lwr; ley <= upr; ++ley) {
					for (coord_t rsx = lwr; rsx <= upr; ++rsx) {
						for (coord_t rsy = lwr; rsy <= upr; ++rsy) {
							for (coord_t rex = lwr; rex <= upr; ++rex) {
								for (coord_t rey = lwr; rey <= upr; ++rey) {
									Line l(Point(lsx, lsy), Point(lex, ley));
									Line r(Point(rsx, rsy), Point(rex, rey));
									auto inter = l.intersection(r);

									line2d el, er;
									el.a.x = lsx; el.a.y = lsy;
									el.b.x = lex; el.b.y = ley;
									er.a.x = rsx; er.a.y = rsy;
									er.b.x = rex; er.b.y = rey;
									line2d en;
									dbl aprop = 0, bprop = 0;
									bool inf;
									bool result = inter2d(el, er, true, aprop, bprop, en, inf);
									pair<bool, Line> einter(result, Line(Point(en.a.x, en.a.y), Point(en.b.x, en.b.y)));

									if (inter.second.s > inter.second.e) swap(inter.second.s, inter.second.e);
									if (einter.second.s > einter.second.e) swap(einter.second.s, einter.second.e);
									if (inter.first != einter.first || inter.first == true && (
										(einter.second.s - inter.second.s).mag() > EPS ||
										(einter.second.e - inter.second.e).mag() > EPS)) {
										cerr << "FAILURE\t" << l << "\t" << r << endl;
										cerr << "\t" << inter.second << endl;
										cerr << "\t" << einter.second << endl;
										cerr << "\t" << (inter.first?"true":"false") << " " << (einter.first?"true":"false") << endl;
									}
								}
							}
						}	
					}	
				}	
			}	
		}
	}
}
