# Big-Number in `C++`

Have you ever imaged how to calculate `1000!` in `C++`? Well, actually

$$1000! = 4.0238726 \times 10^{2567},$$

so it absolutely exceeds the limit of `int`, even `unsigned long long`, which is merely `1e20`. But luckily, `C++` provide us with `array`, so it's possible to combine multiple `int`s to expand the boundaries of the representable range. Comparing to ordinary `C array`, the `stl vector` is obviously more convenient, and that's how I realize the Big-Number in `C++`.

## `bigNumber.h`

In `bigNumber.h`, I construct couples of classes: `uInt`, `Int`, `Frac`, `Real`, `Complex`, `Inf` and `Poly` and overload operators to fit the custom operating-habits.

### `uInt` class

1. **Construction & Assignment:** You can create a `uInt` object from `uint32_t` or `string`:

```cpp
uInt a = 65472;
uInt A = 12345678901234567890ULL; // support unsigned long long
uInt b("1234567891011121314151617181920");
uInt c = b, d;
std::cout << "You can also input the number you want: ";
std::cin >> d;
```

⚠️ **WARNING:** Directly assign `uInt` with a <u>negative</u> number will convert it to `uint64_t` before taken into the constructor.

```cpp
uInt largeNegative = -100; // 18446744073709551516
```
* `string` can only consist of numbers and leading signs (which will be omitted).

```cpp
uInt positiveInt = std::string("-2718"); // 2718
uInt hilariousDemo("-+-+-+-1414213562"); // 1414213562
// uInt errorDemo0 = "2275191666"        // string != const char*
// uInt errorDemo1("114514+1919810")     // Constructors don't do math
// uInt errorDemo2("0xABCDEF")           // only base-10 readable
```

2. **Comparison:** `uInt`s are comparable (`<, >, ==,...`). Moreover, If you perfer `a < x < b` like Python, you can use `uInt::between(A, B, includeA = true, includeB = false)`.

```cpp
std::cout << "d > 10000? " << (d > 10000) << std::endl;
std::cout << "a <= d < b? " << d.between(a, b) << std::endl;
```

3. **Calculation:** You can do the calculation between `uInt`s or between `uInt` and `uint32_t`.
```cpp
uInt e = a + b; // 1234567891011121314151617247392
a -= 65430;
uInt f = c * d;
b /= uInt("1145141919810"); // 1078091605637804892
```

* `^, >>, <<, ~, !` are redesigned for special purposes. If you'd like the original bit-op, you could use `Bin`.

```cpp
// ^: exponent, e.g. 2^4 = 16.
uInt g = b ^ 3;
std::cout << g << std::endl; 
>>> 1,253045939,477188663,853722164,624612006,875048962,971300288
// >>, <<: shift in base-10
std::cout << (g >> 43) << std::endl;
>>> 125,304593947
// ~: square root, for ~g is a bit similar to √g
std::cout << ~g << std::endl;
>>> 1,119395345,477722032,435959354
// !: factorial
std::cout << !a << std::endl; // 42!
1405006,117752879,898543142,606244511,569936384,000000000
```

⚠️ **WARNING 1:** Directly operating `uInt` with `string` is strongly discouraged. It's **NOT** JavaScript.

```cpp
std::cout << f + "314159265358" << std::endl;       // this is evil.
std::cout << f + uInt("314159265358") << std::endl; // use type-conversion at least
```

⚠️ **WARNING 2:** The `^` operator has lower precedence than `+-`, even `==`. What's worse, unlike math, `^` combines left. So please use `()` when doing mixed operations involving power `^`.

```cpp
f = 2 + uInt(3) ^ 4;   // = 5 ^ 4 = 625
f = 2 + (uInt(3) ^ 4); // = 2 + 81 = 83
f = 3 ^ uInt(3) ^ 3;   // = 27 ^ 3 = 19683
```

As compensation, `g(n, 0)` is prvided to calculate `g^n` while `g(n, 1)` for the nth root of `g`, which is of the first precedence.

```cpp
uInt h = g(40, 1);     // 40th root of g = 22
```

4. **Member Function:** 

```cpp
// uInt::toString(base = 10, suffix = false)
// returns the number in `string` format.
std::cout << g.toString(16, true) << std::endl; 
>>> D1518D2D6F87B20C2805158F9BCEB1475AF4AF9D271C0(16)

// uInt::length(base = 10)
// returns the number of digit of `A`.
std::cout << g.length() << std::endl;
>>> 55

// uInt::sciNote(deciLength = 9)
// returns the number in scientific notation format.
std::cout << g.sciNote() << std::endl; 
>>> 1.253045939 x 10^54

// uInt::setDelimiter(delimiter = ',', interval = 9)
// set the delimiter notation and the interval between.
uInt::setDelimiter(' ', 3);
std::cout << b << std::endl; 
>>> 1 078 091 605 637 804 892

// uInt::divmod(A)
// returns (quotient, remainder) pair
```
* by the way, `sciNote(-1)` shows all the decimal digits. (because `size_t(-1)` is the largest `size_t`)

more functions to be continued...

### `Int` class (TODO)

almost same as `uInt` class, adding a sign.

### `Frac` class (TODO)

Fraction, consisting of `Int` numerator and `uInt` denominator, also, you can create a `Frac` object by `int`, `double`, `string`...

(quiet like `Fraction package` in `Python`, lol...)

### `Real` class (TODO)

consisting of `Int` as the integer part and `vector<int>` as the decimal part.

### `Complex` class (TODO)

consisting of two `Real` as the real part and imaginary part.

### `Inf` class

Infinity, consisiting of sign and order. Numbers could compare or operator with `Inf`. Const `INF` is provided.

```cpp
b < INF; // false
c + INF; // INF
```

### `Poly` class (TODO)

Polynomial, consisting of multiple `Real` coefficients.

## `bigMath.h`

this `.h` has some math functions.

```cpp
// exp2(A) returns 2^A, much faster than (2 ^ A)
std::cout << exp2(114514).sciNote() << std::endl;
>>> 1.409040464 x 10^34472

// log2(A) returns the max `n` that `2^n <= A`.
std::cout << log2(g) << std::endl;
>>> 179

// exp2fit(A) returns the max `2^n` that `<= A`.
std::cout << exp2fit(g) << std::endl;
>>> 766247770,432944429,179173513,575154591,809369561,091801088
```


you can alse create numbers that extremely buge, such as `1000!`, 100000th Fibonacci numbers.

## example

With these `.h`s you can easily see how big `1000!` is:

```cpp
#include "bigMath.h"
std::cout << factorial(1000) << std::endl;
>>> 402,387260077,093773543,702433923,... // see 1000!.txt
```

## advantages

I've saw many big-integer codes, most of them use `string`, which is low of effeciency actually, here I make use of `uint32_t` and `unit64_t`, and which I think it closer to the original calculate-model.

## disadvantages

Multiplication and division are inefficient, I know, and most importantly, `Python` naturally use arbitrary length integer!

I still have a lot of unfinished work left. I'll finish this one day.