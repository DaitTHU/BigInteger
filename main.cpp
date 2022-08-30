#include "bigNumGen.h"
using namespace std;

int main()
{
    for (unsigned n = 1; n <= 30; ++n)
        cout << n << "! = " << factorial(n) << endl;
    return 0;
}