#include "datetime.hpp"

#include <cstdio>
#include <fmt/core.h>
#include <regex>

namespace caesar {

DateTime::DateTime(int year,
                   int month,
                   int day,
                   int hour,
                   int minute,
                   int second,
                   int millisecond,
                   int microsecond,
                   int nanosecond,
                   int picosecond)
    : components_{year,   month,       day,         hour,       minute,
                  second, millisecond, microsecond, nanosecond, picosecond}
{
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
    if (millisecond < 0 or millisecond >= 1000) {
        throw std::invalid_argument("invalid millisecond");
    }
    if (microsecond < 0 or microsecond >= 1000) {
        throw std::invalid_argument("invalid microsecond");
    }
    if (nanosecond < 0 or nanosecond >= 1000) {
        throw std::invalid_argument("invalid nanosecond");
    }
    if (picosecond < 0 or picosecond >= 1000) {
        throw std::invalid_argument("invalid picosecond");
    }
}

DateTime::DateTime(std::string_view datetime_str)
    : DateTime([=]() {
          const auto pattern =
                  std::regex(R"((\d{4}-\d{2}-\d{2}))" // YYYY-MM-DD
                             "(T| )"                  // date/time separator
                             R"((\d{2}:\d{2}:\d{2}))" // hh:mm:ss
                             R"((\.(\d{1,12}))?)");   // optional sub-seconds

          // Use a regex to check input string format and get subexpressions.
          const char* data = datetime_str.data();
          std::match_results<const char*> match;
          if (not std::regex_match(data, match, pattern)) {
              throw std::invalid_argument("bad datetime string format");
          }

          // Parse date components.
          int y = 0, mo = 0, d = 0;
          std::sscanf(match[1].first, "%4d-%2d-%2d", &y, &mo, &d);

          // Parse hour, minute, and second components.
          int h = 0, mi = 0, s = 0;
          std::sscanf(match[3].first, "%2d:%2d:%2d", &h, &mi, &s);

          // Append zeros to the input string up to the specified length.
          const auto zeropad = [](const auto& str, size_t length) {
              std::string out = str;
              out.resize(length, '0');
              return out;
          };

          // Parse sub-seconds components.
          int ms = 0, us = 0, ns = 0, ps = 0;
          if (match[5].length() > 0) {
              const std::string ss_str = zeropad(match[5], 12);
              std::sscanf(ss_str.data(), "%3d%3d%3d%3d", &ms, &us, &ns, &ps);
          }

          return DateTime(y, mo, d, h, mi, s, ms, us, ns, ps);
      }())
{}

DateTime::operator std::string() const
{
    // Format datetime to string, excluding sub-second components.
    const std::string ymdhms_str =
            fmt::format("{:04d}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}", year(),
                        month(), day(), hour(), minute(), second());

    const int ms = millisecond();
    const int us = microsecond();
    const int ns = nanosecond();
    const int ps = picosecond();

    // Early exit if sub-second components are zero.
    if (ms == 0 and us == 0 and ns == 0 and ps == 0) {
        return ymdhms_str;
    }

    // Format sub-second components to string.
    const std::string ss_str =
            fmt::format("{:03d}{:03d}{:03d}{:03d}", ms, us, ns, ps);

    // Strip trailing zeros from string.
    const auto trim = [](const std::string& str) {
        const auto n = str.find_last_not_of('0');
        if (n == std::string::npos) {
            return str;
        } else {
            return str.substr(0, n + 1);
        }
    };

    return fmt::format("{}.{}", ymdhms_str, trim(ss_str));
}

} // namespace caesar
