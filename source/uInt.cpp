#include "bigNumber.h"
#include "bigMath.h"
#include <assert.h>  // assert
#include <algorithm> // reverse
#include <array>     // array
#include <iomanip>   // setw, setfill
using namespace std;

#define LG2_32_9 241726409 / 225843117 // 32 * lg2 / 9
#define LOG2_10 1079882313 / 325076968 // DO NOT TOUCH

static const uint32_t LOG2_[] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3};
static const uint32_t EXP10_[] = {
    1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000};
static const char ALPHABET[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char uInt::delimiter = ',';
unsigned uInt::interval = uInt::LEN;

uInt::uInt(const uint64_t &_num)
{
    if (_num < MAXL)
        num.push_back(static_cast<uint32_t>(_num));
    else
        for (auto carry = _num; carry > 0; carry /= MAXL)
            num.push_back(static_cast<uint32_t>(carry % MAXL));
}

uInt::uInt(const string &_num)
{
    if (_num[0] == '+' || _num[0] == '-')
    {
        *this = uInt(_num.substr(1)); // omit sign
        return;
    }
    bool isZero = true;
    for (auto &_digit : _num)
        if (_digit < '0' || _digit > '9')
        {
            cout << "WARNING: N/A string in constructor, set as 0." << endl;
            num.push_back(0);
            return;
        }
        else if (isZero && _digit != '0')
            isZero = false;
    if (isZero) // all 0
    {
        num.push_back(0);
        return;
    }
    int pos;
    for (pos = _num.length() - LEN; pos > 0; pos -= LEN)
        num.push_back(stoi(_num.substr(pos, LEN)));
    num.push_back(stoi(_num.substr(0, LEN + pos))); // p <= 0
    while (num.back() == 0)
        num.pop_back();
}

uInt::operator uint64_t() const
{
    switch (_size())
    {
    case 1:
        return static_cast<uint64_t>(num[0]);
    case 2:
        return MAXL * static_cast<uint64_t>(num[1]) + static_cast<uint64_t>(num[0]);
    default:
        return MAXL * MAXL * static_cast<uint64_t>(num[2]) +
               MAXL * static_cast<uint64_t>(num[1]) + static_cast<uint64_t>(num[0]);
    }
}

bool uInt::operator<(const uInt &A) const
{
    if (_size() != A._size())
        return _size() < A._size();
    for (int i = _size() - 1; i >= 0; --i)
        if (num[i] != A[i])
            return num[i] < A[i];
    return false;
}

uInt &uInt::operator+=(const uInt &A)
{
    if (_size() < A._size())
        num.resize(A._size(), 0);
    uint32_t carry = 0;
    size_t i = 0;
    for (; i < A._size(); ++i)
        num[i] = _adder(num[i], A[i], carry);
    for (; i < num.size(); ++i)
        num[i] = _adder(num[i], 0, carry);
    if (carry > 0)
        num.push_back(carry);
    return *this;
}

uInt &uInt::operator-=(const uInt &A)
{
    if (*this == A) // guarantee *this >= A for effi.
        return *this = 0;
    bool carry = false;
    for (size_t i = 0; i < A._size(); ++i)
        num[i] = _suber(num[i], A[i], carry);
    for (size_t i = A._size(); i < _size(); ++i)
        num[i] = _suber(num[i], 0, carry);
    if (carry)
        --num.back();
    while (num.back() == 0)
        num.pop_back();
    return *this;
}

uInt &uInt::operator*=(const uint32_t &_num)
{
    uint64_t carry = 0;
    for (auto &part : num)
    {
        uint64_t c = static_cast<uint64_t>(part) * static_cast<uint64_t>(_num) + carry;
        carry = c / MAXL;
        part = static_cast<uint32_t>(c % MAXL);
    }
    if (carry == 0)
        return *this;
    if (carry < MAXL)
        num.push_back(static_cast<uint32_t>(carry));
    else
    {
        num.push_back(static_cast<uint32_t>(carry % MAXL));
        num.push_back(static_cast<uint32_t>(carry / MAXL));
    }
    return *this;
}

uInt &uInt::operator*=(const uInt &A)
{
    vector<uint32_t> prod(_size() + A._size(), 0); // production
    for (size_t i = 0; i < A._size(); ++i)
    {
        uint64_t carry = 0; // < MAXL
        for (size_t j = 0; j < _size(); ++j)
            _muler(A[i], num[j], prod[i + j], carry);
        if (carry > 0)
            prod[i + _size()] += carry;
    }
    if (prod.back() == 0)
        prod.pop_back(); // at most 1.
    num = move(prod);
    return *this;
}

uInt &uInt::operator/=(const uint32_t &_num)
{
    this->div(_num);
    return *this;
}

uInt &uInt::operator%=(const uint32_t &_num)
{
    if (_num >= MAX)
        return *this = this->divmod(uInt(_num)).second;
    if (*this < _num)
    {
        num.assign(_num, 1);
        return *this;
    }
    uint32_t remainder = num.back() % _num;
    for (auto part = num.rbegin() + 1; part != num.rend(); ++part)
        remainder = (MAXL * static_cast<uint64_t>(remainder) + static_cast<uint64_t>(*part)) % _num;
    num.assign(1, remainder);
    return *this;
}

uInt &uInt::operator^=(const uInt &A)
{
    if (*this < 2)
        return *this;
    switch (static_cast<size_t>(A))
    {
    case 0:
        return *this = 1;
    case 1:
        return *this;
    case 2:
        return *this *= *this;
    }
    uInt base = *this;
    uInt power = 1, halfA = A / 2;
    for (; power <= halfA; power *= 2)
        *this *= *this; // quick
    for (; power < A; ++power)
        *this *= base; // extremely slow
    return *this;
}

uInt &uInt::operator>>=(const size_t &_num)
{
    size_t sizeLen = _num / LENL;
    if (sizeLen >= _size())
        return *this = 0;
    else if (sizeLen > 0)
        for (size_t i = 0; i < _size() - sizeLen; ++i)
            num[i] = num[i + sizeLen];
    num.resize(_size() - sizeLen);
    uint32_t digitLen = _num % LEN;
    if (digitLen == 0)
        return *this;
    uint32_t shift = EXP10_[digitLen], kept = EXP10_[LEN - digitLen];
    uint32_t nextRemainder = 0;
    for (auto part = num.rbegin(); part != num.rend(); ++part)
    {
        uint32_t prevRemainder = nextRemainder;
        nextRemainder = *part % shift; // wasted in the last step
        *part = *part / shift + prevRemainder * kept;
    }
    if (num.back() == 0 && num.size() > 1)
        num.pop_back();
    return *this;
}

uInt &uInt::operator<<=(const size_t &_num)
{
    if (*this == 0)
        return *this;
    size_t sizeLen = _num / LENL;
    if (sizeLen > 0)
    {
        num.resize(_size() + sizeLen);
        for (size_t i = _size(); i >= sizeLen; --i)
            num[i] = num[i - sizeLen];
        for (size_t i = 0; i < sizeLen; ++i)
            num[i] = 0;
    }
    uint32_t digitLen = static_cast<uint32_t>(_num % LENL);
    if (digitLen == 0)
        return *this;
    uint32_t shift = EXP10_[digitLen], kept = EXP10_[LEN - digitLen];
    uint32_t nextCarry = 0;
    for (size_t i = sizeLen; i < _size(); ++i)
    {
        uint32_t prevCarry = nextCarry;
        nextCarry = num[i] / kept; // wasted in the last step
        num[i] = num[i] % kept * shift + prevCarry;
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

uInt uInt::operator~()
{
    if (*this < 2)
        return *this;
    uInt root(vector<uint32_t>(_size() / 2 + 2, 0));
    root.num.back() = 1;
    do // Newton
        root = (root + *this / root) / 2;
    while (root * root > *this);
    return root;
}

/** @return nthRoot ? this ^ N : NthRoot(this) */
uInt uInt::operator()(const uInt &N, const bool &nthRoot = false) const
{
    if (*this < 2)
        return *this;
    if (!nthRoot)
        return *this ^ N;
    size_t n = static_cast<size_t>(N);
    uInt root(vector<uint32_t>(_size() / n + 2, 0));
    root.num.back() = 1;
    uInt rootn = root ^ (n - 1);
    do {
        root = ((n - 1) * root + *this / rootn) / n;
        rootn = root ^ (n - 1);
    } while (root * rootn > *this);
    return root;
}

void uInt::setDelimiter(const char &_c, const unsigned &_interval)
{
    if (_c == ' ' || _c == ',' || _c == ';' || _c == '\'')
        delimiter = _c;
    else
        cout << "ERROR: N/A delimiter, only \' \', \',\', \'\'\' and \';\' allowed." << endl;
    if (_interval == 0 || _interval == 3 || _interval == 9) // 4 or 10? maybe.
        interval = _interval;
    else
        cout << "ERROR: N/A interval, only 0, 3 and 9 allowed." << endl;
}

/** @return A <= this < B */
bool uInt::between(const uInt &A, const uInt &B, const bool &includeA, const bool &includeB) const
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

/** @param divident @return pair(quotient, remainder) */
pair<uInt, uInt> uInt::divmod(const uInt &A) const
{
    if (A._size() < 2) // if A.empty will throw errro
    {
        uInt thisCopy = *this;
        uint32_t remainder = thisCopy.div(A[0]);
        return pair<uInt, uInt>(thisCopy, remainder);
    }
    if (*this < A)
        return pair<uInt, uInt>(0, *this);
    // attempt div, result Quot + 1 >= real Q
    size_t exceedLen = A.length() - LENL;
    uInt maxQ = (*this >> exceedLen) / (A >> exceedLen)[0];
    uInt QA = maxQ * A;
    if (*this >= QA)
        return pair<uInt, uInt>(maxQ, *this - QA);
    // real Q > maxQ * 100'000'000 / 100'000'001
    uInt minQ = (maxQ << 8) / 100000001;
    while (minQ + 1 < maxQ) // *this < maxQ * A
    {
        uInt midQ = (maxQ + minQ) / 2;
        uInt midQA = midQ * A;
        if (*this > midQA)
        {
            minQ = move(midQ);
            QA = move(midQA);
        }
        else if (*this < midQA)
            maxQ = move(midQ);
        else
            return pair<uInt, uInt>(midQ, 0);
    }
    return pair<uInt, uInt>(minQ, *this - QA);
}

uInt uInt::coarseDiv(const uInt &A, const std::size_t exactDigit) const
{
    return 0;
}

/** @return largest (2^n, n) that 2^n <= *this */
pair<uInt, uint64_t> uInt::approxExp2() const
{
    if (*this == 0)
        return pair<uInt, uint32_t>(0, 0);
    size_t len = LENL * (_size() - 1), i = 1;
    for (; EXP10_[i] <= num.back(); ++i)
        ++len;
    uint32_t firstNum = num.back() / EXP10_[i - 1];
    uint32_t power = LOG2_[firstNum] + len * LOG2_10; // error: -1~0
    uInt expo = exp2(power), expo2 = expo * 2;
    if (*this < expo2)
        return pair<uInt, uint32_t>(expo, power);
    return pair<uInt, uint32_t>(expo2, power + 1);
}

uInt exp10(const uInt &N)
{
    size_t n = static_cast<size_t>(N);
    vector<uint32_t> expo(n / uInt::LENL + 1, 0);
    expo.back() = EXP10_[n % uInt::LENL];
    return uInt(move(expo));
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
    uInt thisCopy = *this;
    string str(1, ALPHABET[thisCopy.div(base)]);
    while (bool(thisCopy))
        str += ALPHABET[thisCopy.div(base)];
    reverse(str.begin(), str.end());
    if (suffix)
        str += '(' + to_string(base) + ')';
    return str;
}

string uInt::sciNote(const size_t &_deciLength) const
{
    string str = to_string(num.back()), subDeci;
    size_t power = length() - 1;
    size_t deciLength = min(_deciLength, power);
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
    auto back = (end <= _size() ? num.begin() + end : num.end());
    return uInt(vector<uint32_t>(num.begin() + begin, back));
}

size_t uInt::length(const unsigned &base) const
{
    if (base == 10)
    {
        size_t len = LENL * (_size() - 1);
        for (unsigned i = 1; EXP10_[i] <= num.back(); ++i)
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

void uInt::_normalize()
{
    while (num.back() == 0 && _size() > 1)
        num.pop_back();
    uInt carry = 0;
    for (auto &digit : num)
    {
        uInt c = digit + carry;
        carry = c / MAXL;
        digit = static_cast<uint32_t>(c % MAXL);
    }
    // if (carry > 0)
    // num.push_back(carry);
}

/** @brief a + b + inCarry = c & outCarry. */
inline uint32_t uInt::_adder(const uint32_t &a, const uint32_t &b, uint32_t &carry) const
{
    uint32_t c = a + b + carry; // max(c) < 3 * MAX < 0xFFFFFFFF, uint32_t is OK.
    carry = c / MAX;
    return c % MAX;
}

/** @brief MAX + a - b - prevBorrow = c & nextBorrow. */
inline uint32_t uInt::_suber(const uint32_t &a, const uint32_t &b, bool &borrow) const
{
    uint32_t c = MAX + a - b - borrow;
    borrow = (c < MAX); // carry is either 0 or 1.
    return c % MAX;
}

/** @brief a * b + p + inCarry = c & outCarry. */
inline void uInt::_muler(const uint32_t &a, const uint32_t &b, uint32_t &p, uint64_t &carry) const
{
    uint64_t c = static_cast<uint64_t>(a) * static_cast<uint64_t>(b) + static_cast<uint64_t>(p) + carry;
    carry = c / MAXL;
    p = static_cast<uint32_t>(c % MAXL);
}

/** @brief a / b + prevRemainder = c & nextRemainder. */
inline uint32_t uInt::_diver(const uint32_t &a, const uint32_t &b, uint32_t &remainder) const
{
    uint64_t c = MAXL * static_cast<uint64_t>(remainder) + static_cast<uint64_t>(a);
    remainder = static_cast<uint32_t>(c % b);
    return static_cast<uint32_t>(c / b);
}

/** @param _divident < MAX, @return remainder */
uint32_t uInt::div(const uint32_t &_divident)
{
    if (_divident >= MAX)
    {
        auto quoRem = this->divmod(uInt(_divident));
        *this = quoRem.first; // can't use auto [,]
        return static_cast<uint32_t>(quoRem.second);
    }
    if (*this < _divident)
    {
        *this = 0;
        return _divident;
    }
    uint32_t remainder = 0;
    for (auto part = num.rbegin(); part != num.rend(); ++part)
        *part = _diver(*part, _divident, remainder);
    while (num.back() == 0 && _size() > 1) // at most once
        num.pop_back();
    return remainder;
}
