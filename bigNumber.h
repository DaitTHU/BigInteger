#pragma once
#include <iostream> // std::ostream...
// #include <fstream>
#include <vector>  // std::vector
#include <string>  // std::string
#include <utility> // std::pair

typedef uint32_t unit; // 4 bytes
typedef uint64_t twin;
typedef int32_t snit; // unit: unsigned; snit: signed - make sense!
typedef int64_t swin;

class uInt
{
protected:
	std::vector<unit> num{};
	static const unit LEN = 9;             // utilization: 9 / (32lg2) = 93.4%
	static const unit MAX = 1'000'000'000; // 0x100000000 = 4'294'967'296
	static char delimiter;
	static unsigned interval;

public:
	uInt(){};
	uInt(const twin &_num);
	uInt(const swin &_num, const bool &sign) : uInt(sign ? _num : -_num) {}
	uInt(const std::vector<unit> &_num) : num(_num) {}
	uInt(std::vector<unit> &&_num) : num(std::move(_num)) {}
	uInt(const std::string &_num);
	uInt(const uInt &A) : num(A.num) {}
	uInt(uInt &&A) = default;
	virtual ~uInt() = default;
	// basic: =, <, ==, +=, -=,...
	uInt &operator=(const uInt &A) { num = A.num; return *this; }
	uInt &operator=(uInt &&A) { num = std::move(A.num); return *this; }
	bool operator<(const uInt &A) const;
	bool operator==(const uInt &A) const { return num == A.num; }
	uInt &operator+=(const uInt &A);
	uInt &operator-=(const uInt &A);
	uInt operator*(const uInt &A) const;
	uInt operator/(const uInt &A) const { return divmod(A).first; }
	uInt operator%(const uInt &A) const { return divmod(A).second; }
	uInt operator^(const uInt &A) const;
	uInt &operator>>=(const uInt &A);
	uInt &operator<<=(const uInt &A);
	uInt &operator&=(const uInt &A) = delete;
	uInt &operator|=(const uInt &A) = delete;
	// unary arithmetic
	uInt &operator+() { return *this; };
	uInt operator-() = delete; // sorry, can't return Int.
	uInt operator~() = delete;
	template <typename T>
	explicit operator T() const { return static_cast<T>(num[0]); }
	explicit operator twin() const;
	// derivative
	bool operator>(const uInt &A) const { return A < *this; }
	bool operator>=(const uInt &A) const { return !(*this < A); }
	bool operator<=(const uInt &A) const { return !(A < *this); }
	bool operator!=(const uInt &A) const { return !(*this == A); }
	uInt operator+(const uInt &A) const { return uInt(*this) += A; }
	uInt operator-(const uInt &A) const { return uInt(*this) -= A; }
	uInt &operator*=(const uInt &A) { return *this = *this * A; }
	uInt &operator/=(const uInt &A) { return *this = *this / A; }
	uInt &operator%=(const uInt &A) { return *this = *this % A; }
	uInt &operator^=(const uInt &A) { return *this = *this ^ A; }
	uInt operator>>(const uInt &A) const { return uInt(*this) >>= A; }
	uInt operator<<(const uInt &A) const { return uInt(*this) <<= A; }
	uInt operator&(const uInt &A) const = delete;
	uInt operator|(const uInt &A) const = delete;
	// right relational
	friend bool operator<(const unit _num, const uInt &A) { return A > _num; }
	friend bool operator>(const unit _num, const uInt &A) { return A < _num; }
	friend bool operator==(const unit _num, const uInt &A) { return A == _num; }
	friend bool operator<=(const unit _num, const uInt &A) { return A >= _num; }
	friend bool operator>=(const unit _num, const uInt &A) { return A <= _num; }
	friend bool operator!=(const unit _num, const uInt &A) { return A != _num; }
	// right binary arithmetic
	friend uInt operator+(const unit _num, const uInt &A) { return A + _num; }
	friend uInt operator-(const unit _num, const uInt &A) { return uInt(_num) - A; }
	friend uInt operator*(const unit _num, const uInt &A) { return A * _num; }
	friend uInt operator/(const unit _num, const uInt &A) { return uInt(_num) / A; }
	friend uInt operator%(const unit _num, const uInt &A) { return uInt(_num) % A; }
	friend uInt operator^(const unit _num, const uInt &A) { return uInt(_num) ^ A; }
	friend uInt operator>>(const unit _num, const uInt &A) { return uInt(_num) >> A; }
	friend uInt operator<<(const unit _num, const uInt &A) { return uInt(_num) << A; }
	// ++/--
	uInt operator++() { return *this += 1; }
	uInt operator++(int) { return *this += 1; } // may change, i don't konw.
	uInt operator--() { return *this -= 1; }
	uInt operator--(int) { return *this -= 1; }
	// I/O stream
	friend std::ostream &operator<<(std::ostream &os, const uInt &A);
	friend std::istream &operator>>(std::istream &is, uInt &A);
	static void setDelimiter(const char &_c = ',', const unsigned &_interval = LEN);
	// others
	bool between(const uInt &A, const uInt &B, bool includeA = true, bool includeB = false) const;
	std::pair<uInt, uInt> divmod(const unit &_num) const;
	std::pair<uInt, uInt> divmod(const uInt &A) const;
	std::pair<uInt, uInt> approxExp2() const;
	uInt sqrt() const;
	std::string toString(const unsigned &base = 10, const bool &suffix = false) const;
	std::string sciNote(unit deciLength = LEN) const; // whether should for ostream, not string?
	uInt sub(const unsigned &begin = 0, const unsigned &end = MAX) const;
	uInt length(const unsigned &base = 10) const;

private:
	unit operator[](const unit &i) const { return num[i]; }
	twin size() const { return num.size(); }
	void normalize();
	unit adder(const unit &a, const unit &b, unit &carry) const;
	unit suber(const unit &a, const unit &b, bool &borrow) const;
	void muler(const unit &a, const unit &b, unit &p, unit &carry) const;
	unit diver(const unit &a, const unit &b, unit &remainder) const;
};

class Int : public uInt
{
protected:
	bool p = true; // positive

public:
	Int(){};
	Int(const int64_t &_num) : uInt(abs(_num)), p(_num >= 0) {}
	Int(const std::vector<unit> &_num, bool _p = true) : uInt(_num), p(_p) {}
	Int(const uInt &A, bool _p = true) : uInt(A), p(_p) {}
	Int(uInt &&A, bool _p = true) : uInt(A), p(_p) {}
	Int(const std::string &_num) : uInt(_num), p(_num[0] != '-') {}
	Int(const Int &A) = default;
	Int(Int &&A) = default;
	virtual ~Int() = default;
	// assignment
	Int &operator=(const Int &A) { num = A.num, p = A.p; return *this; }
	Int &operator=(Int &&A)
	{
		num = std::move(A.num), p = A.p;
		return *this;
	}
	// relational
	bool operator>(const Int &A) const { return p == A.p ? p ? uInt::operator>(A.num) : uInt::operator<(A.num) : p; }
	bool operator<(const Int &A) const { return A > *this; }
	bool operator==(const Int &A) const { return num == A.num && p == A.p; }
	bool operator<=(const Int &A) const { return !operator>(A); }
	bool operator>=(const Int &A) const { return !operator<(A); }
	bool operator!=(const Int &A) const { return !operator==(A); }
	// unary arithmetic
	Int operator+() const { return Int(num); } // abs
	Int operator-() const { return Int(num, !p); }
	Int operator~() = delete;
	//  binary arithmetic
	Int operator+(const Int &A) const; // { return p == A.p ? Int(uInt::operator+(A.num), p) : operator-(-A); }
	Int operator-(const Int &A) const; // { return p == A.p ? uInt::operator>(A.num) ? Int(uInt::operator-(A.num), p) : Int(uInt(A.num) - uInt(*this), !p) : operator+(-A); }
	Int operator*(const Int &A) const; // { return p == A.p ? Int(uInt::operator*(A.num)) : Int(uInt::operator*(A.num), false); }
	Int operator/(const Int &A) const; // { return p == A.p ? Int(uInt::operator/(A.num)) : Int(uInt::operator/(A.num), false); }
	Int operator%(const Int &A) const; // { return p == A.p ? Int(uInt::operator%(A.num)) : Int(uInt::operator%(A.num), false); }
	Int operator^(const Int &A) const;
	Int operator&(const Int &A) const = delete;
	Int operator|(const Int &A) const = delete;
	// right relational
	friend bool operator<(const snit _num, const Int &A) { return A > _num; }
	friend bool operator>(const snit _num, const Int &A) { return A < _num; }
	friend bool operator==(const snit _num, const Int &A) { return A == _num; }
	friend bool operator<=(const snit _num, const Int &A) { return A >= _num; }
	friend bool operator>=(const snit _num, const Int &A) { return A <= _num; }
	friend bool operator!=(const snit _num, const Int &A) { return A != _num; }
	// right binary arithmetic
	friend Int operator+(const snit _num, const Int &A) { return A + _num; }
	friend Int operator-(const snit _num, const Int &A) { return Int(_num) - A; }
	friend Int operator*(const snit _num, const Int &A) { return A * _num; }
	friend Int operator/(const snit _num, const Int &A) { return Int(_num) / A; }
	friend Int operator%(const snit _num, const Int &A) { return Int(_num) % A; }
	friend Int operator^(const snit _num, const Int &A) { return Int(_num) ^ A; }
	// arithmetic-assignment
	Int &operator+=(const Int &A) { return *this = *this + A; }
	Int &operator-=(const Int &A) { return *this = *this - A; }
	Int &operator*=(const Int &A) { return *this = *this * A; }
	Int &operator/=(const Int &A) { return *this = *this / A; }
	Int &operator%=(const Int &A) { return *this = *this % A; }
	Int &operator^=(const Int &A) { return *this = *this ^ A; }
	// ++/--
	Int operator++() { return *this += 1; }
	Int operator++(int) { return *this += 1; } // may change, i don't konw.
	Int operator--() { return *this -= 1; }
	Int operator--(int) { return *this -= 1; }
	// I/O stream
	friend std::ostream &operator<<(std::ostream &os, const Int &A);
	friend std::istream &operator>>(std::istream &is, Int &A);
};

class Frac // Fraction
{
protected:
	Int nume = 0;  // numerator
	uInt deno = 1; // denominator

public:
	Frac() {}
	Frac(const snit &_num) : nume(_num), deno(1) {}
	Frac(const Int &_nume, const uInt &_deno = 1) : nume(_nume), deno(_deno) {}
	// Frac(Real _num);
	Frac(const std::string _f);
	Frac(const Frac &A) = default;
	Frac(Frac &&A) = default;
	~Frac() = default;
	// assignment
	Frac &operator=(const Frac &A)
	{
		nume = A.nume, deno = A.deno;
		return *this;
	}
	Frac &operator=(Frac &&A)
	{
		nume = std::move(A.nume), deno = std::move(A.deno);
		return *this;
	}
	// relational
	bool operator<(const Frac &A) const { return nume * A.deno < A.nume * deno; };
	bool operator>(const Frac &A) const { return A < *this; }
	bool operator==(const Frac &A) const { return nume == A.nume && deno == A.deno; }
	bool operator<=(const Frac &A) const { return !operator>(A); }
	bool operator>=(const Frac &A) const { return !operator<(A); }
	bool operator!=(const Frac &A) const { return !operator==(A); }
	// unary arithmetic
	Frac operator+() const { return Frac(+nume, deno); } // abs
	Frac operator-() const { return Frac(-nume, deno); }
	Frac operator~() = delete;
	// binary arithmetic
	Frac operator+(const Frac &A) const;
	Frac operator-(const Frac &A) const;
	Frac operator*(const Frac &A) const;
	Frac operator/(const Frac &A) const;
	Frac operator%(const Frac &A) const;
	Frac operator^(const Frac &A) const;
	Frac operator&(const Frac &A) const = delete;
	Frac operator|(const Frac &A) const = delete;
	// arithmetic-assignment
	Frac &operator+=(const Frac &A) { return *this = *this + A; }
	Frac &operator-=(const Frac &A) { return *this = *this - A; }
	Frac &operator*=(const Frac &A) { return *this = *this * A; }
	Frac &operator/=(const Frac &A) { return *this = *this / A; }
	Frac &operator%=(const Frac &A) { return *this = *this % A; }
	Frac &operator^=(const Frac &A) { return *this = *this ^ A; }
	// ++/--
	Frac operator++() { return *this += 1; }
	Frac operator++(int) { return *this += 1; } // may change, i don't konw.
	Frac operator--() { return *this -= 1; }
	Frac operator--(int) { return *this -= 1; }
	// I/O stream
	friend std::ostream &operator<<(std::ostream &os, const Frac &A);
	friend std::istream &operator>>(std::istream &is, Frac &A);
};

class Real : public Int
{
protected:
	std::vector<unit> dec{}; // decimal

public:
	Real() {}
	Real(const snit &_r) : Int(_r) {}
	Real(const double &_r) {}
	Real(const std::string &_r);
	Real(const Int &A, std::vector<unit> _d = {}) : Int(A), dec(_d) {}
	Real(const Real &A) = default;
	Real(Real &&A) = default;
	~Real() = default;
	// assignment
	Real &operator=(const Real &A)
	{
		num = A.num, p = A.p, dec = A.dec;
		return *this;
	}
	Real &operator=(Real &&A)
	{
		num = move(A.num), p = A.p, dec = move(A.dec);
		return *this;
	}
	// relational
	bool operator<(const Real &A) const;
	bool operator>(const Real &A) const { return A < *this; }
	bool operator==(const Real &A) const { return num == A.num; }
	bool operator<=(const Real &A) const { return !operator>(A); }
	bool operator>=(const Real &A) const { return !operator<(A); }
	bool operator!=(const Real &A) const { return !operator==(A); }
	// unary arithmetic
	// Real operator+() const { return operator<(0) ? 0 - *this : *this; } // abs
	// Real operator-() const {}
	Real operator~() = delete;
	// binary arithmetic
	Real operator+(const Real &A) const;
	Real operator-(const Real &A) const;
	Real operator*(const Real &A) const;
	Real operator/(const Real &A) const;
	Real operator%(const Real &A) const;
	Real operator^(const Real &A) const;
	Real operator&(const Real &A) const = delete;
	Real operator|(const Real &A) const = delete;
	// arithmetic-assignment
	Real &operator+=(const Real &A) { return *this = *this + A; }
	Real &operator-=(const Real &A) { return *this = *this - A; }
	Real &operator*=(const Real &A) { return *this = *this * A; }
	Real &operator/=(const Real &A) { return *this = *this / A; }
	Real &operator%=(const Real &A) { return *this = *this % A; }
	Real &operator^=(const Real &A) { return *this = *this ^ A; }
	// ++/--
	Real operator++() { return *this += 1; }
	Real operator++(int) { return *this += 1; } // may change, i don't konw.
	Real operator--() { return *this -= 1; }
	Real operator--(int) { return *this -= 1; }
	// I/O stream
	friend std::ostream &operator<<(std::ostream &os, const Real &A);
	friend std::istream &operator>>(std::istream &is, Real &A);
};

class Complex
{
protected:
	Real a = 0, b = 0; // a + ib

public:
	Complex() {}
	Complex(double _a, double _b = 0) : a(_a), b(_b) {}
	Complex(Real _a, Real _b) : a(_a), b(_b) {}
	Complex(const std::string &_c);
	Complex(const Complex &C) = default;
	Complex(Complex &&C) = default;
	~Complex() = default;
	// assignment
	Complex &operator=(const Complex &C);
	Complex &operator=(Complex &&C);
	// relational
	bool operator<(const Complex &C) = delete;
	bool operator>(const Complex &C) = delete;
	bool operator==(const Complex &C) const { return a == C.a && b == C.b; }
	bool operator<=(const Complex &C) = delete;
	bool operator>=(const Complex &C) = delete;
	bool operator!=(const Complex &C) const { return !operator==(C); }
	// unary arithmetic
	Complex operator+();
	Complex operator-();
	Complex operator~() = delete;
	// binary arithmetic
	Complex operator+(const Complex &C) const { return Complex(a + C.a, b + C.b); }
	Complex operator-(const Complex &C) const { return Complex(a - C.a, b - C.b); }
	Complex operator*(const Complex &C) const { return Complex(a * C.a - b * C.b, a * C.b + b * C.a); }
	Complex operator/(const Real &c) const { return Complex(a / c, b / c); }
	Complex operator/(const Complex &C) const; // { return Complex(a * C.a + b * C.b, b * C.a - a * C.b) / C.norm2(); }
	Complex operator%(const Complex &C) = delete;
	Complex operator^(const Complex &C) const;
	Complex operator&(const Complex &C) const = delete;
	Complex operator|(const Complex &C) const = delete;
	// other
	// Real norm2() const { return a ^ 2 + b ^ 2; }
};

template <typename F> // Real, Complex
class Poly			  // Polynomial
{
protected:
	std::vector<F> coef; // coefficient
public:
	Poly(){};
	Poly(const F &_c0) { coef.push_back(_c0); }
	Poly(const std::vector<F> &_c) : coef(_c) {}
	Poly(const Poly &A) = default;
	Poly(Poly &&A) = default;
	~Poly() = default;
	F operator()(const F &x, const unsigned &dOrder = 0) const; // f(x)
	F &operator[](const unsigned &i) { return coef[i]; }
};
