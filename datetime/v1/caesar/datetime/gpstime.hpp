#pragma once

#include "timedelta.hpp"

#include <chrono>
#include <date/date.h>
#include <date/tz.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

// Migration path to C++20
//
// All symbols in the `date::` namespace used herein have been standardized in
// C++20 as part of the `<chrono>` library. If/when support for C++17 is
// dropped, we can simply replace the namespace with `std::chrono::`.
//
// In addition, C++20 adds increment & decrement operators for
// `std::chrono::time_point`, which we could use to clean up the implementation
// of the corresponding GPSTime operators.

namespace caesar {

/**
 * A date/time point in Global Positioning System (GPS) time with picosecond
 * resolution
 *
 * The GPS time system is an atomic time scale implemented by GPS satellites and
 * ground stations. Unlike UTC time, GPS time is a continuous linear time scale
 * -- leap seconds are never inserted. Therefore, the offset between GPS and UTC
 * time is not fixed but rather changes each time a leap second adjustment is
 * made to UTC.
 *
 * GPSTime can be broken down into individual date and time-of-day components.
 * The date components follow the proleptic Gregorian calendar, which allows the
 * representation of dates prior to the calendar's introduction in 1582. Dates
 * before year 1 or after year 9999 may not be represented by GPSTime. The time
 * components describe time since midnight in a 24-hour clock system.
 *
 * Internally, GPSTime stores a 128-bit integer timestamp with a picosecond tick
 * interval.
 *
 * \see UTCTime
 * \see TimeDelta
 */
class GPSTime {
public:
    /**
     * A [*Clock*](https://en.cppreference.com/w/cpp/named_req/Clock)
     * representing the time system on which GPSTime is measured
     */
    using Clock = date::gps_clock;

    /**
     * A `std::chrono::duration` capable of exactly representing durations
     * between GPS time points
     */
    using Duration = typename TimeDelta::Duration;

    /**
     * A signed arithmetic type representing the number of ticks of the duration
     */
    using Rep = typename Duration::rep;

    /**
     * A `std::ratio` representing the tick period of the duration (i.e. the
     * number of ticks per second)
     */
    using Period = typename Duration::period;

    /** A `std::chrono::time_point` with the same Clock, Rep, and Period */
    using TimePoint = std::chrono::time_point<Clock, Duration>;

    /**
     * Construct a new GPSTime object.
     *
     * \param[in] year          year component
     * \param[in] month         month component, encoded 1 through 12
     * \param[in] day           day component
     * \param[in] hour          hour component, in the range [0, 24)
     * \param[in] minute        minute component, in the range [0, 60)
     * \param[in] second        second component, in the range [0, 60)
     * \param[in] microsecond   microsecond component, in the range [0, 1000000)
     * \param[in] picosecond    picosecond component, in the range [0, 1000000)
     *
     * \throws std::invalid_argument if any component is outside the expected
     * range
     */
    constexpr GPSTime(int year,
                      int month,
                      int day,
                      int hour,
                      int minute,
                      int second,
                      int microsecond = 0,
                      int picosecond = 0);

    /**
     * Construct a new GPSTime object from a std::chrono::time_point.
     *
     * \throws std::out_of_range if time_point is < GPSTime::min() or >
     * GPSTime::max()
     */
    explicit constexpr GPSTime(const TimePoint& time_point)
        : time_point_(time_point)
    {
        if (*this < GPSTime::min() or *this > GPSTime::max()) {
            throw std::out_of_range(
                    "input time point is outside of valid GPSTime range");
        }
    }

    /**
     * Construct a new GPSTime object from a string representation.
     *
     * \throws std::invalid_argument if the input string does not match the
     * expected format
     */
    explicit GPSTime(std::string_view datetime_string);

    /** Convert to std::chrono::time_point. */
    [[nodiscard]] explicit constexpr operator TimePoint() const noexcept
    {
        return time_point_;
    }

    /** Return a string representation of the GPS time. */
    [[nodiscard]] explicit operator std::string() const;

    /** Return the earliest valid GPSTime. */
    [[nodiscard]] static constexpr GPSTime
    min() noexcept
    {
        return GPSTime(1, 1, 1, 0, 0, 0);
    }

    /** Return the latest valid GPSTime. */
    [[nodiscard]] static constexpr GPSTime
    max() noexcept
    {
        return GPSTime(9999, 12, 31, 23, 59, 59, 999'999, 999'999);
    }

    /**
     * Return the smallest possible difference between non-equal GPSTime
     * objects.
     */
    [[nodiscard]] static constexpr TimeDelta
    resolution() noexcept
    {
        return TimeDelta::resolution();
    }

    /** Return the current time in GPS time. */
    [[nodiscard]] static GPSTime
    now()
    {
        return GPSTime(Clock::now());
    }

    /** Return the date component. */
    [[nodiscard]] constexpr date::year_month_day
    date() const
    {
        // Convert timestamp to Unix time (change of epoch).
        using ToClock = date::local_t;
        const auto unix_time = date::clock_cast<ToClock>(time_point_);

        // Round down to nearest day.
        const auto unix_days = std::chrono::floor<date::days>(unix_time);

        // Convert to calendar date.
        return date::year_month_day(unix_days);
    }

    /** Return the time of day. */
    [[nodiscard]] constexpr date::hh_mm_ss<Duration>
    time_of_day() const
    {
        auto d = time_point_.time_since_epoch();

        // Subtract the number of whole days to get time since midnight.
        using Days = std::chrono::duration<Rep, std::ratio<86400>>;
        d -= std::chrono::floor<Days>(d);

        return date::hh_mm_ss<Duration>(d);
    }

    /** Return the year component. */
    [[nodiscard]] constexpr int
    year() const
    {
        const auto y = date().year();
        return int(y);
    }

    /** Return the month component, encoded 1 through 12. */
    [[nodiscard]] constexpr int
    month() const
    {
        const auto m = date().month();
        return static_cast<int>(unsigned(m));
    }

    /** Return the day component. */
    [[nodiscard]] constexpr int
    day() const
    {
        const auto d = date().day();
        return static_cast<int>(unsigned(d));
    }

    /** Return the day of the week. */
    [[nodiscard]] constexpr date::weekday
    weekday() const
    {
        // Convert calendar date to days since Unix epoch (1970-01-01).
        const auto unix_days = date::sys_days(date());
        return date::weekday(unix_days);
    }

    /** Return the hour component. */
    [[nodiscard]] constexpr int
    hour() const
    {
        const auto h = time_of_day().hours();
        return static_cast<int>(h.count());
    }

    /** Return the minute component. */
    [[nodiscard]] constexpr int
    minute() const
    {
        const auto m = time_of_day().minutes();
        return static_cast<int>(m.count());
    }

    /** Return the second component. */
    [[nodiscard]] constexpr int
    second() const
    {
        const auto s = time_of_day().seconds();
        return static_cast<int>(s.count());
    }

    /** Return the microsecond component. */
    [[nodiscard]] constexpr int
    microsecond() const
    {
        // Get subseconds component.
        const Duration ss = time_of_day().subseconds();

        // Truncate to microseconds (time of day is always nonnegative, so no
        // need to use `std::chrono::floor` -- duration_cast is slightly
        // more efficient).
        using Microsecs = std::chrono::duration<Rep, std::micro>;
        const auto us = std::chrono::duration_cast<Microsecs>(ss);

        return static_cast<int>(us.count());
    }

    /** Return the picosecond component. */
    [[nodiscard]] constexpr int
    picosecond() const
    {
        // Get subseconds component.
        const Duration ss = time_of_day().subseconds();

        // Subtract the number of whole microseconds to get picoseconds
        // component.
        using Microsecs = std::chrono::duration<Rep, std::micro>;
        const auto us = std::chrono::duration_cast<Microsecs>(ss);
        const auto ps = ss - us;

        return static_cast<int>(ps.count());
    }

    /** Increment the tick count. */
    constexpr GPSTime&
    operator++()
    {
        time_point_ += Duration(1);
        return *this;
    }

    /** \copydoc GPSTime::operator++() */
    constexpr GPSTime
    operator++(int)
    {
        return GPSTime(time_point_ + Duration(1));
    }

    /** Decrement the tick count. */
    constexpr GPSTime&
    operator--()
    {
        time_point_ -= Duration(1);
        return *this;
    }

    /** \copydoc GPSTime::operator--() */
    constexpr GPSTime
    operator--(int)
    {
        return GPSTime(time_point_ - Duration(1));
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
    [[nodiscard]] friend constexpr GPSTime
    operator+(const GPSTime& lhs, const TimeDelta& rhs)
    {
        auto out = lhs;
        out += rhs;
        return out;
    }

    /** \copydoc operator+(const GPSTime&, const TimeDelta&) */
    [[nodiscard]] friend constexpr GPSTime
    operator+(const TimeDelta& lhs, const GPSTime& rhs)
    {
        auto out = rhs;
        out += lhs;
        return out;
    }

    /** Subtract a TimeDelta from a GPSTime. */
    [[nodiscard]] friend constexpr GPSTime
    operator-(const GPSTime& lhs, const TimeDelta& rhs)
    {
        auto out = lhs;
        out -= rhs;
        return out;
    }

    /** Compute the difference between two GPSTime objects. */
    [[nodiscard]] friend constexpr TimeDelta
    operator-(const GPSTime& lhs, const GPSTime& rhs)
    {
        return TimeDelta(lhs.time_point_ - rhs.time_point_);
    }

    /** Compare two GPSTime objects. */
    [[nodiscard]] friend constexpr bool
    operator==(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return lhs.time_point_ == rhs.time_point_;
    }

    /** \copydoc operator==(const GPSTime&, const GPSTime&) */
    [[nodiscard]] friend constexpr bool
    operator!=(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return not(lhs == rhs);
    }

    /** \copydoc operator==(const GPSTime&, const GPSTime&) */
    [[nodiscard]] friend constexpr bool
    operator<(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return lhs.time_point_ < rhs.time_point_;
    }

    /** \copydoc operator==(const GPSTime&, const GPSTime&) */
    [[nodiscard]] friend constexpr bool
    operator>(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return lhs.time_point_ > rhs.time_point_;
    }

    /** \copydoc operator==(const GPSTime&, const GPSTime&) */
    [[nodiscard]] friend constexpr bool
    operator<=(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return not(lhs > rhs);
    }

    /** \copydoc operator==(const GPSTime&, const GPSTime&) */
    [[nodiscard]] friend constexpr bool
    operator>=(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return not(lhs < rhs);
    }

    /** Serialize a string representation to an output stream. */
    friend std::ostream&
    operator<<(std::ostream& os, const GPSTime& t)
    {
        return os << std::string(t);
    }

private:
    TimePoint time_point_;
};

constexpr GPSTime::GPSTime(int year,
                           int month,
                           int day,
                           int hour,
                           int minute,
                           int second,
                           int microsecond,
                           int picosecond)
    : time_point_([=]() {
          // Get the last day in the specified year/month.
          const auto last_day_in_month = [](int y, int m) {
              const auto yy = date::year(y);
              const auto mm = date::month(static_cast<unsigned>(m));
              const auto mdl = date::month_day_last(mm);
              const auto ymdl = date::year_month_day_last(yy, mdl);
              const date::day d = ymdl.day();
              return static_cast<int>(unsigned(d));
          };

          if (year < 1 or year > 9999) {
              throw std::invalid_argument("invalid year");
          }
          if (month < 1 or month > 12) {
              throw std::invalid_argument("invalid month");
          }
          if (day < 1 or day > last_day_in_month(year, month)) {
              throw std::invalid_argument("invalid day");
          }
          if (hour < 0 or hour >= 24) {
              throw std::invalid_argument("invalid hour");
          }
          if (minute < 0 or minute >= 60) {
              throw std::invalid_argument("invalid minute");
          }
          if (second < 0 or second >= 60) {
              throw std::invalid_argument("invalid second");
          }
          if (microsecond < 0 or microsecond >= 1'000'000) {
              throw std::invalid_argument("invalid microsecond");
          }
          if (picosecond < 0 or picosecond >= 1'000'000) {
              throw std::invalid_argument("invalid picosecond");
          }

          const auto y = date::year(year);
          const auto m = date::month(static_cast<unsigned>(month));
          const auto d = date::day(static_cast<unsigned>(day));
          const auto ymd = date::year_month_day(y, m, d);

          // Convert calendar date to days since Unix epoch (1970-01-01).
          const auto unix_days = date::local_days(ymd);

          // Convert to days since GPS epoch (1980-01-06).
          const auto gps_days = date::clock_cast<Clock>(unix_days);

          using Hours = std::chrono::duration<Rep, std::ratio<3600>>;
          using Minutes = std::chrono::duration<Rep, std::ratio<60>>;
          using Seconds = std::chrono::duration<Rep>;
          using Microsecs = std::chrono::duration<Rep, std::micro>;
          using Picosecs = std::chrono::duration<Rep, std::pico>;

          // Convert time of day to duration since midnight.
          const auto since_midnight = Hours(hour) + Minutes(minute) +
                                      Seconds(second) + Microsecs(microsecond) +
                                      Picosecs(picosecond);

          return TimePoint(gps_days) + since_midnight;
      }())
{}

} // namespace caesar
