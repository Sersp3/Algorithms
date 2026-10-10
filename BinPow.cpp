template <typename T>
T BinPow(T x, int pow) {
  T ans = 1;
  while (pow != 0) {
    if (pow % 2 == 1) ans *= x;
    pow /= 2; x *= x;
  }
  return ans;
}