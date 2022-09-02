#include "bigNumber.h"
#include "bigMath.h"
using namespace std;

/*
Int Int::operator-(const Int &A) const
{
    if (p == A.p)
        if (uInt::operator>(A.num))
            return Int(uInt::operator-(A.num), p);
        else
            return Int(uInt(A.num) - uInt(*this), !p);
    return operator+(-A);
}
*/

ostream &operator<<(ostream &os, const Int &A)
{
    if (!A.p)
        os << '-';
    os << uInt(A.num);
    return os;
}

istream &operator>>(istream &is, Int &A)
{
    string str;
    is >> str;
    A = Int(str);
    return is;
}