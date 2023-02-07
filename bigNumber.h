#ifndef BIG_NUMBER_H_
#define BIG_NUMBER_H_
#include <iostream> // std::ostream...
// #include <fstream>
#include <vector>  // std::vector
#include <string>  // std::string
#include <utility> // std::pair
#include <set>     // std::set

class uInt
{
protected:
    std::vector<uint32_t> num_{};
    static const uint32_t LEN_ = 9;			    // utilization: 9 / (32lg2) = 93.4%
    static const uint32_t MAX_ = 1'000'000'000; // 0x100000000 = 4'294'967'296
    static char delimiter_;
    static unsigned interval_;

public:
    uInt(const uint64_t& n = 0);
    uInt(const std::string& n);
    uInt(const uInt& A) : num_(A.num_) {}
    uInt(uInt&& A) : num_(std::move(A.num_)) {}
    virtual ~uInt() = default;
    // unary arithmetic
    uInt& operator+() { return *this; };
    uInt operator-() = delete; // sorry, can't return Int.
    uInt operator!() const;
    uInt operator~() const { return operator()(2, 1); }
    template <typename T>
    explicit operator T() const { return static_cast<T>(num_[0]); }
    explicit operator uint32_t() const { return num_[0]; }
    explicit operator uint64_t() const;
    explicit operator std::string() const { return toString(); }
    explicit operator bool() const { return num_.back() > 0; } // != 0
    // basic: =, <, ==, +=, -=,...
    uInt& operator=(const uInt& A) { num_ = A.num_; return *this; }
    uInt& operator=(uInt&& A) { num_ = std::move(A.num_); return *this; }
    bool operator<(const uInt& A) const;
    bool operator==(const uInt& A) const { return num_ == A.num_; }
    uInt& operator+=(const uInt& A);
    uInt& operator-=(const uInt& A);
    uInt& operator*=(const uint32_t& n);
    uInt& operator*=(const uInt& A);
    uInt& operator/=(const uint32_t& n) { this->div_(n); return *this; }
    uInt& operator%=(const uint32_t& n);
    uInt& operator^=(const uInt& A);
    uInt& operator>>=(const std::size_t& n);
    uInt& operator<<=(const std::size_t& n);
    bool operator|(const uInt& A) const;
    uInt& operator&=(const uInt& A) = delete;
    uInt& operator|=(const uInt& A) = delete;
    // similar to the basic
    bool operator>(const uInt& A) const { return A < *this; }
    bool operator>=(const uInt& A) const { return !(*this < A); }
    bool operator<=(const uInt& A) const { return !(*this > A); }
    bool operator!=(const uInt& A) const { return !(*this == A); }
    uInt operator+(const uInt& A) const { return uInt(*this) += A; }
    uInt operator-(const uInt& A) const { return uInt(*this) -= A; }
    uInt operator*(const uint32_t& n) const { return uInt(*this) *= n; }
    uInt operator*(const uInt& A) const { return uInt(*this) *= A; }
    uInt operator/(const uint32_t& n) const { return uInt(*this) /= n; }
    uInt operator%(const uint32_t& n) const { return uInt(*this) %= n; }
    uInt operator/(const uInt& A) const { return divmod(A).first; }
    uInt operator%(const uInt& A) const { return divmod(A).second; }
    uInt& operator/=(const uInt& A) { return *this = *this / A; }
    uInt& operator%=(const uInt& A) { return *this = *this % A; }
    uInt operator^(const uInt& A) const { return uInt(*this) ^= A; }
    uInt& operator>>=(const uInt& A) { return *this >>= static_cast<std::size_t>(A); }
    uInt& operator<<=(const uInt& A) { return *this <<= static_cast<std::size_t>(A); }
    uInt operator>>(const std::size_t& n) { return uInt(*this) >>= n; }
    uInt operator<<(const std::size_t& n) { return uInt(*this) <<= n; }
    uInt operator>>(const uInt& A) const { return uInt(*this) >>= A; }
    uInt operator<<(const uInt& A) const { return uInt(*this) <<= A; }
    uInt operator()(const uInt& N, const bool& NthRoot) const;
    // right
    friend bool operator<(const uint64_t& n, const uInt& A) { return A > n; }
    friend bool operator>(const uint64_t& n, const uInt& A) { return A < n; }
    friend bool operator==(const uint64_t& n, const uInt& A) { return A == n; }
    friend bool operator<=(const uint64_t& n, const uInt& A) { return A >= n; }
    friend bool operator>=(const uint64_t& n, const uInt& A) { return A <= n; }
    friend bool operator!=(const uint64_t& n, const uInt& A) { return A != n; }
    friend uInt operator+(const uint64_t& n, const uInt& A) { return A + n; }
    friend uInt operator-(const uint64_t& n, const uInt& A) { return uInt(n) - A; }
    friend uInt operator*(const uint64_t& n, const uInt& A) { return A * n; }
    friend uInt operator/(const uint64_t& n, const uInt& A) { return uInt(n) / A; }
    friend uInt operator%(const uint64_t& n, const uInt& A) { return uInt(n) % A; }
    friend uInt operator^(const uint64_t& n, const uInt& A) { return uInt(n) ^ A; }
    friend uInt operator>>(const uint64_t& n, const uInt& A) { return uInt(n) >> A; }
    friend uInt operator<<(const uint64_t& n, const uInt& A) { return uInt(n) << A; }
    friend bool operator|(const uint64_t& n, const uInt& A);
    // ++/--
    uInt& operator++() { return *this += 1; }
    uInt& operator++(int) { return *this += 1; } // may change, i don't konw.
    uInt& operator--() { return *this -= 1; }
    uInt& operator--(int) { return *this -= 1; }
    // I/O stream
    friend std::ostream& operator<<(std::ostream& os, const uInt& A);
    friend std::istream& operator>>(std::istream& is, uInt& A);
    static void setDelimiter(const char& _c = ',', const unsigned& _interval = LEN_);
    // others
    bool between(const uInt& A, const uInt& B, const bool& includeA = true, const bool& includeB = false) const;
    std::pair<uInt, uInt> divmod(const uInt& A) const;
    uInt coarseDiv(const uInt& A, const std::size_t& _exactDigit = LEN_) const;
    friend std::pair<uInt, uint64_t> exp2Floor(const uInt& N);
    friend uInt exp2(const uInt& N);
    friend uInt exp10(const uInt& N);
    std::size_t length(const unsigned& _base = 10) const;
    std::string toString(const unsigned& _base = 10, const bool& _suffix = false) const;
    std::string sciNote(const std::size_t& _deciLength = LEN_) const; // whether for ostream, or string?
    uInt subInt(const std::size_t& _beginDigit = 0, const std::size_t& _endDigit = 0xFFFFFFFFFFFFFFFF) const;

protected:
    static constexpr size_t LENL_ = static_cast<std::size_t>(LEN_); // size_t
    static constexpr uint64_t MAXL_ = static_cast<uint64_t>(MAX_);  // uint64_t
    // restricted constructor
    uInt(const std::vector<uint32_t>& n) : num_(n) {}
    uInt(std::vector<uint32_t>&& n) : num_(std::move(n)) {}
    std::pair<uInt, uInt> operator&(const uInt& A) const { return std::make_pair(*this, A); }
    uint32_t operator[](const uint32_t& i) const { return num_[i]; }
    std::size_t size_() const { return num_.size(); }
    void normalize_();

private:
    uint32_t adder_(const uint32_t& a, const uint32_t& b, uint32_t& carry) const;
    uint32_t suber_(const uint32_t& a, const uint32_t& b, bool& borrow) const;
    void muler_(const uint32_t& a, const uint32_t& b, uint32_t& p, uint64_t& carry) const;
    uint32_t diver_(const uint32_t& a, const uint32_t& b, uint32_t& remainder) const;
    uint32_t div_(const uint32_t& _divident) noexcept(false);
};

class Int : public uInt
{
protected:
    bool p_; // positive
    Int(const std::vector<uint32_t>& n, bool p = true) : uInt(n), p_(p) {}
    Int(std::vector<uint32_t>&& n, bool p = true) : uInt(std::move(n)), p_(p) {}

public:
    Int(const int64_t& n = 0) : uInt(abs(n)), p_(n >= 0) {}
    Int(const std::string& n) : uInt(n), p_(n[0] != '-') {}
    Int(const uInt& A, bool p = true) : uInt(A), p_(p) {}
    Int(uInt&& A, bool p = true) : uInt(A), p_(p) {}
    Int(const Int& A) = default;
    Int(Int&& A) = default;
    virtual ~Int() = default;
    // assignment
    Int& operator=(const Int& A) { num_ = A.num_, p_ = A.p_; return *this; }
    Int& operator=(Int&& A) { num_ = std::move(A.num_), p_ = A.p_; return *this; }
    // relational
    bool operator>(const Int& A) const { return p_ == A.p_ ? p_ ? uInt::operator>(A) : uInt::operator<(A) : p_; }
    bool operator<(const Int& A) const { return A > *this; }
    bool operator==(const Int& A) const { return num_ == A.num_ && p_ == A.p_; }
    bool operator<=(const Int& A) const { return !operator>(A); }
    bool operator>=(const Int& A) const { return !operator<(A); }
    bool operator!=(const Int& A) const { return !operator==(A); }
    // unary arithmetic
    Int operator+() const { return Int(num_); } // abs
    Int operator-() const { return Int(num_, !p_); }
    Int operator~() = delete;
    Int operator!() = delete;
    // binary arithmetic
    Int& operator+=(const Int& A);
    Int& operator-=(const Int& A);
    Int& operator*=(const Int& A) { p_ = (p_ == A.p_), uInt::operator*=(A); return *this; }
    Int& operator/=(const Int& A) { p_ = (p_ == A.p_), uInt::operator/=(A); return *this; } // -A / B = -(A / B)
    Int& operator%=(const Int& A) { uInt::operator%=(A); return *this; } // A = A / B * B + A % B
    Int& operator^=(const Int& A);
    Int& operator>>=(const ssize_t& n);
    Int& operator<<=(const ssize_t& n);
    Int& operator&=(const Int& A) = delete;
    Int& operator|=(const Int& A) = delete;
    // right relational
    friend bool operator<(const int64_t n, const Int& A) { return A > n; }
    friend bool operator>(const int64_t n, const Int& A) { return A < n; }
    friend bool operator==(const int64_t n, const Int& A) { return A == n; }
    friend bool operator<=(const int64_t n, const Int& A) { return A >= n; }
    friend bool operator>=(const int64_t n, const Int& A) { return A <= n; }
    friend bool operator!=(const int64_t n, const Int& A) { return A != n; }
    Int operator+(const Int& A) const { return Int(*this) += A; }
    Int operator-(const Int& A) const { return Int(*this) -= A; }
    Int operator*(const Int& A) const { return Int(*this) *= A; }
    Int operator/(const Int& A) const { return Int(*this) /= A; }
    Int operator%(const Int& A) const { return Int(*this) %= A; }
    Int operator^(const Int& A) const { return Int(*this) ^= A; }
    friend Int operator+(const int32_t n, const Int& A) { return A + n; }
    friend Int operator-(const int32_t n, const Int& A) { return Int(n) - A; }
    friend Int operator*(const int32_t n, const Int& A) { return A * n; }
    friend Int operator/(const int32_t n, const Int& A) { return Int(n) / A; }
    friend Int operator%(const int32_t n, const Int& A) { return Int(n) % A; }
    friend Int operator^(const int32_t n, const Int& A) { return Int(n) ^ A; }
    Int& operator++() { return *this += 1; }
    Int& operator++(int) { return *this += 1; }
    Int& operator--() { return *this -= 1; }
    Int& operator--(int) { return *this -= 1; }
    // I/O stream
    friend std::ostream& operator<<(std::ostream& os, const Int& A);
    friend std::istream& operator>>(std::istream& is, Int& A);
    // other
    bool between(const Int& A, const Int& B, const bool& includeA = true, const bool& includeB = false) const;
    std::string toString(const unsigned& _base = 10, const bool& _suffix = false) const;
    std::string sciNote(const std::size_t& _deciLength = LEN_) const;
    Int subInt(const std::size_t& _beginDigit = 0, const std::size_t& _endDigit = 0xFFFFFFFFFFFFFFFF) const;
};

class Frac // Fraction
{
protected:
    Int nume_ = 0;  // numerator
    uInt deno_ = 1; // denominator

public:
    Frac(const Int& nume = 0, const uInt& deno = 1);
    // Frac(Real n);
    Frac(const std::string& f);
    Frac(const Frac& A) = default;
    Frac(Frac&& A) = default;
    ~Frac() = default;
    // assignment
    Frac& operator=(const Frac& A) { nume_ = A.nume_, deno_ = A.deno_; return *this; }
    Frac& operator=(Frac&& A) { nume_ = std::move(A.nume_), deno_ = std::move(A.deno_); return *this; }
    // relational
    bool operator<(const Frac& A) const { return nume_ * A.deno_ < A.nume_* deno_; };
    bool operator>(const Frac& A) const { return A < *this; }
    bool operator==(const Frac& A) const { return nume_ == A.nume_ && deno_ == A.deno_; }
    bool operator<=(const Frac& A) const { return !operator>(A); }
    bool operator>=(const Frac& A) const { return !operator<(A); }
    bool operator!=(const Frac& A) const { return !operator==(A); }
    // unary arithmetic
    Frac operator+() const { return Frac(+nume_, deno_); } // abs
    Frac operator-() const { return Frac(-nume_, deno_); }
    Frac operator~() = delete;
    // binary arithmetic
    Frac operator+(const Frac& A) const;
    Frac operator-(const Frac& A) const;
    Frac operator*(const Frac& A) const;
    Frac operator/(const Frac& A) const;
    Frac operator%(const Frac& A) const;
    Frac operator^(const Frac& A) const;
    Frac operator&(const Frac& A) const = delete;
    Frac operator|(const Frac& A) const = delete;
    // arithmetic-assignment
    Frac& operator+=(const Frac& A) { return *this = *this + A; }
    Frac& operator-=(const Frac& A) { return *this = *this - A; }
    Frac& operator*=(const Frac& A) { return *this = *this * A; }
    Frac& operator/=(const Frac& A) { return *this = *this / A; }
    Frac& operator%=(const Frac& A) { return *this = *this % A; }
    Frac& operator^=(const Frac& A) { return *this = *this ^ A; }
    // ++/--
    // Frac operator++() { return *this += 1; }
    // Frac operator++(int) { return *this += 1; } // may change, i don't konw.
    // Frac operator--() { return *this -= 1; }
    // Frac operator--(int) { return *this -= 1; }
    // I/O stream
    friend std::ostream& operator<<(std::ostream& os, const Frac& A);
    friend std::istream& operator>>(std::istream& is, Frac& A);
};

#ifdef REAL_COMPLETED_
class Real : public Int
{
protected:
    std::size_t dot_ = 0;
    Real(const Int& n, const std::size_t& d) : Int(n), dot_(d) {}
    Real(Int&& n, const std::size_t& d) : Int(std::move(n)), dot_(d) {}

public:
    Real() {}
    // Real(const int32_t& _r) : Int(_r) {}
    Real(const double& r) {}
    Real(const std::string& r);
    Real(const Real& A) = default;
    Real(Real&& A) = default;
    ~Real() = default;
    // assignment
    Real& operator=(const Real& A) { Int::operator=(A), dot_ = A.dot_; return *this; }
    Real& operator=(Real&& A) { Int::operator=(std::move(A)), dot_ = A.dot_; return *this; }
    // relational
    bool operator<(const Real& A) const;
    bool operator>(const Real& A) const { return A < *this; }
    bool operator==(const Real& A) const; // { return num_ == A.num_; }
    bool operator<=(const Real& A) const { return !operator>(A); }
    bool operator>=(const Real& A) const { return !operator<(A); }
    bool operator!=(const Real& A) const { return !operator==(A); }
    // unary arithmetic
    // Real operator+() const { return operator<(0) ? 0 - *this : *this; } // abs
    Real operator-() const { return Real(Int::operator-(), dot_); }
    Real operator~();
    // binary arithmetic
    Real operator+(const Real& A) const;
    Real operator-(const Real& A) const;
    Real operator*(const Real& A) const;
    Real operator/(const Real& A) const;
    Real operator%(const Real& A) const;
    Real operator^(const Real& A) const;
    Real operator&(const Real& A) const = delete;
    Real operator|(const Real& A) const = delete;
    // arithmetic-assignment
    Real& operator+=(const Real& A) { return *this = *this + A; }
    Real& operator-=(const Real& A) { return *this = *this - A; }
    Real& operator*=(const Real& A) { return *this = *this * A; }
    Real& operator/=(const Real& A) { return *this = *this / A; }
    Real& operator%=(const Real& A) { return *this = *this % A; }
    Real& operator^=(const Real& A) { return *this = *this ^ A; }
    // ++/--
    Real operator++() { return *this += 1; }
    Real operator++(int) { return *this += 1; } // may change, i don't konw.
    Real operator--() { return *this -= 1; }
    Real operator--(int) { return *this -= 1; }
    // I/O stream
    friend std::ostream& operator<<(std::ostream& os, const Real& A);
    friend std::istream& operator>>(std::istream& is, Real& A);
};

class Complex
{
public:
    Real a_, b_; // a + ib
    Complex(Real a = 0, Real b = 0) : a_(a), b_(b) {}
    Complex(const std::string& c);
    Complex(const Complex& C) = default;
    Complex(Complex&& C) = default;
    ~Complex() = default;
    // assignment
    Complex& operator=(const Complex& C) { a_ = C.a_, b_ = C.b_; return *this; }
    Complex& operator=(Complex&& C) { a_ = std::move(C.a_), b_ = std::move(C.b_); return *this; }
    // relational
    bool operator<(const Complex& C) = delete;
    bool operator>(const Complex& C) = delete;
    bool operator==(const Complex& C) const { return a_ == C.a_ && b_ == C.b_; }
    bool operator<=(const Complex& C) = delete;
    bool operator>=(const Complex& C) = delete;
    bool operator!=(const Complex& C) const { return !(*this == C); }
    // unary arithmetic
    Complex operator+();
    Complex operator-() const { return Complex(-a_, -b_); }
    Complex operator~() const { return Complex(a_, -b_); }
    Complex& operator+=(const Complex& C) { a_ += C.a_, b_ += C.b_; return *this; }
    Complex& operator-=(const Complex& C) { a_ -= C.a_, b_ -= C.b_; return *this; }
    Complex& operator*=(const Real& R) { a_ *= R, b_ *= R; return *this; }
    Complex& operator*=(const Complex& C);
    Complex& operator/=(const Real& R) { a_ /= R, b_ /= R; return *this; }
    Complex& operator/=(const Complex& C);
    Complex& operator%=(const Complex& C) = delete;
    Complex& operator^=(const Complex& C);
    Complex& operator>>=(const ssize_t& n) = delete;
    Complex& operator<<=(const ssize_t& n) = delete;
    Complex& operator&=(const Complex& C) = delete;
    Complex& operator|=(const Complex& C) = delete;
    // other
    Real norm2() const { return a_ * a_ + b_ * b_; }
    Real norm() const { return ~norm2(); }
    Real arg() const { return b_ / a_; } // arctan
};

class Inf
{
private:
    bool p_;             // postive
    double order = 1.0; // infinity order
    Inf(bool p, double _o) : p_(p), order(_o) {}

public:
    Inf(bool positive = true) : p_(positive) {}
    template <typename T>
    Inf(const T& A) : p_(A > 0), order(0) { if (A == 0) std::cerr << "ERROR: assign 0 to infinity" << std::endl; }
    Inf(const Inf& _inf) = default;
    Inf(Inf&& _inf) = default;
    ~Inf() = default;
    Inf& operator=(const Inf& _inf) { p_ = _inf.p_, order = _inf.order; return *this; }
    Inf operator+() const { return Inf(p_, order); }
    Inf operator-() const { return Inf(-p_, order); }
    Inf operator~() const { return Inf(p_, -order); }
    template <typename T>
    bool operator>(const T& A) { return p_; }
    template <typename T>
    friend bool operator>(const T& A, const Inf& _inf) { return !_inf.p_; }
    friend bool operator>(const Complex& A, const Inf& _inf) = delete;
    bool operator>(const Complex& A) = delete;
    bool operator>(const Inf& _inf) = delete;
    template <typename T>
    bool operator==(const T& A) = delete;
    // below uses above
    template <typename T>
    bool operator<(const T& A) { return A > *this; }
    template <typename T>
    bool operator>=(const T& A) { return !(*this < A); }
    template <typename T>
    bool operator<=(const T& A) { return !(*this > A); }
    template <typename T>
    bool operator!=(const T& A) { return !(*this == A); }
    template <typename T>
    friend bool operator==(const T& A, const Inf& _inf) { return _inf == A; }
    template <typename T>
    friend bool operator<(const T& A, const Inf& _inf) { return _inf > A; }
    template <typename T>
    friend bool operator>=(const T& A, const Inf& _inf) { return _inf <= A; }
    template <typename T>
    friend bool operator<=(const T& A, const Inf& _inf) { return _inf >= A; }
    template <typename T>
    friend bool operator!=(const T& A, const Inf& _inf) { return _inf != A; }
    // operator
    Inf& operator+=(const Inf& _inf);
    Inf& operator-=(const Inf& _inf) { return *this += -_inf; }
    Inf& operator*=(const Inf& _inf);
    Inf& operator/=(const Inf& _inf) { return *this *= ~_inf; }
    template <typename T>
    Inf operator+(const T& A) const { return Inf(*this) += A; }
    template <typename T>
    Inf operator-(const T& A) const { return Inf(*this) -= A; }
    template <typename T>
    Inf operator*(const T& A) const { return Inf(*this) *= A; }
    template <typename T>
    Inf operator/(const T& A) const { return Inf(*this) /= A; }
    template <typename T>
    friend Inf operator+(const T& A, const Inf& _inf) { return _inf + A; }
    template <typename T>
    friend Inf operator-(const T& A, const Inf& _inf) { return -_inf + A; }
    template <typename T>
    friend Inf operator*(const T& A, const Inf& _inf) { return _inf * A; }
    template <typename T>
    friend T operator/(const T& A, const Inf& _inf) { return static_cast<T>(0); };
    Inf& operator++() { return *this; }
    Inf& operator++(int) { return *this; }
    Inf& operator--() { return *this; }
    Inf& operator--(int) { return *this; }
    friend std::ostream& operator<<(std::ostream& os, const Inf& _inf) { os << (_inf.p_ ? "inf" : "-inf"); return os; }
};

template <typename F = Real> // Real, Complex
class Poly					 // Polynomial
{
protected:
    std::vector<F> coef; // coefficient
public:
    Poly() {}
    Poly(const F& _c0) { coef.push_back(_c0); }
    Poly(const std::vector<F>& _c) : coef(_c) {}
    Poly(const Poly& A) = default;
    Poly(Poly&& A) = default;
    ~Poly() = default;
    F operator()(const F& x, const unsigned& dOrder = 0) const; // f(x)
    F& operator[](const unsigned& i) { return coef[i]; }
};
#endif // real completed

// namespace Constant


#endif