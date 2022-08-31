#include "bigMath.h"
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

uInt &uInt::operator=(const uInt &A)
{
    num = A.num;
    return *this;
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

pair<uInt, uInt> uInt::divmod(const uInt &A) const
{
    assert(A > 0);
    vector<unit> quot(num.size() - A.num.size() + 1, 0); // quotient
    unit remainder;
    for (int i = quot.size() - 1; i >= 0; --i)
    {
        while (A * quot < 0)
            ++quot[i];
    }
    return pair<uInt, uInt>(quot, remainder);
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

uInt uInt::sub(unsigned begin, unsigned end) const
{
    auto back = (end <= num.size() ? num.begin() + end : num.end());
    return uInt(vector<unit>(num.begin() + begin, back));
}

uInt uInt::length(unsigned base) const
{
    uInt len = LEN * (num.size() - 1);
    for (uInt expo = 1; expo <= num.back(); expo *= 10)
        ++len;
    return len;
}
