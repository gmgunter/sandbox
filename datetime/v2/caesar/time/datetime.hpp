#pragma once

#include <array>
#include <chrono>
#include <date/date.h>
#include <iostream>
#include <range/v3/algorithm.hpp>
#include <ratio>
#include <string>
#include <string_view>

namespace caesar {

/// A "broken-down" date & time representation with picosecond resolution
///
/// A DateTime object consists of date and time-of-day components. The date
/// components follow the proleptic Gregorian calendar. Dates before year 1 or
/// after year 9999 may not be represented. The time components use a 24-hour
/// clock system.
///
/// The DateTime class provides an interface for constructing and inspecting
/// datetimes in terms of human-interpretable calendar and clock components. The
/// derived classes [GPSTime]() and [UTCTime]() represent datetimes in specific
/// time standards.
///
/// string formatting and parsing operations
///
/// ### See also
/// [TimePoint]()
/// [GPSTime]()
/// [UTCTime]()
class DateTime {
public:
    /// Construct a new DateTime object.
    ///
    /// \param[in] year          year component
    /// \param[in] month         month component, encoded 1 through 12
    /// \param[in] day           day component
    /// \param[in] hour          hour component, in the range [0, 24)
    /// \param[in] minute        minute component, in the range [0, 60)
    /// \param[in] second        second component, in the range [0, 60)
    /// \param[in] millisecond   millisecond component, in the range [0, 1000)
    /// \param[in] microsecond   microsecond component, in the range [0, 1000)
    /// \param[in] nanosecond    nanosecond component, in the range [0, 1000)
    /// \param[in] picosecond    picosecond component, in the range [0, 1000)
    ///
    /// \throws std::invalid_argument if any component is outside the valid
    /// range
    DateTime(int year,
             int month,
             int day,
             int hour,
             int minute,
             int second,
             int millisecond = 0,
             int microsecond = 0,
             int nanosecond = 0,
             int picosecond = 0);

    /// Construct a new DateTime object from a string representation.
    ///
    /// The input is expected to conform to ISO 8601 specifications for
    /// combined date and time representations.
    ///
    /// ```ebnf
    /// <datetime> ::= <date> ("T" | " ") <time>
    /// <date> ::= <year> "-" <month> "-" <day>
    /// <time> ::= <hour> ":" <minute> ":" <second> ["." <subsecond>]
    /// ```
    ///
    /// The `<year>` component should be exactly four digits. Each of `<month>`,
    /// `<day>`, `<hour>`, `<minute>`, and `<second>` should consist of exactly
    /// two digits. The `<subsecond>` component, if present, should contain at
    /// least one but no more than twelve digits.
    ///
    /// Examples of valid string representations include:
    /// ```cpp
    /// "0001-01-01T00:00:00"
    /// "2000-01-02 03:04:05.6789"
    /// "2020-03-20T23:50:59.50"
    /// "9999-12-31 23:59:59.999999999999"
    /// ```
    ///
    /// \throws std::invalid_argument if the input string does not match the
    /// expected format
    /// \throws std::invalid_argument if any component is outside the valid
    /// range
    explicit DateTime(std::string_view datetime_str);

    /// Return a string representation of the DateTime in ISO 8601 format.
    [[nodiscard]] explicit operator std::string() const;

    /// Return the earliest valid DateTime.
    [[nodiscard]] static DateTime
    min() noexcept
    {
        return DateTime(1, 1, 1, 0, 0, 0);
    }

    /// Return the latest valid DateTime.
    [[nodiscard]] static DateTime
    max() noexcept
    {
        return DateTime(9999, 12, 31, 23, 59, 59, 999, 999, 999, 999);
    }

    /// \group components Component access
    ///
    /// Get the stored values of individual date/time components.
    ///
    /// ### Return value
    /// 1\) year component
    /// 2\) month component, encoded 1 through 12
    /// 3\) day component
    /// 4\) hour component
    /// 5\) minute component
    /// 6\) second component
    /// 7\) millisecond component
    /// 8\) microsecond component
    /// 9\) nanosecond component
    /// 10\) picosecond component
    [[nodiscard]] constexpr int
    year() const noexcept
    {
        return components_[0];
    }

    /// \group components
    [[nodiscard]] constexpr int
    month() const noexcept
    {
        return components_[1];
    }

    /// \group components
    [[nodiscard]] constexpr int
    day() const noexcept
    {
        return components_[2];
    }

    /// \group components
    [[nodiscard]] constexpr int
    hour() const noexcept
    {
        return components_[3];
    }

    /// \group components
    [[nodiscard]] constexpr int
    minute() const noexcept
    {
        return components_[4];
    }

    /// \group components
    [[nodiscard]] constexpr int
    second() const noexcept
    {
        return components_[5];
    }

    /// \group components
    [[nodiscard]] constexpr int
    millisecond() const noexcept
    {
        return components_[6];
    }

    /// \group components
    [[nodiscard]] constexpr int
    microsecond() const noexcept
    {
        return components_[7];
    }

    /// \group components
    [[nodiscard]] constexpr int
    nanosecond() const noexcept
    {
        return components_[8];
    }

    /// \group components
    [[nodiscard]] constexpr int
    picosecond() const noexcept
    {
        return components_[9];
    }

    /// Return the calendar date.
    [[nodiscard]] constexpr date::year_month_day
    date() const
    {
        const auto y = date::year(year());
        const auto m = date::month(static_cast<unsigned>(month()));
        const auto d = date::day(static_cast<unsigned>(day()));
        return {y, m, d};
    }

    /// Return the day of the week.
    [[nodiscard]] constexpr Weekday
    weekday() const
    {
        // Convert calendar date to days since Unix epoch (1970-01-01).
        const auto d = date::sys_days(date());

        // Get day of week.
        return Weekday(d);
    }

    /// \group comparison Comparison operators
    ///
    /// Compare two DateTime objects.
    [[nodiscard]] friend constexpr bool
    operator==(const DateTime& lhs, const DateTime& rhs) noexcept
    {
        return ranges::equal(lhs.components(), rhs.components());
    }

    /// \group comparison
    [[nodiscard]] friend constexpr bool
    operator!=(const DateTime& lhs, const DateTime& rhs) noexcept
    {
        return not(lhs == rhs);
    }

    /// \group comparison
    [[nodiscard]] friend bool
    operator<(const DateTime& lhs, const DateTime& rhs) noexcept
    {
        return ranges::lexicographical_compare(
                lhs.components(), rhs.components(), ranges::less{});
    }

    /// \group comparison
    [[nodiscard]] friend bool
    operator>(const DateTime& lhs, const DateTime& rhs) noexcept
    {
        return ranges::lexicographical_compare(
                lhs.components(), rhs.components(), ranges::greater{});
    }

    /// \group comparison
    [[nodiscard]] friend bool
    operator<=(const DateTime& lhs, const DateTime& rhs) noexcept
    {
        return not(lhs > rhs);
    }

    /// \group comparison
    [[nodiscard]] friend bool
    operator>=(const DateTime& lhs, const DateTime& rhs) noexcept
    {
        return not(lhs < rhs);
    }

    /// Insert a textual representation of a DateTime object into an output
    /// stream.
    friend std::ostream&
    operator<<(std::ostream& os, const DateTime& t)
    {
        os << std::string(t);
        return os;
    }

protected:
    /// Return an array of datetime components, in order from most significant
    /// to least significant.
    [[nodiscard]] constexpr const std::array<int, 10>&
    components() const noexcept
    {
        return components_;
    }

private:
    std::array<int, 10> components_;
};

} // namespace caesar
