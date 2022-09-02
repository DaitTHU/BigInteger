#include "bigNumber.h"
#include "bigMath.h"
#include <assert.h>  // assert
#include <algorithm> // reverse
#include <array>     // array
#include <iomanip>   // setw, setfill
using namespace std;

#define LOG2_10 1079882313 / 325076968 // DO NOT TOUCH

char uInt::delimiter = ',';
unsigned uInt::interval = uInt::LEN;

uInt::uInt(const string &_num)
{
    if (_num[0] == '+' || _num[0] == '-')
    {
        *this = uInt(_num.substr(1)); // omit sign
        return;
    }
    for (auto &digit : _num)
        if (digit < '0' || digit > '9')
        {
            cout << "WARNING: N/A string in uInt constructor, set as 0." << endl;
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
    if (size() != A.size())
        return size() < A.size();
    for (int i = size() - 1; i >= 0; --i)
        if (num[i] != A[i])
            return num[i] < A[i];
    return false;
}

uInt uInt::operator-(const uInt &A) const
{
    if (*this <= A)
        return uInt(0);
    vector<unit> diff(size()); // difference
    bool carry = false;
    for (unsigned i = 0; i < A.size(); ++i)
        diff[i] = suber(num[i], A[i], carry);
    for (unsigned i = A.size(); i < size(); ++i)
        diff[i] = suber(num[i], 0, carry);
    if (carry)
        --diff.back();
    while (diff.back() == 0)
        diff.pop_back();
    return uInt(diff);
}

uInt uInt::operator*(const uInt &A) const
{
    vector<unit> prod(size() + A.size(), 0); // production
    unit carry;
    for (unsigned i = 0; i < A.size(); ++i)
    {
        carry = 0;
        for (unsigned j = 0; j < size(); ++j)
            muler(A[i], num[j], prod[i + j], carry);
        if (carry > 0)
            prod[i + size()] += carry;
    }
    if (prod.back() == 0)
        prod.pop_back(); // at most 1.
    return uInt(prod);
}

uInt uInt::operator^(const uInt &A) const
{
    uInt expo = *this; // exponent
    uInt power = 1;
    // array<uInt, 20> exp_ = {1};
    while (true)
    {
        uInt power2 = power * 2;
        if (power2 > A)
            break;
        expo *= expo; // quick
        power = move(power2);
    }
    for (; power < A; ++power)
        expo *= *this;
    return expo;
}

ostream &operator<<(ostream &os, const uInt &A)
{
    if (uInt::interval == 3) // this is really a piece of sheet!
    {
        string subNum = to_string(*A.num.rbegin());
        unsigned firstLen = subNum.length();
        if (firstLen < 4)
            os << subNum;
        else if (firstLen < 7)
            os << subNum.substr(0, firstLen - 4) << uInt::delimiter
               << subNum.substr(firstLen - 4, 3);
        else
            os << subNum.substr(0, firstLen - 7) << uInt::delimiter
               << subNum.substr(firstLen - 7, 3) << uInt::delimiter
               << subNum.substr(firstLen - 4, 3);
        for (auto part = A.num.rbegin() + 1; part != A.num.rend(); ++part)
        {
            subNum = to_string(*part);
            subNum = string(uInt::LEN - subNum.length(), '0') + subNum;
            os << uInt::delimiter << subNum.substr(0, 3)
               << uInt::delimiter << subNum.substr(3, 3)
               << uInt::delimiter << subNum.substr(6, 3);
        }
        return os;
    }
    os << *A.num.rbegin();
    if (uInt::interval == 9)
        for (auto part = A.num.rbegin() + 1; part != A.num.rend(); ++part)
            os << uInt::delimiter << setfill('0') << setw(uInt::LEN) << *part;
    else // uInt::interval == 0
        for (auto part = A.num.rbegin() + 1; part != A.num.rend(); ++part)
            os << setfill('0') << setw(uInt::LEN) << *part;
    return os;
}

istream &operator>>(istream &is, uInt &A)
{
    string str;
    is >> str;
    A = uInt(str);
    return is;
}

void uInt::setDelimiter(const char &_c, const unsigned &_interval)
{
    if (_c == ' ' || _c == ',' || _c == ';' || _c == '\'')
        delimiter = _c;
    else
        cout << "ERROR: N/A delimiter, only \' \', "
             << "\',\', \'\'\' and \';\' allowed." << endl;
    if (_interval == 0 || _interval == 3 || _interval == 9) // 4 or 10? maybe.
        interval = _interval;
    else
        cout << "ERROR: N/A interval, only 0, 3 and 9 allowed." << endl;
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

pair<uInt, uInt> uInt::divmod(const unit &divident) const
{
    assert(divident != 0);
    if (*this < divident)
        return pair<uInt, uInt>(0, *this);
    vector<unit> quot(size(), 0); // quotient
    unit remainder = 0;
    for (int i = quot.size() - 1; i >= 0; --i)
        quot[i] = diver(num[i], divident, remainder);
    if (quot.back() == 0)
        quot.pop_back();
    return pair<uInt, uInt>(quot, remainder);
}

pair<uInt, uInt> uInt::divmod(const uInt &A) const
{
    if (A.size() < 2) // if A.empty will throw errro
        return divmod(A[0]);
    if (*this < A)
        return pair<uInt, uInt>(0, *this);
    return pair<uInt, uInt>(0, *this); // to be cancelled.
}

/** @brief approx. to power of 2
 *  @return largest (2^n, n) that 2^n <= *this */
pair<uInt, uInt> uInt::approxPo2() const
{
    if (*this == 0)
        return pair<uInt, uInt>(0, 0);
    uInt len = LEN * (size() - 1);
    unit exp10;
    for (exp10 = 1; exp10 <= num.back(); exp10 *= 10)
        ++len;
    unit firstNum = num.back() / (exp10 / 10);
    static const unsigned short log2_[] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3};
    uInt power = log2_[firstNum] + (len - 1) * LOG2_10; // error: -1 ~ 0
    uInt expo = exp2(power), expo2 = expo * 2;
    if (*this < expo2)
        return pair<uInt, uInt>(expo, power);
    else
        return pair<uInt, uInt>(expo2, power + 1);
}

string uInt::toString(const unsigned &base, const bool &suffix) const
{
    if (base == 10)
    {
        string str = to_string(num.back()), subNum;
        for (auto part = num.rbegin() + 1; part != num.rend(); ++part)
        {
            subNum = to_string(*part);
            str += string(LEN - subNum.length(), '0') + subNum;
        }
        if (suffix)
            str += "(10)";
        return str;
    }
    else if (base == 2)
    {
        string str = "";
    }
    assert(base <= 37);
    static const char alphaBet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    pair<uInt, uInt> qr = divmod(base);
    string str(1, alphaBet[unit(qr.second)]);
    while (qr.first != 0)
    {
        qr = qr.first.divmod(base);
        str += alphaBet[unit(qr.second)];
    }
    reverse(str.begin(), str.end());
    if (suffix)
        str += '(' + to_string(base) + ')';
    return str;
}

string uInt::sciNote(unit deciLength) const
{
    string str = to_string(num.back()), subDeci;
    unit power = unit(length()) - 1;
    deciLength = min(deciLength, power);
    if (deciLength == 0)
        return str.substr(0, 1) + " x 10^" + to_string(power);
    str.insert(1, 1, '.');
    for (auto part = num.rbegin() + 1; str.length() < 2 + deciLength; ++part)
    {
        subDeci = to_string(*part);
        str += string(LEN - subDeci.length(), '0') + subDeci;
    }
    return str.substr(0, 2 + deciLength) + " x 10^" + to_string(power);
}

uInt uInt::sub(const unsigned &begin, const unsigned &end) const
{
    auto back = (end <= size() ? num.begin() + end : num.end());
    return uInt(vector<unit>(num.begin() + begin, back));
}

uInt uInt::length(const unsigned &base) const
{
    if (base == 10)
    {
        uInt len = LEN * (size() - 1);
        for (uInt expo = 1; expo <= num.back(); expo *= 10)
            ++len;
        return len;
    }
    else if (base == 2)
        return approxPo2().second;
    else if (base == 4)
        return approxPo2().second / 2;
    else if (base == 8)
        return approxPo2().second / 3;
    else if (base == 16)
        return approxPo2().second / 4;
    return toString(base).length(); // shit
}

// private function

void uInt::normalize()
{
    unit carry = 0;
    for (unsigned i = 0; i < num.size(); ++i)
        num[i] = adder(num[i], 0, carry);
    if (carry > 0)
        num.push_back(carry);
    else
    {
        while (num.back() == 0)
            num.pop_back();
        if (num.empty())
            num.push_back(0);
    }
}

/** @brief a + b + inCarry = c & outCarry. */
inline unit uInt::adder(const unit &a, const unit &b, unit &carry) const
{
    unit c = a + b + carry; // max(c) < 3 * MAX < 0xFFFFFFFF, uint32_t is OK.
    carry = c / MAX;
    return c % MAX;
}

/** @brief MAX + a - b - prevBorrow = c & nextBorrow. */
inline unit uInt::suber(const unit &a, const unit &b, bool &borrow) const
{
    unit c = MAX + a - b - borrow;
    borrow = (c < MAX); // carry is either 0 or 1.
    return c % MAX;
}

/** @brief a * b + p + inCarry = c & outCarry. */
inline void uInt::muler(const unit &a, const unit &b, unit &p, unit &carry) const
{
    twin c = static_cast<twin>(a) * static_cast<twin>(b) + static_cast<twin>(p) + static_cast<twin>(carry);
    carry = static_cast<unit>(c / MAX);
    p = static_cast<unit>(c % MAX);
}

/** @brief a / b + prevRemainder = c & nextRemainder. */
inline unit uInt::diver(const unit &a, const unit &b, unit &remainder) const
{
    twin c = static_cast<twin>(MAX) * static_cast<twin>(remainder) + static_cast<twin>(a);
    remainder = static_cast<unit>(c % b);
    return static_cast<unit>(c / b);
}

/** @brief left add, num.size() >= A.num.size() */
uInt uInt::lAdd(const uInt &A) const
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

/** @brief cut a sub-number, no check. */
vector<unit> uInt::cut(unsigned begin, unsigned length) const
{
    return vector<unit>(num.begin() + begin, num.begin() + begin + length);
}
