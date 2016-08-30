#include <climits>

int max_sub_array(int *arr, int sz) {
  int max = INT_MIN;
  int val = 0;
  for (int i = 0; i < sz; i++) {
    int tval = val + arr[i];
    if (tval > max) {
      max = tval;
    }
    if (tval > 0) {
      val = val + arr[i];
    } else {
      val = 0;
    }
  }
  return max;
}
