#ifndef BIG_MATH_H_
#define BIG_MATH_H_
#include "bigNumber.h"

/** @return 2^A */
uInt exp2(const uInt& N);

inline uInt exp10(const std::size_t& n) { return exp10(uInt(n)); }

/** @return largest 2^n that <= A */
inline uInt exp2fit(const uInt& A) { return A.approxExp2().first; }

/** @return largest n that 2^n <= A */
inline uInt log2(const uInt& A) { return A.approxExp2().second; }

inline uInt sqrt(const uInt& A) { return ~A; }

// Real sqrt(const Real &A);

uInt gcd(const uInt& A, const uInt& B);

uInt exgcd(const uInt& A, const uInt& B);

uInt lcm(const uInt& A, const uInt& B);

inline uInt factorial(const uInt& N) { return !N; }

/** @brief calculate the nth fibonacci number: \n
 * f(1) = f(2) = 1, f(n) = f(n-1) + f(n-2). */
uInt fibonacci(const uInt& N, const bool& startFrom0 = false);

bool isPrime(const uInt& A);

uInt prime(const uInt& A);

uInt permutation(const uInt& n, const uInt& k);

uInt combination(const uInt& n, const uInt& k);

#endif