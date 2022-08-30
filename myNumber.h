#ifndef MYNUMBER_H_
#define MYNUMBER_H_
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
typedef uint32_t unit;
typedef uint64_t doub;
typedef int32_t nit;

class uInt
{
protected:
	std::vector<unit> num{};
	static const unit LEN = 9;
	static const unit MAX = 1000000000; // 0xFFFFFFFF = 4,294,967,295

public:
	uInt(){};
	uInt(unit _num) { num.push_back(_num); }
	uInt(std::vector<unit> &_num) : num(_num) {}
	uInt(const std::string &_num);
	uInt(const uInt &A) = default;
	uInt(uInt &&A) = default;
	~uInt() = default;
	// compare-operator overloading
	bool operator<(const uInt &A) const;
	bool operator>(const uInt &A) const { return A < *this; }
	bool operator==(const uInt &A) const { return num == A.num; }
	bool operator<=(const uInt &A) const { return !operator>(A); }
	bool operator>=(const uInt &A) const { return !operator<(A); }
	bool operator!=(const uInt &A) const { return !operator==(A); }
	void operator=(const uInt &A) { num = A.num; }
	void operator=(uInt &&A) { num = move(A.num); }
	// calculate-operator overloading
	uInt operator+(const uInt &A) const { return num.size() > A.num.size() ? add(A) : A.add(*this); }
	friend uInt operator+(const unit _num, const uInt &A) { return A + _num; }
	uInt &operator+=(const uInt &A)
	{
		*this = *this + A;
		return *this;
	}
	uInt operator-(const uInt &A) const;
	friend uInt operator-(const unit _num, const uInt &A) { return uInt(_num) - A; }
	uInt &operator-=(const uInt &A)
	{
		*this = *this - A;
		return *this;
	}
	uInt operator*(const uInt &A) const;
	friend uInt operator*(const unit _num, const uInt &A) { return A * _num; }
	uInt &operator*=(const uInt &A)
	{
		*this = *this * A;
		return *this;
	}
	// TODO: realize operator / and %
	uInt operator/(const uInt &A) const;
	friend uInt operator/(const unit _num, const uInt &A) { return uInt(_num) / A; }
	uInt &operator/=(const uInt &A)
	{
		*this = *this / A;
		return *this;
	}
	uInt operator%(const uInt &A) const;
	friend uInt operator%(const unit _num, const uInt &A) { return uInt(_num) % A; }
	uInt &operator%=(const uInt &A)
	{
		*this = *this % A;
		return *this;
	}
	// above is unfinished
	uInt operator^(const uInt &A) const;
	friend uInt operator^(const unit _num, const uInt &A) { return uInt(_num) ^ A; }
	uInt operator^=(const uInt &A)
	{
		*this = *this ^ A;
		return *this;
	}
	uInt operator++()
	{
		*this += 1;
		return *this;
	}
	uInt operator--()
	{
		*this -= 1;
		return *this;
	}
	// cin & cout overloading
	friend std::ostream &operator<<(std::ostream &out, const uInt &A);
	std::string toString() const;

	// math
	/** @return largest 2^i that <= *this */
	friend uInt exp2fit(const uInt &A);
	/** @return floor(log2(num)) */
	friend uInt log2(const uInt &A);
	/** @return number of digits */
	friend uInt digit(const uInt &A);

private:
	/** @brief a + b + inCarry = c & outCarry. */
	static unit adder(unit a, unit b, unit &carry)
	{
		unit c = a + b + carry; // max(c) < 3 * MAX < 0xFFFFFFFF, uint32_t is OK.
		carry = c / MAX;
		return c % MAX;
	}
	/** @brief MAX + a - b - inCarry = c & outCarry. */
	static unit suber(unit a, unit b, bool &carry)
	{
		unit c = MAX + a - b - carry;
		carry = (c < MAX); // carry is either 0 or 1.
		return c % MAX;
	}
	/** @brief a * b + p + inCarry = c & outCarry. */
	static unit muler(unit a, unit b, unit p, unit &carry)
	{
		doub c = static_cast<doub>(a) * static_cast<doub>(b) + static_cast<doub>(p) + static_cast<doub>(carry);
		carry = static_cast<unit>(c / MAX);
		return static_cast<unit>(c % MAX);
	}
	/** @brief num.size() >= A.num.size() */
	uInt add(const uInt &A) const;
};

class Int : public uInt
{
protected:
	bool p = true;

public:
	Int(){};
	Int(nit _num) : uInt(abs(_num)), p(_num >= 0) {}
	Int(std::vector<unit> &_num, bool _p = true) : uInt(_num), p(_p) {}
	Int(const uInt &A, bool _p = true) : uInt(A), p(_p) {}
	Int(uInt &&A, bool _p = true) : uInt(A), p(_p) {}
	Int(const std::string &_num);
	Int(const Int &A) = default;
	Int(Int &&A) = default;
	~Int() = default;
	// compare-operator overloading
	bool operator<(const Int &A) const;
	bool operator==(const Int &A) const { return (p == A.p) && (num == A.num); }
	void operator=(const Int &A) { num = A.num, p = A.p; }
	void operator=(Int &&A) { num = move(A.num), p = A.p; }
};

class Fraction
{
protected:
	Int nume = 0;
	uInt deno = 1;
};

class Real : public Int
{
protected:
	std::vector<unit> dec{};

public:
	Real() {}
	Real(double _r) {}
};

class Complex
{
protected:
	Real a = 0, b = 0;
};

#endif