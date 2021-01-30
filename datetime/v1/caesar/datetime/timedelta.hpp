#pragma once

#include <absl/numeric/int128.h>
#include <chrono>
#include <limits>
#include <ratio>
#include <type_traits>

namespace caesar {

/**
 * Represents a signed duration, the difference between two timepoints, with
 * picosecond resolution
 *
 * Internally, TimeDelta stores a 128-bit integer tick count of picoseconds,
 * allowing it to represent spans of trillions of years without loss of
 * precision.
 *
 * TimeDelta objects can be constructed from or converted to instances of
 * std::chrono::duration. In addition, the factory functions `days()`,
 * `hours()`, `minutes()`, `seconds()`, `milliseconds()`, `microseconds()`,
 * `nanoseconds()`, and `picoseconds()` can be used to create TimeDelta objects.
 *
 * \see GPSTime
 * \see UTCTime
 */
class TimeDelta {
    /**
     * \private
     * Checks whether `T` is a (built-in or user-defined) arithmetic type.
     *
     * Formally, this checks whether `std::numeric_limits` is specialized for
     * `T`. `std::numeric_limits` is specialized for all built-in arithmetic
     * types and, unlike `std::is_arithmetic`, may be specialized for
     * user-defined types.
     */
    template<typename T>
    constexpr static bool is_arithmetic =
            std::numeric_limits<T>::is_specialized;

public:
    /** A signed arithmetic type representing the number of ticks */
    using Rep = absl::int128;

    /**
     * A `std::ratio` representing the tick period (i.e. the number of ticks per
     * second).
     */
    using Period = std::pico;

    /** Construct a new TimeDelta object representing a zero-length duration. */
    TimeDelta() = default;

    /**
     * Construct a new TimeDelta object from a std::chrono::duration.
     *
     * If the input has sub-picosecond resolution, it will be truncated to an
     * integer multiple of `TimeDelta::resolution`.
     *
     * ## Notes
     *
     * Casting from a floating-point duration is subject to undefined behavior
     * when the floating-point value is NaN, infinity, or too large to be
     * representable by `TimeDelta::Rep`. Otherwise, casting is subject to
     * truncation as with any `static_cast` to `TimeDelta::Rep`.
     */
    template<class FromRep, class FromPeriod>
    explicit constexpr TimeDelta(
            const std::chrono::duration<FromRep, FromPeriod>& d);

    /**
     * Convert to std::chrono::duration.
     *
     * If the output has wider resolution than `TimeDelta::resolution`, the
     * conversion is subject to truncation.
     *
     * ## Notes
     *
     * Casting is subject to truncation as with any `static_cast` to `ToRep`.
     */
    template<class ToRep, class ToPeriod>
    explicit constexpr operator std::chrono::duration<ToRep, ToPeriod>() const;

    /** Return the smallest representable TimeDelta. */
    static constexpr TimeDelta
    min() noexcept
    {
        return TimeDelta(std::chrono::duration<Rep, Period>::min());
    }

    /** Return the largest representable TimeDelta. */
    static constexpr TimeDelta
    max() noexcept
    {
        return TimeDelta(std::chrono::duration<Rep, Period>::max());
    }

    /**
     * Return the smallest possible difference between non-equal TimeDelta
     * objects.
     */
    static constexpr TimeDelta
    resolution() noexcept
    {
        return TimeDelta(std::chrono::duration<Rep, Period>(1));
    }

    /**
     * Return a TimeDelta object representing the specified number of days.
     *
     * \tparam T an arithmetic type representing the number of days
     *
     * ## Notes
     *
     * Casting from a floating-point value is subject to undefined behavior
     * when the value is NaN, infinity, or too large to be representable by
     * `TimeDelta::Rep`. Otherwise, casting is subject to truncation as with any
     * `static_cast` to `TimeDelta::Rep`.
     */
    template<typename T>
    static constexpr TimeDelta
    days(T d)
    {
        static_assert(is_arithmetic<T>);
        using Days = std::chrono::duration<T, std::ratio<86400>>;
        return TimeDelta(Days(d));
    }

    /**
     * Return a TimeDelta object representing the specified number of hours.
     *
     * \tparam T an arithmetic type representing the number of hours
     *
     * ## Notes
     *
     * Casting from a floating-point value is subject to undefined behavior
     * when the value is NaN, infinity, or too large to be representable by
     * `TimeDelta::Rep`. Otherwise, casting is subject to truncation as with any
     * `static_cast` to `TimeDelta::Rep`.
     */
    template<typename T>
    static constexpr TimeDelta
    hours(T h)
    {
        static_assert(is_arithmetic<T>);
        using Hours = std::chrono::duration<T, std::ratio<3600>>;
        return TimeDelta(Hours(h));
    }

    /**
     * Return a TimeDelta object representing the specified number of minutes.
     *
     * \tparam T an arithmetic type representing the number of minutes
     *
     * ## Notes
     *
     * Casting from a floating-point value is subject to undefined behavior
     * when the value is NaN, infinity, or too large to be representable by
     * `TimeDelta::Rep`. Otherwise, casting is subject to truncation as with any
     * `static_cast` to `TimeDelta::Rep`.
     */
    template<typename T>
    static constexpr TimeDelta
    minutes(T m)
    {
        static_assert(is_arithmetic<T>);
        using Minutes = std::chrono::duration<T, std::ratio<60>>;
        return TimeDelta(Minutes(m));
    }

    /**
     * Return a TimeDelta object representing the specified number of seconds.
     *
     * \tparam T an arithmetic type representing the number of seconds
     *
     * ## Notes
     *
     * Casting from a floating-point value is subject to undefined behavior
     * when the value is NaN, infinity, or too large to be representable by
     * `TimeDelta::Rep`. Otherwise, casting is subject to truncation as with any
     * `static_cast` to `TimeDelta::Rep`.
     */
    template<typename T>
    static constexpr TimeDelta
    seconds(T s)
    {
        static_assert(is_arithmetic<T>);
        using Seconds = std::chrono::duration<T>;
        return TimeDelta(Seconds(s));
    }

    /**
     * Return a TimeDelta object representing the specified number of
     * milliseconds.
     *
     * \tparam T an arithmetic type representing the number of milliseconds
     *
     * ## Notes
     *
     * Casting from a floating-point value is subject to undefined behavior
     * when the value is NaN, infinity, or too large to be representable by
     * `TimeDelta::Rep`. Otherwise, casting is subject to truncation as with any
     * `static_cast` to `TimeDelta::Rep`.
     */
    template<typename T>
    static constexpr TimeDelta
    milliseconds(T ms)
    {
        static_assert(is_arithmetic<T>);
        using Millisecs = std::chrono::duration<T, std::milli>;
        return TimeDelta(Millisecs(ms));
    }

    /**
     * Return a TimeDelta object representing the specified number of
     * microseconds.
     *
     * \tparam T an arithmetic type representing the number of microseconds
     *
     * ## Notes
     *
     * Casting from a floating-point value is subject to undefined behavior
     * when the value is NaN, infinity, or too large to be representable by
     * `TimeDelta::Rep`. Otherwise, casting is subject to truncation as with any
     * `static_cast` to `TimeDelta::Rep`.
     */
    template<typename T>
    static constexpr TimeDelta
    microseconds(T us)
    {
        static_assert(is_arithmetic<T>);
        using Microsecs = std::chrono::duration<T, std::micro>;
        return TimeDelta(Microsecs(us));
    }

    /**
     * Return a TimeDelta object representing the specified number of
     * nanoseconds.
     *
     * \tparam T an arithmetic type representing the number of nanoseconds
     *
     * ## Notes
     *
     * Casting from a floating-point value is subject to undefined behavior
     * when the value is NaN, infinity, or too large to be representable by
     * `TimeDelta::Rep`. Otherwise, casting is subject to truncation as with any
     * `static_cast` to `TimeDelta::Rep`.
     */
    template<typename T>
    static constexpr TimeDelta
    nanoseconds(T ns)
    {
        static_assert(is_arithmetic<T>);
        using Nanosecs = std::chrono::duration<T, std::nano>;
        return TimeDelta(Nanosecs(ns));
    }

    /**
     * Return a TimeDelta object representing the specified number of
     * picoseconds.
     *
     * \tparam T an arithmetic type representing the number of picoseconds
     *
     * ## Notes
     *
     * Casting from a floating-point value is subject to undefined behavior
     * when the value is NaN, infinity, or too large to be representable by
     * `TimeDelta::Rep`. Otherwise, casting is subject to truncation as with any
     * `static_cast` to `TimeDelta::Rep`.
     */
    template<typename T>
    static constexpr TimeDelta
    picoseconds(T ps)
    {
        static_assert(is_arithmetic<T>);
        using Picosecs = std::chrono::duration<T, std::pico>;
        return TimeDelta(Picosecs(ps));
    }

    /** Return the tick count. */
    constexpr Rep
    count() const noexcept
    {
        return duration_.count();
    }

    /** Return the total number of seconds in the duration. */
    double
    total_seconds() const
    {
        using Seconds = std::chrono::duration<double>;
        return Seconds(*this).count();
    }

    /** Returns a copy of the TimeDelta object. */
    constexpr TimeDelta
    operator+() const noexcept
    {
        return *this;
    }

    /** Returns the negation of the TimeDelta object. */
    constexpr TimeDelta
    operator-() const
    {
        return TimeDelta(-duration_);
    }

    /** Increment the tick count. */
    constexpr TimeDelta&
    operator++()
    {
        ++duration_;
        return *this;
    }

    /** \copydoc TimeDelta::operator++() */
    constexpr TimeDelta
    operator++(int)
    {
        return TimeDelta(duration_++);
    }

    /** Decrement the tick count. */
    constexpr TimeDelta&
    operator--()
    {
        --duration_;
        return *this;
    }

    /** \copydoc TimeDelta::operator--() */
    constexpr TimeDelta
    operator--(int)
    {
        return TimeDelta(duration_--);
    }

    /** Perform addition in-place and return the modified result. */
    constexpr TimeDelta&
    operator+=(const TimeDelta& other)
    {
        duration_ += other.duration_;
        return *this;
    }

    /** Perform subtraction in-place and return the modified result. */
    constexpr TimeDelta&
    operator-=(const TimeDelta& other)
    {
        duration_ -= other.duration_;
        return *this;
    }

    /** Perform multiplication in-place and return the modified result. */
    constexpr TimeDelta&
    operator*=(Rep mul)
    {
        duration_ *= mul;
        return *this;
    }

    /**
     * Perform division in-place, truncating toward zero, and return the
     * modified result.
     *
     * ## Notes
     *
     * Division by zero has undefined behavior.
     */
    constexpr TimeDelta&
    operator/=(Rep div)
    {
        duration_ /= div;
        return *this;
    }

    /**
     * Perform modulo operation in-place and return the modified result.
     *
     * ## Notes
     *
     * The behavior is undefined if the modulus is zero.
     */
    constexpr TimeDelta&
    operator%=(Rep mod)
    {
        duration_ %= mod;
        return *this;
    }

    /**
     * \copydoc TimeDelta::operator%=(Rep)
     *
     * Equivalent to `*this %= other.count()`.
     */
    constexpr TimeDelta&
    operator%=(const TimeDelta& other)
    {
        duration_ %= other.duration_;
        return *this;
    }

    /** Add two TimeDelta objects. */
    friend constexpr TimeDelta
    operator+(const TimeDelta& lhs, const TimeDelta& rhs)
    {
        auto out = lhs;
        out += rhs;
        return out;
    }

    /** Subtract a TimeDelta object from another. */
    friend constexpr TimeDelta
    operator-(const TimeDelta& lhs, const TimeDelta& rhs)
    {
        auto out = lhs;
        out -= rhs;
        return out;
    }

    /** Multiply a TimeDelta object by a scalar. */
    friend constexpr TimeDelta
    operator*(const TimeDelta& lhs, Rep rhs)
    {
        auto out = lhs;
        out *= rhs;
        return out;
    }

    /** \copydoc operator*(const TimeDelta&, Rep) */
    friend constexpr TimeDelta
    operator*(Rep lhs, const TimeDelta& rhs)
    {
        auto out = rhs;
        out *= lhs;
        return out;
    }

    /**
     * Divide a TimeDelta object by a scalar, truncating toward zero.
     *
     * ## Notes
     *
     * Division by zero has undefined behavior.
     */
    friend constexpr TimeDelta
    operator/(const TimeDelta& lhs, Rep rhs)
    {
        auto out = lhs;
        out /= rhs;
        return out;
    }

    /**
     * Compute the remainder after division.
     *
     * ## Notes
     *
     * The behavior is undefined if the modulus is zero.
     */
    friend constexpr TimeDelta
    operator%(const TimeDelta& lhs, Rep rhs)
    {
        auto out = lhs;
        out %= rhs;
        return out;
    }

    /**
     * \copydoc operator%(const TimeDelta&, Rep)
     *
     * Equivalent to `lhs % rhs.count()`.
     */
    friend constexpr TimeDelta
    operator%(const TimeDelta& lhs, const TimeDelta& rhs)
    {
        return lhs % rhs.count();
    }

    /** Compare two TimeDelta objects. */
    friend constexpr bool
    operator==(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return lhs.duration_ == rhs.duration_;
    }

    /** \copydoc operator==(const TimeDelta&, const TimeDelta&) */
    friend constexpr bool
    operator!=(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return not(lhs == rhs);
    }

    /** \copydoc operator==(const TimeDelta&, const TimeDelta&) */
    friend constexpr bool
    operator<(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return lhs.duration_ < rhs.duration_;
    }

    /** \copydoc operator==(const TimeDelta&, const TimeDelta&) */
    friend constexpr bool
    operator>(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return lhs.duration_ > rhs.duration_;
    }

    /** \copydoc operator==(const TimeDelta&, const TimeDelta&) */
    friend constexpr bool
    operator<=(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return not(lhs > rhs);
    }

    /** \copydoc operator==(const TimeDelta&, const TimeDelta&) */
    friend constexpr bool
    operator>=(const TimeDelta& lhs, const TimeDelta& rhs) noexcept
    {
        return not(lhs < rhs);
    }

private:
    std::chrono::duration<Rep, Period> duration_ = {};
};

/** Return the absolute value of the input TimeDelta. */
constexpr TimeDelta
abs(const TimeDelta& dt)
{
    using Duration = std::chrono::duration<TimeDelta::Rep, TimeDelta::Period>;
    const auto d = Duration(dt);
    return TimeDelta(std::chrono::abs(d));
}

template<class FromRep, class FromPeriod>
constexpr TimeDelta::TimeDelta(
        const std::chrono::duration<FromRep, FromPeriod>& d)
    : duration_([=]() {
          static_assert(is_arithmetic<FromRep>);
          using ToDuration = std::chrono::duration<Rep, Period>;

          if constexpr (std::is_floating_point_v<FromRep>) {
              // Convert from input tick period to floating-point picoseconds.
              using PicosecsFP = std::chrono::duration<FromRep, Period>;
              const auto p = std::chrono::duration_cast<PicosecsFP>(d);

              // Cast floating-point to int128.
              const auto r = static_cast<Rep>(p.count());
              return ToDuration(r);
          } else {
              // For integral durations, we can just use duration_cast.
              return std::chrono::duration_cast<ToDuration>(d);
          }
      }())
{}

template<class ToRep, class ToPeriod>
constexpr TimeDelta::operator std::chrono::duration<ToRep, ToPeriod>() const
{
    static_assert(is_arithmetic<ToRep>);
    using ToDuration = std::chrono::duration<ToRep, ToPeriod>;

    if constexpr (std::is_floating_point_v<ToRep>) {
        // Cast duration to floating-point picoseconds.
        const auto r = static_cast<ToRep>(count());
        const auto p = std::chrono::duration<ToRep, Period>(r);

        // Convert to output tick period.
        return std::chrono::duration_cast<ToDuration>(p);
    } else {
        // For integral durations, we can just use duration_cast.
        return std::chrono::duration_cast<ToDuration>(duration_);
    }
}

} // namespace caesar
