#include "bigMath.h"
using namespace std;

static const uint32_t FIBON[] = {
    0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584,
    4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229,
    832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817,
    39088169, 63245986, 102334155, 165580141, 267914296, 433494437, 701'408'733 };
static const uint8_t PRIME[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
    79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163,
    167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251 };

tuple<uInt, uInt, uInt> exgcd(const uInt& A, const uInt& B)
{
    if (B == 0)
        return tuple<uInt, uInt, uInt>(1, 0, A);
    auto gcdTuple = exgcd(B, A % B);
    return tuple<uInt, uInt, uInt>(get<1>(gcdTuple),
        get<0>(gcdTuple) - A / B * get<1>(gcdTuple), get<2>(gcdTuple));
}

uInt fibonacci(const uInt& N, const bool& startFrom0)
{
    if (startFrom0)
        return fibonacci(N + 1);
    if (N < 45)
        return static_cast<uInt>(FIBON[static_cast<size_t>(N)]);
    uInt a = fibonacci(N / 2);
    if (2 | N)
        return a * (a + 2 * fibonacci(N / 2 - 1)); // f(2k) = f(k) * (f(k) + 2f(k-1))
    else
        return (fibonacci(N / 2 + 1) ^ 2) + (a ^ 2); // f(2k+1) = f(k+1)^2 + f(k)^2
}

bool isPrime(const uInt& A)
{
    uInt sqrtA = ~A;
    for (auto& p : PRIME)
        if (p > sqrtA)
            return true;
        else if (p | A)
            return false;
    uInt p = 257; // = 5(mod6)
    bool addingFour = false;
    while (p <= sqrtA) { // extremely slow
        if (p | A)
            return false;
        addingFour ? p += 2 : p += 4;
        addingFour = !addingFour;
    }
    return true;
}

uInt prime(const uInt& N)
{
    if (N <= 54)
        return static_cast<uInt>(PRIME[static_cast<uint32_t>(N) - 1]);
    return 541;
}

uInt permutation(const uInt& N, const uInt& k)
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

uInt combination(const uInt& N, const uInt& k)
{
    if (k == 0)
        return uInt(1);
    else if (k > N)
        return uInt(0);
    else if (k * 2 > N)
        return combination(N, N - k);
    uInt Ank = permutation(N, k);
    if (k <= 12)
        return Ank / factorial(k);
    for (unsigned i = uint32_t(k); i > 12; --i)
        Ank /= i;
    return Ank / 479001600; // 12!
}

#ifdef Real_Completed_
Real sin(Real theta)
{
    if (theta > PI)
        do {
            theta -= PI * 2;
        } while (theta <= PI);
    else if (theta <= -PI)
        do {
            theta += PI * 2;
        } while (theta > -PI);
    
}
#endif