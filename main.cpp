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

	// uInt b("1234567891011121314151617181920");

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
	uInt c(999'999'999);
	cout << b << endl;
	// b /= 0;
	// b %= 0;
	// b.divmod(0);
	// for (unsigned i = 1; i <= 100; i += 1)
	// {
	// 	uInt a = fibonacci(i);
	// 	uInt c = (b / a) * a;
	// 	if (!c.between(b - a, b, false, true))
	// 		cout << i << ' ' << (c <= b - 2*a) << endl;
	// }

	// uInt a = fibonacci(45);
	// uInt c = (b >> 1).divmod(unit(a >> 1)).first;
	// uInt d = (c * static_cast<uInt>(1'000'000'000 - 1)).divmod(1'000'000'000).first;
	// uInt e = (b / a);
	// cout << c.sciNote() << endl
	// 	 << (c - d).sciNote() << endl
	// 	 << (c - e);
	// cout << a.sciNote() << endl
	// 	 << (b - c).sciNote();
	// cout << endl;
	return 0;
}