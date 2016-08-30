template<typename T>
T *binary_search(T *start, T *end, const T& val) {
  while (end > start) {
    T *mid = start + (end-start)/2;
    if (*mid == val) {
      return mid;
    } else if (*mid > val) {
      end = mid-1;
    } else {
      start = mid+1;
    }
  }
  return end;
}
