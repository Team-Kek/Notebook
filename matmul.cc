#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>

constexpr int64_t kModuloValue = 1000000007;

template<typename T>
struct Mat {
  T &operator()(size_t i, size_t j) {
    return m[i * cols + j];
  }
  const T &operator()(size_t i, size_t j) const {
    return m[i * cols + j];
  }
  Mat(size_t r, size_t c, T val = 0)
    : cols(c), rows(r), m(c * r, val) { }

  Mat operator*(const Mat &that) const {
    if (cols != that.rows) {
      std::cerr << "Error: incompatible dims." << std::endl;
      exit(1);
    }
    Mat next(rows, that.cols);
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < that.cols; j++) {
        for (size_t k = 0; k < cols; k++) {
          next(i,j) += (((*this)(i, k) * that(k, j)) % kModuloValue);
          next(i,j) %= kModuloValue;
        }
      }
    }
    return next;
  }
  size_t cols, rows;
  std::vector<T> m;
};

template<typename T>
Mat<T> Exp(const Mat<T> &m, size_t n) {
  if (m.cols != m.rows) {
    std::cerr << "Error: not square." << std::endl;
    exit(1);
  }
  if (n == 0) {
    Mat<T> eye(m.cols, m.cols);
    for (size_t i = 0; i < m.cols; i++) {
      eye(i,i) = 1;
    }
    return eye;
  }
  if (n == 1) return m;
  Mat<T> next = Exp(m, n/2);
  next = next * next;
  if (n%2) return next * m;
  return next;
}
