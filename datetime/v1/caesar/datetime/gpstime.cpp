#include "gpstime.hpp"

#include <cstdio>
#include <fmt/core.h>
#include <regex>
#include <stdexcept>

namespace caesar {

GPSTime::GPSTime(std::string_view datetime_string)
    : GPSTime([=]() {
          const auto pattern = std::regex(
                  R"((\d{4}-\d{2}-\d{2})T(\d{2}:\d{2}:\d{2})(\.(\d{1,12}))?)");

          std::match_results<const char*> match;
          if (not std::regex_match(datetime_string.data(), pattern)) {
              throw std::invalid_argument("bad datetime string");
          }

          // Parse date components.
          int y = 0, mo = 0, d = 0;
          std::sscanf(match[1].data(), "%4d-%2d-%2d", &y, &mo, &d);

          // Parse hour, minute, and second components.
          int h = 0, mi = 0, s = 0;
          std::sscanf(match[2].first, "%2d:%2d:%2d", &h, &mi, &s);

          // Append zeros to the input string up to the specified length.
          const auto zeropad = [](const auto& str, size_t length) {
              std::string out = str;
              out.resize(length, '0');
              return out;
          };

          // Parse sub-seconds components.
          int us = 0, ps = 0;
          if (match[4].length() > 0) {
              const std::string subseconds_string = zeropad(match[4], 12);
              std::sscanf(subseconds_string.data(), "%6d%6d", &us, &ps);
          }

          return GPSTime(y, mo, d, h, mi, s, us, ps);
      }())
{}

GPSTime::operator std::string() const
{
    // Format datetime to string, excluding sub-second components.
    const std::string ymdhms_string =
            fmt::format("{:04d}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}", year(),
                        month(), day(), hour(), minute(), second());

    const int us = microsecond();
    const int ps = picosecond();

    // Early exit if sub-second components are zero.
    if (us == 0 and ps == 0) {
        return ymdhms_string;
    }

    // Format sub-second components to string.
    const std::string subseconds_string = fmt::format("{:06d}{:06d}", us, ps);

    // Strip trailing zeros from string.
    const auto trim = [](const auto& str) {
        const auto n = str.find_last_not_of('0');
        if (n == std::string::npos) {
            return str;
        } else {
            return str.substr(0, n + 1);
        }
    };

    return fmt::format("{}.{}", ymdhms_string, trim(subseconds_string));
}

} // namespace caesar
