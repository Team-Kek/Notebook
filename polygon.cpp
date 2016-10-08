#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long double coord_t;

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

typedef vector<Point> Polygon;

coord_t poly_twice_area(const Polygon& poly) {
	coord_t twice_area = 0;
	Point prev = poly.back();
	for (Point curr : poly) {
		twice_area += prev.cross(curr);
		prev = curr;
	}
	return twice_area;
}
coord_t poly_area(const Polygon& poly) { return poly_twice_area(poly) / 2; }

pair<int, bool> poly_winding(const Polygon& poly, Point pt) {
	int wn = 0;
	Point prev = poly.back() - pt;
	for (Point curr : poly) {
		curr = curr - pt;
		if (prev.cross(curr) == 0 && prev * curr <= 0) return pair<int, bool>(0, true);
		else if (prev.y <= 0 && 0 < curr.y && prev.cross(curr) > 0) ++wn;
		else if (curr.y <= 0 && 0 < prev.y && prev.cross(curr) < 0) --wn;
		prev = curr;
	}
	return pair<int, bool>(wn, false);
}

pair<int, bool> poly_curl(const Polygon& poly) {
	int curl = 0;
	Point ref = poly[1] - poly[0];
	Point srt = poly.back(), end = poly.front();
	for (Point cur : poly) {
		if (cur == end) continue;
		if ((cur-end).cross(end-srt) == 0 && (cur-end) * (end-srt) < 0) return pair<int, bool>(0, true);
		else if (0 < ref.cross(cur-end) && ref.cross(end-srt) <= 0 && 0 < (end-srt).cross(cur-end)) ++curl;
		else if (0 < ref.cross(end-srt) && ref.cross(cur-end) <= 0 && 0 < (cur-end).cross(end-srt)) --curl;
		srt = end; end = cur;
	}
	return pair<int, bool>(curl, false);
}

Point poly_centroid(const Polygon& poly) {
	Point num;
	coord_t den = 0;
	Point prev = poly.back();
	for (Point curr : poly) {
		num = num + (prev + curr) * prev.cross(curr);
		den += prev.cross(curr);
		prev = curr;
	}
	return num / den / 3;
}

Polygon convex_hull(vector<Point> pts) {
	sort(pts.begin(), pts.end());
	Polygon upr, lwr;
	for (Point pt : pts) {
		while (upr.size() >= 2 && (upr.back() - upr[upr.size()-2]).cross(pt - upr.back()) >= 0)
			upr.pop_back();
		upr.push_back(pt);
		while (lwr.size() >= 2 && (lwr.back() - lwr[lwr.size()-2]).cross(pt - lwr.back()) <= 0)
			lwr.pop_back();
		lwr.push_back(pt);
	}
	reverse(upr.begin(), upr.end()); 
	upr.pop_back(); lwr.pop_back();
	lwr.insert(lwr.end(), upr.begin(), upr.end()); 
	return lwr;
}
