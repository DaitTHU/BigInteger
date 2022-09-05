#include "bigMath.h"
#include <time.h>
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

	uInt Y("1234567891011121314151617181920");
	cout << uInt("1078091605637804892") << endl
		 << Y / uInt("1145141919810") << endl
		 << (uInt("123456789101112131415161718") / 114514191) << endl
		 << (uInt("123456789101112131415161718") / 114514191) * 99999999 << endl;

	cout << factorial(10000).length() / 9 << endl
		 << factorial(10000).toString(16).length() / 8 << endl;
	clock_t tic = clock();
	uInt h = log2(114514);
	cout << h.sciNote() << endl << "traditional 2^(1e6) time cost: "
		 << (double)(clock() - tic) / CLOCKS_PER_SEC << "s" << endl;

	for (unsigned n = 10'000; n <= 100'000; n += 10'000)
		cout << n << ": " << fibonacci(n).sciNote(20) << endl;

	for (unsigned i = 0; i <= 100; ++i)
		cout << i << "\t: " << combination(200, i) << endl;
	*/

	/*
	uInt A = fibonacci(100000);
	cout << A.sciNote() << endl;

	for (unsigned i = 100; i <= 1000; i += 10)
	{
		uInt a = fibonacci(i), b = A / a;
		// uInt c = b * a;
		cout << i << " " << flush;
	}

	uInt a = fibonacci(62);

	 */

	uInt b("1234567891011121314151617181920");
	b /= uInt("1145141919810");
	uInt g = b ^ 3;
	cout << g(40, 1) << endl;

	/*
	for (unsigned i = 0; i < 100; i += 1)
		cout << i << " <<:\t" << (c << i) << endl;
	*/
	return 0;
}