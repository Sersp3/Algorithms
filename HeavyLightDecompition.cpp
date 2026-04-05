#include <iostream>
#include <vector>

const int cMax = 1e9;

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

  void SetPos(int info, int pos) {
    if (l == r) {
      if (pos == l) {
        max = info;
      }
      return;
    }

    if (pos <= (l + r) / 2) {
      ls->SetPos(info, pos);
    } else {
      rs->SetPos(info, pos);
    }
    max = std::max(ls->max, rs->max);
  }

  void IncPos(int value, int pos) {
    if (l == r) {
      if (pos == l) {
        max += value;
      }
      return;
    }

    if (pos <= (l + r) / 2) {
      ls->IncPos(value, pos);
    } else {
      rs->IncPos(value, pos);
    }
    max = std::max(ls->max, rs->max);
  }

  int GetMax(int ql, int qr) const {
    if (ql > qr) {
      return GetMax(qr, ql);
    }
    if (r < ql || qr < l) {
      return 0;
    }
    if (ql <= l && r <= qr) {
      return max;
    }
    return std::max(ls->GetMax(ql, qr), rs->GetMax(ql, qr));
  }

 private:
  int l;
  int r;
  int max = 0;
  ST* ls = nullptr;
  ST* rs = nullptr;
};

class HLD {
 public:
  HLD(const std::vector<std::vector<int>>& g, std::vector<int>& p) : parent(p),
                                                                     graph(g),
                                                                     tin(p.size(), -1),
                                                                     tout(p.size(), -1),
                                                                     size(p.size(), -1),
                                                                     seg_tree(0, p.size() - 1),
                                                                     head(p.size(), -1) {
    SetSize();
    head[0] = 0;
    DFS();
  }

  void SetPos(int value, int pos) {
    seg_tree.SetPos(value, tin[pos]);
  }

  void IncPos(int value, int pos) {
    seg_tree.IncPos(value, tin[pos]);
  }

  int FindMax(int a, int b) {
    int total = 0;
    Up(a, b, total);
    Up(b, a, total);
    total = std::max(total, seg_tree.GetMax(tin[a], tin[b]));
    return total;
  }


 private:
  std::vector<int> parent;
  std::vector<std::vector<int>> graph;
  std::vector<int> tin;
  std::vector<int> tout;
  std::vector<int> size;
  std::vector<int> head;
  ST seg_tree;

  bool Ancestor (int a, int b) {
    return tin[a] <= tin[b] && tin[b] < tout[a];
  }

  void Up(int& a, int& b, int& total) {
    while (!Ancestor(head[a], b)) {
      total = std::max(total, seg_tree.GetMax(tin[a], tin[head[a]]));
      a = parent[head[a]];
    }
  }

  void DFS(int start = 0) {
    static int timer = 0;
    tin[start] = timer++;
    for (int next : graph[start]) {
      head[next] = (next == graph[start][0] ? head[start] : next);
      DFS(next);
    }
    tout[start] = timer;
  }

  void SetSize(int start = 0) {
    size[start] = 1;
    for (int& next : graph[start]) {
      SetSize(next);
      size[start] += size[next];
      if (size[next] > size[graph[start][0]]) {
        std::swap(next, graph[start][0]);
      }
    }
  }
};

void SplitTree(const std::vector<std::vector<int>>& graph,
               std::vector<std::vector<int>>& out_graph,
               std::vector<int>& out_parent) {
  std::vector<bool> used(graph.size(), false);
  out_graph.resize(graph.size());
  out_parent.resize(graph.size());

  auto DFS = [&](auto self, int start = 0) -> void {
    used[start] = true;
    for (int next : graph[start]) {
      if (used[next]) {
        out_parent[start] = next;
      } else {
        out_graph[start].push_back(next);
        self(self, next);
      }
    }
  };

  DFS(DFS);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  

  int n;
  std::cin >> n;
  std::vector<std::vector<int>> temp(n);
  std::vector<std::vector<int>> g;
  std::vector<int> p;
  for (int i = 0; i < n - 1; ++i) {
    int u, v;
    std::cin >> u >> v;
    --u;
    --v;
    temp[u].push_back(v);
    temp[v].push_back(u);
  }
  SplitTree(temp, g, p);
  HLD test(g, p);
  int q;
  std::cin >> q;
  for (int i = 0; i < q; ++i) {
    char type;
    std::cin >> type;
    if (type == 'I') {
      int u, v;
      std::cin >> u >> v;
      --u;
      test.IncPos(v, u);
    } else {
      int u, v;
      std::cin >> u >> v;
      --u;
      --v;
      std::cout << test.FindMax(u, v) << '\n';
    }
  }
  return 0;
}