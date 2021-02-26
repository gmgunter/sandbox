#include <caesar/spheroid.hpp>

#include <cmath>
#include <gtest/gtest.h>

namespace cs = caesar;

TEST(SpheroidTest, Construct)
{
    const double a = 2.0;
    const double f = 0.5;
    const auto spheroid = cs::Spheroid(a, f);

    EXPECT_DOUBLE_EQ(spheroid.a(), a);
    EXPECT_DOUBLE_EQ(spheroid.semimajor_axis(), a);
    EXPECT_DOUBLE_EQ(spheroid.f(), f);
    EXPECT_DOUBLE_EQ(spheroid.flattening(), f);
}

TEST(SpheroidTest, SemiminorAxis)
{
    const double a = 4.0;

    // Oblate spheroid
    {
        const double f = 0.5;
        const auto spheroid = cs::Spheroid(a, f);

        EXPECT_DOUBLE_EQ(spheroid.b(), 2.0);
        EXPECT_DOUBLE_EQ(spheroid.semiminor_axis(), 2.0);
    }

    // Prolate spheroid
    {
        const double f = -0.25;
        const auto spheroid = cs::Spheroid(a, f);

        EXPECT_DOUBLE_EQ(spheroid.b(), 5.0);
        EXPECT_DOUBLE_EQ(spheroid.semiminor_axis(), 5.0);
    }

    // Sphere
    {
        const double f = 0.0;
        const auto spheroid = cs::Spheroid(a, f);

        EXPECT_DOUBLE_EQ(spheroid.b(), 4.0);
        EXPECT_DOUBLE_EQ(spheroid.semiminor_axis(), 4.0);
    }
}

TEST(SpheroidTest, InverseFlattening)
{
    const double a = 1.0;
    const double f = 0.5;
    const auto spheroid = cs::Spheroid(a, f);

    EXPECT_DOUBLE_EQ(spheroid.inverse_flattening(), 2.0);
}

TEST(SpheroidTest, ThirdFlattening)
{
    const double a = 2.0;

    {
        const double f = 0.5;
        const auto spheroid = cs::Spheroid(a, f);

        EXPECT_DOUBLE_EQ(spheroid.third_flattening(), 1.0 / 3.0);
    }

    {
        const double f = -0.5;
        const auto spheroid = cs::Spheroid(a, f);

        EXPECT_DOUBLE_EQ(spheroid.third_flattening(), -0.2);
    }
}

TEST(SpheroidTest, Eccentricity)
{
    const double a = 2.0;

    {
        const double f = 0.0;
        const auto spheroid = cs::Spheroid(a, f);

        EXPECT_DOUBLE_EQ(spheroid.squared_eccentricity(), 0.0);
        EXPECT_DOUBLE_EQ(spheroid.eccentricity(), 0.0);
    }

    {
        const double f = 0.5;
        const auto spheroid = cs::Spheroid(a, f);

        EXPECT_DOUBLE_EQ(spheroid.squared_eccentricity(), 0.75);
        EXPECT_DOUBLE_EQ(spheroid.eccentricity(), std::sqrt(0.75));
    }

    {
        const double f = 1.0;
        const auto spheroid = cs::Spheroid(a, f);

        EXPECT_DOUBLE_EQ(spheroid.squared_eccentricity(), 1.0);
        EXPECT_DOUBLE_EQ(spheroid.eccentricity(), 1.0);
    }
}

TEST(SpheroidTest, Compare)
{
    const auto spheroid1 = cs::Spheroid(2., 0.5);
    const auto spheroid2 = cs::Spheroid(2., 0.5);
    const auto spheroid3 = cs::Spheroid(1., 1.0);

    EXPECT_TRUE(spheroid1 == spheroid2);
    EXPECT_TRUE(spheroid1 != spheroid3);
}

TEST(SpheroidTest, WGS84Ellipsoid)
{
    EXPECT_DOUBLE_EQ(cs::wgs84_ellipsoid.semimajor_axis(), 6378137.0);
    EXPECT_DOUBLE_EQ(cs::wgs84_ellipsoid.inverse_flattening(), 298.257223563);
}
