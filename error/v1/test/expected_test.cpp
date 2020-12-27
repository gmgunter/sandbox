#include <caesar/error.hpp>

#include <gtest/gtest.h>
#include <stdexcept>

namespace cs = caesar;

static cs::Expected<int>
safe_divide(int x, int y)
{
    if (y == 0) {
        return cs::Error(cs::DomainError::DivisionByZero);
    }
    return x / y;
}

TEST(ExpectedTest, HasValue)
{
    {
        const auto result = safe_divide(6, 3);
        EXPECT_TRUE(result.has_value());
    }

    {
        const auto result = safe_divide(1, 0);
        EXPECT_FALSE(result.has_value());
    }
}

TEST(ExpectedTest, Truthiness)
{
    {
        const auto result = safe_divide(6, 3);
        EXPECT_TRUE(result);
    }

    {
        const auto result = safe_divide(1, 0);
        EXPECT_FALSE(result);
    }
}

TEST(ExpectedTest, HasError)
{
    {
        const auto result = safe_divide(6, 3);
        EXPECT_FALSE(result.has_error());
    }

    {
        const auto result = safe_divide(1, 0);
        EXPECT_TRUE(result.has_error());
    }
}

TEST(ExpectedTest, Value)
{
    {
        const auto result = safe_divide(6, 3);
        EXPECT_EQ(result.value(), 2);
    }

    {
        const auto result = safe_divide(1, 0);
        EXPECT_THROW({ result.value(); }, std::domain_error);
    }
}

TEST(ExpectedTest, Dereference)
{
    {
        const auto result = safe_divide(6, 3);
        EXPECT_EQ(*result, 2);
    }

    {
        const auto result = safe_divide(1, 0);
        EXPECT_THROW({ *result; }, std::domain_error);
    }
}

TEST(ExpectedTest, DereferenceMember)
{
    struct Foo {
        int bar;
    };

    {
        const cs::Expected<Foo> foo = Foo{123};
        EXPECT_EQ(foo->bar, 123);
    }

    {
        const cs::Expected<Foo> foo =
                cs::Error(cs::DomainError::DivisionByZero);
        EXPECT_THROW({ foo->bar; }, std::domain_error);
    }
}

TEST(ExpectedTest, Error)
{
    {
        const auto result = safe_divide(6, 3);
        EXPECT_THROW({ result.error(); }, std::runtime_error);
    }

    {
        const auto result = safe_divide(1, 0);
        EXPECT_THROW({ result.value(); }, std::domain_error);
    }
}
