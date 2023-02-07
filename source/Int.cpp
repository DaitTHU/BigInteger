#include "bigNumber.h"
#include "bigMath.h"
using namespace std;

Int& Int::operator+=(const Int& A)
{
    if (p_ == A.p_)
        uInt::operator+=(A);
    else {
        uInt::operator-=(A);
        if (uInt::operator<(A)) // abs: *this < A
            p_ = !p_;
        else if (uInt::operator==(0)) // *this = A
            p_ = true;
    }
    return *this;
}

Int& Int::operator-=(const Int& A)
{
    if (p_ == A.p_) {
        uInt::operator-=(A);
        if (uInt::operator<(A))
            p_ = !p_;
        else if (uInt::operator==(0))
            p_ = true;
    } else
        uInt::operator+=(A);
    return *this;
}

Int& Int::operator^=(const Int& A)
{
    if (!A.p_) // A < 0
        return *this = static_cast<Int>(0);
    p_ |= (A[0] % 2 == 0);
    uInt::operator^=(A);
    return *this;
}

Int& Int::operator>>=(const ssize_t& n)
{
    n < 0 ? uInt::operator<<=(-n) : uInt::operator>>=(n);
    return *this;
}

Int& Int::operator<<=(const ssize_t& n)
{
    n < 0 ? uInt::operator>>=(-n) : uInt::operator<<=(n);
    return *this;
}

ostream& operator<<(ostream& os, const Int& A)
{
    if (!A.p_)
        os << '-';
    os << uInt(A);
    return os;
}

istream& operator>>(istream& is, Int& A)
{
    string str;
    is >> str;
    A = Int(str);
    return is;
}

bool Int::between(const Int& A, const Int& B, const bool& includeA, const bool& includeB) const
{
    switch ((includeA << 1) | includeB) {
    case 0b10:
        return (A <= *this) && (*this < B);
    case 0b11:
        return (A <= *this) && (*this <= B);
    case 0b00:
        return (A < *this) && (*this < B);
    default:
        return (A < *this) && (*this <= B);
    }
}

string Int::toString(const unsigned& _base, const bool& _suffix) const
{
    return p_ ? uInt::toString(_base, _suffix) : '-' + uInt::toString(_base, _suffix);
}

string Int::sciNote(const size_t& _deciLength) const
{
    return p_ ? uInt::sciNote(_deciLength) : '-' + uInt::sciNote(_deciLength);
}

Int Int::subInt(const size_t& _beginDigit, const size_t& _endDigit) const
{
    return Int(uInt::subInt(_beginDigit, _endDigit), p_);
}

