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
  DisjointSparseTable(std::vector<T> arr) {
    int n = arr.size();
    int log_n = 0;
    while (n > (1 << log_n)) ++log_n;

    arr.resize(1 << log_n);
    n = arr.size();

    FillHighestBit(n);

    table.assign(log_n, std::vector<T>(n));
    table[0].assign(arr);
    
    for (int layer = 1; layer < log_n; ++layer) {
      for (int offset = 0; offset < n; offset += (1 << (log_n + 1))) {
        table[layer][offset + (1 << layer) - 1] = arr[offset + (1 << layer) - 1];
        for (int i = (1 << layer) - 2; i >= 0; --i) {
          table[layer][i] = table[layer][i + 1] + arr[i];
        }
        table[layer][offset + (1 << layer)] = arr[offset + (1 << layer)];
        for (int i = (1 << layer) + 1; i < (1 << (layer + 1)); ++i) {
          table[layer][i] = table[layer][i - 1] + arr[i];
        }
      }
    }
  }

  T SegmentQuery(int l, int r) {
    int index = highest_bit[l ^ r];
    if (index == 0) {
      return table[0][l];
    }
    --index;
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