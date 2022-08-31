#include "myNumber.h"
using namespace std;

uInt::uInt(const string &_num)
{
    for (auto digit : _num)
        if (digit < '0' || digit > '9')
        {
            cout << "ERROR: Not a uInteger, set as 0." << endl;
            num.push_back(0);
            return;
        }
    int pos;
    for (pos = _num.length() - LEN; pos > 0; pos -= LEN)
        num.push_back(stoi(_num.substr(pos, LEN)));
    num.push_back(stoi(_num.substr(0, pos + LEN)));
}

bool uInt::operator<(const uInt &A) const
{
    if (num.size() != A.num.size())
        return num.size() < A.num.size();
    for (int i = num.size() - 1; i >= 0; --i)
        if (num[i] != A.num[i])
            return num[i] < A.num[i];
    return false;
}

bool uInt::between(const uInt &A, const uInt &B, bool includeA, bool includeB) const
{
    if (includeA && !includeB)
        return operator>=(A) && operator<(B); // A <= x < B
    else if (includeA && includeB)
        return operator>=(A) && operator<=(B); // A <= x <= B
    else if (!includeA && !includeB)
        return operator>(A) && operator<(B); // A < x < B
    return operator>(A) && operator<=(B);    // A < x <= B
}

uInt uInt::add(const uInt &A) const
{
    vector<unit> sum(num.size());
    unit carry = 0;
    for (unsigned i = 0; i < A.num.size(); ++i)
        sum[i] = adder(num[i], A.num[i], carry);
    for (unsigned i = A.num.size(); i < num.size(); ++i)
        sum[i] = adder(num[i], 0, carry);
    if (carry > 0)
        sum.push_back(carry);
    return uInt(sum);
}

uInt uInt::operator-(const uInt &A) const
{
    vector<unit> diff(num.size()); // difference
    bool carry = false;
    for (unsigned i = 0; i < A.num.size(); ++i)
        diff[i] = suber(num[i], A.num[i], carry);
    for (unsigned i = A.num.size(); i < num.size(); ++i)
        diff[i] = suber(num[i], 0, carry);
    if (carry)
        --diff.back();
    while (diff.back() == 0)
        diff.pop_back();
    return uInt(diff);
}

uInt uInt::operator*(const uInt &A) const
{
    vector<unit> prod(num.size() + A.num.size(), 0); // production
    unit carry;
    for (unsigned i = 0; i < A.num.size(); ++i)
    {
        carry = 0;
        for (unsigned j = 0; j < num.size(); ++j)
            prod[i + j] = muler(A.num[i], num[j], prod[i + j], carry);
        if (carry > 0)
            prod[i + num.size()] += carry;
    }
    if (prod.back() == 0)
        prod.pop_back(); // at most 1.
    return uInt(prod);
}

uInt uInt::operator/(const uInt &A) const
{
    vector<unit> quot(num.size() - A.num.size() + 1, 0); // quotient
    unit carry;
    for (int i = quot.size() - 1; i >= 0; --i)
    {
    }
    return uInt(quot);
}

uInt uInt::operator^(const uInt &A) const
{
    uInt i, expo = *this; // exponent
    for (i = 1; i < exp2fit(A); i *= 2)
        expo *= expo; // quick
    for (; i < A; ++i)
        expo *= *this;
    return expo;
}

ostream &operator<<(ostream &os, const uInt &A)
{
    os << *A.num.rbegin();
    for (auto part = A.num.rbegin() + 1; part != A.num.rend(); ++part)
        os << ',' << setfill('0') << setw(uInt::LEN) << *part;
    return os;
}

istream &operator>>(istream &is, uInt &A)
{
    string str;
    is >> str;
    A = uInt(str);
    return is;
}

string uInt::toString(unsigned base) const
{
    // if (base == 10)
    {
        string str = to_string(num.back()), subNum;
        for (auto part = num.rbegin() + 1; part != num.rend(); ++part)
        {
            subNum = to_string(*part);
            str += string(LEN - subNum.length(), '0') + subNum;
        }
        return str;
    }
    static const char alphaBeta[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
}

uInt exp2fit(const uInt &A)
{
    if (A == 0)
        return uInt(0);
    uInt expo = 1;
    while (true)
    {
        uInt doubleExpo = 2 * expo;
        if (doubleExpo > A)
            return expo;
        expo = move(doubleExpo);
    }
}

uInt log2(const uInt &A)
{
    if (A == 0)
        return uInt(0);
    uInt power = 0;
    for (uInt expo = 1; expo <= A; expo *= 2)
        ++power;
    return power - 1;
}

uInt digit(const uInt &A)
{
    uInt digit = uInt::LEN * (A.num.size() - 1);
    for (uInt expo = 1; expo <= A.num.back(); expo *= 10)
        ++digit;
    return digit;
}
