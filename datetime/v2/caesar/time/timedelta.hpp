#pragma once

#include <caesar/type_traits.hpp>

#include <absl/numeric/int128.h>
#include <chrono>
#include <ratio>
#include <type_traits>

namespace caesar {

/// A signed duration between two time points, with picosecond resolution
///
/// TimeDelta describes a span of time using a unit-safe, fixed-point
/// representation. Internally, a TimeDelta stores a 128-bit integer tick count
/// of picoseconds, allowing it to represent a wide range of values without
/// overflow or loss of precision. It supports all of the natural integer-like
/// arithmetic operations.
///
/// TimeDelta objects can be created using unit-specific static factory methods.
///
/// ```cpp
/// // A TimeDelta representing approximately 1.5 days
/// auto dt1 = TimeDelta::days(1.5);
///
/// // Same as dt1
/// auto dt2 = 1.5 * TimeDelta::days(1);
///
/// // A TimeDelta representing exactly 1.5 seconds
/// auto dt3 = TimeDelta::seconds(1) + TimeDelta::milliseconds(500);
///
/// // Same as dt3
/// auto dt4 = TimeDelta::seconds(1) + 500 * TimeDelta::milliseconds(1);
/// ```
///
/// ### See also
/// [TimePoint]()
class TimeDelta {
public:
    /// A signed integral type representing the number of ticks
    using Rep = absl::int128;

    /// A compile-time rational constant representing the tick period (i.e. the
    /// number of ticks per second)
    using Period = std::pico;

    /// A std::chrono::duration with the same rep and period
    using Duration = std::chrono::duration<Rep, Period>;

    /// Construct a new TimeDelta object representing a zero-length duration.
    TimeDelta() = default;

    /// Construct a new TimeDelta object from a [std::chrono::duration]().
    ///
    /// If the input duration has a smaller tick period, the conversion is
    /// subject to truncation.
    ///
    /// ### Notes
    /// Conversion from a floating-point duration is subject to undefined
    /// behavior when the floating-point value is NaN, infinity, or too large to
    /// be representable by TimeDelta. Otherwise, casting is subject to
    /// truncation as with any `static_cast`.
    template<class R, class P>
    explicit constexpr TimeDelta(const std::chrono::duration<R, P>& d)
        : duration_([=]() {
              static_assert(caesar::is_arithmetic_v<R>);
              if constexpr (std::is_floating_point_v<R>) {
                  // First, convert units to TimeDelta::Period. Then, cast to
                  // TimeDelta::Rep.
                  using D = std::chrono::duration<R, Period>;
                  const auto p = std::chrono::duration_cast<D>(d);
                  const auto r = static_cast<Rep>(p.count());
                  return Duration(r);
              } else {
                  return std::chrono::duration_cast<Duration>(d);
              }
          }())
    {}

    /// Convert to a [std::chrono::duration]().
    ///
    /// If the resulting duration has a larger tick period, the conversion is
    /// subject to truncation.
    ///
    /// ### Notes
    /// Casting is subject to truncation as with any `static_cast`.
    template<class R, class P>
    [[nodiscard]] explicit constexpr
    operator std::chrono::duration<R, P>() const
    {
        static_assert(caesar::is_arithmetic_v<R>);
        using D = std::chrono::duration<R, P>;
        if constexpr (std::is_floating_point_v<R>) {
            // First, cast to floating-point type. Then, convert units.
            const auto r = static_cast<R>(count());
            const auto p = std::chrono::duration<R, Period>(r);
            return std::chrono::duration_cast<D>(p);
        } else {
            return std::chrono::duration_cast<D>(duration_);
        }
    }

    /// Return the smallest representable TimeDelta.
    [[nodiscard]] static constexpr TimeDelta
    min() noexcept
    {
        return TimeDelta(Duration::min());
    }

    /// Return the largest representable TimeDelta.
    [[nodiscard]] static constexpr TimeDelta
    max() noexcept
    {
        return TimeDelta(Duration::max());
    }

    /// Return the smallest possible difference between non-equal TimeDelta
    /// objects.
    [[nodiscard]] static constexpr TimeDelta
    resolution() noexcept
    {
        return TimeDelta(Duration(1));
    }

    /// \group factory Unit-specific factory methods
    ///
    /// Create a TimeDelta object representing a number of units of time
    /// indicated by the method name.
    ///
    /// Each factory method corresponds to an SI unit of time. An SI minute
    /// always contains exactly 60 seconds. An SI hour always contains exactly
    /// 3600 seconds. An SI day always contains exactly 86400 seconds.
    ///
    /// ### Template parameters
    /// - `T` - an arithmetic type
    ///
    /// ### Parameters
    /// - `d` - number of days
    /// - `h` - number of hours
    /// - `m` - number of minutes
    /// - `s` - number of seconds
    /// - `ms` - number of milliseconds
    /// - `us` - number of microseconds
    /// - `ns` - number of nanoseconds
    /// - `ps` - number of picoseconds
    ///
    /// ### Return value
    /// 1\) a TimeDelta object representing the specified number of days
    /// 2\) a TimeDelta object representing the specified number of hours
    /// 3\) a TimeDelta object representing the specified number of seconds
    /// 4\) a TimeDelta object representing the specified number of milliseconds
    /// 5\) a TimeDelta object representing the specified number of microseconds
    /// 6\) a TimeDelta object representing the specified number of nanoseconds
    /// 7\) a TimeDelta object representing the specified number of picoseconds
    ///
    /// ### Notes
    /// Conversion from a floating-point duration is subject to undefined
    /// behavior when the floating-point value is NaN, infinity, or too large to
    /// be representable by TimeDelta. Otherwise, casting is subject to
    /// truncation as with any `static_cast`.
    template<typename T>
    [[nodiscard]] static constexpr TimeDelta
    days(T d)
    {
        static_assert(caesar::is_arithmetic_v<T>);
        using Days = std::chrono::duration<T, std::ratio<86400>>;
        return TimeDelta(Days(d));
    }

    /// \group factory
    template<typename T>
    [[nodiscard]] static constexpr TimeDelta
    hours(T h)
    {
        static_assert(caesar::is_arithmetic_v<T>);
        using Hours = std::chrono::duration<T, std::ratio<3600>>;
        return TimeDelta(Hours(h));
    }

    /// \group factory
    template<typename T>
    [[nodiscard]] static constexpr TimeDelta
    minutes(T m)
    {
        static_assert(caesar::is_arithmetic_v<T>);
        using Minutes = std::chrono::duration<T, std::ratio<60>>;
        return TimeDelta(Minutes(m));
    }

    /// \group factory
    template<typename T>
    [[nodiscard]] static constexpr TimeDelta
    seconds(T s)
    {
        static_assert(caesar::is_arithmetic_v<T>);
        using Seconds = std::chrono::duration<T>;
        return TimeDelta(Seconds(s));
    }

    /// \group factory
    template<typename T>
    [[nodiscard]] static constexpr TimeDelta
    milliseconds(T ms)
    {
        static_assert(caesar::is_arithmetic_v<T>);
        using Millisecs = std::chrono::duration<T, std::milli>;
        return TimeDelta(Millisecs(ms));
    }

    /// \group factory
    template<typename T>
    [[nodiscard]] static constexpr TimeDelta
    microseconds(T us)
    {
        static_assert(caesar::is_arithmetic_v<T>);
        using Microsecs = std::chrono::duration<T, std::micro>;
        return TimeDelta(Microsecs(us));
    }

    /// \group factory
    template<typename T>
    [[nodiscard]] static constexpr TimeDelta
    nanoseconds(T ns)
    {
        static_assert(caesar::is_arithmetic_v<T>);
        using Nanosecs = std::chrono::duration<T, std::nano>;
        return TimeDelta(Nanosecs(ns));
    }

    /// \group factory
    template<typename T>
    [[nodiscard]] static constexpr TimeDelta
    picoseconds(T ps)
    {
        static_assert(caesar::is_arithmetic_v<T>);
        using Picosecs = std::chrono::duration<T, std::pico>;
        return TimeDelta(Picosecs(ps));
    }

    /// Return the tick count.
    [[nodiscard]] constexpr Rep
    count() const noexcept
    {
        return duration_.count();
    }

    /// Return the total number of seconds in the duration.
    [[nodiscard]] constexpr double
    total_seconds() const
    {
        using Seconds = std::chrono::duration<double>;
        return Seconds(*this).count();
    }

    /// \group unary Unary arithmetic operators
    ///
    /// Implements the unary plus and unary minus operators.
    ///
    /// ### Return value
    /// 1\) a copy of the TimeDelta object
    /// 2\) the negation of the TimeDelta object
    [[nodiscard]] constexpr TimeDelta
    operator+() const noexcept
    {
        return *this;
    }

    /// \group unary
    [[nodiscard]] constexpr TimeDelta
    operator-() const
    {
        return TimeDelta(-duration_);
    }

    /// \group incr_decr Increment/decrement operators
    ///
    /// Increment or decrement the tick count.
    ///
    /// 1,2\) Increments the tick count.
    /// 3,4\) Decrements the tick count.
    ///
    /// ### Return value
    /// 1,3\) a reference to this TimeDelta after modification
    /// 2,4\) a copy of this TimeDelta made before modification
    constexpr TimeDelta&
    operator++()
    {
        ++duration_;
        return *this;
    }

    /// \group incr_decr
    constexpr TimeDelta
    operator++(int)
    {
        return TimeDelta(duration_++);
    }

    /// \group incr_decr
    constexpr TimeDelta&
    operator--()
    {
        --duration_;
        return *this;
    }

    /// \group incr_decr
    constexpr TimeDelta
    operator--(int)
    {
        return TimeDelta(duration_--);
    }

    /// \group compound_assignment Compound assignment operators
    ///
    /// Implements compound assignment.
    ///
    /// Performs an in-place arithmetic operation and returns a reference to the
    /// modified result.
    ///
    /// 1\) Performs in-place addition.
    /// 2\) Performs in-place subtraction.
    /// 3\) Performs in-place multiplication.
    /// 4\) Performs in-place division, truncating toward zero.
    /// 5\) Performs modulo operation in-place.
    /// 6\) Performs modulo operation in-place. Equivalent to `*this %=
    /// other.count()`.
    ///
    /// ### Template parameters
    /// - `T` - an arithmetic type
    ///
    /// ### Parameters
    /// - `other` - another TimeDelta
    /// - `mul` - multiplier
    /// - `div` - divisor
    /// - `mod` - modulus
    ///
    /// ### Return value
    /// a reference to this TimeDelta after modification
    ///
    /// ### Notes
    /// 4\) Division by zero has undefined behavior.
    /// 5,6\) The behavior is undefined if the modulus is zero.
    constexpr TimeDelta&
    operator+=(const TimeDelta& other)
    {
        duration_ += other.duration_;
        return *this;
    }

    /// \group compound_assignment
    constexpr TimeDelta&
    operator-=(const TimeDelta& other)
    {
        duration_ -= other.duration_;
        return *this;
    }

    /// \group compound_assignment
    template<typename T>
    constexpr TimeDelta&
    operator*=(T mul)
    {
        static_assert(caesar::is_arithmetic_v<T>);
        if constexpr (std::is_floating_point_v<T>) {
            auto c = static_cast<T>(count());
            c *= mul;
            duration_ = Duration(static_cast<Rep>(c));
        } else {
            duration_ *= mul;
        }
        return *this;
    }

    /// \group compound_assignment
    template<typename T>
    constexpr TimeDelta&
    operator/=(T div)
    {
        static_assert(caesar::is_arithmetic_v<T>);
        if constexpr (std::is_floating_point_v<T>) {
            auto c = static_cast<T>(count());
            c /= div;
            duration_ = Duration(static_cast<Rep>(c));
        } else {
            duration_ /= div;
        }
        return *this;
    }

    /// \group compound_assignment
    constexpr TimeDelta&
    operator%=(Rep mod)
    {
        duration_ %= mod;
        return *this;
    }

    /// \group compound_assignment
    constexpr TimeDelta&
    operator%=(const TimeDelta& other)
    {
        duration_ %= other.duration_;
        return *this;
    }

    /// \group binary_arithmetic Binary arithmetic operators
    ///
    /// Performs arithmetic operations on the two inputs.
    ///
    /// 1\) Add two TimeDelta objects.
    /// 2\) Subtract a TimeDelta object from another.
    /// 3,4\) Multiply a TimeDelta object by a scalar.
    /// 5\) Multiply a TimeDelta object by a scalar, truncating toward zero.
    /// 6\) Compute the remainder after division.
    /// 7\) Compute the remainder after division. Equivalent to `lhs %
    /// rhs.count()`.
    ///
    /// ### Template parameters
    /// - `T` - an arithmetic type
    ///
    /// ### Parameters
    /// - `lhs`, `rhs` - input TimeDelta objects
    /// - `mul` - multiplier
    /// - `div` - divisor
    /// - `mod` - modulus
    ///
    /// ### Return value
    /// 1\) sum of the inputs
    /// 2\) difference of the inputs
    /// 3,4\) product of the inputs
    /// 5\) quotient of the inputs
    /// 6,7\) remainder of the inputs
    ///
    /// ### Notes
    /// 5\) Division by zero has undefined behavior.
    /// 6,7\) The behavior is undefined if the modulus is zero.
    [[nodiscard]] friend constexpr TimeDelta
    operator+(const TimeDelta& lhs, const TimeDelta& rhs)
    {
        auto out = lhs;
        out += rhs;
        return out;
    }

    /// \group binary_arithmetic
    [[nodiscard]] friend constexpr TimeDelta
    operator-(const TimeDelta& lhs, const TimeDelta& rhs)
    {
        auto out = lhs;
        out -= rhs;
        return out;
    }

    /// \group binary_arithmetic
    template<typename T>
    [[nodiscard]] friend constexpr TimeDelta
    operator*(const TimeDelta& lhs, T mul)
    {
        auto out = lhs;
        out *= mul;
        return out;
    }

    /// \group binary_arithmetic
    template<typename T>
    [[nodiscard]] friend constexpr TimeDelta
    operator*(T mul, const TimeDelta& rhs)
    {
        auto out = rhs;
        out *= mul;
        return out;
    }

    /// \group binary_arithmetic
    template<typename T>
    [[nodiscard]] friend constexpr TimeDelta
    operator/(const TimeDelta& lhs, T div)
    {
        auto out = lhs;
        out /= div;
        return out;
    }

    /// \group binary_arithmetic
    [[nodiscard]] friend constexpr TimeDelta
    operator%(const TimeDelta& lhs, Rep mod)
    {
        auto out = lhs;
        out %= mod;
        return out;
    }

    /// \group binary_arithmetic
    [[nodiscard]] friend constexpr TimeDelta
    operator%(const TimeDelta& lhs, const TimeDelta& rhs)
    {
        return lhs % rhs.count();
    }

    /// \group comparison Comparison operators
    ///
    /// Compare two TimeDelta objects.
    [[nodiscard]] friend constexpr bool
    operator==(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return lhs.duration_ == rhs.duration_;
    }

    /// \group comparison
    [[nodiscard]] friend constexpr bool
    operator!=(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return not(lhs == rhs);
    }

    /// \group comparison
    [[nodiscard]] friend constexpr bool
    operator<(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return lhs.duration_ < rhs.duration_;
    }

    /// \group comparison
    [[nodiscard]] friend constexpr bool
    operator>(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return lhs.duration_ > rhs.duration_;
    }

    /// \group comparison
    [[nodiscard]] friend constexpr bool
    operator<=(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return not(lhs > rhs);
    }

    /// \group comparison
    [[nodiscard]] friend constexpr bool
    operator>=(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return not(lhs < rhs);
    }

    /// Insert a textual representation of a TimeDelta object into an output
    /// stream.
    ///
    /// The output format depends on the magnitude of dt.
    ///
    /// ### Example
    /// ```cpp
    /// auto dt1 = TimeDelta::picoseconds(123);
    /// auto dt2 = TimeDelta::picoseconds(1230);
    /// auto dt3 = TimeDelta::microseconds(12) + TimeDelta::nanoseconds(345);
    /// auto dt4 = TimeDelta::seconds(-1) + TimeDelta::milliseconds(1);
    /// auto dt5 = TimeDelta::seconds(754);
    /// auto dt6 = -TimeDelta::hours(1) - TimeDelta::picoseconds(1);
    /// auto dt7 = TimeDelta::days(1) + TimeDelta::hours(23) +
    ///         TimeDelta::minutes(45) + TimeDelta::seconds(67) +
    ///         TimeDelta::milliseconds(890);
    ///
    /// std::cout << "dt1 = " << dt1 << std::endl;
    /// std::cout << "dt2 = " << dt2 << std::endl;
    /// std::cout << "dt3 = " << dt3 << std::endl;
    /// std::cout << "dt4 = " << dt4 << std::endl;
    /// std::cout << "dt5 = " << dt5 << std::endl;
    /// std::cout << "dt6 = " << dt6 << std::endl;
    /// std::cout << "dt7 = " << dt7 << std::endl;
    /// ```
    ///
    /// Possible output:
    /// ```
    /// dt1 = 123ps
    /// dt2 = 1.23ns
    /// dt3 = 12.345us
    /// dt4 = -999ms
    /// dt5 = 12m34s
    /// dt6 = -1h0m0.000000000001s
    /// dt7 = 1d23h45m67.89s
    /// ```
    ///
    /// Stream formatting flags such as [std::showpos]() and [std::showpoint]()
    /// can be used to manipulate output. Other flags affecting representation
    /// of arithmetic types are ignored.
    ///
    /// ```cpp
    /// std::cout << std::showpos << TimeDelta::seconds(10) << std::endl;
    /// std::cout << std::showpoint << TimeDelta::seconds(-10) << std::endl;
    /// ```
    ///
    /// Possible output:
    /// ```
    /// +10s
    /// -10.0s
    /// ```
    friend std::ostream&
    operator<<(std::ostream& os, const TimeDelta& dt);

private:
    Duration duration_ = {};
};

/// Return the absolute value of the input TimeDelta.
///
/// ### Example
/// ```cpp
/// auto dt1 = TimeDelta::seconds(123);
/// auto dt2 = -dt1;
///
/// std::cout << "abs(" << dt1 << ") = " << abs(dt1) << std::endl;
/// std::cout << "abs(" << dt2 << ") = " << abs(dt2) << std::endl;
/// ```
///
/// Possible output:
/// ```
/// abs(123s) = 123s
/// abs(-123s) = 123s
/// ```
[[nodiscard]] constexpr TimeDelta
abs(const TimeDelta& dt)
{
    const auto d = TimeDelta::Duration(dt);
    return TimeDelta(std::chrono::abs(d));
}

/// Truncate to a multiple of the specified period.
///
/// Returns the nearest integer multiple of period not greater in magnitude than
/// dt.
///
/// ### Example
/// ```cpp
/// auto dt1 = -TimeDelta::minutes(3) - TimeDelta::seconds(30);
/// auto dt2 = TimeDelta::days(1) - TimeDelta::picoseconds(1);
///
/// std::cout << trunc(dt1, TimeDelta::minutes(1)) << std::endl;
/// std::cout << trunc(dt2, TimeDelta::hours(1)) << std::endl;
/// ```
///
/// Possible output:
/// ```
/// -3m
/// 23h
/// ```
///
/// ### Notes
/// The behavior is undefined if `period` is zero.
///
/// ### See also
/// [floor(const TimeDelta&, const TimeDelta&)]
/// [ceil(const TimeDelta&, const TimeDelta&)]
/// [round(const TimeDelta&, const TimeDelta&)]
[[nodiscard]] constexpr TimeDelta
trunc(const TimeDelta& dt, const TimeDelta& period)
{
    return dt - (dt % period);
}

/// Round down to a multiple of the specified period.
///
/// Returns the nearest integer multiple of period that is less than or equal to
/// dt.
///
/// ### Example
/// ```cpp
/// auto dt1 = -TimeDelta::minutes(3) - TimeDelta::seconds(30);
/// auto dt2 = TimeDelta::days(1) - TimeDelta::picoseconds(1);
///
/// std::cout << floor(dt1, TimeDelta::minutes(1)) << std::endl;
/// std::cout << floor(dt2, TimeDelta::hours(1)) << std::endl;
/// ```
///
/// Possible output:
/// ```
/// -4m
/// 23h
/// ```
///
/// ### Notes
/// The behavior is undefined if `period` is zero.
///
/// ### See also
/// [trunc(const TimeDelta&, const TimeDelta&)]
/// [ceil(const TimeDelta&, const TimeDelta&)]
/// [round(const TimeDelta&, const TimeDelta&)]
[[nodiscard]] constexpr TimeDelta
floor(const TimeDelta& dt, const TimeDelta& period)
{
    const auto t = trunc(dt, period);
    return t <= dt ? t : t - abs(period);
}

/// Round up to a multiple of the specified period.
///
/// Returns the nearest integer multiple of period that is greater than or equal
/// to dt.
///
/// ### Example
/// ```cpp
/// auto dt1 = -TimeDelta::minutes(3) - TimeDelta::seconds(30);
/// auto dt2 = TimeDelta::days(1) - TimeDelta::picoseconds(1);
///
/// std::cout << ceil(dt1, TimeDelta::minutes(1)) << std::endl;
/// std::cout << ceil(dt2, TimeDelta::hours(1)) << std::endl;
/// ```
///
/// Possible output:
/// ```
/// -3m
/// 24h
/// ```
///
/// ### Notes
/// The behavior is undefined if `period` is zero.
///
/// ### See also
/// [trunc(const TimeDelta&, const TimeDelta&)]
/// [floor(const TimeDelta&, const TimeDelta&)]
/// [round(const TimeDelta&, const TimeDelta&)]
[[nodiscard]] constexpr TimeDelta
ceil(const TimeDelta& dt, const TimeDelta& period)
{
    const auto t = trunc(dt, period);
    return t >= dt ? t : t + abs(period);
}

/// Round to the nearest multiple of the specified period.
///
/// Returns the integer multiple of period that is closest to dt. If there are
/// two such values, returns the one that is an even multiple of period.
///
/// ### Example
/// ```cpp
/// auto dt1 = -TimeDelta::minutes(3) - TimeDelta::seconds(30);
/// auto dt2 = TimeDelta::days(1) - TimeDelta::picoseconds(1);
///
/// std::cout << round(dt1, TimeDelta::minutes(1)) << std::endl;
/// std::cout << round(dt2, TimeDelta::hours(1)) << std::endl;
/// ```
///
/// Possible output:
/// ```
/// -4m
/// 24h
/// ```
///
/// ### Notes
/// The behavior is undefined if `period` is zero.
///
/// ### See also
/// [trunc(const TimeDelta&, const TimeDelta&)]
/// [floor(const TimeDelta&, const TimeDelta&)]
/// [ceil(const TimeDelta&, const TimeDelta&)]
[[nodiscard]] constexpr TimeDelta
round(const TimeDelta& dt, const TimeDelta& period)
{
    const auto lower = floor(dt, period);
    const auto upper = lower + abs(period);

    const auto lower_diff = dt - lower;
    const auto upper_diff = upper - dt;

    if (lower_diff < upper_diff) {
        return lower;
    }
    if (upper_diff < lower_diff) {
        return upper;
    }

    // In halfway cases, return the value that's an even multiple of period.
    return (lower.count() / period.count()) & 1 ? upper : lower;
}

} // namespace caesar
