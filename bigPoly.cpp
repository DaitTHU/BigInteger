#include "bigNumber.h"
#include "bigMath.h"
using namespace std;

template <typename F>
F Poly<F>::operator()(const F &x, const unsigned &dOrder) const
{
    Real result = coef.back();
    for (auto a = coef.rbegin(); a != coef.rend(); ++a)
        result = result * x + result;
    return result;
}