#ifndef BIG_MATH_H_
#define BIG_MATH_H_
#include "bigNumber.h"
#include <tuple>

//namespace math {
    /** @return 2^n */
    inline uInt exp2(const std::size_t& n) { return exp2(uInt(n)); }

    /** @return 2^n */
    inline uInt exp10(const std::size_t& n) { return exp10(uInt(n)); }

    /** @return largest 2^n that <= A */
    inline uInt maxExp2(const uInt& A) { return exp2Floor(A).first; }

    /** @return largest n that 2^n <= A */
    inline uInt log2(const uInt& A) { return exp2Floor(A).second; }

    /** @return largest n that 10^n <= A */
    inline uInt log10(const uInt& A) { return A.length() - 1; }

    /** @return the square root of A */
    inline uInt sqrt(const uInt& A) { return ~A; }

    // Real sqrt(const Real &A);

    /** @return Greatest Common Divisor, use Euclidean algorithm */
    inline uInt gcd(const uInt& A, const uInt& B) { return B == 0 ? A : gcd(B, A % B); }

    std::tuple<uInt, uInt, uInt> exgcd(const uInt& A, const uInt& B);

    /** @return Least Common Multiple , use A * B = gcd(A, B) * lcm(A, B) */
    inline uInt lcm(const uInt& A, const uInt& B) { return A * B / gcd(A, B); }

    inline uInt factorial(const uInt& N) { return !N; }

    /** @brief calculate the nth fibonacci number: \n
     * f(1) = f(2) = 1, f(n) = f(n-1) + f(n-2). */
    uInt fibonacci(const uInt& N, const bool& startFrom0 = false);

    bool isPrime(const uInt& A);

    uInt prime(const uInt& A);

    uInt permutation(const uInt& n, const uInt& k);

    uInt combination(const uInt& n, const uInt& k);

#ifdef REAL_COMPLETED_
    Real exp(Real a);

    Real ln(Real a);

    static const Real PI("3.1415926535797932384626");

    Real sin(Real theta);

    Real cos(Real theta);
    
    Real tan(Real theta);

    Real arcsin(Real a);
#endif // real completed

//} // namespace math

#endif