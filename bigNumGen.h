#ifndef BIG_NUM_GEN_H_
#define BIG_NUM_GEN_H_
#include "myNumber.h"

/** @return n! */
uInt factorial(unsigned n)
{
    uInt result = 1;
    for (unsigned i = 2; i <= n; ++i)
        result *= i;
    return result;
}

/** @brief calculate the nth fibonacci number: f(0) = f(1) = 1, f(n) = f(n-1) + f(n-2). */
uInt fibonacci(unsigned n, bool startFrom1 = false)
{
    if (startFrom1)
        assert(n > 0), --n;
    if (n == 0 || n == 1)
        return uInt(1);
    uInt A(1), B(1);
    unsigned i;
    for (i = 1; i < n; i += 2)
        B += A, A += B;
    return i > n ? B : A;
}
#endif // big-number generator