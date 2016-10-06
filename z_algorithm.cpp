#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Computes the Z function.
// Suppose we are given a string s of length n. The Z-function for this string is an array of length n where
// the i-th element is equal to the greatest number of characters starting
// from the position i that coincide with the first characters of s.
// Tested on UVA 455 - Periodic Strings.
//-RECIPE-START-//
// return_vector[i] is the length of the longest common prefix
// between s and the suffix of s starting at i.
vector<int> zalgorithm(const string &s) {
    int n = s.size();
    vector<int> z(n);
    int L = 0, R = 0;
    for (int i = 1; i < n; i++) {
        if (i > R) {
            L = R = i;
            while (R < n && s[R-L] == s[R]) R++;
            z[i] = R-L; R--;
        } else {
            int k = i-L;
            if (z[k] < R-i+1) z[i] = z[k];
            else {
                L = i;
                while (R < n && s[R-L] == s[R]) R++;
                z[i] = R-L; R--;
            }
        }
    }
    z[0] = n;
    return z;
}
//-RECIPE-END-//

int main() {
    string s;
    cin >> s;
    auto z = zalgorithm(s);
    for (int i : z)
    	cout << i << " ";
    cout << endl;
    return 0;
}
