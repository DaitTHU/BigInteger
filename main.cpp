#include "bigMath.h"
#include <time.h>
#include <fstream>
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


	uInt b("1234567891011121314151617181920");
	b /= uInt("1145141919810");
	uInt g = b ^ 3;

	clock_t tic = clock();
	cout << factorial(100).sciNote() << endl
		 << factorial(1000).sciNote() << endl
		 << factorial(10000).sciNote() << endl
		 << (double)(clock() - tic) / CLOCKS_PER_SEC << "s" << endl;

	for (unsigned i = 1; i <= 100; ++i)
		cout << i << ": " << factorial(i).length() / 9 << endl;

	static const double LOG10_[] = {
		0, .30103, .477121, .60206, .69897, .778151, .845098, .90309, .954243};
	static const double LG_2PI = .79817986835811505;
	static const uint32_t EXP10_[] = {
		1, 10, 100, 1'000, 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000, 1'000'000'000};

	for (size_t i = 10000; i <= 50000; i += 10000)
	{
		clock_t tic = clock();
		cout << factorial(i).sciNote() << endl
			 << (double)(clock() - tic) / CLOCKS_PER_SEC << "s" << endl;
	}

	ofstream out;
	out.open("1000!.txt");
	out << "1000! = " << endl
		<< factorial(1000).toString() << endl;

	 // clock_t tic = clock();
	for (unsigned i = 100; i <= 1000; i += 10)
		if (!fibonacci(i).approxExp2().first.between(fibonacci(i).approxExp2().first, fibonacci(i).approxExp2().first * 2))
			cout << fibonacci(i).approxExp2().first << " " << fibonacci(i).approxExp2().second << endl
			<< fibonacci(i).approxExp2().first << endl
			<< fibonacci(i).approxExp2().first * 2 << endl << endl;
	// << (double)(clock() - tic) / CLOCKS_PER_SEC << "s" << endl;
	 */

	cout << fibonacci(100).toString().length() << endl << fibonacci(100).length() << endl;

	/*
	for (unsigned i = 0; i < 100; i += 1)
		cout << i << " <<:\t" << (c << i) << endl;
	*/

	return 0;
}