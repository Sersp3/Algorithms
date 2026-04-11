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

