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

class OnlineBridges {
 public:
  OnlineBridges(int size)
      : strong_connectivity_(size),
        forest_trees_(size),
        parent_(size, -1),
        used_(size, -1),
        id_(size, -1) {}

  int GetBridgesCount() const { return bridges_; }

  const std::set<int>& GetBridges() const { return bridges_id_; }

  void AddEdge(int a, int b, int id) {
    a = strong_connectivity_.FindSet(a);
    b = strong_connectivity_.FindSet(b);

    if (a == b) {
      return;
    }

    int fa = forest_trees_.FindSet(a);
    int fb = forest_trees_.FindSet(b);

    if (fa != fb) {
      if (forest_trees_.GetSize(fa) < forest_trees_.GetSize(fb)) {
        std::swap(a, b);
        std::swap(fa, fb);
      }
      MakeRoot(b);
      parent_[b] = a;
      id_[b] = id;
      bridges_id_.insert(id);
      ++bridges_;

      forest_trees_.Union(a, b);
    } else {
      auto cycle = FindLCA(a, b);
      bridges_ -= static_cast<int>(cycle.size()) - 1;
      for (size_t i = 1; i < cycle.size(); ++i) {
        bridges_id_.erase(id_[cycle[i]]);
      }
      strong_connectivity_.Union(cycle);
      strong_connectivity_.SetRoot(cycle[0]);
    }
  }

 private:
  DSU strong_connectivity_;
  DSU forest_trees_;
  std::vector<int> parent_;
  std::vector<int> used_;
  std::vector<int> id_;
  int curr_used_ = 0;
  int bridges_ = 0;
  std::set<int> bridges_id_;

  std::vector<int> FindLCA(int a, int b) {
    a = strong_connectivity_.FindSet(a);
    b = strong_connectivity_.FindSet(b);
    std::vector<int> fs;
    std::vector<int> sc;

    int lca = -1;
    ++curr_used_;
    while (true) {
      if (a != -1 && used_[a] == curr_used_) {
        lca = a;
        break;
      }
      if (a != -1) {
        used_[a] = curr_used_;
        fs.push_back(a);
        a = strong_connectivity_.FindSet(parent_[a]);
      }

      if (b != -1 && used_[b] == curr_used_) {
        lca = b;
        break;
      }

      if (b != -1) {
        used_[b] = curr_used_;
        sc.push_back(b);
        b = strong_connectivity_.FindSet(parent_[b]);
      }
    }
    std::vector<int> total;
    total.push_back(lca);
    for (int x : fs) {
      if (x == lca) {
        break;
      }
      total.push_back(x);
    }
    for (int x : sc) {
      if (x == lca) {
        break;
      }
      total.push_back(x);
    }
    return total;
  }

  void MakeRoot(int a) {
    a = strong_connectivity_.FindSet(a);
    int prev = -1;
    int prev_id = -1;
    while (a != -1) {
      int next = strong_connectivity_.FindSet(parent_[a]);
      int next_id = id_[a];
      parent_[a] = prev;
      id_[a] = prev_id;
      prev_id = next_id;
      prev = a;
      a = next;
    }
  }
};

int main() {
  int n;
  int m;
  std::cin >> n >> m;
  OnlineBridges temp(n);
  std::vector<int> sizes(m, 0);
  for (int i = 0; i < m; ++i) {
    int u;
    int v;
    std::cin >> u >> v;
    temp.AddEdge(u - 1, v - 1, i + 1);
  }

  auto result = temp.GetBridges();
  std::cout << temp.GetBridgesCount() << '\n';
  for (int x : result) {
    std::cout << x << ' ';
  }
  return 0;
}