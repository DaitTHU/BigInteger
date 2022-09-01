#ifndef BIG_NUMBER_H_
#define BIG_NUMBER_H_
#include <iostream> // std::ostream...
// #include <fstream>
#include <vector>  // std::vector
#include <string>  // std::string
#include <utility> // std::pair

typedef uint32_t unit;
typedef uint64_t twin;
typedef int32_t snit; // unit: unsigned; snit: signed - make sense!

class uInt
{
protected:
	std::vector<unit> num{};
	static const unit LEN = 9;
	static const unit MAX = 1'000'000'000; // 0x100000000 = 4'294'967'296
	static char delimiter;
	static unsigned interval;

public:
	uInt(){};
	uInt(const unit &_num) { num.push_back(_num); }
	uInt(const std::vector<unit> &_num) : num(_num) {}
	uInt(const std::string &_num);
	uInt(const uInt &A) : num(A.num) {}
	uInt(uInt &&A) = default;
	~uInt() = default;
	// assignment
	uInt &operator=(const uInt &A);
	// relational
	bool operator<(const uInt &A) const;
	bool operator>(const uInt &A) const { return A < *this; }
	bool operator==(const uInt &A) const { return num == A.num; }
	bool operator<=(const uInt &A) const { return !operator>(A); }
	bool operator>=(const uInt &A) const { return !operator<(A); }
	bool operator!=(const uInt &A) const { return !operator==(A); }
	// unary arithmetic
	uInt operator+() = delete;
	uInt operator-() = delete;
	uInt operator~() = delete;
	template <typename T>
	explicit operator T() const { return num[0]; }
	// binary arithmetic
	uInt operator+(const uInt &A) const { return size() > A.size() ? lAdd(A) : A.lAdd(*this); }
	uInt operator-(const uInt &A) const;
	uInt operator*(const uInt &A) const; // O(n^2)
	uInt operator/(const uInt &A) const { return divmod(A).first; }
	uInt operator%(const uInt &A) const { return divmod(A).second; }
	uInt operator^(const uInt &A) const;
	uInt operator&(const uInt &A) const = delete;
	uInt operator|(const uInt &A) const = delete;
	// right relational
	template <typename T>
	friend bool operator<(const T _num, const uInt &A) { return A > _num; }
	template <typename T>
	friend bool operator>(const T _num, const uInt &A) { return A < _num; }
	template <typename T>
	friend bool operator==(const T _num, const uInt &A) { return A == _num; }
	template <typename T>
	friend bool operator<=(const T _num, const uInt &A) { return A >= _num; }
	template <typename T>
	friend bool operator>=(const T _num, const uInt &A) { return A <= _num; }
	template <typename T>
	friend bool operator!=(const T _num, const uInt &A) { return A != _num; }
	// right binary arithmetic
	template <typename T>
	friend uInt operator+(const T _num, const uInt &A) { return A + _num; }
	template <typename T>
	friend uInt operator-(const T _num, const uInt &A) { return uInt(_num) - A; }
	template <typename T>
	friend uInt operator*(const T _num, const uInt &A) { return A * _num; }
	template <typename T>
	friend uInt operator/(const T _num, const uInt &A) { return uInt(_num) / A; }
	template <typename T>
	friend uInt operator%(const T _num, const uInt &A) { return uInt(_num) % A; }
	template <typename T>
	friend uInt operator^(const T _num, const uInt &A) { return uInt(_num) ^ A; }
	// arithmetic-assignment
	uInt &operator+=(const uInt &A) { return *this = *this + A; }
	uInt &operator-=(const uInt &A) { return *this = *this - A; }
	uInt &operator*=(const uInt &A) { return *this = *this * A; }
	uInt &operator/=(const uInt &A) { return *this = *this / A; }
	uInt &operator%=(const uInt &A) { return *this = *this % A; }
	uInt &operator^=(const uInt &A) { return *this = *this ^ A; }
	// ++/--
	uInt operator++() { return (*this += 1); }
	uInt operator++(int) { return (*this += 1); } // may change, i don't konw.
	uInt operator--() { return (*this -= 1); }
	uInt operator--(int) { return (*this -= 1); }
	// I/O stream
	friend std::ostream &operator<<(std::ostream &os, const uInt &A);
	friend std::istream &operator>>(std::istream &is, uInt &A);
	static void setDelimiter(const char &_c = ',', const unsigned &_interval = LEN);
	// others
	bool between(const uInt &A, const uInt &B, bool includeA = true, bool includeB = false) const;
	std::pair<uInt, uInt> divmod(const unit &_num) const;
	std::pair<uInt, uInt> divmod(const uInt &A) const;
	std::pair<uInt, uInt> approxPo2() const;
	std::string toString(const unsigned &base = 10, const bool &suffix = false) const;
	std::string sciNote(unit deciLength = LEN) const; // if should for ostream, not string?
	uInt sub(const unsigned &begin = 0, const unsigned &end = MAX) const;
	uInt length(const unsigned &base = 10) const;

private:
	inline unit operator[](const unit &i) const { return num[i]; }
	inline unit size() const { return num.size(); }
	void normalize();
	unit adder(const unit &a, const unit &b, unit &carry) const;
	unit suber(const unit &a, const unit &b, bool &borrow) const;
	unit muler(const unit &a, const unit &b, const unit &p, unit &carry) const;
	unit diver(const unit &a, const unit &b, unit &remainder) const;
	uInt lAdd(const uInt &A) const;
	std::vector<unit> cut(unsigned begin = 0, unsigned length = 1) const;
};

class Int : public uInt
{
protected:
	bool p = true; // positive

public:
	Int(){};
	Int(const snit &_num) : uInt(abs(_num)), p(_num >= 0) {}
	Int(const std::vector<unit> &_num, bool _p = true) : uInt(_num), p(_p) {}
	Int(const uInt &A, bool _p = true) : uInt(A), p(_p) {}
	Int(uInt &&A, bool _p = true) : uInt(A), p(_p) {}
	Int(const std::string &_num);
	Int(const Int &A) = default;
	Int(Int &&A) = default;
	~Int() = default;
	// compare-operator overloading
	bool operator<(const Int &A) const;
	bool operator==(const Int &A) const { return (p == A.p) && (num == A.num); }
	// void operator=(Int &&A) { num = move(A.num), p = A.p; }
};

class Fraction
{
protected:
	Int nume = 0;  // numerator
	uInt deno = 1; // denominator

public:
	Fraction() {}
	Fraction(Int _num) : nume(_num) {}
};

class Real : public Int
{
protected:
	std::vector<unit> dec{}; // decimal

public:
	Real() {}
	Real(const double &_r);
	Real(const std::string &_r);
};

class Complex
{
protected:
	Real a = 0, b = 0; // a + ib

public:
	Complex() {}
	Complex(double _a, double _b = 0) : a(_a), b(_b) {}
};

class Polynomial
{
protected:
	std::vector<Real> coef; // coefficient
};

#endif