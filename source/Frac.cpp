#include "bigNumber.h"
#include "bigMath.h"
using namespace std;

Frac::Frac(const Int& nume, const uInt& deno) : nume_(nume), deno_(deno)
{
    auto ndGcd = gcd(nume, deno);
    if(ndGcd > 1)
        nume_ /= ndGcd, deno_ /= ndGcd;
}

Frac::Frac(const string& f)
{
    
}