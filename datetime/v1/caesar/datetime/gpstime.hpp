#pragma once

#include "timedelta.hpp"

#include <chrono>
#include <date/date.h>
#include <date/tz.h>
#include <iostream>
#include <string>
#include <string_view>

namespace caesar {

/**
 * A date/time point in Global Positioning System (GPS) time with picosecond
 * resolution
 *
 * GPS time is an atomic time scale implemented by GPS satellites and ground
 * stations. Unlike UTC time, GPS time is a continuous time scale. Leap seconds
 * are not inserted. Therefore, the offset between GPS and UTC time is not fixed
 * but rather changes each time a leap second adjustment is made to UTC.
 *
 * The date components follow the proleptic Gregorian calendar, which allows the
 * representation of dates prior to the calendar's introduction in 1582.
 * XXX Should dates before year 0000 or after year 9999 be considered valid???
 *
 * Internally, GPSTime represents time as a 128-bit integer count of picoseconds
 * relative to some reference epoch.
 *
 * \see UTCTime
 * \see TimeDelta
 */
class GPSTime {
public:
    /**
     * A type representing the GPS time system which meets the named
     * requirements of
     * [Clock](https://en.cppreference.com/w/cpp/named_req/Clock).
     */
    using Clock = date::gps_clock;

    /** \see TimeDelta::Rep */
    using Rep = TimeDelta::Rep;

    /** \see TimeDelta::Period */
    using Period = TimeDelta::Period;

    /** ... */
    using Duration = std::chrono::duration<Rep, Period>;

    /**
     * Construct a new GPSTime object.
     *
     * \param[in] year          year component
     * \param[in] month         month component, encoded 1 through 12
     * \param[in] day           day component, encoded 1 through 31
     * \param[in] hour          hour component, in the range [0, 24)
     * \param[in] minute        minute component, in the range [0, 60)
     * \param[in] second        second component, in the range [0, 60)
     * \param[in] microsecond   microsecond component, in the range [0, 1000000)
     * \param[in] picosecond    picosecond component, in the range [0, 1000000)
     */
    constexpr GPSTime(int year,
                      int month,
                      int day,
                      int hour,
                      int minute,
                      int second,
                      int microsecond = 0,
                      int picosecond = 0);

    /** Construct a new GPSTime object from a std::chrono::time_point. */
    explicit constexpr GPSTime(
            const std::chrono::time_point<Clock, Duration>& time_point) noexcept
        : time_point_(time_point)
    {}

    /** Construct a new GPSTime object from a string representation. */
    explicit GPSTime(std::string_view datetime_string);

    /** Convert to std::chrono::time_point. */
    explicit operator std::chrono::time_point<Clock, Duration>() const
    {
        return time_point_;
    }

    /** Return a string representation of the GPS time. */
    explicit operator std::string() const;

    /** Return the earliest valid GPSTime. */
    static GPSTime
    min() noexcept
    {
        return GPSTime(1, 1, 1, 0, 0, 0);
    }

    /** Return the latest valid GPSTime. */
    static GPSTime
    max() noexcept
    {
        return GPSTime(9999, 12, 31, 23, 59, 59, 999'999, 999'999);
    }

    /**
     * Return the smallest possible difference between non-equal GPSTime
     * objects.
     */
    static constexpr TimeDelta
    resolution() noexcept
    {
        return TimeDelta::resolution();
    }

    /** Return the current time in GPS time. */
    static GPSTime
    now()
    {
        return GPSTime(Clock::now());
    }

    /** Return the year component. */
    int
    year() const;

    /** Return the month component, encoded 1 through 12. */
    int
    month() const;

    /** Return the day component, encoded 1 through 31. */
    int
    day() const;

    /** Return the day of the week. */
    date::weekday
    weekday() const;

    /** Return the hour component. */
    int
    hour() const;

    /** Return the minute component. */
    int
    minute() const;

    /** Return the second component. */
    int
    second() const;

    /** Return the microsecond component. */
    int
    microsecond() const;

    /** Return the picosecond component. */
    int
    picosecond() const;

    /** Increment the tick count. */
    constexpr GPSTime&
    operator++()
    {
        ++time_point_;
        return *this;
    }

    /** \copydoc GPSTime::operator++() */
    constexpr GPSTime
    operator++(int)
    {
        return GPSTime(time_point_++);
    }

    /** Decrement the tick count. */
    constexpr GPSTime&
    operator--()
    {
        --time_point_;
        return *this;
    }

    /** \copydoc GPSTime::operator--() */
    constexpr GPSTime
    operator--(int)
    {
        return GPSTime(time_point_--);
    }

    /** Perform addition in-place and return the modified result. */
    constexpr GPSTime&
    operator+=(const TimeDelta& dt)
    {
        time_point_ += Duration(dt);
        return *this;
    }

    /** Perform subtraction in-place and return the modified result. */
    constexpr GPSTime&
    operator-=(const TimeDelta& dt)
    {
        time_point_ -= Duration(dt);
        return *this;
    }

    /** Add a TimeDelta to a GPSTime. */
    friend constexpr GPSTime
    operator+(const GPSTime& lhs, const TimeDelta& rhs)
    {
        auto out = lhs;
        out += rhs;
        return out;
    }

    /** \copydoc operator+(const GPSTime&, const TimeDelta&) */
    friend constexpr GPSTime
    operator+(const TimeDelta& lhs, const GPSTime& rhs)
    {
        auto out = rhs;
        out += lhs;
        return out;
    }

    /** Subtract a TimeDelta from a GPSTime. */
    friend constexpr GPSTime
    operator-(const GPSTime& lhs, const TimeDelta& rhs)
    {
        auto out = lhs;
        out -= rhs;
        return out;
    }

    /** Compute the difference between two GPSTime objects. */
    friend constexpr TimeDelta
    operator-(const GPSTime& lhs, const GPSTime& rhs)
    {
        return TimeDelta(lhs.time_point_ - rhs.time_point_);
    }

    /** Compare two GPSTime objects. */
    friend constexpr bool
    operator==(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return lhs.time_point_ == rhs.time_point_;
    }

    /** \copydoc operator==(const GPSTime&, const GPSTime&) */
    friend constexpr bool
    operator!=(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return not(lhs == rhs);
    }

    /** \copydoc operator==(const GPSTime&, const GPSTime&) */
    friend constexpr bool
    operator<(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return lhs.time_point_ < rhs.time_point_;
    }

    /** \copydoc operator==(const GPSTime&, const GPSTime&) */
    friend constexpr bool
    operator>(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return lhs.time_point_ > rhs.time_point_;
    }

    /** \copydoc operator==(const GPSTime&, const GPSTime&) */
    friend constexpr bool
    operator<=(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return not(lhs > rhs);
    }

    /** \copydoc operator==(const GPSTime&, const GPSTime&) */
    friend constexpr bool
    operator>=(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return not(lhs < rhs);
    }

    /** Serialize a string representation to an output stream. */
    friend std::ostream&
    operator<<(std::ostream& os, const GPSTime& ts)
    {
        return os << std::string(ts);
    }

private:
    std::chrono::time_point<Clock, Duration> time_point_;
};

} // namespace caesar
