#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using Ll = long long;
const Ll cInf = 1e18;

// E * V^2 Diniza
// E*V log C MashtabDiniza

struct Edge {
  int to;
  int symm_ind;
  Ll capacity;
  Ll flow;
  int id;

  Edge(int t, int i, Ll c, Ll f, int id)
      : to(t), symm_ind(i), capacity(c), flow(f), id(id) {}
};

Ll Diniza(std::vector<std::vector<Edge>>& g, int s, int t, Ll value = 1) {
  int n = static_cast<int>(g.size());
  std::vector<size_t> pos(n, 0);
  std::vector<int> layer(n, -1);

  auto bfs = [&]() -> bool {
    layer.assign(n, -1);
    pos.assign(n, 0);
    std::queue<int> queue;
    queue.push(s);
    layer[s] = 0;
    bool reachable = false;
    while (!queue.empty()) {
      int curr = queue.front();
      if (curr == t) {
        reachable = true;
      }
      queue.pop();
      for (const Edge& edge : g[curr]) {
        if (layer[edge.to] != -1 || edge.capacity - edge.flow < value) {
          continue;
        }
        queue.push(edge.to);
        layer[edge.to] = layer[curr] + 1;
      }
    }
    return reachable;
  };

  auto dfs = [&](auto dfs, int curr, Ll flow = cInf) -> Ll {
    if (flow == 0) {
      return 0;
    }
    if (curr == t) {
      return flow;
    }

    while (pos[curr] != g[curr].size()) {
      Edge& edge = g[curr][pos[curr]];

      if (layer[edge.to] != layer[curr] + 1) {
        ++pos[curr];
        continue;
      }

      Ll pushed =
          dfs(dfs, edge.to,
              std::min(flow, ((edge.capacity - edge.flow) / value) * value));
      if (pushed == 0) {
        ++pos[curr];
        continue;
      }
      edge.flow += pushed;
      g[edge.to][edge.symm_ind].flow -= pushed;
      return pushed;
    }
    return 0;
  };

  while (bfs()) {
    while (dfs(dfs, s) != 0) {
    }
  }

  Ll max_flow = 0;
  for (const Edge& edge : g[s]) {
    max_flow += edge.flow;
  }
  return max_flow;
}

Ll MashtabDiniza(std::vector<std::vector<Edge>>& g, int s, int t) {
  Ll flow_bound = 0;
  for (const Edge& edge : g[s]) {
    flow_bound += edge.capacity;
  }

  Ll log_n = 0;

  while (flow_bound >= (1LL << log_n)) {
    ++log_n;
  }

  for (Ll i = log_n; i >= 0; --i) {
    Diniza(g, s, t, 1LL << i);
  }
  Ll total_flow = 0;
  for (const Edge& edge : g[s]) {
    total_flow += edge.flow;
  }
  return total_flow;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int n;
  int m;
  std::cin >> n >> m;

  std::vector<std::vector<Edge>> g(n);
  for (int i = 0; i < m; ++i) {
    int u;
    int v;
    Ll c;
    std::cin >> u >> v >> c;
    --u, --v;
    Edge f(v, g[v].size(), c, 0, i);
    Edge s(u, g[u].size(), 0, 0, i);
    g[u].push_back(f);
    g[v].push_back(s);
  }

  Ll flow = MashtabDiniza(g, 0, n - 1);
  std::cout << flow << '\n';
  std::vector<Ll> edge_flow(m, 0);
  for (int i = 0; i < n; ++i) {
    for (const Edge& edge : g[i]) {
      if (edge.capacity != 0) {
        edge_flow[edge.id] = edge.flow;
      }
    }
  }
  for (Ll tmp : edge_flow) {
    std::cout << tmp << '\n';
  }
  return 0;
}