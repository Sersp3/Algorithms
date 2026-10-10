#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
#define int long long

struct Complex {
    double re;
    double im;

    Complex(double re, double im) : re(re), im(im) {}

    Complex(double re) : Complex(re, 0) {}

    Complex() : Complex(0, 0) {}

    Complex& operator+=(const Complex& other) {
        re += other.re;
        im += other.im;
        return *this;
    }
    
    Complex operator+(const Complex& other) const {
        Complex tmp = *this;
        return tmp += other;
    }

    Complex& operator-=(const Complex& other) {
        re -= other.re;
        im -= other.im;
        return *this;
    }

    Complex operator-(const Complex& other) const {
        Complex tmp = *this;
        return tmp -= other;
    }

    Complex& operator*=(const Complex& other) {
        // (a + bi) * (c + di) == ac - bd + (ad + bc)i
        double fs = re * other.re - im * other.im;
        double sc = re * other.im + im * other.re;
        re = fs;
        im = sc;
        return *this;
    }

    Complex operator*(const Complex& other) const {
        Complex tmp = *this;
        return tmp *= other;
    }

    Complex operator-() const {
        return *this * Complex(-1, 0);
    }

    Complex& operator/=(const Complex& other) {
        *this *= other.Conjugate();
        re /= other.Length();
        im /= other.Length();
        return *this;
    }

    Complex operator/(const Complex& other) const {
        Complex tmp = *this;
        return tmp /= other;
    }

    Complex Conjugate() const {
        return Complex(re, -im);
    }

    double Length() const {
        return im * im + re * re;
    }

    static Complex SqrtN(int i, int n) {
        double angle = 2 * M_PI * double(i) / double(n);
        return Complex(cos(angle), sin(angle));
    }
};

template <typename Data>
void InverseOrder(vector<Data>& arr) {
    int n = arr.size();
    int log = 1;
    while ((1 << log) != n) ++log;
    int rev = 0;

    for (int i = 1; i < n; ++i) {
        int bit = n >> 1;

        while (rev & bit) {
            rev ^= bit;
            bit >>= 1;
        }

        rev ^= bit;

        if (i < rev) swap(arr[i], arr[rev]);
    }
}

// size must be power of two
// inverse=false : coefficients -> values
// inverse=true : values -> coefficients
template <typename Data>
vector<Data>& InplaceFFT(vector<Data>& arr, bool inverse = false) {
    int n = arr.size();
    int log = 1;
    while ((1 << log) != n) ++log;

    if (inverse) {
        for (int i = log; i >= 1; --i) {
            int sz = (1 << i);
            Data step = Data::SqrtN(sz - 1, sz);
            for (int s = 0; s < n; s += sz) {
                Data w = Data(1);
                for (int j = 0; j < sz / 2; ++j) {

                    Data x = arr[s + j];
                    Data y = arr[s + j + sz / 2];

                    arr[s + j] = x + y;
                    arr[s + j + sz / 2] = (x - y) * w;
                    w *= step;
                }
            }
        }

        for (int i = 0; i < n; ++i) arr[i] /= n;
        InverseOrder(arr);
    } else {
        InverseOrder(arr);
        for (int i = 1; i <= log; ++i) {
            int sz = (1 << i);
            Data step = Data::SqrtN(1, sz);
            for (int s = 0; s < n; s += sz) {
                Data w = Data(1);
                for (int j = 0; j < sz / 2; ++j) {
                    
                    Data x = arr[s + j];
                    Data tmp = arr[s + j + sz / 2] * w;

                    arr[s + j] = x + tmp;
                    arr[s + j + sz / 2] = x - tmp;
                    w *= step;
                }
            }
        }
    }
    return arr;
}

// Magic idea FFT(A + iB) = FFT(A) + iFFT(B)
template <typename Data>
vector<Data> Multiply(const vector<Data>& fs, const vector<Data>& sc) {
    int n = fs.size();
    int m = sc.size();

    int size = 1;
    while (size < n + m - 1) size <<= 1;
    vector<Data> a(size);

    for (int i = 0; i < n; ++i)
        a[i].re = fs[i].re;

    for (int i = 0; i < m; ++i)
        a[i].im = sc[i].re;

    InplaceFFT(a, false);

    vector<Data> res(size);

    for (int k = 0; k < size; ++k) {
        int other = (size - k) & (size - 1);

        Data x = a[k];
        Data y = a[other].Conjugate();

        Data d = (x + y) * (x - y);

        res[k] = Data(d.im / 4.0, -d.re / 4.0);
    }

    // Один обратный FFT
    InplaceFFT(res, true);

    return res;
}

signed main() {
    int n;
    cin >> n;
    vector<Complex> fs(n);
    for (auto& x : fs) {
        int y;
        cin >> y;
        x = Complex(y);
    }

    int m;
    cin >> m;
    vector<Complex> sc(m);
    for (auto& x : sc) {
        int y;
        cin >> y;
        x = Complex(y);
    }

    auto res = Multiply(fs, sc);
    for (int i = 0; i < n + m - 1; ++i) {
        int tmp = round(res[i].re);
        cout << tmp << ' ';
    }
    return 0;
}