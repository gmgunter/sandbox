#include "timedelta.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <ios>
#include <string>
#include <tuple>

namespace caesar {

std::ostream&
operator<<(std::ostream& os, const TimeDelta& dt)
{
    // Returns the quotient and remainder of integer division of the arguments.
    const auto divmod = [](auto x, auto y) {
        const auto q = x / y;
        const auto r = x % y;
        return std::make_tuple(q, r);
    };

    // Insert sign prefix.
    if (dt < TimeDelta{}) {
        os << "-";
    } else if (os.flags() & std::ios::showpos) {
        os << "+";
    }

    // Get magnitude of input TimeDelta.
    const auto a = abs(dt);

    // Recursively split the TimeDelta into whole & fractional components of
    // temporal units in descending order of significance (days, hours,
    // minutes).
    TimeDelta::Rep whole = 0;
    TimeDelta::Rep frac = a.count();

    if (a >= TimeDelta::days(1)) {
        std::tie(whole, frac) = divmod(frac, TimeDelta::days(1).count());
        fmt::print(os, "{}d", whole);
    }
    if (a >= TimeDelta::hours(1)) {
        std::tie(whole, frac) = divmod(frac, TimeDelta::hours(1).count());
        fmt::print(os, "{}h", whole);
    }
    if (a >= TimeDelta::minutes(1)) {
        std::tie(whole, frac) = divmod(frac, TimeDelta::minutes(1).count());
        fmt::print(os, "{}m", whole);
    }

    // If the input TimeDelta was at least one second, any remaining duration
    // is represented in seconds (possibly as a decimal fraction). For smaller
    // durations, an appropriate SI-prefixed unit is chosen instead.
    const auto [period, digits, suffix] = [=]() {
        if (a >= TimeDelta::seconds(1)) {
            return std::make_tuple(TimeDelta::seconds(1), 12, "s");
        }
        if (a >= TimeDelta::milliseconds(1)) {
            return std::make_tuple(TimeDelta::milliseconds(1), 9, "ms");
        }
        if (a >= TimeDelta::microseconds(1)) {
            return std::make_tuple(TimeDelta::microseconds(1), 6, "us");
        }
        if (a >= TimeDelta::nanoseconds(1)) {
            return std::make_tuple(TimeDelta::nanoseconds(1), 3, "ns");
        }
        return std::make_tuple(TimeDelta::picoseconds(1), 0, "ps");
    }();

    // Strips trailing zeros from string.
    const auto trim = [](const std::string& str) {
        const auto n = str.find_last_not_of('0');
        if (n == std::string::npos) {
            return str;
        } else {
            return str.substr(0, n + 1);
        }
    };

    std::tie(whole, frac) = divmod(frac, period.count());
    fmt::print(os, "{}", whole);
    if (frac) {
        // Prepend zeros to the fractional component to get the correct number
        // of decimal digits. Then, strip any trailing zeros.
        const std::string str = fmt::format(".{:0>{}}", frac, digits);
        os << trim(str);
    } else if (os.flags() & std::ios::showpoint) {
        os << ".0";
    }
    os << suffix;

    return os;
}

} // namespace caesar
