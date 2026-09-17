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

  Ll GetValue() const {
    return value;
  }

  ModInt& operator/=(const ModInt& other) {
    return this->operator*=(Reverse(other.value));
  }

  ModInt operator/(const ModInt& other) const {
    ModInt tmp = *this;
    return tmp /= other;
  }

  static ModInt Power(ModInt num, Ll pow) {
    ModInt res = 1;
    while (pow != 0) {
      if (pow % 2 == 1) res *= num;
      num *= num;
      pow /= 2;
    }
    return res;
  }
 private:
  Ll value;

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

