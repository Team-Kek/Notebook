#include <climits>
#include <cstdio>

// Get the maximum sum of a submatrix in m.
int get_max_sum(int **m, int c, int r)
{
  // allocate memory for array of partial sums
  int **partials = new int*[r];
  for (int i = 0; i < r; i++) {
    partials[i] = new int[c];
  }
  // fill out the first row of partial sums
  for (int i = 0; i < c; i++) {
    partials[0][i] = m[0][i];
  }

  // fill out remaining rows
  for (int i = 1; i < r; i++) {
    for (int j = 0; j < c; j++) {
      partials[i][j] = partials[i-1][j] + m[i][j];
    }
  }

  int mat_best = INT_MIN;
  // look through all the pairs of top and bottom rows
  for (int i = 0; i < r; i++) {
    for (int j = i; j < r; j++) {
      // Find the best subarray of the array.
      int sum = 0;
      int arr_best = INT_MIN;
      for (int k = 0; k < c; k++) {
        int val = sum + (partials[j][k] - partials[i][k] + m[i][k]);
        // Update current best.
        if (val > arr_best) {
          arr_best = val;
        }
        // Update sum
        sum = val > 0 ? val : 0;
      }
      // Update current matrix best
      if (arr_best > mat_best) {
        mat_best = arr_best;
      }
    }
  }

  // free memory
  for (int i = 0; i < r; i++) {
    delete partials[i];
  }
  delete partials;
  return mat_best;
}
