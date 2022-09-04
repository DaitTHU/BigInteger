#include "bigNumber.h"
#include "bigMath.h"
#include <assert.h>  // assert
#include <algorithm> // reverse
#include <array>     // array
#include <iomanip>   // setw, setfill
using namespace std;

#define LG2_32_9 241726409 / 225843117 // 32 * lg2 / 9
#define LOG2_10 1079882313 / 325076968 // DO NOT TOUCH

static const unit log2_[] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3};
static const unit exp10_[] = {
    1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000};
static const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char uInt::delimiter = ',';
unsigned uInt::interval = uInt::LEN;

uInt::uInt(const twin &_num)
{
    if (_num < _MAX)
        num.push_back(static_cast<unit>(_num));
    else
        for (twin carry = _num; carry > 0; carry /= _MAX)
            num.push_back(static_cast<unit>(carry % _MAX));
}

uInt::uInt(const string &_num)
{
    if (_num[0] == '+' || _num[0] == '-')
    {
        *this = uInt(_num.substr(1)); // omit sign
        return;
    }
    bool nonzero = false;
    for (auto &digit : _num)
        if (digit < '0' || digit > '9')
        {
            cout << "WARNING: N/A string in constructor, set as 0." << endl;
            num.push_back(0);
            return;
        }
        else if (digit != '0')
            nonzero = true;
    if (!nonzero) // all 0
    {
        num.push_back(0);
        return;
    }
    int pos;
    for (pos = _num.length() - LEN; pos > 0; pos -= LEN)
        num.push_back(stoi(_num.substr(pos, LEN)));
    num.push_back(stoi(_num.substr(0, pos + LEN)));
    while (num.back() == 0)
        num.pop_back();
}

uInt::operator twin() const
{
    switch (size())
    {
    case 1:
        return static_cast<twin>(num[0]);
    case 2:
        return _MAX * static_cast<twin>(num[1]) + static_cast<twin>(num[0]);
    default:
        return _MAX * _MAX * static_cast<twin>(num[2]) +
               _MAX * static_cast<twin>(num[1]) + static_cast<twin>(num[0]);
    }
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

uInt &uInt::operator+=(const uInt &A)
{
    unit carry = 0, i;
    if (size() < A.size())
        num.resize(A.size(), 0);
    for (i = 0; i < A.num.size(); ++i)
        num[i] = adder(num[i], A[i], carry);
    for (; i < num.size(); ++i)
        num[i] = adder(num[i], 0, carry);
    if (carry > 0)
        num.push_back(carry);
    return *this;
}

uInt &uInt::operator-=(const uInt &A)
{
    if (*this == A) // guarantee *this >= A for effi.
        return *this = 0;
    bool carry = false;
    for (unsigned i = 0; i < A.size(); ++i)
        num[i] = suber(num[i], A[i], carry);
    for (unsigned i = A.size(); i < size(); ++i)
        num[i] = suber(num[i], 0, carry);
    if (carry)
        --num.back();
    while (num.back() == 0)
        num.pop_back();
    return *this;
}

uInt &uInt::operator*=(const uInt &A)
{
    vector<unit> prod(size() + A.size(), 0); // production
    for (unsigned i = 0; i < A.size(); ++i)
    {
        unit carry = 0;
        for (unsigned j = 0; j < size(); ++j)
            muler(A[i], num[j], prod[i + j], carry);
        if (carry > 0)
            prod[i + size()] += carry;
    }
    if (prod.back() == 0)
        prod.pop_back(); // at most 1.
    num = move(prod);
    return *this;
}

uInt &uInt::operator/=(const unit &_num)
{
    if (_num >= MAX)
        return *this /= uInt(_num);
    if (*this < _num)
        return *this = 0;
    unit remainder = 0;
    for (auto digit = num.rbegin(); digit != num.rend(); ++digit)
        *digit = diver(*digit, _num, remainder);
    while (num.back() == 0 && size() > 1)
        num.pop_back();
    return *this;
}

uInt &uInt::operator%=(const unit &_num)
{
    if (_num >= MAX)
        return *this %= uInt(_num);
    if (*this < _num)
        return *this = _num;
    unit remainder = num.back() % _num;
    for (auto digit = num.rbegin() + 1; digit != num.rend(); ++digit)
        remainder = (_MAX * static_cast<twin>(remainder) + static_cast<twin>(*digit)) % _num;
    num.assign(1, remainder);
    return *this;
}

uInt &uInt::operator^=(const uInt &A)
{
    uInt base = *this;
    uInt power = 1;
    for (; power * 2 <= A; power *= 2)
        *this *= *this; // quick
    for (; power < A; ++power)
        *this *= base; // slow
    return *this;
}

uInt &uInt::operator>>=(const uInt &A)
{
    auto section = static_cast<pair<unit, unit>>(A.divmod(9));
    if (section.first >= size())
        return *this = 0;
    else if (section.first > 0)
        for (unit i = 0; i < size() - section.first; ++i)
            num[i] = num[i + section.first];
    num.resize(size() - section.first);
    if (section.second == 0)
        return *this;
    unit divisor = exp10_[section.second], multiplier = exp10_[LEN - section.second];
    unit nextRemainder = 0;
    for (auto digit = num.rbegin(); digit != num.rend(); ++digit)
    {
        unit prevRemainder = nextRemainder;
        nextRemainder = *digit % divisor; // wasted in the last step
        *digit = *digit / divisor + prevRemainder * multiplier;
    }
    if (num.back() == 0 && num.size() > 1)
        num.pop_back();
    return *this;
}

uInt &uInt::operator<<=(const uInt &A)
{
    auto section = static_cast<pair<unit, unit>>(A.divmod(9));
    if (section.first > 0)
    {
        num.resize(size() + section.first);
        for (unit i = size(); i >= section.first; --i)
            num[i] = num[i - section.first];
        for (unit i = 0; i < section.first; ++i)
            num[i] = 0;
    }
    if (section.second == 0)
        return *this;
    unit divisor = exp10_[LEN - section.second], multiplier = exp10_[section.second];
    unit nextCarry = 0;
    for (unit i = section.first; i < size(); ++i)
    {
        unit prevCarry = nextCarry;
        nextCarry = num[i] / divisor; // wasted in the last step
        num[i] = num[i] % divisor * multiplier + prevCarry;
    }
    if (nextCarry > 0)
        num.push_back(nextCarry);
    return *this;
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
    switch ((includeA << 1) | includeB)
    {
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

pair<uInt, unit> uInt::divmod(const unit &divident) const
{
    assert(divident != 0);
    if (*this < divident)
        return pair<uInt, unit>(0, num[0]);
    vector<unit> quot(size(), 0); // quotient
    unit remainder = 0;
    for (int i = quot.size() - 1; i >= 0; --i)
        quot[i] = diver(num[i], divident, remainder);
    if (quot.back() == 0)
        quot.pop_back();
    return pair<uInt, unit>(quot, remainder);
}

/** @return pair(quotient, remainder) */
pair<uInt, uInt> uInt::divmod(const uInt &A) const
{
    if (A.size() < 2) // if A.empty will throw errro
        return divmod(A[0]);
    if (*this < A)
        return pair<uInt, uInt>(0, *this);
    // attempt div, result Quot >= real Q
    unit exceedLen = A.length() - LEN;
    uInt maxQ = (*this >> exceedLen) / (A >> exceedLen)[0];
    if (*this == maxQ * A)
        return pair<uInt, uInt>(maxQ, 0);
    // real Q > maxQ * MAX / (MAX + 1)
    // here couldn't div (MAX + 1), replace smaller (MAX - 1) / MAX
    uInt minQ = ((maxQ * static_cast<uInt>(MAX - 8)) >> LEN);
    while (minQ + 1 < maxQ) // *this < maxQ * A
    {
        uInt midQ = (maxQ + minQ) / 2;
        uInt QA = midQ * A;
        if (*this > QA)
            minQ = move(midQ);
        else if (*this < QA)
            maxQ = move(midQ);
        else
            return pair<uInt, uInt>(midQ, 0);
    }
    return pair<uInt, uInt>(minQ, *this - minQ * A);
}

/** @return largest (2^n, n) that 2^n <= *this */
pair<uInt, unit> uInt::approxExp2() const
{
    if (*this == 0)
        return pair<uInt, unit>(0, 0);
    unit len = LEN * (size() - 1);
    unsigned i = 1;
    for (; exp10_[i] <= num.back(); ++i)
        ++len;
    unit firstNum = num.back() / exp10_[i - 1];
    unit power = log2_[firstNum] + len * LOG2_10; // error: -1~0
    uInt expo = exp2(power), expo2 = expo * 2;
    if (*this < expo2)
        return pair<uInt, unit>(expo, power);
    else
        return pair<uInt, unit>(expo2, power + 1);
}

uInt uInt::sqrt() const
{
    if (*this < 2)
        return *this;
    uInt sqrtA = vector<unit>(size() / 2 + 1, 0);
    while (*this < sqrtA * sqrtA)
        sqrtA = (sqrtA + *this / sqrtA) / 2;
    return sqrtA;
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
    assert(base <= 37);
    auto qr = divmod(base);
    string str(1, alphabet[qr.second]);
    while (qr.first != 0)
    {
        qr = qr.first.divmod(base);
        str += alphabet[qr.second];
    }
    reverse(str.begin(), str.end());
    if (suffix)
        str += '(' + to_string(base) + ')';
    return str;
}

string uInt::sciNote(unit deciLength) const
{
    string str = to_string(num.back()), subDeci;
    unit power = length() - 1;
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

uInt uInt::subInt(const unsigned &begin, const unsigned &end) const
{
    auto back = (end <= size() ? num.begin() + end : num.end());
    return uInt(vector<unit>(num.begin() + begin, back));
}

unit uInt::length(const unsigned &base) const
{
    if (base == 10)
    {
        unit len = LEN * (size() - 1);
        for (unsigned i = 1; exp10_[i] <= num.back(); ++i)
            ++len;
        return len + 1;
    }
    else if (base == 2)
        return approxExp2().second;
    else if (base == 4)
        return approxExp2().second / 2;
    else if (base == 8)
        return approxExp2().second / 3;
    else if (base == 16)
        return approxExp2().second / 4;
    return toString(base).length(); // shit
}

// private function

void uInt::normalize()
{
    twin carry = 0;
    for (unsigned i = 0; i < num.size(); ++i)
    {
        twin c = static_cast<twin>(num[i]) + carry;
        carry = c / _MAX;
        num[i] = static_cast<unit>(c % _MAX);
    }
    if (carry > 0)
    {
        num.push_back(carry);
        return;
    }
    while (num.back() == 0 && size() > 1)
        num.pop_back();
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
    twin c = _MAX * static_cast<twin>(remainder) + static_cast<twin>(a);
    remainder = static_cast<unit>(c % b);
    return static_cast<unit>(c / b);
}