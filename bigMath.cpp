#include "bigMath.h"
using namespace std;

uInt exp2(const uInt &A)
{
    static const unit exp_2[] = {
        1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384,
        32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304,
        8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912};
    if (A < 30)
        return uInt(exp_2[unit(A)]);
    uInt expo = uInt(exp_2[29]) * 2;
    uInt power = 30;
    while (true)
    {
        uInt power2 = power * 2;
        if (power2 > A)
            break;
        expo *= expo; // quick
        power = move(power2);
    }
    for (int i = 29; i > 0; --i)
        while (power + i <= A)
        {
            expo *= exp_2[i];
            power += i;
        }
    return expo;
}

uInt exp2fit(const uInt &A) { return A.approxPo2().first; }

uInt log2(const uInt &A) { return A.approxPo2().second; }

uInt factorial(unsigned n)
{
    uInt result = 1;
    for (unsigned i = 2; i <= n; ++i)
        result *= i;
    return result;
}

uInt fibonacci(unsigned n, bool startFrom0)
{
    if (startFrom0)
        ++n;
    if (n < 2)
        return uInt(n);
    uInt a = fibonacci(n / 2);
    uInt b = fibonacci(n / 2 - 1);
    if (n % 2)
    {
        b += a;
        return b * b + a * a; // f(2k+1) = f(k+1)^2 + f(k)^2
    }
    return a * (a + 2 * b); // f(2k) = f(k) * (f(k) + 2f(k-1))
}

