#include "bigNumber.h"
using namespace std;

Inf &Inf::operator+=(const Inf &_inf)
{
    if (order < _inf.order)
        return *this = _inf;
    else if (p != _inf.p && order == _inf.order)
        cerr << "ERROR: The difference between infinity of the same order may be an arbitrarily finite.";
    return *this;
}

Inf &Inf::operator*=(const Inf &_inf)
{
    order += _inf.order;
    if (order == 0.0)
        cerr << "ERROR: The quotient between infinity of the same order may be an arbitrarily finite.";
    p = (p == _inf.p);
    return *this;
}
