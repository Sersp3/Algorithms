#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

class DSU {
 public:
  DSU(int size) : parent_(size), size_(size, 1) {
    for (int i = 0; i < size; ++i) {
      parent_[i] = i;
    }
  }

  int FindSet(int a) {
    if (a == -1) {
      return -1;
    }
    if (parent_[a] == a) {
      return a;
    }
    return parent_[a] = FindSet(parent_[a]);
  }

  int GetSize(int a) { return size_[FindSet(a)]; }

  void SetRoot(int a) {
    int root = a;
    a = FindSet(a);
    parent_[a] = root;
    parent_[root] = root;
    size_[root] = size_[a];
  }

  void Union(const std::vector<int>& array) {
    for (size_t i = 1; i < array.size(); ++i) {
      Union(array[0], array[i]);
    }
  }

  void Union(int a, int b) {
    a = FindSet(a);
    b = FindSet(b);
    if (a == b) {
      return;
    }
    if (size_[a] < size_[b]) {
      std::swap(a, b);
    }
    parent_[b] = a;
    size_[a] += size_[b];
  }

 private:
  std::vector<int> parent_;
  std::vector<int> size_;
};
