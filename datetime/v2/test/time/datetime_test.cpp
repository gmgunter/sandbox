#include <caesar/time.hpp>

#include <doctest/doctest.h>
#include <sstream>
#include <stdexcept>

namespace cs = caesar;

TEST_CASE("time.datetime.from_components")
{
    SUBCASE("after_epoch")
    {
        int Y = 2000, M = 1, D = 2;
        int h = 3, m = 4, s = 5;
        int ms = 6, us = 7, ns = 8, ps = 9;

        const auto t = cs::DateTime(Y, M, D, h, m, s, ms, us, ns, ps);

        CHECK_EQ(t.year(), Y);
        CHECK_EQ(t.month(), M);
        CHECK_EQ(t.day(), D);
        CHECK_EQ(t.hour(), h);
        CHECK_EQ(t.minute(), m);
        CHECK_EQ(t.second(), s);
        CHECK_EQ(t.millisecond(), ms);
        CHECK_EQ(t.microsecond(), us);
        CHECK_EQ(t.nanosecond(), ns);
        CHECK_EQ(t.picosecond(), ps);
    }

    SUBCASE("before_epoch")
    {
        int Y = 900, M = 8, D = 7;
        int h = 6, m = 5, s = 4;
        int ms = 3, us = 2, ns = 1, ps = 0;

        const auto t = cs::DateTime(Y, M, D, h, m, s, ms, us, ns, ps);

        CHECK_EQ(t.year(), Y);
        CHECK_EQ(t.month(), M);
        CHECK_EQ(t.day(), D);
        CHECK_EQ(t.hour(), h);
        CHECK_EQ(t.minute(), m);
        CHECK_EQ(t.second(), s);
        CHECK_EQ(t.millisecond(), ms);
        CHECK_EQ(t.microsecond(), us);
        CHECK_EQ(t.nanosecond(), ns);
        CHECK_EQ(t.picosecond(), ps);
    }

    const auto check_bad_gpstime = [](int Y, int M, int D, int h, int m, int s,
                                      int ms, int us, int ns, int ps) {
        CHECK_THROWS_AS({ cs::DateTime(Y, M, D, h, m, s, ms, us, ns, ps); },
                        std::invalid_argument);
    };

    SUBCASE("invalid_year")
    {
        check_bad_gpstime(0, 1, 1, 0, 0, 0, 0, 0, 0, 0);
        check_bad_gpstime(10000, 1, 1, 0, 0, 0, 0, 0, 0, 0);
    }

    SUBCASE("invalid_month")
    {
        check_bad_gpstime(2000, 0, 1, 0, 0, 0, 0, 0, 0, 0);
        check_bad_gpstime(2000, 13, 1, 0, 0, 0, 0, 0, 0, 0);
    }

    SUBCASE("invalid_day")
    {
        check_bad_gpstime(2000, 1, 0, 0, 0, 0, 0, 0, 0, 0);
        check_bad_gpstime(2000, 1, 32, 0, 0, 0, 0, 0, 0, 0);
        check_bad_gpstime(2000, 2, 30, 0, 0, 0, 0, 0, 0, 0);
        check_bad_gpstime(2001, 2, 29, 0, 0, 0, 0, 0, 0, 0);
        check_bad_gpstime(2000, 4, 31, 0, 0, 0, 0, 0, 0, 0);
    }

    SUBCASE("invalid_hour")
    {
        check_bad_gpstime(2000, 1, 1, -1, 0, 0, 0, 0, 0, 0);
        check_bad_gpstime(2000, 1, 1, 24, 0, 0, 0, 0, 0, 0);
    }

    SUBCASE("invalid_minute")
    {
        check_bad_gpstime(2000, 1, 1, 0, -1, 0, 0, 0, 0, 0);
        check_bad_gpstime(2000, 1, 1, 0, 60, 0, 0, 0, 0, 0);
    }

    SUBCASE("invalid_second")
    {
        check_bad_gpstime(2000, 1, 1, 0, 0, -1, 0, 0, 0, 0);
        check_bad_gpstime(2000, 1, 1, 0, 0, 60, 0, 0, 0, 0);
    }

    SUBCASE("invalid_millisecond")
    {
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, -1, 0, 0, 0);
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, 1000, 0, 0, 0);
    }

    SUBCASE("invalid_microsecond")
    {
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, 0, -1, 0, 0);
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, 0, 1000, 0, 0);
    }

    SUBCASE("invalid_nanosecond")
    {
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, 0, 0, -1, 0);
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, 0, 0, 1000, 0);
    }

    SUBCASE("invalid_picosecond")
    {
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, 0, 0, 0, -1);
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, 0, 0, 0, 1000);
    }
}

TEST_CASE("time.datetime.from_string")
{
    SUBCASE("from_chars")
    {
        const auto s = "2001-02-03T04:05:06.789";
        const auto t = cs::DateTime(s);

        CHECK_EQ(t.year(), 2001);
        CHECK_EQ(t.month(), 2);
        CHECK_EQ(t.day(), 3);
        CHECK_EQ(t.hour(), 4);
        CHECK_EQ(t.minute(), 5);
        CHECK_EQ(t.second(), 6);
        CHECK_EQ(t.millisecond(), 789);
        CHECK_EQ(t.microsecond(), 0);
        CHECK_EQ(t.nanosecond(), 0);
        CHECK_EQ(t.picosecond(), 0);
    }

    SUBCASE("from_string")
    {
        const std::string s = "2000-01-02T03:04:05.006007008009";
        const auto t = cs::DateTime(s);

        CHECK_EQ(t.year(), 2000);
        CHECK_EQ(t.month(), 1);
        CHECK_EQ(t.day(), 2);
        CHECK_EQ(t.hour(), 3);
        CHECK_EQ(t.minute(), 4);
        CHECK_EQ(t.second(), 5);
        CHECK_EQ(t.millisecond(), 6);
        CHECK_EQ(t.microsecond(), 7);
        CHECK_EQ(t.nanosecond(), 8);
        CHECK_EQ(t.picosecond(), 9);
    }

    SUBCASE("integer_seconds")
    {
        const auto s = "2001-02-03T04:05:06";
        const auto t = cs::DateTime(s);

        CHECK_EQ(t.year(), 2001);
        CHECK_EQ(t.month(), 2);
        CHECK_EQ(t.day(), 3);
        CHECK_EQ(t.hour(), 4);
        CHECK_EQ(t.minute(), 5);
        CHECK_EQ(t.second(), 6);
        CHECK_EQ(t.millisecond(), 0);
        CHECK_EQ(t.microsecond(), 0);
        CHECK_EQ(t.nanosecond(), 0);
        CHECK_EQ(t.picosecond(), 0);
    }

    SUBCASE("whitespace_separator")
    {
        const auto s = "2001-02-03 04:05:06.78";
        const auto t = cs::DateTime(s);

        CHECK_EQ(t.year(), 2001);
        CHECK_EQ(t.month(), 2);
        CHECK_EQ(t.day(), 3);
        CHECK_EQ(t.hour(), 4);
        CHECK_EQ(t.minute(), 5);
        CHECK_EQ(t.second(), 6);
        CHECK_EQ(t.millisecond(), 780);
        CHECK_EQ(t.microsecond(), 0);
        CHECK_EQ(t.nanosecond(), 0);
        CHECK_EQ(t.picosecond(), 0);
    }

    SUBCASE("bad_format")
    {
        const auto s = "asdf";
        CHECK_THROWS_AS({ cs::DateTime{s}; }, std::invalid_argument);
    }

    SUBCASE("bad_separator")
    {
        const auto s = "2001-02-03*04:05:06.78";
        CHECK_THROWS_AS({ cs::DateTime{s}; }, std::invalid_argument);
    }

    SUBCASE("too_many_digits")
    {
        // Only up to 12 digits are allowed for subsecond components.
        const std::string s = "2001-02-03T04:05:06.0000000000001";
        CHECK_THROWS_AS({ cs::DateTime{s}; }, std::invalid_argument);
    }
}

TEST_CASE("time.datetime.to_string")
{
    {
        const auto t = cs::DateTime(2000, 1, 2, 3, 4, 5, 6, 7, 8, 9);
        CHECK_EQ(std::string(t), "2000-01-02T03:04:05.006007008009");
    }

    {
        const auto t = cs::DateTime(2000, 1, 2, 3, 4, 5);
        CHECK_EQ(std::string(t), "2000-01-02T03:04:05");
    }

    {
        const auto t = cs::DateTime(2000, 1, 2, 3, 4, 5, 678, 900);
        CHECK_EQ(std::string(t), "2000-01-02T03:04:05.6789");
    }
}

TEST_CASE("time.datetime.min")
{
    const auto t = cs::DateTime::min();

    CHECK_EQ(t.year(), 1);
    CHECK_EQ(t.month(), 1);
    CHECK_EQ(t.day(), 1);
    CHECK_EQ(t.hour(), 0);
    CHECK_EQ(t.minute(), 0);
    CHECK_EQ(t.second(), 0);
    CHECK_EQ(t.millisecond(), 0);
    CHECK_EQ(t.microsecond(), 0);
    CHECK_EQ(t.nanosecond(), 0);
    CHECK_EQ(t.picosecond(), 0);
}

TEST_CASE("time.datetime.max")
{
    const auto t = cs::DateTime::max();

    CHECK_EQ(t.year(), 9999);
    CHECK_EQ(t.month(), 12);
    CHECK_EQ(t.day(), 31);
    CHECK_EQ(t.hour(), 23);
    CHECK_EQ(t.minute(), 59);
    CHECK_EQ(t.second(), 59);
    CHECK_EQ(t.millisecond(), 999);
    CHECK_EQ(t.microsecond(), 999);
    CHECK_EQ(t.nanosecond(), 999);
    CHECK_EQ(t.picosecond(), 999);
}

TEST_CASE("time.datetime.date")
{
    const auto t = cs::DateTime(2001, 2, 3, 4, 5, 6, 7, 8);
    const date::year_month_day date = t.date();

    CHECK_EQ(date.year(), date::year(2001));
    CHECK_EQ(date.month(), date::month(2));
    CHECK_EQ(date.day(), date::day(3));
}

TEST_CASE("time.datetime.weekday")
{
    {
        const auto t = cs::DateTime(2021, 4, 3, 0, 0, 0);
        CHECK_EQ(t.weekday(), date::Saturday);
    }

    {
        const auto t = cs::DateTime(1969, 12, 31, 23, 59, 59, 999, 999, 999, 999);
        CHECK_EQ(t.weekday(), date::Wednesday);
    }

    {
        const auto t = cs::DateTime(1920, 2, 29, 0, 0, 0);
        CHECK_EQ(t.weekday(), date::Sunday);
    }

    {
        const auto t = cs::DateTime(1920, 3, 1, 0, 0, 0);
        CHECK_EQ(t.weekday(), date::Monday);
    }
}

TEST_CASE("time.datetime.compare_eq")
{
    const auto t1 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t3 = cs::DateTime(2000, 1, 1, 0, 0, 0, 0, 0, 0, 1);

    CHECK(t1 == t1);
    CHECK(t1 == t2);
    CHECK_FALSE(t1 == t3);
}

TEST_CASE("time.datetime.compare_ne")
{
    const auto t1 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::DateTime(2000, 1, 1, 0, 0, 0, 0, 0, 0, 1);
    const auto t3 = cs::DateTime(2000, 1, 1, 0, 0, 0, 0, 0, 0, 1);

    CHECK(t1 != t2);
    CHECK_FALSE(t2 != t3);
}

TEST_CASE("time.datetime.compare_lt")
{
    const auto t1 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t3 = cs::DateTime(2000, 1, 1, 0, 0, 0, 0, 1);
    const auto t4 = cs::DateTime(1999, 12, 31, 23, 59, 59);

    CHECK(t1 < t3);
    CHECK(t4 < t1);
    CHECK_FALSE(t1 < t2);
    CHECK_FALSE(t3 < t4);
}

TEST_CASE("time.datetime.compare_gt")
{
    const auto t1 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t3 = cs::DateTime(2000, 1, 1, 0, 0, 0, 0, 1);
    const auto t4 = cs::DateTime(1999, 12, 31, 23, 59, 59);

    CHECK(t3 > t1);
    CHECK(t1 > t4);
    CHECK_FALSE(t1 > t2);
    CHECK_FALSE(t4 > t3);
}

TEST_CASE("time.datetime.compare_le")
{
    const auto t1 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t3 = cs::DateTime(2000, 1, 1, 0, 0, 0, 0, 1);
    const auto t4 = cs::DateTime(1999, 12, 31, 23, 59, 59);

    CHECK(t1 <= t3);
    CHECK(t4 <= t1);
    CHECK(t1 <= t2);
    CHECK_FALSE(t3 <= t4);
}

TEST_CASE("time.datetime.compare_ge")
{
    const auto t1 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::DateTime(2000, 1, 1, 0, 0, 0);
    const auto t3 = cs::DateTime(2000, 1, 1, 0, 0, 0, 0, 1);
    const auto t4 = cs::DateTime(1999, 12, 31, 23, 59, 59);

    CHECK(t3 >= t1);
    CHECK(t1 >= t4);
    CHECK(t1 >= t2);
    CHECK_FALSE(t4 >= t3);
}

TEST_CASE("time.datetime.stream_insert")
{
    const auto t = cs::DateTime(2000, 1, 2, 3, 4, 5, 678, 900);
    std::ostringstream ss;
    ss << t;
    CHECK_EQ(ss.str(), "2000-01-02T03:04:05.6789");
}
