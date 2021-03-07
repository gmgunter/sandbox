#include <caesar/datetime.hpp>

#include <doctest/doctest.h>
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
    const auto min = cs::GPSTime(2021, 3, 6, 0, 0, 0);

    CHECK_GE(t, min);
}
