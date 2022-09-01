#ifndef BIG_MATH_H_
#define BIG_MATH_H_
#include "bigNumber.h"

/** @return 2^A */
uInt exp2(const uInt &A);

/** @return largest 2^n that <= A */
uInt exp2fit(const uInt &A);

/** @return largest n that 2^n <= A */
uInt log2(const uInt &A);

uInt sqrt(const uInt &A);

uInt gcd(const uInt &A, const uInt &B);

uInt exgcd(const uInt &A, const uInt &B);

uInt lcm(const uInt &A, const uInt &B);

/** @return n! */
uInt factorial(unsigned n);

/** @brief calculate the nth fibonacci number: \n
 * f(1) = f(2) = 1, f(n) = f(n-1) + f(n-2). */
uInt fibonacci(unsigned n, bool startFrom0 = false);

#endif // big-number generator