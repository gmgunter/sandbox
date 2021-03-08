#include <caesar/datetime.hpp>

#include <doctest/doctest.h>
#include <sstream>
#include <stdexcept>

namespace cs = caesar;

TEST_CASE("datetime.gpstime.from_components")
{
    SUBCASE("after_epoch")
    {
        int Y = 2001, M = 2, D = 3;
        int h = 4, m = 5, s = 6;
        int us = 7, ps = 8;

        const auto t = cs::GPSTime(Y, M, D, h, m, s, us, ps);

        CHECK_EQ(t.year(), Y);
        CHECK_EQ(t.month(), M);
        CHECK_EQ(t.day(), D);
        CHECK_EQ(t.hour(), h);
        CHECK_EQ(t.minute(), m);
        CHECK_EQ(t.second(), s);
        CHECK_EQ(t.microsecond(), us);
        CHECK_EQ(t.picosecond(), ps);
    }

    SUBCASE("before_epoch")
    {
        int Y = 900, M = 8, D = 7;
        int h = 6, m = 5, s = 4;
        int us = 3, ps = 2;

        const auto t = cs::GPSTime(Y, M, D, h, m, s, us, ps);

        CHECK_EQ(t.year(), Y);
        CHECK_EQ(t.month(), M);
        CHECK_EQ(t.day(), D);
        CHECK_EQ(t.hour(), h);
        CHECK_EQ(t.minute(), m);
        CHECK_EQ(t.second(), s);
        CHECK_EQ(t.microsecond(), us);
        CHECK_EQ(t.picosecond(), ps);
    }

    const auto check_bad_gpstime = [](int Y, int M, int D, int h, int m, int s,
                                      int us, int ps) {
        CHECK_THROWS_AS({ cs::GPSTime(Y, M, D, h, m, s, us, ps); },
                        std::invalid_argument);
    };

    SUBCASE("invalid_year")
    {
        check_bad_gpstime(0, 1, 1, 0, 0, 0, 0, 0);
        check_bad_gpstime(10000, 1, 1, 0, 0, 0, 0, 0);
    }

    SUBCASE("invalid_month")
    {
        check_bad_gpstime(2000, 0, 1, 0, 0, 0, 0, 0);
        check_bad_gpstime(2000, 13, 1, 0, 0, 0, 0, 0);
    }

    SUBCASE("invalid_day")
    {
        check_bad_gpstime(2000, 1, 0, 0, 0, 0, 0, 0);
        check_bad_gpstime(2000, 1, 32, 0, 0, 0, 0, 0);
        check_bad_gpstime(2000, 2, 30, 0, 0, 0, 0, 0);
        check_bad_gpstime(2001, 2, 29, 0, 0, 0, 0, 0);
        check_bad_gpstime(2000, 4, 31, 0, 0, 0, 0, 0);
    }

    SUBCASE("invalid_hour")
    {
        check_bad_gpstime(2000, 1, 1, -1, 0, 0, 0, 0);
        check_bad_gpstime(2000, 1, 1, 24, 0, 0, 0, 0);
    }

    SUBCASE("invalid_minute")
    {
        check_bad_gpstime(2000, 1, 1, 0, -1, 0, 0, 0);
        check_bad_gpstime(2000, 1, 1, 0, 60, 0, 0, 0);
    }

    SUBCASE("invalid_second")
    {
        check_bad_gpstime(2000, 1, 1, 0, 0, -1, 0, 0);
        check_bad_gpstime(2000, 1, 1, 0, 0, 60, 0, 0);
    }

    SUBCASE("invalid_microsecond")
    {
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, -1, 0);
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, 1'000'000, 0);
    }

    SUBCASE("invalid_picosecond")
    {
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, 0, -1);
        check_bad_gpstime(2000, 1, 1, 0, 0, 0, 0, 1'000'000);
    }
}

TEST_CASE("datetime.gpstime.from_time_point")
{
    // A std::chrono::time_point representing one hour, two minutes, and three
    // seconds after midnight on 1980-01-06 (the GPS epoch).
    const auto gps_epoch = cs::GPSTime::TimePoint() + std::chrono::hours(1) +
                           std::chrono::minutes(2) + std::chrono::seconds(3);

    const auto t = cs::GPSTime(gps_epoch);

    CHECK_EQ(t.year(), 1980);
    CHECK_EQ(t.month(), 1);
    CHECK_EQ(t.day(), 6);
    CHECK_EQ(t.hour(), 1);
    CHECK_EQ(t.minute(), 2);
    CHECK_EQ(t.second(), 3);
    CHECK_EQ(t.microsecond(), 0);
    CHECK_EQ(t.picosecond(), 0);
}

TEST_CASE("datetime.gpstime.from_string")
{
    SUBCASE("from_chars")
    {
        const auto s = "2001-02-03T04:05:06.789";
        const auto t = cs::GPSTime(s);

        CHECK_EQ(t.year(), 2001);
        CHECK_EQ(t.month(), 2);
        CHECK_EQ(t.day(), 3);
        CHECK_EQ(t.hour(), 4);
        CHECK_EQ(t.minute(), 5);
        CHECK_EQ(t.second(), 6);
        CHECK_EQ(t.microsecond(), 789'000);
        CHECK_EQ(t.picosecond(), 0);
    }

    SUBCASE("from_string")
    {
        const std::string s = "2001-02-03T04:05:06.000007000008";
        const auto t = cs::GPSTime(s);

        CHECK_EQ(t.year(), 2001);
        CHECK_EQ(t.month(), 2);
        CHECK_EQ(t.day(), 3);
        CHECK_EQ(t.hour(), 4);
        CHECK_EQ(t.minute(), 5);
        CHECK_EQ(t.second(), 6);
        CHECK_EQ(t.microsecond(), 7);
        CHECK_EQ(t.picosecond(), 8);
    }

    SUBCASE("no_subseconds")
    {
        const auto s = "2001-02-03T04:05:06";
        const auto t = cs::GPSTime(s);

        CHECK_EQ(t.year(), 2001);
        CHECK_EQ(t.month(), 2);
        CHECK_EQ(t.day(), 3);
        CHECK_EQ(t.hour(), 4);
        CHECK_EQ(t.minute(), 5);
        CHECK_EQ(t.second(), 6);
        CHECK_EQ(t.microsecond(), 0);
        CHECK_EQ(t.picosecond(), 0);
    }

    SUBCASE("bad_format")
    {
        const auto s = "asdf";
        CHECK_THROWS_AS({ cs::GPSTime{s}; }, std::invalid_argument);
    }

    SUBCASE("too_many_digits")
    {
        // Only up to 12 digits are allowed for subsecond components.
        const std::string s = "2001-02-03T04:05:06.0000000000001";
        CHECK_THROWS_AS({ cs::GPSTime{s}; }, std::invalid_argument);
    }
}

TEST_CASE("datetime.gpstime.to_string")
{
    {
        const auto t = cs::GPSTime(2000, 1, 2, 3, 4, 5, 6, 7);
        CHECK_EQ(std::string(t), "2000-01-02T03:04:05.000006000007");
    }

    {
        const auto t = cs::GPSTime(2000, 1, 2, 3, 4, 5);
        CHECK_EQ(std::string(t), "2000-01-02T03:04:05");
    }

    {
        const auto t = cs::GPSTime(2000, 1, 2, 3, 4, 5, 678'900);
        CHECK_EQ(std::string(t), "2000-01-02T03:04:05.6789");
    }
}

TEST_CASE("datetime.gpstime.min")
{
    const auto t = cs::GPSTime::min();

    CHECK_EQ(t.year(), 1);
    CHECK_EQ(t.month(), 1);
    CHECK_EQ(t.day(), 1);
    CHECK_EQ(t.hour(), 0);
    CHECK_EQ(t.minute(), 0);
    CHECK_EQ(t.second(), 0);
    CHECK_EQ(t.microsecond(), 0);
    CHECK_EQ(t.picosecond(), 0);
}

TEST_CASE("datetime.gpstime.max")
{
    const auto t = cs::GPSTime::max();

    CHECK_EQ(t.year(), 9999);
    CHECK_EQ(t.month(), 12);
    CHECK_EQ(t.day(), 31);
    CHECK_EQ(t.hour(), 23);
    CHECK_EQ(t.minute(), 59);
    CHECK_EQ(t.second(), 59);
    CHECK_EQ(t.microsecond(), 999'999);
    CHECK_EQ(t.picosecond(), 999'999);
}

TEST_CASE("datetime.gpstime.resolution")
{
    const cs::TimeDelta resolution = cs::GPSTime::resolution();
    const cs::TimeDelta ps = cs::TimeDelta::picoseconds(1);

    CHECK_EQ(resolution, ps);
}

TEST_CASE("datetime.gpstime.now")
{
    // Not much we can test here. now() must be some time after the day this
    // test was written.
    const auto t = cs::GPSTime::now();
    const auto today = cs::GPSTime(2021, 3, 6, 0, 0, 0);

    CHECK_GE(t, today);
}

TEST_CASE("datetime.gpstime.date")
{
    const auto t = cs::GPSTime(2001, 2, 3, 4, 5, 6, 7, 8);
    const date::year_month_day date = t.date();

    CHECK_EQ(date.year(), date::year(2001));
    CHECK_EQ(date.month(), date::month(2));
    CHECK_EQ(date.day(), date::day(3));
}

TEST_CASE("datetime.gpstime.time_of_day")
{
    const auto t = cs::GPSTime(2001, 2, 3, 4, 5, 6, 7, 8);
    const auto tod = t.time_of_day();

    CHECK_EQ(tod.hours(), std::chrono::hours(4));
    CHECK_EQ(tod.minutes(), std::chrono::minutes(5));
    CHECK_EQ(tod.seconds(), std::chrono::seconds(6));

    const auto subseconds = decltype(tod)::precision(7'000'008);
    CHECK_EQ(tod.subseconds(), subseconds);
}

TEST_CASE("datetime.gpstime.increment")
{
    cs::GPSTime t(2001, 1, 1, 0, 0, 0);

    SUBCASE("prefix_increment")
    {
        CHECK_EQ((++t).picosecond(), 1);
        CHECK_EQ(t.picosecond(), 1);
    }

    SUBCASE("postfix_increment")
    {
        CHECK_EQ((t++).picosecond(), 0);
        CHECK_EQ(t.picosecond(), 1);
    }
}

TEST_CASE("datetime.gpstime.decrement")
{
    auto t = cs::GPSTime(2001, 1, 1, 0, 0, 0);

    SUBCASE("prefix_decrement")
    {
        CHECK_EQ((--t).picosecond(), 999'999);
        CHECK_EQ(t.picosecond(), 999'999);
    }

    SUBCASE("postfix_decrement")
    {
        CHECK_EQ((t--).picosecond(), 0);
        CHECK_EQ(t.picosecond(), 999'999);
    }
}

TEST_CASE("datetime.gpstime.add_timedelta")
{
    auto t = cs::GPSTime(2000, 1, 2, 3, 4, 5, 6, 7);
    const auto dt = cs::TimeDelta::days(12) + cs::TimeDelta::minutes(34) +
                    cs::TimeDelta::seconds(56) +
                    cs::TimeDelta::microseconds(78) +
                    cs::TimeDelta::picoseconds(90);

    const auto sum = cs::GPSTime(2000, 1, 14, 3, 39, 1, 84, 97);

    SUBCASE("compound_assignment")
    {
        t += dt;
        CHECK_EQ(t, sum);
    }

    SUBCASE("binary_operator") { CHECK_EQ(t + dt, sum); }
}

TEST_CASE("datetime.gpstime.subtract_timedelta")
{
    auto t = cs::GPSTime(2001, 2, 3, 4, 5, 6, 7, 8);
    const auto dt = cs::TimeDelta::days(12) + cs::TimeDelta::minutes(34) +
                    cs::TimeDelta::seconds(56) +
                    cs::TimeDelta::microseconds(78) +
                    cs::TimeDelta::picoseconds(90);

    const auto diff = cs::GPSTime(2001, 1, 22, 3, 30, 9, 999'928, 999'918);

    SUBCASE("compound_assignment")
    {
        t -= dt;
        CHECK_EQ(t, diff);
    }

    SUBCASE("binary_operator") { CHECK_EQ(t - dt, diff); }
}

TEST_CASE("datetime.gpstime.subtract_gpstime")
{
    const auto t1 = cs::GPSTime(2001, 2, 3, 4, 5, 6, 7, 8);
    const auto t2 = cs::GPSTime(2001, 1, 22, 3, 30, 9, 999'928, 999'918);

    const auto dt = cs::TimeDelta::days(12) + cs::TimeDelta::minutes(34) +
                    cs::TimeDelta::seconds(56) +
                    cs::TimeDelta::microseconds(78) +
                    cs::TimeDelta::picoseconds(90);

    CHECK_EQ(t2 - t1, -dt);
    CHECK_EQ(t1 - t2, dt);
}

TEST_CASE("datetime.gpstime.compare_eq")
{
    const auto t1 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t3 = cs::GPSTime(2000, 1, 1, 0, 0, 0, 0, 1);

    CHECK(t1 == t1);
    CHECK(t1 == t2);
    CHECK_FALSE(t1 == t3);
}

TEST_CASE("datetime.gpstime.compare_ne")
{
    const auto t1 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::GPSTime(2000, 1, 1, 0, 0, 0, 0, 1);
    const auto t3 = cs::GPSTime(2000, 1, 1, 0, 0, 0, 0, 1);

    CHECK(t1 != t2);
    CHECK_FALSE(t2 != t3);
}

TEST_CASE("datetime.gpstime.compare_lt")
{
    const auto t1 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t3 = cs::GPSTime(2000, 1, 1, 0, 0, 0, 0, 1);
    const auto t4 = cs::GPSTime(1999, 12, 31, 23, 59, 59);

    CHECK(t1 < t3);
    CHECK(t4 < t1);
    CHECK_FALSE(t1 < t2);
    CHECK_FALSE(t3 < t4);
}

TEST_CASE("datetime.gpstime.compare_gt")
{
    const auto t1 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t3 = cs::GPSTime(2000, 1, 1, 0, 0, 0, 0, 1);
    const auto t4 = cs::GPSTime(1999, 12, 31, 23, 59, 59);

    CHECK(t3 > t1);
    CHECK(t1 > t4);
    CHECK_FALSE(t1 > t2);
    CHECK_FALSE(t4 > t3);
}

TEST_CASE("datetime.gpstime.compare_le")
{
    const auto t1 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t3 = cs::GPSTime(2000, 1, 1, 0, 0, 0, 0, 1);
    const auto t4 = cs::GPSTime(1999, 12, 31, 23, 59, 59);

    CHECK(t1 <= t3);
    CHECK(t4 <= t1);
    CHECK(t1 <= t2);
    CHECK_FALSE(t3 <= t4);
}

TEST_CASE("datetime.gpstime.compare_ge")
{
    const auto t1 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t2 = cs::GPSTime(2000, 1, 1, 0, 0, 0);
    const auto t3 = cs::GPSTime(2000, 1, 1, 0, 0, 0, 0, 1);
    const auto t4 = cs::GPSTime(1999, 12, 31, 23, 59, 59);

    CHECK(t3 >= t1);
    CHECK(t1 >= t4);
    CHECK(t1 >= t2);
    CHECK_FALSE(t4 >= t3);
}

TEST_CASE("datetime.gpstime.to_stream")
{
    const auto t = cs::GPSTime(2000, 1, 2, 3, 4, 5, 678'000);
    std::ostringstream ss;
    ss << t;
    CHECK_EQ(ss.str(), "2000-01-02T03:04:05.678");
}
