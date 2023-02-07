#include "bigMath.h"
#include <time.h>
#include <fstream>
using namespace std;

int main()
{
#ifdef TEST_ONLY_
	// print the first 30 factorials
	for (unsigned n = 1; n <= 30; ++n)
		cout << n << "! = " << factorial(n) << endl;

	// print the first 30 fibonacci
	for (unsigned n = 1; n <= 30; ++n)
		cout << fibonacci(n) << ' ';
	cout << endl;

	// print big factorial
	for (size_t i = 10000; i <= 50000; i += 10000)
	{
		clock_t tic = clock();
		cout << factorial(i).sciNote() << endl
			 << (double)(clock() - tic) / CLOCKS_PER_SEC << "s" << endl;
	}

	// print big fibonacci
	for (unsigned n = 10'000; n <= 100'000; n += 10'000)
		cout << n << ": " << fibonacci(n).sciNote(20) << endl;
	
	// print big combination
	for (unsigned i = 0; i <= 100; ++i)
		cout << i << "\t: " << combination(200, i) << endl;

	// example in readme
	uInt b("1234567891011121314151617181920");
	b /= uInt("1145141919810"); // 1078091605637804892
	uInt g = b ^ 3;

	// divide test
	uInt Y("1234567891011121314151617181920");
	cout << Y / uInt("1145141919810") << endl // 1078091605637804892
		 << (uInt("123456789101112131415161718") / 114514191) << endl
		 << (uInt("123456789101112131415161718") / 114514191) * 99999999 << endl;

	// 10-base and 16-base restore length compare
	cout << factorial(10000).length() / 9 << endl
		 << factorial(10000).length(16) / 8 << endl;

	// 2^(1e6) calculating time consume
	clock_t tic = clock();
	uInt h = exp2(1000000);
	cout << h.sciNote() << endl << "traditional 2^(1e6) time cost: "
		 << (double)(clock() - tic) / CLOCKS_PER_SEC << "s" << endl;

	// big number division, really slow
	uInt A = fibonacci(100000);
	cout << A.sciNote() << endl;
	for (unsigned i = 100; i <= 1000; i += 10)
	{
		uInt a = fibonacci(i), b = A / a;
		// uInt c = b * a;
		cout << i << " " << flush;
	}
	// output 1000! as in example
	ofstream out;
	out.open("1000!.txt");
	out << "1000! = " << endl
		<< factorial(1000).toString() << endl;

	// test the accuracy of maxExp2
	for (unsigned i = 100; i <= 1000; i += 10)
		if (!maxExp2(fibonacci(i)).between(maxExp2(fibonacci(i)), maxExp2(fibonacci(i)) * 2))
			cout << maxExp2(fibonacci(i)) << " " << log2(fibonacci(i)) << endl
				 << maxExp2(fibonacci(i)) << endl
				 << maxExp2(fibonacci(i)) * 2 << endl << endl;
	// << (double)(clock() - tic) / CLOCKS_PER_SEC << "s" << endl;

	// test the accuracy of sqrt
	for (size_t i = 1; i <= 100; i += 1) {
		uInt c = factorial(i);
		uInt d = ~c; //
		if (!c.between(d ^ 2, (d + 1) ^ 2))
			cout << (d ^ 2) << endl 
				 << c << endl
				 << ((d + 1) ^ 2) << endl;
	}

	// valgrind --tool=memcheck ./w
	// Invalid write of size 4
	uInt c("12345678910121314151617181920212223");
	c <<= 100;
	cout << c << endl << (c << 100) << endl;
	// step by step
	for (unsigned i = 0; i < 100; i += 1)
		cout << i << " <<:\t" << (c << i) << endl;
#endif // test only

	
	return 0;
}