#include <bigMath.h>
using namespace std;

uInt exp2fit(const uInt &A)
{
    if (A == 0)
        return uInt(0);
    uInt expo = 1;
    while (true)
    {
        uInt doubleExpo = 2 * expo;
        if (doubleExpo > A)
            return expo;
        expo = move(doubleExpo);
    }
}

uInt log2(const uInt &A)
{
    if (A == 0)
        return uInt(0);
    uInt power = 0;
    for (uInt expo = 1; expo <= A; expo *= 2)
        ++power;
    return power - 1;
}



uInt factorial(unsigned n)
{
    uInt result = 1;
    for (unsigned i = 2; i <= n; ++i)
        result *= i;
    return result;
}

uInt fibonacci(unsigned n, bool startFrom1 = false)
{
    if (startFrom1)
        assert(n > 0), --n;
    if (n == 0 || n == 1)
        return uInt(1);
    // f(2k) = f(k) (2f(k+1) - f(k))
    // f(2k+1) = f(k+1)^2 + f(k)^2
    uInt A(1), B(1);
    unsigned i;
    for (i = 1; i < n; i += 2)
        B += A, A += B;
    return i > n ? B : A;
}