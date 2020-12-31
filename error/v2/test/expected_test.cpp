#include <caesar/error.hpp>

#include <gmock/gmock.h>
#include <stdexcept>

namespace cs = caesar;

static cs::Expected<int>
safe_divide(int x, int y)
{
    if (y == 0) {
        return cs::DomainError::DivisionByZero;
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

TEST(ExpectedTest, Value)
{
    {
        const auto result = safe_divide(6, 3);
        EXPECT_EQ(result.value(), 2);
    }

    {
        const auto result = safe_divide(1, 0);
        EXPECT_THROW({ result.value(); }, std::exception);
    }
}

TEST(ExpectedTest, Dereference)
{
    const auto result = safe_divide(6, 3);
    EXPECT_EQ(*result, 2);
}

TEST(ExpectedTest, DereferenceMember)
{
    struct Foo {
        int bar;
    };

    const cs::Expected<Foo> foo = Foo{123};
    EXPECT_EQ(foo->bar, 123);
}

TEST(ExpectedTest, Error)
{
    const auto result = safe_divide(1, 0);
    const auto error = result.error();

    EXPECT_EQ(error.error_code(), cs::DomainError::DivisionByZero);
    EXPECT_EQ(error.line(), 11);
    EXPECT_THAT(error.file(), testing::EndsWith("expected_test.cpp"));
}
