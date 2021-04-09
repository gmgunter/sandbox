# Header file `timedelta.hpp`

``` cpp
namespace caesar
{
    class TimeDelta;

    constexpr caesar::TimeDelta abs(caesar::TimeDelta const& dt);

    constexpr caesar::TimeDelta trunc(caesar::TimeDelta const& dt, caesar::TimeDelta const& period);

    constexpr caesar::TimeDelta floor(caesar::TimeDelta const& dt, caesar::TimeDelta const& period);

    constexpr caesar::TimeDelta ceil(caesar::TimeDelta const& dt, caesar::TimeDelta const& period);

    constexpr caesar::TimeDelta round(caesar::TimeDelta const& dt, caesar::TimeDelta const& period);
}
```

### Class `caesar::TimeDelta`

``` cpp
class TimeDelta
{
public:
    using Rep = absl::int128;

    using Period = std::pico;

    using Duration = std::chrono::duration<Rep, Period>;

    TimeDelta() = default;

    template <class R, class P>
    constexpr TimeDelta(std::chrono::duration<R, P> const& d);

    template <class R, class P>
    explicit operator std::chrono::duration<R, P>() const;

    static constexpr caesar::TimeDelta min() noexcept;

    static constexpr caesar::TimeDelta max() noexcept;

    static constexpr caesar::TimeDelta resolution() noexcept;

    //=== Unit-specific factory methods ===//
    template <typename T>
    static constexpr caesar::TimeDelta days(T d);
    template <typename T>
    static constexpr caesar::TimeDelta hours(T h);
    template <typename T>
    static constexpr caesar::TimeDelta minutes(T m);
    template <typename T>
    static constexpr caesar::TimeDelta seconds(T s);
    template <typename T>
    static constexpr caesar::TimeDelta milliseconds(T ms);
    template <typename T>
    static constexpr caesar::TimeDelta microseconds(T us);
    template <typename T>
    static constexpr caesar::TimeDelta nanoseconds(T ns);
    template <typename T>
    static constexpr caesar::TimeDelta picoseconds(T ps);

    constexpr Rep count() const noexcept;

    constexpr double total_seconds() const;

    //=== Unary arithmetic operators ===//
    constexpr caesar::TimeDelta operator+() const noexcept;
    constexpr caesar::TimeDelta operator-() const;

    //=== Increment/decrement operators ===//
    constexpr caesar::TimeDelta& operator++();
    constexpr caesar::TimeDelta operator++(int);
    constexpr caesar::TimeDelta& operator--();
    constexpr caesar::TimeDelta operator--(int);

    //=== Compound assignment operators ===//
    constexpr caesar::TimeDelta& operator+=(caesar::TimeDelta const& other);
    constexpr caesar::TimeDelta& operator-=(caesar::TimeDelta const& other);
    template <typename T>
    constexpr caesar::TimeDelta& operator*=(T mul);
    template <typename T>
    constexpr caesar::TimeDelta& operator/=(T div);
    constexpr caesar::TimeDelta& operator%=(caesar::TimeDelta::Rep mod);
    constexpr caesar::TimeDelta& operator%=(caesar::TimeDelta const& other);

    //=== Binary arithmetic operators ===//
    friend constexpr caesar::TimeDelta operator+(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs);
    friend constexpr caesar::TimeDelta operator-(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs);
    template <typename T>
    friend constexpr caesar::TimeDelta operator*(caesar::TimeDelta const& lhs, T mul);
    template <typename T>
    friend constexpr caesar::TimeDelta operator*(T mul, caesar::TimeDelta const& rhs);
    template <typename T>
    friend constexpr caesar::TimeDelta operator/(caesar::TimeDelta const& lhs, T div);
    friend constexpr caesar::TimeDelta operator%(caesar::TimeDelta const& lhs, caesar::TimeDelta::Rep mod);
    friend constexpr caesar::TimeDelta operator%(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs);

    //=== Comparison operators ===//
    friend constexpr bool operator==(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;
    friend constexpr bool operator!=(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;
    friend constexpr bool operator<(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;
    friend constexpr bool operator>(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;
    friend constexpr bool operator<=(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;
    friend constexpr bool operator>=(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;

    friend std::ostream& operator<<(std::ostream& os, caesar::TimeDelta const& dt);
};
```

A signed duration between two time points, with picosecond resolution

TimeDelta describes a span of time using a unit-safe, fixed-point representation. Internally, a TimeDelta stores a 128-bit integer tick count of picoseconds, allowing it to represent a wide range of values without overflow or loss of precision. It supports all of the natural integer-like arithmetic operations.

TimeDelta objects can be created using unit-specific static factory methods.

``` cpp
// A TimeDelta representing approximately 1.5 days
auto dt1 = TimeDelta::days(1.5);

// Same as dt1
auto dt2 = 1.5 * TimeDelta::days(1);

// A TimeDelta representing exactly 1.5 seconds
auto dt3 = TimeDelta::seconds(1) + TimeDelta::milliseconds(500);

// Same as dt3
auto dt4 = TimeDelta::seconds(1) + 500 * TimeDelta::milliseconds(1);
```

##### See also

`TimePoint`

### Type alias `caesar::TimeDelta::Rep`

``` cpp
using Rep = absl::int128;
```

A signed integral type representing the number of ticks

-----

### Type alias `caesar::TimeDelta::Period`

``` cpp
using Period = std::pico;
```

A compile-time rational constant representing the tick period (i.e. the number of ticks per second)

-----

### Type alias `caesar::TimeDelta::Duration`

``` cpp
using Duration = std::chrono::duration<Rep, Period>;
```

A std::chrono::duration with the same rep and period

-----

### Constructor `caesar::TimeDelta::TimeDelta`

``` cpp
TimeDelta() = default;
```

Construct a new TimeDelta object representing a zero-length duration.

-----

### Constructor `caesar::TimeDelta::TimeDelta`

``` cpp
template <class R, class P>
constexpr TimeDelta(std::chrono::duration<R, P> const& d);
```

Construct a new TimeDelta object from a [`std::chrono::duration`](http://en.cppreference.com/mwiki/index.php?title=Special%3ASearch&search=std::chrono::duration).

If the input duration has a smaller tick period, the conversion is subject to truncation.

##### Notes

Conversion from a floating-point duration is subject to undefined behavior when the floating-point value is NaN, infinity, or too large to be representable by TimeDelta. Otherwise, casting is subject to truncation as with any `static_cast`.

-----

### Conversion operator `caesar::TimeDelta::operator std::chrono::duration<R,P>`

``` cpp
template <class R, class P>
explicit operator std::chrono::duration<R, P>() const;
```

Convert to a [`std::chrono::duration`](http://en.cppreference.com/mwiki/index.php?title=Special%3ASearch&search=std::chrono::duration).

If the resulting duration has a larger tick period, the conversion is subject to truncation.

##### Notes

Casting is subject to truncation as with any `static_cast`.

-----

### Function `caesar::TimeDelta::min`

``` cpp
static constexpr caesar::TimeDelta min() noexcept;
```

Return the smallest representable TimeDelta.

-----

### Function `caesar::TimeDelta::max`

``` cpp
static constexpr caesar::TimeDelta max() noexcept;
```

Return the largest representable TimeDelta.

-----

### Function `caesar::TimeDelta::resolution`

``` cpp
static constexpr caesar::TimeDelta resolution() noexcept;
```

Return the smallest possible difference between non-equal TimeDelta objects.

-----

### Unit-specific factory methods

``` cpp
(1) template <typename T>
static constexpr caesar::TimeDelta days(T d);

(2) template <typename T>
static constexpr caesar::TimeDelta hours(T h);

(3) template <typename T>
static constexpr caesar::TimeDelta minutes(T m);

(4) template <typename T>
static constexpr caesar::TimeDelta seconds(T s);

(5) template <typename T>
static constexpr caesar::TimeDelta milliseconds(T ms);

(6) template <typename T>
static constexpr caesar::TimeDelta microseconds(T us);

(7) template <typename T>
static constexpr caesar::TimeDelta nanoseconds(T ns);

(8) template <typename T>
static constexpr caesar::TimeDelta picoseconds(T ps);
```

Create a TimeDelta object representing a number of units of time indicated by the method name.

Each factory method corresponds to an SI unit of time. An SI minute always contains exactly 60 seconds. An SI hour always contains exactly 3600 seconds. An SI day always contains exactly 86400 seconds.

##### Template parameters

  - `T` - an arithmetic type

##### Parameters

  - `d` - number of days

  - `h` - number of hours

  - `m` - number of minutes

  - `s` - number of seconds

  - `ms` - number of milliseconds

  - `us` - number of microseconds

  - `ns` - number of nanoseconds

  - `ps` - number of picoseconds

##### Return value

1\) a TimeDelta object representing the specified number of days 2) a TimeDelta object representing the specified number of hours 3) a TimeDelta object representing the specified number of seconds 4) a TimeDelta object representing the specified number of milliseconds 5) a TimeDelta object representing the specified number of microseconds 6) a TimeDelta object representing the specified number of nanoseconds 7) a TimeDelta object representing the specified number of picoseconds

##### Notes

Conversion from a floating-point duration is subject to undefined behavior when the floating-point value is NaN, infinity, or too large to be representable by TimeDelta. Otherwise, casting is subject to truncation as with any `static_cast`.

-----

### Function `caesar::TimeDelta::count`

``` cpp
constexpr Rep count() const noexcept;
```

Return the tick count.

-----

### Function `caesar::TimeDelta::total_seconds`

``` cpp
constexpr double total_seconds() const;
```

Return the total number of seconds in the duration.

-----

### Unary arithmetic operators

``` cpp
(1) constexpr caesar::TimeDelta operator+() const noexcept;

(2) constexpr caesar::TimeDelta operator-() const;
```

Implements the unary plus and unary minus operators.

##### Return value

1\) a copy of the TimeDelta object 2) the negation of the TimeDelta object

-----

### Increment/decrement operators

``` cpp
(1) constexpr caesar::TimeDelta& operator++();

(2) constexpr caesar::TimeDelta operator++(int);

(3) constexpr caesar::TimeDelta& operator--();

(4) constexpr caesar::TimeDelta operator--(int);
```

Increment or decrement the tick count.

1,2) Increments the tick count. 3,4) Decrements the tick count.

##### Return value

1,3) a reference to this TimeDelta after modification 2,4) a copy of this TimeDelta made before modification

-----

### Compound assignment operators

``` cpp
(1) constexpr caesar::TimeDelta& operator+=(caesar::TimeDelta const& other);

(2) constexpr caesar::TimeDelta& operator-=(caesar::TimeDelta const& other);

(3) template <typename T>
constexpr caesar::TimeDelta& operator*=(T mul);

(4) template <typename T>
constexpr caesar::TimeDelta& operator/=(T div);

(5) constexpr caesar::TimeDelta& operator%=(caesar::TimeDelta::Rep mod);

(6) constexpr caesar::TimeDelta& operator%=(caesar::TimeDelta const& other);
```

Implements compound assignment.

Performs an in-place arithmetic operation and returns a reference to the modified result.

1\) Performs in-place addition. 2) Performs in-place subtraction. 3) Performs in-place multiplication. 4) Performs in-place division, truncating toward zero. 5) Performs modulo operation in-place. 6) Performs modulo operation in-place. Equivalent to `*this %= other.count()`.

##### Template parameters

  - `T` - an arithmetic type

##### Parameters

  - `other` - another TimeDelta

  - `mul` - multiplier

  - `div` - divisor

  - `mod` - modulus

##### Return value

a reference to this TimeDelta after modification

##### Notes

4\) Division by zero has undefined behavior. 5,6) The behavior is undefined if the modulus is zero.

-----

### Binary arithmetic operators

``` cpp
(1) friend constexpr caesar::TimeDelta operator+(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs);

(2) friend constexpr caesar::TimeDelta operator-(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs);

(3) template <typename T>
friend constexpr caesar::TimeDelta operator*(caesar::TimeDelta const& lhs, T mul);

(4) template <typename T>
friend constexpr caesar::TimeDelta operator*(T mul, caesar::TimeDelta const& rhs);

(5) template <typename T>
friend constexpr caesar::TimeDelta operator/(caesar::TimeDelta const& lhs, T div);

(6) friend constexpr caesar::TimeDelta operator%(caesar::TimeDelta const& lhs, caesar::TimeDelta::Rep mod);

(7) friend constexpr caesar::TimeDelta operator%(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs);
```

Performs arithmetic operations on the two inputs.

1\) Add two TimeDelta objects. 2) Subtract a TimeDelta object from another. 3,4) Multiply a TimeDelta object by a scalar. 5) Multiply a TimeDelta object by a scalar, truncating toward zero. 6) Compute the remainder after division. 7) Compute the remainder after division. Equivalent to `lhs % rhs.count()`.

##### Template parameters

  - `T` - an arithmetic type

##### Parameters

  - `lhs`, `rhs` - input TimeDelta objects

  - `mul` - multiplier

  - `div` - divisor

  - `mod` - modulus

##### Return value

1\) sum of the inputs 2) difference of the inputs 3,4) product of the inputs 5) quotient of the inputs 6,7) remainder of the inputs

##### Notes

5\) Division by zero has undefined behavior. 6,7) The behavior is undefined if the modulus is zero.

-----

### Comparison operators

``` cpp
(1) friend constexpr bool operator==(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;

(2) friend constexpr bool operator!=(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;

(3) friend constexpr bool operator<(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;

(4) friend constexpr bool operator>(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;

(5) friend constexpr bool operator<=(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;

(6) friend constexpr bool operator>=(caesar::TimeDelta const& lhs, caesar::TimeDelta const& rhs) noexcept;
```

Compare two TimeDelta objects.

-----

-----

### Function `caesar::abs`

``` cpp
constexpr caesar::TimeDelta abs(caesar::TimeDelta const& dt);
```

Return the absolute value of the input TimeDelta.

##### Example

``` cpp
auto dt1 = TimeDelta::seconds(123);
auto dt2 = -dt1;

std::cout << "abs(" << dt1 << ") = " << abs(dt1) << std::endl;
std::cout << "abs(" << dt2 << ") = " << abs(dt2) << std::endl;
```

Possible output:

    abs(123s) = 123s
    abs(-123s) = 123s

-----

### Function `caesar::trunc`

``` cpp
constexpr caesar::TimeDelta trunc(caesar::TimeDelta const& dt, caesar::TimeDelta const& period);
```

Truncate to a multiple of the specified period.

Returns the nearest integer multiple of period not greater in magnitude than dt.

##### Example

``` cpp
auto dt1 = -TimeDelta::minutes(3) - TimeDelta::seconds(30);
auto dt2 = TimeDelta::days(1) - TimeDelta::picoseconds(1);

std::cout << trunc(dt1, TimeDelta::minutes(1)) << std::endl;
std::cout << trunc(dt2, TimeDelta::hours(1)) << std::endl;
```

Possible output:

    -3m
    23h

##### Notes

The behavior is undefined if `period` is zero.

##### See also

\[floor(const TimeDelta&, const TimeDelta&)\] \[ceil(const TimeDelta&, const TimeDelta&)\] \[round(const TimeDelta&, const TimeDelta&)\]

-----

### Function `caesar::floor`

``` cpp
constexpr caesar::TimeDelta floor(caesar::TimeDelta const& dt, caesar::TimeDelta const& period);
```

Round down to a multiple of the specified period.

Returns the nearest integer multiple of period that is less than or equal to dt.

##### Example

``` cpp
auto dt1 = -TimeDelta::minutes(3) - TimeDelta::seconds(30);
auto dt2 = TimeDelta::days(1) - TimeDelta::picoseconds(1);

std::cout << floor(dt1, TimeDelta::minutes(1)) << std::endl;
std::cout << floor(dt2, TimeDelta::hours(1)) << std::endl;
```

Possible output:

    -4m
    23h

##### Notes

The behavior is undefined if `period` is zero.

##### See also

\[trunc(const TimeDelta&, const TimeDelta&)\] \[ceil(const TimeDelta&, const TimeDelta&)\] \[round(const TimeDelta&, const TimeDelta&)\]

-----

### Function `caesar::ceil`

``` cpp
constexpr caesar::TimeDelta ceil(caesar::TimeDelta const& dt, caesar::TimeDelta const& period);
```

Round up to a multiple of the specified period.

Returns the nearest integer multiple of period that is greater than or equal to dt.

##### Example

``` cpp
auto dt1 = -TimeDelta::minutes(3) - TimeDelta::seconds(30);
auto dt2 = TimeDelta::days(1) - TimeDelta::picoseconds(1);

std::cout << ceil(dt1, TimeDelta::minutes(1)) << std::endl;
std::cout << ceil(dt2, TimeDelta::hours(1)) << std::endl;
```

Possible output:

    -3m
    24h

##### Notes

The behavior is undefined if `period` is zero.

##### See also

\[trunc(const TimeDelta&, const TimeDelta&)\] \[floor(const TimeDelta&, const TimeDelta&)\] \[round(const TimeDelta&, const TimeDelta&)\]

-----

### Function `caesar::round`

``` cpp
constexpr caesar::TimeDelta round(caesar::TimeDelta const& dt, caesar::TimeDelta const& period);
```

Round to the nearest multiple of the specified period.

Returns the integer multiple of period that is closest to dt. If there are two such values, returns the one that is an even multiple of period.

##### Example

``` cpp
auto dt1 = -TimeDelta::minutes(3) - TimeDelta::seconds(30);
auto dt2 = TimeDelta::days(1) - TimeDelta::picoseconds(1);

std::cout << round(dt1, TimeDelta::minutes(1)) << std::endl;
std::cout << round(dt2, TimeDelta::hours(1)) << std::endl;
```

Possible output:

    -4m
    24h

##### Notes

The behavior is undefined if `period` is zero.

##### See also

\[trunc(const TimeDelta&, const TimeDelta&)\] \[floor(const TimeDelta&, const TimeDelta&)\] \[ceil(const TimeDelta&, const TimeDelta&)\]

-----
