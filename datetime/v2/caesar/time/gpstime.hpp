#pragma once

#include "datetime.hpp"
#include "timedelta.hpp"

#include <date/tz.h>

namespace caesar {

/// A [DateTime]() in Global Positioning System (GPS) time
///
/// The GPS time system is an atomic time scale implemented by GPS satellites
/// and ground stations. Unlike UTC time, GPS time is a continuous linear time
/// scale -- leap seconds are never inserted. Therefore, the offset between GPS
/// and UTC time is not fixed but rather changes each time a leap second
/// adjustment is made to UTC.
///
/// ### See also
/// [DateTime]()
/// [TimePoint]()
/// [UTCTime]()
class GPSTime : public DateTime {
    using Base = DateTime;

public:
    using Base::Base;
    using Base::operator=;

    /// A [*Clock*](https://en.cppreference.com/w/cpp/named_req/Clock)
    /// representing the time system on which GPSTime is measured
    using Clock = date::gps_clock;

    /// Return the current time in GPS time.
    [[nodiscard]] static GPSTime
    now()
    {
        return GPSTime(Clock::now());
    }

    /// \group comparison Comparison operators
    ///
    /// Compare two GPSTime objects.
    [[nodiscard]] friend constexpr bool
    operator==(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return Base(lhs) == Base(rhs);
    }

    /// \group comparison
    [[nodiscard]] friend constexpr bool
    operator!=(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return not(lhs == rhs);
    }

    /// \group comparison
    [[nodiscard]] friend constexpr bool
    operator<(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return Base(lhs) < Base(rhs);
    }

    /// \group comparison
    [[nodiscard]] friend constexpr bool
    operator>(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return Base(lhs) > Base(rhs);
    }

    /// \group comparison
    [[nodiscard]] friend constexpr bool
    operator<=(const GPSTime& lhs, const GPSTime& rhs) noexcept
    {
        return not(lhs > rhs);
    }

    /// \group comparison
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
};

} // namespace caesar
