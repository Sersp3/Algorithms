#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> ZFunction(const std::string& str) {
  int n = str.size();
  std::vector<int> z(n, 0);
  int l = 0, r = 0;
  for (int i = 1; i < n; ++i) {
    if (r >= i) z[i] = std::min(r - i + 1, z[i - l]);
    while (z[i] + i < n && str[z[i] + i] == str[z[i]]) ++z[i];
    if (z[i] + i - 1 > r) l = i, r = z[i] + i - 1;
  }
  return z;
}