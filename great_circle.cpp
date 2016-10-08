/* GreatCircle.cpp
 * Dijkstras on Bijkstras Programming Competition Notebook 2014
 * Tested on UVa 535
 */

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>

typedef int64_t i64;
typedef uint64_t ui64;

#define forl(i,a,b) for(int64_t i = (a); i < (b); ++i)

using namespace std;

//-RECIPE-START-//

struct City {
	double lat;
	double lon;
};

//#undef M_PI
//#define M_PI 3.141592653589793

const double piOver180 = M_PI/180.0;
// Degrees to Radians
double deg2Rad(double deg) {
	return deg * piOver180;
}

// Finds the Great Circle dist in km from (lat1, lon1) -> (lat2, lon2), inputs
// in radians.
double solve(double lat1, double lon1, double lat2, double lon2) {
	const double earthRadius = 6378.0; // In km

	// This algorithm generates enough numerical error to get answers wrong.
	//double d = earthRadius * acos( cos(lat1) * cos(lat2) * cos(lon1 - lon2) +
	//                               sin(lat1) * sin(lat2) );
	
	// This one is much more accurate, but more typing.
	double tmp1 = cos(lat2)*sin(lon2-lon1);
	double tmp2 = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(lon2-lon1);
	double d = earthRadius * atan2(sqrt(tmp1*tmp1 + tmp2*tmp2),
			( sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(lon2-lon1) ));
	//cerr << "Distance (" << lat1 << ", " << lon1 << ") - 
	//		(" << lat2 << ", " << lon2 << "): " << d << endl;
	return d;
}

map<string, City> cities;

//-RECIPE-END-//

int main() {
	while (true) {
		string name;
		City c;
		cin >> name;
		if (name == "#") break;
		cin >> c.lat >> c.lon;
		cities[name] = c;
	}
	while (true) {
		string a, b;
		cin >> a >> b;
		if (a == "#") break;
		cout << a << " - " << b << endl;
		if (cities.count(a) && cities.count(b)) {
			double dist = solve(deg2Rad(cities[a].lat), deg2Rad(cities[a].lon),
					deg2Rad(cities[b].lat), deg2Rad(cities[b].lon));
//			cerr << dist << endl;
			i64 ans = round(dist);
			cout << ans << " km" << endl;
		}
		else {
			cout << "Unknown" << endl;
		}
	}
	return 0;
}
