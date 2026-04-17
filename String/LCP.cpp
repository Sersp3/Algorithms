#include <iostream>
#include <vector>
#include <algorithm>

// LCP Calculates maximum prefix size of two suffixes

void StableBucketSort(std::vector<int>& to_sort, const std::vector<int>& values, int n) {
  std::vector<int> bucket(n, 0);
  for (int i = 0; i < values.size(); ++i) {
    ++bucket[values[i]];
  }
  for (int i = 1; i < n; ++i) {
    bucket[i] = bucket[i - 1] + bucket[i];
  }
  std::vector<int> result(n, 0);
  for (int i = n - 1; i >= 0; --i) {
    result[--bucket[values[to_sort[i]]]] = to_sort[i];
  }
  to_sort = result;
}

//Calculates the order of suffixes in lexical order
// Needs to be added a smallest symbol at the end
// O(n log n)
std::vector<int> SuffixArray(std::string s) {
  int n = s.size();
  int log_n = 0;
  while (n > (1 << log_n)) ++log_n;

  std::vector<int> classes(n, 0);
  std::vector<int> order(n, 0);
  for (int i = 0; i < n; ++i) {
    order[i] = i;
  }

  auto Equal = [&] (int fs, int sc, int size) -> bool {
    if (size == 1) return s[fs] == s[sc];
    std::pair<int, int> fs1(classes[fs], classes[(fs + size / 2) % n]);
    std::pair<int, int> sc1(classes[sc], classes[(sc + size / 2) % n]);
    return fs1 == sc1;
  };

  for (int size = 1; size <= (1 << log_n); size <<= 1) {
    for (int i = 0; i < n; ++i) {
      order[i] = (order[i] + n - size / 2) % n;
    }

    if (size == 1) {
      std::sort(order.begin(), order.end(), [&] (int fs, int sc) { return s[fs] < s[sc];});
    } else {
      StableBucketSort(order, classes, n);
    }

    std::vector<int> nw_classes(n, 0);
    for (int i = 1; i < n; ++i) {
      if (!Equal(order[i], order[i - 1], size)) {
        nw_classes[order[i]] = nw_classes[order[i - 1]] + 1;
      } else {
        nw_classes[order[i]] = nw_classes[order[i - 1]];
      }
    }
    classes = nw_classes;
  }
  return order;
}

std::vector<int> LCP(std::string s) {
  s.push_back('$');
  int n = s.size();
  std::vector<int> suff_arr = SuffixArray(s);

  std::vector<int> rev_suff_arr(n, 0);
  for (int i = 0; i < n; ++i) {
    rev_suff_arr[suff_arr[i]] = i;
  }

  std::vector<int> neighbour_lcp(n - 1, 0);
  int curr = 0;
  for (int index = 0; index < n; ++index) {
    int arr_pos = rev_suff_arr[index];
    if (arr_pos == 0) {
      continue;
    }
    int prev_index = suff_arr[arr_pos - 1];

    while (s[index + curr] == s[prev_index + curr]) ++curr;

    neighbour_lcp[arr_pos - 1] = curr;
    curr = std::max(curr - 1, 0);
  }
  return neighbour_lcp;
}

int main() {
  std::string s;
  std::cin >> s;
  return 0;
}