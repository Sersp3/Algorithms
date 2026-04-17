#include <algorithm>
#include <iostream>
#include <vector>

using Ll = long long;
const Ll cInf = 1e18;

struct Line {
  Ll k;
  Ll b;
  
  Line(Ll k, Ll b) : k(k), b(b) {}

  Line() : k(0), b(0) {}

  bool IsZero() const {
    return k == 0 && b == 0;
  }

  Ll operator()(Ll x) const {
    return x * k + b;
  }
};

// Vector for li chao queries
// Must be sorted before first usage of class
std::vector<Ll> li_chao_query;

// on min
class LiChao {
 public:
  LiChao(int l, int r) : l(l), r(r) {
    if (l != r) {
      ls = new LiChao(l, (l + r) / 2);
      rs = new LiChao((l + r) / 2 + 1, r);
    }
  }

  ~LiChao() {
    delete ls;
    delete rs;
  }

  void AddLine(const Line& new_line) {
    if (zero_line) {
      zero_line = false;
      line = new_line;
      return;
    }

    if (l == r) {
      if (new_line(li_chao_query[l]) > line(li_chao_query[l])) {
        line = new_line;
      }
      return;
    }

    std::pair<Ll, Line> x1{line(li_chao_query[ls->r]), line};
    std::pair<Ll, Line> x2{new_line(li_chao_query[ls->r]), new_line};
    if (x1.first > x2.first) {
      std::swap(x1, x2);
    }
    line = x1.second;
    if (x1.second.k >= x2.second.k) {
      rs->AddLine(x2.second);
    } else {
      ls->AddLine(x2.second);
    }
  }

  Ll GetMin(int pos) const {
    Ll value = zero_line ? cInf : line(li_chao_query[pos]);
    if (l == r) {
      return value;
    }
    if (pos <= (l + r) / 2) {
      return std::min(value, ls->GetMin(pos));
    }
    return std::min(value, rs->GetMin(pos));
  }

 private:
  int l;
  int r;
  LiChao* ls = nullptr;
  LiChao* rs = nullptr;
  Line line;
  bool zero_line = true;
};

void PrepareLiChao(const std::vector<Ll>& query) {
  int n = query.size();
  li_chao_query.clear();
  li_chao_query.resize(n);
  for (int i = 0; i < n; ++i) {
    li_chao_query[i] = query[i];
  }
  std::sort(li_chao_query.begin(), li_chao_query.end());
}

int FindPos(Ll value, const std::vector<Ll>& arr) {
  int l = -1;
  int r = arr.size();
  while (l < r - 1) {
    int mid = (l + r) / 2;
    if (arr[mid] <= value) {
      l = mid;
    } else {
      r = mid;
    }
  }
  return l;
}

int main() {
  int n;
  std::cin >> n;
  std::vector<Ll> a(n);
  std::vector<Ll> pref(n + 1, 0);
  std::vector<Ll> b(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
    pref[i + 1] = pref[i] + a[i];
  }
  for (int i = 0; i < n; ++i) {
    std::cin >> b[i];
  }

  PrepareLiChao(b);
  LiChao tree(0, n - 1);

  std::vector<Ll> dp(n, 0);
  tree.AddLine(Line(0, 0));
  
  for (int i = 1; i < n; ++i) {
    dp[i] = tree.GetMin(FindPos(b[i], li_chao_query)) + pref[i + 1] * b[i];
    tree.AddLine(Line(-pref[i], dp[i]));
  }

  for (int i = 0; i < n; ++i) {
    std::cout << dp[i] << ' ';
  }
  return 0;
}