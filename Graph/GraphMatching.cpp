#include <algorithm>
#include <iostream>
#include <vector>

// Maximal matching in graph

std::vector<int> Blossom(std::vector<std::vector<int>>& graph) {
  int n = graph.size();
  int timer = -1;
  std::vector<int> match(n, -1);
  std::vector<int> colour(n);
  std::vector<int> parent(n);
  std::vector<int> base(n);
  std::vector<int> aux(n, -1);
  std::vector<int> q;

  auto lca = [&](int x, int y) -> int {
    for (timer++;; std::swap(x, y)) {
      if (x == -1) {
        continue;
      }
      if (aux[x] == timer) {
        return x;
      }
      aux[x] = timer;
      x = (match[x] == -1 ? -1 : base[parent[match[x]]]);
    }
  };

  auto blossom = [&](int v, int w, int a) {
    while (base[v] != a) {
      parent[v] = w;
      w = match[v];
      if (colour[w] == 1) {
        colour[w] = 0;
        q.push_back(w);
      }
      base[v] = base[w] = a;
      v = parent[w];
    }
  };

  auto augment = [&](int v) {
    while (v != -1) {
      int pv = parent[v];
      int ppv = match[pv];
      match[v] = pv;
      match[pv] = v;
      v = ppv;
    }
  };

  auto bfs = [&](int root) {
    colour.assign(colour.size(), -1);
    for (size_t i = 0; i < base.size(); ++i) {
      base[i] = i;
    }
    q.clear();
    colour[root] = 0;
    q.push_back(root);
    for (size_t i = 0; i < q.size(); ++i) {
      int v = q[i];
      for (auto x : graph[v]) {
        if (colour[x] == -1) {
          colour[x] = 1;
          parent[x] = v;
          if (match[x] == -1) {
            augment(x);
            return;
          }
          colour[match[x]] = 0;
          q.push_back(match[x]);
        } else if (colour[x] == 0 && base[v] != base[x]) {
          int a = lca(base[v], base[x]);
          blossom(x, v, a);
          blossom(v, x, a);
        }
      }
    }
  };

  for (int i = 0; i < n; ++i) {
    if (match[i] == -1) {
      bfs(i);
    }
  }
  return match;
}

int main() {
  int n;
  int k;
  std::cin >> n >> k;
  std::vector<std::vector<int>> graph(n + k);
  for (int i = 0; i < n; ++i) {
    int x;
    while (std::cin >> x, x != 0) {
      graph[i].push_back(n + x - 1);
    }
  }
  auto match = Blossom(graph);
  std::vector<std::pair<int, int>> total;
  for (int i = 0; i < n; ++i) {
    if (match[i] != -1 && match[i] > i) {
      total.push_back({i + 1, match[i] - n + 1});
    }
  }
  std::cout << total.size() << '\n';
  for (const auto& pair : total) {
    std::cout << pair.first << ' ' << pair.second << '\n';
  }
  return 0;
}