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
#endif // big-number generator