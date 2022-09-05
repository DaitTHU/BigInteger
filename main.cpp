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
	*/

	// uInt Y("1234567891011121314151617181920");
	// cout << uInt("1078091605637804892") << endl
	// 	 << Y / uInt("1145141919810") << endl
	// 	 << (uInt("123456789101112131415161718") / 114514191) << endl
	// 	 << (uInt("123456789101112131415161718") / 114514191) * 99999999 << endl;

	// cout << factorial(5000).sciNote() << endl
	//	 << factorial(5000).approxExp2().first.sciNote() << endl;

	/*
	cout << factorial(10000).length() / 9 << endl
		 << factorial(10000).toString(16).length() / 8 << endl;
	clock_t tic = clock();
	uInt h = log2(114514);
	cout << h.sciNote() << endl << "traditional 2^(1e6) time cost: "
		 << (double)(clock() - tic) / CLOCKS_PER_SEC << "s" << endl;
	*/
	// for (unsigned n = 10'000; n <= 100'000; n += 10'000)
	//	cout << n << ": " << fibonacci(n).sciNote(20) << endl;

	// for (unsigned i = 0; i <= 100; ++i)
	// 	cout << i << "\t: " << combination(200, i) << endl;

	uInt b = fibonacci(100);
	/*
	for (unsigned i = 1; i <= 100; i += 1)
	{
		uInt a = fibonacci(i);
		uInt c = (b / a) * a;
		if (!c.between(b - a, b, false, true))
			cout << i << ' ' << (c <= b - 2 * a) << endl;
	}
	uInt a = fibonacci(62);
	cout << b << " / " << a << endl;
	cout << (b >> 4) << " / " << (a >> 4) << endl
		 << (b >> 4) / (a >> 4) << endl;
	*/
	uInt::setDelimiter(',', 0);
	uInt c("12345678910121314151617181920212223");
	// for (unsigned i = 0; i < 100; i += 1)
	//	cout << i << " <<:\t" << (c << i) << endl;
	cout << (c << 1000).sciNote(50) << endl;
	return 0;
}