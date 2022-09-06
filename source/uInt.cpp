#include "bigNumber.h"
#include "bigMath.h"
#include <assert.h>  // assert
#include <algorithm> // reverse
#include <array>     // array
#include <iomanip>   // setw, setfill
using namespace std;

#define LG2_32_9 241726409 / 225843117 // 32 * lg2 / 9
#define LOG2_10 1079882313 / 325076968 // DO NOT TOUCH

static const uint32_t LOG2_[] = { 0, 0, 1, 1, 2, 2, 2, 2, 3, 3 };
static const uint32_t EXP10_[] = {
    1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000 };
static const char ALPHABET[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char uInt::delimiter_ = ',';
unsigned uInt::interval_ = uInt::LEN_;

uInt::uInt(const uint64_t& n)
{
    if (n < MAXL_)
        num_.push_back(static_cast<uint32_t>(n));
    else
        for (auto carry = n; carry > 0; carry /= MAXL_)
            num_.push_back(static_cast<uint32_t>(carry % MAXL_));
}

uInt::uInt(const string& n)
{
    if (n[0] == '+' || n[0] == '-') {
        *this = uInt(n.substr(1)); // omit sign
        return;
    }
    bool isZero = true;
    for (auto& _digit : n)
        if (_digit < '0' || _digit > '9') {
            cout << "WARNING: N/A string in constructor, set as 0." << endl;
            num_.push_back(0);
            return;
        } else if (isZero && _digit != '0')
            isZero = false;
        if (isZero) {
            num_.push_back(0);
            return;
        }
        int pos;
        for (pos = n.length() - LEN_; pos > 0; pos -= LEN_)
            num_.push_back(stoi(n.substr(pos, LEN_)));
        num_.push_back(stoi(n.substr(0, LEN_ + pos))); // p <= 0
        while (num_.back() == 0)
            num_.pop_back();
}

uInt::operator uint64_t() const
{
    switch (size_()) {
    case 1:
        return static_cast<uint64_t>(num_[0]);
    case 2:
        return MAXL_ * static_cast<uint64_t>(num_[1]) + static_cast<uint64_t>(num_[0]);
    default:
        return MAXL_ * MAXL_ * static_cast<uint64_t>(num_[2]) +
            MAXL_ * static_cast<uint64_t>(num_[1]) + static_cast<uint64_t>(num_[0]);
    }
}

bool uInt::operator<(const uInt& A) const
{
    if (size_() != A.size_())
        return size_() < A.size_();
    for (int i = size_() - 1; i >= 0; --i)
        if (num_[i] != A[i])
            return num_[i] < A[i];
    return false;
}

uInt& uInt::operator+=(const uInt& A)
{
    if (size_() < A.size_())
        num_.resize(A.size_(), 0);
    uint32_t carry = 0;
    size_t i = 0;
    for (; i < A.size_(); ++i)
        num_[i] = adder_(num_[i], A[i], carry);
    for (; i < num_.size(); ++i)
        num_[i] = adder_(num_[i], 0, carry);
    if (carry > 0)
        num_.push_back(carry);
    return *this;
}

uInt& uInt::operator-=(const uInt& A)
{
    if (*this == A) // guarantee *this >= A for effi.
        return *this = 0;
    bool carry = false;
    for (size_t i = 0; i < A.size_(); ++i)
        num_[i] = suber_(num_[i], A[i], carry);
    for (size_t i = A.size_(); i < size_(); ++i)
        num_[i] = suber_(num_[i], 0, carry);
    if (carry)
        --num_.back();
    while (num_.back() == 0)
        num_.pop_back();
    return *this;
}

uInt& uInt::operator*=(const uint32_t& n)
{
    uint64_t carry = 0;
    for (auto& part : num_) {
        uint64_t c = static_cast<uint64_t>(part) * static_cast<uint64_t>(n) + carry;
        carry = c / MAXL_;
        part = static_cast<uint32_t>(c % MAXL_);
    }
    if (carry == 0)
        return *this;
    if (carry < MAXL_)
        num_.push_back(static_cast<uint32_t>(carry));
    else {
        num_.push_back(static_cast<uint32_t>(carry % MAXL_));
        num_.push_back(static_cast<uint32_t>(carry / MAXL_));
    }
    return *this;
}

uInt& uInt::operator*=(const uInt& A)
{
    vector<uint32_t> prod(size_() + A.size_(), 0); // production
    for (size_t i = 0; i < A.size_(); ++i) {
        uint64_t carry = 0; // < MAXL_
        for (size_t j = 0; j < size_(); ++j)
            muler_(A[i], num_[j], prod[i + j], carry);
        if (carry > 0)
            prod[i + size_()] += carry;
    }
    if (prod.back() == 0)
        prod.pop_back(); // at most 1.
    num_ = move(prod);
    return *this;
}

uInt& uInt::operator/=(const uint32_t& n)
{
    this->div_(n);
    return *this;
}

uInt& uInt::operator%=(const uint32_t& n)
{
    if (n >= MAX_)
        return *this = this->divmod(uInt(n)).second;
    if (*this < n) {
        num_.assign(n, 1);
        return *this;
    }
    uint32_t remainder = num_.back() % n;
    for (auto part = num_.rbegin() + 1; part != num_.rend(); ++part)
        remainder = (MAXL_ * static_cast<uint64_t>(remainder) + static_cast<uint64_t>(*part)) % n;
    num_.assign(1, remainder);
    return *this;
}

uInt& uInt::operator^=(const uInt& A)
{
    if (*this < 2)
        return *this;
    switch (static_cast<size_t>(A)) {
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

uInt& uInt::operator>>=(const size_t& n)
{
    size_t sizeLen = n / LENL_;
    if (sizeLen >= size_())
        return *this = 0;
    else if (sizeLen > 0)
        for (size_t i = 0; i < size_() - sizeLen; ++i)
            num_[i] = num_[i + sizeLen];
    num_.resize(size_() - sizeLen);
    uint32_t digitLen = n % LEN_;
    if (digitLen == 0)
        return *this;
    uint32_t shift = EXP10_[digitLen], kept = EXP10_[LEN_ - digitLen];
    uint32_t nextRemainder = 0;
    for (auto part = num_.rbegin(); part != num_.rend(); ++part) {
        uint32_t prevRemainder = nextRemainder;
        nextRemainder = *part % shift; // wasted in the last step
        *part = *part / shift + prevRemainder * kept;
    }
    if (num_.back() == 0 && num_.size() > 1)
        num_.pop_back();
    return *this;
}

uInt& uInt::operator<<=(const size_t& n)
{
    if (*this == 0)
        return *this;
    size_t sizeLen = n / LENL_;
    if (sizeLen > 0) {
        num_.resize(size_() + sizeLen);
        for (size_t i = size_(); i >= sizeLen; --i)
            num_[i] = num_[i - sizeLen];
        for (size_t i = 0; i < sizeLen; ++i)
            num_[i] = 0;
    }
    uint32_t digitLen = static_cast<uint32_t>(n % LENL_);
    if (digitLen == 0)
        return *this;
    uint32_t shift = EXP10_[digitLen], kept = EXP10_[LEN_ - digitLen];
    uint32_t nextCarry = 0;
    for (size_t i = sizeLen; i < size_(); ++i) {
        uint32_t prevCarry = nextCarry;
        nextCarry = num_[i] / kept; // wasted in the last step
        num_[i] = num_[i] % kept * shift + prevCarry;
    }
    if (nextCarry > 0)
        num_.push_back(nextCarry);
    return *this;
}

ostream& operator<<(ostream& os, const uInt& A)
{
    if (uInt::interval_ == 3) // this is really a piece of sheet!
    {
        string subNum = to_string(*A.num_.rbegin());
        unsigned firstLen = subNum.length();
        if (firstLen < 4)
            os << subNum;
        else if (firstLen < 7)
            os << subNum.substr(0, firstLen - 4) << uInt::delimiter_
            << subNum.substr(firstLen - 4, 3);
        else
            os << subNum.substr(0, firstLen - 7) << uInt::delimiter_
            << subNum.substr(firstLen - 7, 3) << uInt::delimiter_
            << subNum.substr(firstLen - 4, 3);
        for (auto part = A.num_.rbegin() + 1; part != A.num_.rend(); ++part) {
            subNum = to_string(*part);
            subNum = string(uInt::LEN_ - subNum.length(), '0') + subNum;
            os << uInt::delimiter_ << subNum.substr(0, 3)
                << uInt::delimiter_ << subNum.substr(3, 3)
                << uInt::delimiter_ << subNum.substr(6, 3);
        }
        return os;
    }
    os << *A.num_.rbegin();
    if (uInt::interval_ == 9)
        for (auto part = A.num_.rbegin() + 1; part != A.num_.rend(); ++part)
            os << uInt::delimiter_ << setfill('0') << setw(uInt::LEN_) << *part;
    else // uInt::interval_ == 0
        for (auto part = A.num_.rbegin() + 1; part != A.num_.rend(); ++part)
            os << setfill('0') << setw(uInt::LEN_) << *part;
    return os;
}

istream& operator>>(istream& is, uInt& A)
{
    string str;
    is >> str;
    A = uInt(str);
    return is;
}

/** @return this! */
uInt uInt::operator!() const
{
    static const double LOG10_[] = {
        0, .30103, .477121, .60206, .69897, .778151, .845098, .90309, .954243 };
    static const double LG_2PI = .79817986835811505;

    uInt result = 1;
    for (size_t i = static_cast<size_t>(*this); i > 1; --i)
        result *= i;
    return result;
}

uInt uInt::operator~() const
{
    if (*this < 2)
        return *this;
    uInt root(vector<uint32_t>(size_() / 2 + 2, 0));
    root.num_.back() = 1;
    do // Newton
        root = (root + *this / root) / 2;
    while (root * root > *this);
    return root;
}

/** @return nthRoot ? this ^ N : NthRoot(this) */
uInt uInt::operator()(const uInt& N, const bool& nthRoot = false) const
{
    if (*this < 2)
        return *this;
    if (!nthRoot)
        return *this ^ N;
    size_t n = static_cast<size_t>(N);
    uInt root(vector<uint32_t>(size_() / n + 2, 0));
    root.num_.back() = 1;
    uInt rootn = root ^ (n - 1);
    do {
        root = ((n - 1) * root + *this / rootn) / n;
        rootn = root ^ (n - 1);
    } while (root * rootn > *this);
    return root;
}

void uInt::setDelimiter(const char& _c, const unsigned& _interval)
{
    if (_c == ' ' || _c == ',' || _c == ';' || _c == '\'')
        delimiter_ = _c;
    else
        cout << "ERROR: N/A delimiter_, only \' \', \',\', \'\'\' and \';\' allowed." << endl;
    if (_interval == 0 || _interval == 3 || _interval == 9) // 4 or 10? maybe.
        interval_ = _interval;
    else
        cout << "ERROR: N/A interval_, only 0, 3 and 9 allowed." << endl;
}

/** @return A <= this < B */
bool uInt::between(const uInt& A, const uInt& B, const bool& includeA, const bool& includeB) const
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

/** @param divident @return pair(quotient, remainder) */
pair<uInt, uInt> uInt::divmod(const uInt& A) const
{
    if (A.size_() < 2) { // if A.empty will throw errro
        uInt thisCopy = *this;
        uint32_t remainder = thisCopy.div_(A[0]);
        return pair<uInt, uInt>(thisCopy, remainder);
    }
    if (*this < A)
        return pair<uInt, uInt>(0, *this);
    // attempt div, result Quot + 1 >= real Q
    size_t exceedLen = A.length() - LENL_;
    uInt maxQ = (*this >> exceedLen) / (A >> exceedLen)[0];
    uInt QA = maxQ * A;
    if (*this >= QA)
        return pair<uInt, uInt>(maxQ, *this - QA);
    // real Q > maxQ * 100'000'000 / 100'000'001
    uInt minQ = (maxQ << 8) / 100000001;
    while (minQ + 1 < maxQ) { // *this < maxQ * A
        uInt midQ = (maxQ + minQ) / 2;
        uInt midQA = midQ * A;
        if (*this > midQA) {
            minQ = move(midQ);
            QA = move(midQA);
        } else if (*this < midQA)
            maxQ = move(midQ);
        else
            return pair<uInt, uInt>(midQ, 0);
    }
    return pair<uInt, uInt>(minQ, *this - QA);
}

uInt uInt::coarseDiv(const uInt& A, const std::size_t& _exactDigit) const
{
    return 0;
}

/** @return largest (2^n, n) that 2^n <= *this */
pair<uInt, uint64_t> uInt::approxExp2() const
{
    if (*this == 0)
        return pair<uInt, uint32_t>(0, 0);
    size_t len = LENL_ * (size_() - 1), i = 1;
    for (; EXP10_[i] <= num_.back(); ++i, ++len);
    uint32_t firstNum = num_.back() / EXP10_[i - 1];
    uint32_t power = LOG2_[firstNum] + len * LOG2_10; // error: -1~0
    uInt expo = exp2(power), expo2 = expo * 2;
    if (*this < expo2)
        return pair<uInt, uint32_t>(expo, power);
    return pair<uInt, uint32_t>(expo2, power + 1);
}

uInt exp10(const uInt& N)
{
    auto n = static_cast<size_t>(N);
    vector<uint32_t> expo(1 + n / uInt::LENL_, 0);
    expo.back() = EXP10_[n % uInt::LENL_];
    return uInt(move(expo));
}

string uInt::toString(const unsigned& _base, const bool& _suffix) const
{
    if (_base == 10) {
        string str = to_string(num_.back()), subNum;
        for (auto part = num_.rbegin() + 1; part != num_.rend(); ++part) {
            subNum = to_string(*part);
            str += string(LEN_ - subNum.length(), '0') + subNum;
        }
        if (_suffix)
            str += "(10)";
        return str;
    }
    assert(_base <= 37);
    uInt thisCopy = *this;
    string str(1, ALPHABET[thisCopy.div_(_base)]);
    while (bool(thisCopy))
        str += ALPHABET[thisCopy.div_(_base)];
    reverse(str.begin(), str.end());
    if (_suffix)
        str += '(' + to_string(_base) + ')';
    return str;
}

string uInt::sciNote(const size_t& _deciLength) const
{
    string str = to_string(num_.back()), subDeci;
    size_t power = length() - 1;
    size_t deciLength = min(_deciLength, power);
    if (deciLength == 0)
        return str.substr(0, 1) + " x 10^" + to_string(power);
    str.insert(1, 1, '.');
    for (auto part = num_.rbegin() + 1; str.length() < 2 + deciLength; ++part) {
        subDeci = to_string(*part);
        str += string(LEN_ - subDeci.length(), '0') + subDeci;
    }
    return str.substr(0, 2 + deciLength) + " x 10^" + to_string(power);
}

uInt uInt::subInt(const size_t& _beginDigit, const size_t& _endDigit) const
{
    if (_beginDigit >= _endDigit)
        return static_cast<uInt>(0);
    size_t beginDigit = min(length(), _beginDigit);
    size_t endDigit = min(length() + 1, _endDigit);
    uInt subN(vector<uint32_t>(num_.begin() + beginDigit / LEN_, num_.begin() + 1 + endDigit / LEN_));
    subN.num_.back() %= EXP10_[endDigit % LENL_];
    subN >>= beginDigit % LEN_;
    while (subN.num_.back() == 0 && subN.size_() > 1)
        subN.num_.pop_back();
    return subN;
}

size_t uInt::length(const unsigned& base) const
{
    if (base == 10) {
        size_t len = LENL_ * (size_() - 1);
        for (unsigned i = 1; EXP10_[i] <= num_.back(); ++i)
            ++len;
        return len + 1;
    } else if (base == 2)
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

void uInt::normalize_()
{
    while (num_.back() == 0 && size_() > 1)
        num_.pop_back();
    uInt carry = 0, c;
    for (auto& digit : num_) {
        c = digit + carry;
        carry = c / MAXL_;
        digit = static_cast<uint32_t>(c % MAXL_);
    }
}

/** @brief a + b + inCarry = c & outCarry. */
inline uint32_t uInt::adder_(const uint32_t& a, const uint32_t& b, uint32_t& carry) const
{
    uint32_t c = a + b + carry; // max(c) < 3 * MAX_ < 0xFFFFFFFF, uint32_t is OK.
    carry = c / MAX_;
    return c % MAX_;
}

/** @brief MAX_ + a - b - prevBorrow = c & nextBorrow. */
inline uint32_t uInt::suber_(const uint32_t& a, const uint32_t& b, bool& borrow) const
{
    uint32_t c = MAX_ + a - b - borrow;
    borrow = (c < MAX_); // borrow is either 0 or 1.
    return c % MAX_;
}

/** @brief a * b + p + inCarry = c & outCarry. */
inline void uInt::muler_(const uint32_t& a, const uint32_t& b, uint32_t& p, uint64_t& carry) const
{
    uint64_t c = static_cast<uint64_t>(a) * static_cast<uint64_t>(b) + static_cast<uint64_t>(p) + carry;
    carry = c / MAXL_;
    p = static_cast<uint32_t>(c % MAXL_);
}

/** @brief a / b + prevRemainder = c & nextRemainder. */
inline uint32_t uInt::diver_(const uint32_t& a, const uint32_t& b, uint32_t& remainder) const
{
    uint64_t c = MAXL_ * static_cast<uint64_t>(remainder) + static_cast<uint64_t>(a);
    remainder = static_cast<uint32_t>(c % b);
    return static_cast<uint32_t>(c / b);
}

/** @param _divident < MAX_, @return remainder */
uint32_t uInt::div_(const uint32_t& _divident)
{
    if (_divident >= MAX_) {
        auto quoRem = this->divmod(uInt(_divident));
        *this = quoRem.first; // can't use auto [,]
        return static_cast<uint32_t>(quoRem.second);
    }
    if (*this < _divident) {
        auto divisor = static_cast<uint32_t>(*this);
        *this = 0;
        return divisor;
    }
    uint32_t remainder = 0;
    for (auto part = num_.rbegin(); part != num_.rend(); ++part)
        *part = diver_(*part, _divident, remainder);
    while (num_.back() == 0 && size_() > 1) // at most once
        num_.pop_back();
    return remainder;
}
