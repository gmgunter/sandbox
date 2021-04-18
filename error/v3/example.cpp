#include <caesar/error.hpp>

#include <cstdio>
#include <regex>
#include <stdexcept>
#include <string_view>

namespace cs = caesar;

struct Date {
    int year;
    int month;
    int day;
};

class InvalidArgument : public cs::Error {
public:
    constexpr InvalidArgument(const char* description,
                              const cs::SourceLocation& location =
                                      cs::SourceLocation::current()) noexcept
        : description_{description}, Error{location}
    {}

    [[nodiscard]] std::string
    description() const
    {
        return description_;
    }

    [[noreturn]] void
    throw_exception() const
    {
        throw std::invalid_argument(error_message());
    }

private:
    const char* description_;
};

class BadDateFormat : public cs::Error {
public:
    constexpr BadDateFromat(const cs::SourceLocation& location =
                                    cs::SourceLocation::current()) noexcept
        : Error{location}
    {}

    [[nodiscard]] std::string
    description() const
    {
        return "Invalid date string format";
    }
};

cs::Expected<Date, InvalidArgument>
make_date(int year, int month, int day)
{
    if (year < 1 or year > 9999) {
        return InvalidArgument{"Invalid year"};
    }
    if (month < 1 or month > 12) {
        return InvalidArgument{"Invalid month"};
    }
    if (day < 1 or day > 31) {
        return InvalidArgument{"Invalid day"};
    }
    return Date{year, month, day};
}

cs::Expected<Date, BadDateFormat, InvalidArgument>
make_date(std::string_view date_str)
{
    const auto pattern = std::regex{R"(\d{4}-\d{2}-\d{2})"};

    std::match_results<const char*> match;
    if (not std::regex_match(date_str.data(), match, pattern)) {
        return BadDateFormat{};
    }

    int year, month, day;
    std::sscanf(match[0].first, "%4d-%2d-%2d", &year, &month, &day);

    return make_date(year, month, day);
}

int
main()
{
    {
        const auto date = make_date(2000, 1, 2);
        if (not date) {
            InvalidArgument error = date.error();
            error.throw_exception();
        }

        std::cout << "year: " << date->year << "month: " << date->month
                  << "day: " << date->day << std::endl;
    }

    {
        const auto date = make_date("asdfadsf");
        if (date.has_error<BadDateFormat>()) {
            BadDateFormat error = date.error();
            error.throw_exception();
        }
        if (date.has_error<InvalidArgument>()) {
            InvalidArgument error = date.error();
            error.throw_exception();
        }
    }

    {

        struct Visitor {
            operator()(const Date& date) const
            {
                std::cout << "year: " << date->year << "month: " << date->month
                          << "day: " << date->day << std::endl;
            }

            operator()(const InvalidArgument& error)
            {
                error.throw_exception();
            }

            operator()(const BadDateFormat& error) { error.throw_exception(); }
        };

        const auto date = make_date("2000-01-02");
        cs::visit(Visitor{}, date);
    }

    return 0;
}
