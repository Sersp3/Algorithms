#include <iostream>
#include <vector>
#include <algorithm>

// Disjoint Sparse Table
// O(n log n) Init
// O(1) query
// Use + as an operation 

template <typename T>
class DisjointSparseTable {
 public:
  DisjointSparseTable() {}

  DisjointSparseTable(std::vector<T> arr) {
    Init(arr);
  }

  void Init(std::vector<T> arr) {
    int n = arr.size();
    int log_n = 0;
    while (n > (1 << log_n)) ++log_n;

    arr.resize(1 << log_n);
    n = arr.size();

    FillHighestBit(n);

    table.assign(log_n, std::vector<T>(n));

    for (int level = 0; level < log_n; ++level) {
      int half = (1 << level);
      int block = (1 << (level + 1));
      for (int mid = half; mid < n; mid += block) {
        table[level][mid - 1] = arr[mid - 1];
        for (int i = mid - 2; i >= mid - half; --i) 
          table[level][i] = arr[i] + table[level][i + 1];

        table[level][mid] = arr[mid];
        for (int i = mid + 1; i < std::min(n, mid + half); ++i)
          table[level][i] = table[level][i - 1] + arr[i];
      }
    }
  }

  T SegmentQuery(int l, int r) {
    int index = highest_bit[l ^ r];
    if (index == 0) {
      return table[index][l];
    }
    return table[index][l] + table[index][r];
  }
 private:
  std::vector<int> highest_bit;
  std::vector<std::vector<T>> table;

  void FillHighestBit(int n) {
    highest_bit.assign(n, 0);
    for (int i = 1; i < n; ++i) {
      highest_bit[i] = 1 + highest_bit[i >> 1];
    }
  }
};
