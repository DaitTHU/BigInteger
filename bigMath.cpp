#include "bigMath.h"
using namespace std;

uInt exp2(const uInt &A)
{
    static const unit exp_2[] = {
        1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384,
        32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304,
        8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536'870'912};
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

uInt exp10(const uInt &A)
{
    static const unit exp_10[] = {1, 10, 100, 1'000, 10'000, 100'000,
                                  1'000'000, 10'000'000, 100'000'000};
    auto section = A.divmod(9); // uInt::LEN
    vector<unit> expo(twin(section.first) + 1, 0);
    expo.back() = exp_10[unit(section.second)];
    return uInt(expo);
}

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

uInt prime(const uInt &A)
{
    static const unsigned short Prime[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
        71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139,
        149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
        227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
        307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383,
        389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
        467, 479, 487, 491, 499, 503, 509, 521, 523, 541};
    if (A <= 101)
        return uInt(Prime[unit(A) - 1]);
    return 541;
}