# Big-Number in `C++`

Have you ever imaged how to calculate `1000!` in `C++`? Well, actually

$$1000! = 4.0238726 \times 10^{2567},$$

so it absolutely exceeds the limit of `int` even `unsigned long long` (which is merely `1e20`). But, luckily, `C++` provide us with `array`, so it's possible to combine multiple `int` to expand the boundaries of the representable range. Comparing to ordinary `C array`, the `stl vector` is obviously more convenient, and that's how I realize the Big-Number in `C++`.

## `bigNumber.h`

In `bigNumber.h`, I construct couples of classes: `uInt`, `Int`, `Fraction`, `Real` and `Complex`, and overload operators to fit the original operating habits.

### 1. `uInt` class

You can create a `uInt` object by `int`, `char*` and `string`:

```cpp
uInt a = 65472;
uInt A = 12345678901234567890ULL; // support unsigned long long
uInt b("1234567891011121314151617181920");
// uInt b = "1234567891011121314151617181920"; // error: can't tell char* from int
uInt c = b, d;
std::cout << "You can also input the number you want: ";
std::cin >> d;
```

⚠️ **WARNING:** ⚠️ If you directly assign `uInt` with a negative number, it'll convert to `unsigned long long` before taken into the constructor.

```cpp
uInt largeNegative = -100; // 18446744073709551516
```

For `string`-constructor, `strings` can only consist of numbers and leading signs (which will be omitted).

```cpp
uInt positiveInt("-2718281828");         // 2718281828
uInt hilariousDemo("-+-+-+-1414213562"); // 1414213562
// uInt errorDemo1("114514+1919810")     // Constructors don't do math
// uInt errorDemo2("0xABCDEF")           // only base-10 readable
```

`uInt`s are comparable. If you perfer `a < x < b` like Python, you can use `uInt::between`!

```cpp
std::cout << "d > 10000? " << (d > 10000) << std::endl;
std::cout << "a <= d < b? " << d.between(a, b) << std::endl;
```

calculate is trivial.

```cpp
uInt e = a + b; // 1234567891011121314151617247392
e -= 100;
uInt f = c * d;
b /= uInt("1145141919810"); // 1078091605637804892 (TODO)
uInt g = b ^ 3;             // exponent, e.g. 2^4 = 16.
std::cout << g << std::endl; 
>>> 1,253045939,477188663,853722164,624612006,875048962,971300288
std::cout << (g >> 43) << std::endl; // shift for base-10
>>> 125,304593947
```
⚠️ **WARNING 1:** ⚠️ Directly operating `uInt` with `string` is strongly discouraged. ~~It's **NOT** JavaScript.~~

```cpp
std::cout << f + "314159265358" << std::endl;       // this is evil.
std::cout << f + uInt("314159265358") << std::endl; // use type-conversion at least
```

⚠️ **WARNING 2:** ⚠️ The `^` operator has lower precedence than `+-`, even `==`. What's worse, unlike math, `^` combines left. So please use `()` when doing mixed operations involving power `^`. (Not elegant! 😫)

```cpp
f = 2 + uInt(3) ^ 4.   // = 5 ^ 4 = 625
f = 2 + (uInt(3) ^ 4). // = 2 + 81 = 83
f = 3 ^ uInt(3) ^ 3.   // = 27 ^ 3 = 19683
```

and many related functions are provided

```cpp
// uInt::toString(base = 10, suffix = false) returns the number in `string` format.
std::cout << g.toString(16, true) << std::endl; 
>>> D1518D2D6F87B20C2805158F9BCEB1475AF4AF9D271C0(16)

// uInt::sciNote(deciLength = 9) returns the number in scientific notation format.
std::cout << g.sciNote() << std::endl; 
>>> 1.253045939 x 10^54

// uInt::length(base = 10) returns the number of digit of `A`.
std::cout << g.length() << std::endl;
>>> 55
```
by the way, `sciNote(-1)` shows all the decimal digits. (cause `unsigned(-1) = 4294967295`)

more functions to be continued...

### 2. `Int` class (TODO)

almost same as `uInt` class, adding a sign.

### 3. `Frac` class (TODO)

Fraction, consisting of `Int` numerator and `uInt` denominator, also, you can create a `Frac` object by `int`, `double`, `string`...

(quiet like `Fraction package` in `Python`, lol...)

### 4. `Real` class (TODO)

consisting of `Int` as the integer part and `vector<int>` as the decimal part.

### 5. `Complex` class (TODO)

consisting of two `Real` as the real part and imaginary part.

### 6. `Poly` class (TODO)

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
std::cout << factorial(1000).sciNote(50) << std::endl;
>>> 4.02387260077093773543702433923003985719374864210714 x 10^2567
```

## advantages

I've saw many big-integer codes, most of them use `string`, which is low of effeciency actually, here I make use of `uint32_t` and `unit64_t`, and which I think it closer to the original calculate-model.

## disadvantages

Multiplication and division are inefficient, I know, and most importantly, `Python` naturally use arbitrary length integer!

I still have a lot of unfinished work left. I'll finish this one day.