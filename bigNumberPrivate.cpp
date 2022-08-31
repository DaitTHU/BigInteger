#include "bigMath.h"
using namespace std;

/** @brief a + b + inCarry = c & outCarry. */
unit uInt::adder(const unit &a, const unit &b, unit &carry)
{
    unit c = a + b + carry; // max(c) < 3 * MAX < 0xFFFFFFFF, uint32_t is OK.
    carry = c / MAX;
    return c % MAX;
}

/** @brief MAX + a - b - inCarry = c & outCarry. */
unit uInt::suber(const unit &a, const unit &b, bool &carry)
{
    unit c = MAX + a - b - carry;
    carry = (c < MAX); // carry is either 0 or 1.
    return c % MAX;
}
/** @brief a * b + p + inCarry = c & outCarry. */
unit uInt::muler(const unit &a, const unit &b, const unit &p, unit &carry)
{
    doub c = static_cast<doub>(a) * static_cast<doub>(b) + static_cast<doub>(p) + static_cast<doub>(carry);
    carry = static_cast<unit>(c / MAX);
    return static_cast<unit>(c % MAX);
}

/** @brief a / b + inCarry = c & outCarry. */
unit uInt::diver(const unit &a, const unit &b, unit &carry)
{
    doub c = static_cast<doub>(MAX) * static_cast<doub>(carry) + static_cast<doub>(a);
    carry = static_cast<unit>(c / b);
    return static_cast<unit>(c % b);
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