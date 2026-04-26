#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

struct RevDSU {
  RevDSU(int n) : comp_size(n), size(n, 1), base(n, 0) {
    for (int i = 0; i < n; ++i) base[i] = i;
  }

  void Record() {
    records.push_back(history.size());
  }

  void Undo() {
    int time = records.back();
    records.pop_back();
    while (history.size() > time) {
      auto tmp = history.back();
      history.pop_back();
      *tmp.first = tmp.second;
    }
  }

  int Root(int a) const {
    if (a == base[a]) {
      return a;
    }
    return Root(base[a]);
  }

  void Union(int a, int b) {
    if ((a = Root(a)) != (b = Root(b))) {
      if (size[a] > size[b]) {
        swap(a, b);
      }
      history.emplace_back(&comp_size, comp_size);
      history.emplace_back(&size[b], size[b]);
      history.emplace_back(&base[a], base[a]);
      base[a] = b;
      size[b] += size[a];
      --comp_size;
    }
  }

  int CompSize() const { return comp_size; }

 private:
  int comp_size = 0;
  vector<int> size;
  vector<int> base;
  vector<pair<int*, int>> history;
  vector<int> records;
};

struct SegTree {
  int l;
  int r;
  int query = -1;
  vector<pair<int, int>> edges = vector<pair<int, int>>();
  SegTree* ls = nullptr;
  SegTree* rs = nullptr;

  SegTree(int l, int r) : l(l), r(r) {
    if (l != r) {
      ls = new SegTree(l, (l + r) / 2);
      rs = new SegTree((l + r) / 2 + 1, r);
    }
  }

  void AddQuery(int ver, int pos) {
    if (l == r) {
      if (pos == l) {
        query = ver;
      }
      return;
    }
    if (pos <= ls->r) {
      ls->AddQuery(ver, pos);
    } else {
      rs->AddQuery(ver, pos);
    }
  }

  void AddEdge(int ql, int qr, pair<int, int> edge) {
    if (qr < l || r < ql) {
      return;
    }
    if (ql <= l && r <= qr) {
      edges.push_back(edge);
      return;
    }
    ls->AddEdge(ql, qr, edge);
    rs->AddEdge(ql, qr, edge);
  }

  vector<int> GetAnswer(int n) {
    RevDSU dsu(n);
    vector<int> total;
    Process(dsu, total);
    return total;
  }

  void Process(RevDSU& dsu, vector<int>& total) {
    dsu.Record();
    for (auto edge : edges) {
      dsu.Union(edge.first, edge.second);
    }
    if (query != -1) {
      total.push_back(dsu.CompSize());
    }
    if (l != r) {
      ls->Process(dsu, total);
      rs->Process(dsu, total);
    }
    dsu.Undo();
  }
};

int main() {
  ifstream in("connect.in");
  ofstream out("connect.out");
  int n, k;
  in >> n >> k;
  if (k == 0) return 0;

  SegTree tree(0, k - 1);

  map<pair<int, int>, int> start;
  for (int i = 0; i < k; ++i) {
    char type;
    in >> type;
    if (type == '?') {
      tree.AddQuery(0, i);
    } else if (type == '+') {
      int u, v;
      in >> u >> v;
      --u, --v;
      start[{u, v}] = i;
      start[{v, u}] = i;
    } else {
      int u, v;
      in >> u >> v;
      --u, --v;
      tree.AddEdge(start[{u, v}], i - 1, {u, v});
      start.erase({u, v});
      start.erase({v, u});
    }
  }
  while (start.size() != 0) {
    auto [pr, ind] = *start.begin();
    auto [u, v] = pr;
    start.erase({u, v});
    start.erase({v, u});
    tree.AddEdge(ind, k - 1, pr);
  }
  auto ans = tree.GetAnswer(n);
  for (int i = 0; i < ans.size(); ++i) {
    out << ans[i] << '\n';
  }
  return 0;
}
