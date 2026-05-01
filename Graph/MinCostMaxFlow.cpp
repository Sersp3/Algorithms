#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using Ll = long long;
const Ll cInf = 1e18;

struct Edge {
  int to;
  int symm_ind;
  Ll capacity;
  Ll flow;
  Ll cost;

  Edge(int t, int s, Ll c, Ll f, Ll co)
      : to(t), symm_ind(s), capacity(c), flow(f), cost(co) {}
};

struct CostFlow {
  Ll cost;
  Ll flow;
};

void AddEdge(std::vector<std::vector<Edge>>& g, int u, int v, Ll cap, Ll cost) {
  Edge e1(v, g[v].size(), cap, 0, cost);
  Edge e2(u, g[u].size(), 0, 0, -cost);
  g[u].push_back(e1);
  g[v].push_back(e2);
}

struct Parent {
  int prev;
  int edge_ind;
};

struct QueueData {
  int vertex;
  Ll dist;

  QueueData(int v, Ll d) : vertex(v), dist(d) {}

  bool operator<(const QueueData& other) const { return dist > other.dist; }
};

CostFlow MinCostMaxFlow(std::vector<std::vector<Edge>>& g, int s, int t) {
  int n = static_cast<int>(g.size());
  std::vector<Ll> pot(n, cInf);
  std::vector<Parent> p(n, {-1, -1});
  pot[s] = 0;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      for (size_t k = 0; k < g[j].size(); ++k) {
        Edge& edge = g[j][k];
        if (edge.capacity == edge.flow) {
          continue;
        }
        int to = edge.to;
        if (pot[to] > pot[j] + edge.cost) {
          pot[to] = pot[j] + edge.cost;
          p[to] = {j, static_cast<int>(k)};
        }
      }
    }
  }

  while (pot[t] != cInf) {
    std::vector<int> path;
    int curr = t;
    while (curr != -1) {
      path.push_back(curr);
      curr = p[curr].prev;
    }
    std::reverse(path.begin(), path.end());
    Ll flow = cInf;
    int size = static_cast<int>(path.size());
    for (int i = 1; i < size; ++i) {
      int prev = p[path[i]].prev;
      int index = p[path[i]].edge_ind;
      Edge& edge = g[prev][index];
      flow = std::min(flow, edge.capacity - edge.flow);
    }
    for (int i = 1; i < size; ++i) {
      int prev = p[path[i]].prev;
      int index = p[path[i]].edge_ind;
      Edge& edge = g[prev][index];
      edge.flow += flow;
      g[path[i]][edge.symm_ind].flow -= flow;
    }
    std::vector<Ll> dist(n, cInf);
    dist[s] = 0;
    p.assign(n, {-1, -1});
    std::priority_queue<QueueData> queue;
    std::vector<bool> used(n, false);
    queue.push(QueueData(s, 0));

    auto expand = [&](QueueData data) -> void {
      int sz = static_cast<int>(g[data.vertex].size());
      for (int i = 0; i < sz; ++i) {
        Edge& edge = g[data.vertex][i];
        if (edge.capacity == edge.flow) {
          continue;
        }
        Ll value = edge.cost - pot[edge.to] + pot[data.vertex];
        if (dist[edge.to] > dist[data.vertex] + value) {
          dist[edge.to] = dist[data.vertex] + value;
          p[edge.to] = {data.vertex, i};
          queue.push(QueueData(edge.to, dist[edge.to]));
        }
      }
    };
    while (!queue.empty()) {
      auto top = queue.top();
      queue.pop();
      if (used[top.vertex]) {
        continue;
      }
      used[top.vertex] = true;
      expand(top);
    }

    for (int i = 0; i < n; ++i) {
      if (dist[i] != cInf) {
        pot[i] += dist[i];
      } else {
        pot[i] = cInf;
      }
    }
  }
  Ll flow = 0;
  Ll cost = 0;
  for (int i = 0; i < n; ++i) {
    for (const auto& edge : g[i]) {
      cost += edge.cost * edge.flow;
    }
  }
  cost /= 2;

  for (const auto& edge : g[s]) {
    flow += edge.flow;
  }
  return CostFlow{cost, flow};
}

int main() {
  int n;
  int m;
  std::cin >> n >> m;

  std::vector<std::vector<Edge>> g(n);

  for (int i = 0; i < m; ++i) {
    int u;
    int v;
    Ll c;
    Ll w;
    std::cin >> u >> v >> c >> w;
    --u, --v;
    AddEdge(g, u, v, c, w);
  }

  auto tmp = MinCostMaxFlow(g, 0, n - 1);
  std::cout << tmp.cost << '\n';
  return 0;
}
