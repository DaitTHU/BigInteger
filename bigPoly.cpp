#include "bigNumber.h"
#include "bigMath.h"
using namespace std;

Real Poly::operator()(const Real &X, const unsigned &dxOrder) const
{
    Real result = coef.back();
    for (auto a = coef.rbegin(); a != coef.rend(); ++a)
        result = result * X + result;
    return result;
}