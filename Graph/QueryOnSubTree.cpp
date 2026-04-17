#include <iostream>
#include <vector>
#include <algorithm>

// Main idea to use euler route so query became on a segment

using Ll = long long;

struct ST {
  Ll sum = 0;
  int l;
  int r;
  ST* ls = nullptr;
  ST* rs = nullptr;

  ST(int l, int r) : l(l), r(r) {
    if (l != r) {
      ls = new ST(l, (l + r) / 2);
      rs = new ST((l + r) / 2 + 1, r);
    }
  }

  void Update() {
    if (l != r) {
      sum = ls->sum + rs->sum;
    }
  }

  void IncPos(Ll value, int pos) {
    if (l == r) {
      if (pos == l) {
        sum += value;
      }
      return;
    }
    if (pos <= (l + r) / 2) {
      ls->IncPos(value, pos);
    } else {
      rs->IncPos(value, pos);
    }
    Update();
  }

  Ll GetSum(int ql, int qr) {
    if (r < ql || l > qr) {
      return 0;
    }
    if (ql <= l && r <= qr) {
      return sum;
    }
    return ls->GetSum(ql, qr) + rs->GetSum(ql, qr);
  }
};

void EulerTour(const std::vector<std::vector<int>>& graph, std::vector<int>& route, int curr = 0) {
  route.push_back(curr);
  for (int next : graph[curr]) {
    EulerTour(graph, route, next);
  }
  route.push_back(curr);
}

int main() {
  int n, q;
  std::cin >> n >> q;
  std::vector<int> nums(n, 0);
  std::vector<std::vector<int>> graph(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> nums[i];
  }
  std::vector<int> parent(n, -1);
  for (int i = 1; i < n; ++i) {
    std::cin >> parent[i];
    graph[parent[i]].push_back(i);
  }
  std::vector<int> route;
  EulerTour(graph, route);

  std::vector<int> first(n, -1);
  std::vector<int> last(n, -1);

  ST tree(0, route.size() - 1);
  for (int i = 0; i < route.size(); ++i) {
    int curr = route[i];
    if (first[curr] == -1) {
      first[curr] = i;
    }
    last[curr] = i;
  }

  for (int i = 0; i < n; ++i) {
    tree.IncPos(nums[i], first[i]);
  }
  for (int i = 0; i < q; ++i) {
     int type;
     std::cin >> type;
     if (type == 0) {
      int u;
      Ll x;
      std::cin >> u >> x;
      tree.IncPos(x, first[u]);
     } else {
      int x;
      std::cin >> x;
      std::cout << tree.GetSum(first[x], last[x]) << '\n';
     }
  }
  return 0;
}