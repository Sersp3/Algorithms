#include <iostream>
#include <vector>
#include <algorithm>

#define int long long
static const int cMod = 998244353;
using namespace std;

class ModInt {
 public:
  ModInt(int val) : value(val) { ToMod(); }

  ModInt() : value(0) {}
  
  ModInt& operator+=(const ModInt& other) {
    value += other.value;
    ToMod();
    return *this;
  }

  ModInt operator+(const ModInt& other) const {
    ModInt tmp = *this;
    return tmp += other;
  }

  ModInt operator-() const {
    return ModInt(-value);
  }

  ModInt& operator-=(const ModInt& other) {
    value -= other.value;
    ToMod();
    return *this;
  }

  ModInt operator-(const ModInt& other) const {
    ModInt tmp = *this;
    return tmp -= other;
  }

  ModInt& operator*=(const ModInt& other) {
    value *= other.value;
    ToMod();
    return *this;
  }

  ModInt operator*(const ModInt& other) const {
    ModInt tmp = *this;
    return tmp *= other;
  }

  int GetValue() const {
    return value;
  }

  ModInt& operator/=(const ModInt& other) {
    return this->operator*=(Reverse(other.value));
  }

  ModInt operator/(const ModInt& other) const {
    ModInt tmp = *this;
    return tmp /= other;
  }

  static ModInt Power(ModInt num, int pow) {
    ModInt res = 1;
    while (pow != 0) {
      if (pow % 2 == 1) res *= num;
      num *= num;
      pow /= 2;
    }
    return res;
  }
 private:
  int value;

  static ModInt Reverse(ModInt num) {
    return Power(num, cMod - 2);
  }

  void ToMod() {
    value %= cMod;
    if (value < 0) {
      value += cMod;
    }
  }
};

ostream& operator<<(std::ostream& output, const ModInt& val) {
  output << val.GetValue();
  return output;
};
 
istream& operator>>(std::istream& input, ModInt& val) {
  int temp;
  std::cin >> temp;
  val = temp;
  return input;
}

vector<ModInt> Factorials;
vector<ModInt> ReverseFactorials;

void Prepare(int n) {
  Factorials.assign(n, 1);
  ReverseFactorials.assign(n + 1, 1);
  for (int i = 2; i <= n; ++i) {
    Factorials[i] = Factorials[i - 1] * i;
    ReverseFactorials[i] = ModInt(1) / Factorials[i];
  }
}

ModInt Combinations(int n, int k) {
  return Factorials[n] * ReverseFactorials[k] * ReverseFactorials[n - k];
}

ModInt CombinationsWithRepeat(int n, int k) {
  return Combinations(n + k - 1, k);
}