#include "bigMath.h"
using namespace std;

int main()
{
	/*
	for (unsigned n = 1; n <= 30; ++n)
		cout << n << "! = " << factorial(n) << endl;

	for (unsigned n = 1; n <= 30; ++n)
		cout << n << ": " << fibonacci(n) << endl;

	uInt b("1078091605637804892");
	uInt g = b ^ 3;
	*/

	uInt b("1234567891011121314151617181920");
	cout << factorial(1000).sciNote(50) << endl;
	return 0;
}