#include <algorithm>
#include <iostream>
#include <vector>

using Ll = long long;
const Ll cInf = 1e18;

// Minimal cost PairMatching
// O(n ^ 3)

std::vector<int> Hungarian(const std::vector<std::vector<Ll>>& input_table) {
  int n = static_cast<int>(input_table.size());
  std::vector<std::vector<Ll>> table(n + 1, std::vector<Ll>(n + 1, 0));
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      table[i][j] = input_table[i - 1][j - 1];
    }
  }

  std::vector<Ll> row(n + 1, 0);
  std::vector<Ll> col(n + 1, 0);
  std::vector<int> match(n + 1, 0);
  std::vector<int> way(n + 1, 0);
  for (int i = 1; i <= n; ++i) {
    match[0] = i;
    int j0 = 0;
    std::vector<Ll> minv(n + 1, cInf);
    std::vector<bool> used(n + 1, false);
    do {
      used[j0] = true;
      int i0 = match[j0];
      Ll delta = cInf;
      int j1;
      for (int j = 1; j <= n; ++j) {
        if (!used[j]) {
          Ll cur = table[i0][j] - row[i0] - col[j];
          if (cur < minv[j]) {
            minv[j] = cur, way[j] = j0;
          }
          if (minv[j] < delta) {
            delta = minv[j], j1 = j;
          }
        }
      }
      for (int j = 0; j <= n; ++j) {
        if (used[j]) {
          row[match[j]] += delta, col[j] -= delta;
        } else {
          minv[j] -= delta;
        }
      }
      j0 = j1;
    } while (match[j0] != 0);
    do {
      int j1 = way[j0];
      match[j0] = match[j1];
      j0 = j1;
    } while (j0 != 0);
  }
  std::vector<int> ans(n);
  for (int j = 1; j <= n; ++j) {
    ans[match[j] - 1] = j - 1;
  }
  return ans;
}

int main() {
  int n;
  std::cin >> n;

  std::vector<std::vector<Ll>> table(n, std::vector<Ll>(n, 0));
  for (auto& x : table) {
    for (auto& y : x) {
      std::cin >> y;
    }
  }

  auto res = Hungarian(table);
  Ll w = 0;
  for (int i = 0; i < n; ++i) {
    w += table[i][res[i]];
  }
  std::cout << w << '\n';
  for (int i = 0; i < n; ++i) {
    std::cout << i + 1 << ' ' << res[i] + 1 << '\n';
  }
  return 0;
}