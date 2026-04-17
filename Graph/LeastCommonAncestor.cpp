#include <iostream>
#include <vector>

const int cMax = 1e9;

struct STData {
  int min;
  int id;

  STData(int min, int id) : min(min), id(id) {}

  STData() : min(cMax), id(-1) {}

  bool operator<(const STData& other) const {
    return min < other.min;
  }
};

class ST {
 public:
  ST(int l, int r) : l(l), r(r) {
    if (l != r) {
      ls = new ST(l, (l + r) / 2);
      rs = new ST((l + r) / 2 + 1, r);
    }
  }

  ST() {}

  ~ST() {
    delete ls;
    delete rs;
  }

  void Init(int ql, int qr) {
    delete ls;
    delete rs;
    l = ql;
    r = qr;
    ls = new ST(l, (l + r) / 2);
    rs = new ST((l + r) / 2 + 1, r);
  }

  void SetPos(STData info, int pos) {
    if (l == r) {
      if (pos == l) {
        data = info;
      }
      return;
    }

    if (pos <= (l + r) / 2) {
      ls->SetPos(info, pos);
    } else {
      rs->SetPos(info, pos);
    }
    data = std::min(ls->data, rs->data);
  }

  STData GetMin(int ql, int qr) const {
    if (r < ql || qr < l) {
      return STData();
    }
    if (ql <= l && r <= qr) {
      return data;
    }
    return std::min(ls->GetMin(ql, qr), rs->GetMin(ql, qr));
  }

 private:
  int l;
  int r;
  STData data;
  ST* ls = nullptr;
  ST* rs = nullptr;
};

class LCA {
 public:
  LCA(const std::vector<std::vector<int>>& g, int root) : graph(g),
                                                          height(g.size(), -1),
                                                          place(g.size(), -1) {
    height[root] = 0;
    DFS(root);
    tree.Init(0, dfs_order.size() - 1);
    for (int i = 0; i < dfs_order.size(); ++i) {
      int curr = dfs_order[i];
      tree.SetPos(STData(height[curr], curr), i);
      place[curr] = i;
    }
  }

  int GetLCA(int a, int b) {
    int fs = std::min(place[a], place[b]);
    int sc = std::max(place[a], place[b]);
    return tree.GetMin(fs, sc).id;
  }

 private:
  ST tree;
  std::vector<std::vector<int>> graph;
  std::vector<int> height;
  std::vector<int> dfs_order;
  std::vector<int> place;

  void DFS(int start) {
    dfs_order.push_back(start);
    for (int next : graph[start]) {
      if (height[next] == -1) {
        height[next] = height[start] + 1;
        DFS(next);
        dfs_order.push_back(start);
      }
    }
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int n;
  std::cin >> n;
  std::vector<std::vector<int>> graph(n);
  for (int i = 0; i < n - 1; ++i) {
    int u;
    int v;
    std::cin >> u >> v;
    graph[v].push_back(u);
    graph[u].push_back(v);
  }
  LCA test(graph, 0);
  int q;
  std::cin >> q;
  for (int i = 0; i < q; ++i) {
    int a, b;
    std::cin >> a >> b;
    std::cout << test.GetLCA(a, b) << '\n';
    std::cout.flush();
  }
  return 0;
}