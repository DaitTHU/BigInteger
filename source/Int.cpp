#include "bigNumber.h"
#include "bigMath.h"
using namespace std;

Int& Int::operator+=(const Int& A)
{
    if (p == A.p)
        uInt::operator+=(A);
    else {
        uInt::operator-=(A);
        if (uInt::operator<(A)) // abs: *this < A
            p = !p;
        else if (uInt::operator==(0)) // *this = A
            p = true;
    }
    return *this;
}

Int& Int::operator-=(const Int& A)
{
    if (p == A.p) {
        uInt::operator-=(A);
        if (uInt::operator<(A))
            p = !p;
        else if (uInt::operator==(0))
            p = true;
    } else
        uInt::operator+=(A);
    return *this;
}

Int& Int::operator^=(const Int& A)
{
    if (!A.p) // A < 0
        return *this = static_cast<Int>(0);
    p |= (A[0] % 2 == 0);
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
    if (!A.p)
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