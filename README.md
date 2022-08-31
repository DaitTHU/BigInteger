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
uInt b("1234567891011121314151617181920"); // only digits allowed
// uInt b = "1234567891011121314151617181920"; // cause error
uInt c = b, d;
std::cout << "You can also input the number you want: ";
std::cin >> d;
```
`uInt`s are comparable. If you perfer `a < x < b` like `python`, you can use `uInt::between`!

```cpp
std::cout << "d > 10000? " << (d > 10000) << std::endl;
std::cout << "a <= d < b? " << d.between(a, b) << std::endl;
```

calculate is trivial.

```cpp
uInt e = a + b; // 1234567891011121314151617247392
e -= 100;       // 1234567891011121314151617247292
uInt f = c * d;
b /= uInt(1145141919810); // 1078091605637804892
uInt g = b^3;             // exponentiation, e.g. 2^4 = 16.
std::cout << g << std::endl; 
// 1,253045939,477188663,853722164,624612006,875048962,971300288
```
and many related functions are provided

```cpp
// uInt::toString(base = 10) returns the number in `string` format.
std::cout << g.toString() << std::endl; 
// 1253045939477188663853722164624612006875048962971300288

// uInt::length(base = 10) returns the number of digit of `A`.
std::cout << g.length() << std::endl;
// 55

// log2(A) returns the max `n` that `2^n <= A`.
std::cout << log2(g) << std::endl;
// 179

// exp2fit(A) returns the max `2^n` that `<= A`.
std::cout << exp2fit(g) << std::endl;
// 766247770,432944429,179173513,575154591,809369561,091801088
```
more functions to be continued...

### `Int` class (unfinished)

almost same as `uInt` class, adding a sign.

### `Fraction` class (unfinished)

consisting of `Int` numerator and `uInt` denominator, also, you can create a `Fraction` object by `int`, `double`, `string`...

(quiet like `python Fraction package`...)

### `Real` class (unfinished)

consisting of `Int` as the integer part and `vector<int>` as the decimal part.

### `Complex` class (unfinished)

consisting of two `Real` as the real part and imaginary part.

### `Polynomial` class (unfinished)

consisting of multiple `Real` coefficients.

## details in `bigNumGen.h` (unfinished)

this `.h` has some functions to create numbers that extremely buge, such as `1000!`, 100000th Fibonacci numbers.

## example

With these `.h`s you can easily see how big `1000!` is:

```cpp
#include "bigNumGen.h"

std::cout << factorial(1000) << std::endl;

>>> 402,387260077,093773543,...,000000000 // I oimt 2538 digits!
```

## advantages

I've saw many big-integer codes, most of them use `string`, which is low of effeciency actually, here I make use of `uint32_t` and `unit64_t`, and which I think it closer to the original calculate-model.

## disadvantages

Multiplication and division are inefficient, I know, and most importantly, `python` naturally use arbitrary length integer!

I still have a lot of unfinished work left. I'll finish this one day.