#include <vector>
using namespace std;

// Return vector x, x[i] - min prime in i'th factorization
vector<int> Sieve(int n) {
  vector<int> lp(n + 1, 0);
  vector<int> pr;
  for (int i = 2; i <= n; ++i) {
    if (lp[i] == 0) {
      pr.push_back(i);
      lp[i] = i;
    }
    for (int j = 0; j < (int)pr.size() && pr[j] <= lp[i] && i * pr[j] <= n; ++j) {
      lp[i * pr[j]] = pr[j];
    }
  }
  return lp;
}

// Return vector of primes <= n
vector<int> SievePrimes(int n) {
  vector<int> lp(n + 1, 0);
  vector<int> pr;
  for (int i = 2; i <= n; ++i) {
    if (lp[i] == 0) {
      pr.push_back(i);
      lp[i] = i;
    }
    for (int j = 0; j < (int)pr.size() && pr[j] <= lp[i] && i * pr[j] <= n; ++j) {
      lp[i * pr[j]] = pr[j];
    }
  }
  return pr;
}