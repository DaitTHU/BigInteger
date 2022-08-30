# Big-Number in `C++`

Have you ever imaged how to calculate `1000!` in `C++`? Well, actually

$$1000! = 4.0238726 \times 10^{2567},$$

so it absolutely exceeds the limit of `int` even `unsigned long long` (which is merely `1e19`). But, luckily, `C++` provide us with `array`, so it's possible to combine multiple `int` to expand the boundaries of the representable range. Comparing to ordinary `C array`, the `stl vector` is obviously more convenient, and that's how I realize the Big-Number in `C++`.

## details in `myNumber.h`

In `myNumber.h`, I construct couples of classes: `uInt`, `Int`, `Fraction`, `Real` and `Complex`, and overload operators to fit the original operating habits.

### `uInt` class

You can create a `uInt` object by `int`, `char*` and `string`:

```cpp
uInt a = 65472;
uInt b = "12345678910111213";
uInt c = b;
std::cin >> c;
```

`uInt` calculation is intuitive:

```cpp
uInt d = a + b;
d -= 100;
uInt e = c * d;
e /= 114514;
uInt f = e^4; // exponentiation
std::cout << f << std::endl;
```

`uInt::toString()` returns the number in `string` format.

`log2(uInt A)` returns `floor(log2(A))`.

`exp2fit(uInt A)` returns the max `2^n` that `<= A`.

`digit(uInt A)` returns the number of digit of `A`.

more functions to be continued...

### `Int` class

almost same as `uInt` class, adding a sign.

### `Fraction` class

consisting of `Int` numerator and `uInt` denominator, also, you can create a `Fraction` object by `int`, `double`, `string`...

(quiet like `python Fraction package`...)

### `Real` class

consisting of `Int` as the integer part and `vector<int>` as the decimal part.

### `Complex` class

consisting of two `Real` as the real part and imaginary part.

## details in `bigNumGen.h`

this `.h` has some functions to create numbers that extremely buge, such as `1000!`, 100000th Fibonacci numbers.

## example

With these `.h`s you can easily see how big `1000!` is:

```cpp
#include "bigNumGen.h"
using namespace std;

cout << factorial(1000) << endl;

>>> 402,387260077,093773543,...,000000000 // I oimt 2538 digits!
```

## advantages

I've saw many big-integer codes, most of them use `string`, which is low of effeciency actually, here I make use of `uint32_t` and `unit64_t`, and which I think it closer to the original calculate-model.

## disadvantages

Multiplication and division are inefficient, I know, and most importantly, `python` naturally use arbitrary length integer!

I still have a lot of unfinished work left. I'll finish this one day.