#include "bigMath.h"
using namespace std;

uInt exp2(const uInt &N)
{
    static const uint32_t exp_2[] = {
        1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384,
        32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304,
        8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536'870'912};
    if (N < 30)
        return uInt(exp_2[uint32_t(N)]);
    uInt expo = uInt(exp_2[29]) * 2;
    unsigned power = 30;
    for (; power * 2 <= N; power *= 2)
        expo *= expo;
    for (; power + 29 <= N; power += 29)
        expo *= exp_2[29];
    return power == N ? expo : expo * exp_2[uint32_t(N) - power];
}

uInt exp10(const uInt &N)
{
    static const uint32_t exp_10[] = {1, 10, 100, 1'000, 10'000, 100'000,
                                  1'000'000, 10'000'000, 100'000'000};
    auto section = static_cast<pair<uint32_t, uint32_t>>(N.divmod(9)); // uInt::LEN
    vector<uint32_t> expo(section.first + 1, 0);
    expo.back() = exp_10[section.second];
    return uInt(expo);
}

uInt factorial(const uInt &N)
{
    uInt result = 1;
    for (unsigned i = 2; i <= N; ++i)
        result *= i;
    return result;
}

uInt fibonacci(const uInt &N, const bool &startFrom0)
{
    static const uInt Fibon[] = {
        0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597,
        2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811,
        514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817,
        39088169, 63245986, 102334155, 165580141, 267914296, 433494437, 701408733};
    uint32_t n = static_cast<uint32_t>(N) + startFrom0;
    if (n < 45)
        return Fibon[n];
    uInt a = fibonacci(n / 2);
    if (n % 2)
        return (fibonacci(n / 2 + 1) ^ 2) + (a ^ 2); // f(2k+1) = f(k+1)^2 + f(k)^2
    else
        return a * (a + 2 * fibonacci(n / 2 - 1)); // f(2k) = f(k) * (f(k) + 2f(k-1))
}

uInt prime(const uInt &N)
{
    static const unsigned short Prime[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
        71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139,
        149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
        227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
        307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383,
        389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
        467, 479, 487, 491, 499, 503, 509, 521, 523, 541};
    if (N <= 101)
        return uInt(Prime[uint32_t(N) - 1]);
    return 541;
}

uInt permutation(const uInt &N, const uInt &k)
{
    if (k == 0)
        return uInt(1);
    else if (k > N)
        return uInt(0);
    uInt result = N;
    for (unsigned i = 1; i < k; ++i)
        result *= N - i;
    return result;
}

uInt combination(const uInt &N, const uInt &k)
{
    if (k == 0)
        return uInt(1);
    else if (k > N)
        return uInt(0);
    else if (k * 2 > N)
        return combination(N, N - k);
    uInt Ank = permutation(N, k);
    if (k <= 12)
        return Ank / factorial(uint32_t(k));
    for (unsigned i = uint32_t(k); i > 12; --i)
        Ank /= i;
    return Ank / 479001600; // 12!
}
