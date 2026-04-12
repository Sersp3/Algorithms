#include <iostream>
#include <vector>
#include <algorithm>

using Ll = long long;
static Ll cMod = 998244353;

class ModInt {
 public:
  ModInt(Ll val) : value(val) {
    ToMod();
  }

  ModInt() : value(0) {}

  ModInt operator+(const ModInt& other) const {
    return ModInt(value + other.value);
  }

  ModInt operator-() const {
    return ModInt(-value);
  }

  ModInt operator-(const ModInt& other) const {
    return ModInt(value - other.value);
  }

  ModInt operator*(const ModInt& other) const {
    return ModInt(value * other.value);
  }

  Ll GetValue() const {
    return value;
  }

  ModInt operator/(const ModInt& other) const {
    return (*this) * Reverse(other);
  }

  static ModInt GetWn(Ll n) {
    return Power(ModInt(3), (cMod - 1) / n);
  }
 private:
  Ll value;

  static ModInt Reverse(ModInt num) {
    return Power(num, cMod - 2);
  }

  static ModInt Power(ModInt num, Ll pow) {
    if (pow == 0) {
      return ModInt(1);
    }
    if (pow == 1) {
      return ModInt(num);
    }
    ModInt temp = Power(num, pow / 2);
    temp = temp * temp;
    if (pow % 2 == 1) {
      temp = temp * num;
    }
    return temp;
  }

  void ToMod() {
    value %= cMod;
    if (value < 0) {
      value += cMod;
    }
  }
};

Ll ReverseBit(Ll num, Ll log_n) {
  Ll nw = 0;
  for (Ll i = 0; i < log_n; ++i) {
    if ((num & (1LL << i)) != 0) {
      nw |= (1LL << (log_n - i - 1));
    }
  }
  return nw;
}

template <typename F>
void InplaceFFT(std::vector<F>& arr, bool invert) {
  Ll n = arr.size();
  Ll log_n = 0;
  while (n != (1 << log_n)) ++log_n;
  
  for (Ll i = 0; i < n; ++i) {
    Ll temp = ReverseBit(i, log_n);
    if (i > temp) {
      std::swap(arr[i], arr[temp]);
    }
  }

  for (Ll len = 2; len <= n; len *= 2) {
    F wn = F::GetWn(len);
    if (invert) {
      wn = F(1) / wn;
    }
    for (Ll a = 0; a < n; a += len) {
      F curr = F(1);
      for (Ll i = 0; i < len / 2; ++i) {
        F fs = arr[a + i];
        F sc = arr[a + i + len / 2] * curr;
        arr[a + i] = fs + sc;
        arr[a + i + len / 2] = fs - sc;
        curr = curr * wn;
      }
    }
  }

  if (invert) {
    for (Ll i = 0; i < n; ++i) {
      arr[i] = arr[i] / F(n);
    }
  }
}

template<typename F>
void Multiply(const std::vector<F>& a, const std::vector<F>& b, std::vector<F>& res) {
  int target_len = (int)a.size() + (int)b.size() - 1;
  
  Ll n = 1;
  while (n < target_len) n <<= 1;

  if (n > (1 << 23)) {
    throw std::runtime_error("FFT size too large for modulus");
  }

  std::vector<F> fa(n, F(0)), fb(n, F(0));
  for (size_t i = 0; i < a.size(); ++i) fa[i] = a[i];
  for (size_t i = 0; i < b.size(); ++i) fb[i] = b[i];

  InplaceFFT(fa, false);
  InplaceFFT(fb, false);
  for (Ll i = 0; i < n; ++i) {
    fa[i] = fa[i] * fb[i];
  }
  InplaceFFT(fa, true);

  res.assign(fa.begin(), fa.begin() + target_len);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, m;
  std::cin >> n;
  std::vector<ModInt> P(n);
  for (int i = 0; i < n; ++i) {
    Ll x; std::cin >> x;
    P[i] = x;
  }

  std::cin >> m;
  std::vector<ModInt> Q(m);
  for (int i = 0; i < m; ++i) {
    Ll x; std::cin >> x;
    Q[i] = x;
  }

  std::vector<ModInt> res;
  Multiply(P, Q, res);

  for (int i = 0; i < n + m - 1; ++i) {
    std::cout << res[i].GetValue() << " ";
  }
  return 0;
}