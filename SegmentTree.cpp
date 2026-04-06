#include <algorithm>
#include <iostream>
#include <vector>

using Ll = long long;

template <typename Data>
class ST {
 public:
  ST(int l, int r) : l(l), r(r) {}

  ~ST() {
    delete ls;
    delete rs;
  }

  void SetPos(Data value, int pos) {
    if (l == r) {
      if (pos == l) {
        data = value;
      }
      return;
    }
    if (pos <= (l + r) / 2) {
      if (ls == nullptr) {
        ls = new ST(l, (l + r) / 2);
      }
      ls->SetPos(value, pos);
    } else {
      if (rs == nullptr) {
        rs = new ST((l + r) / 2 + 1, r);
      }
      rs->SetPos(value, pos);
    }
    UpdateParams();
  }

  Data GetSegData(int ql, int qr) {
    if (r < ql || l > qr) {
      return Data();
    }
    if (ql <= l && r <= qr) {
      return data;
    }
    return (ls == nullptr ? Data() : ls->GetSegData(ql, qr)) +
           (rs == nullptr ? Data() : rs->GetSegData(ql, qr));
  }

  void UpdateParams() {
    if (l != r) {
      data = (ls == nullptr ? Data() : ls->data(ql, qr)) +
             (rs == nullptr ? Data() : rs->data(ql, qr));
    }
  }

 private:
  ST* ls = nullptr;
  ST* rs = nullptr;
  Data data;
  int l;
  int r;
};

// Example of data class
struct Data {
  Ll value;

  Data() : value(1e18) {}

  Data(Ll v) : value(v) {}

  Data operator+(const Data& other) const {
    return Data(std::min(value, other.value));
  }
};