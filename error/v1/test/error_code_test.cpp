#include <caesar/error.hpp>

#include <gtest/gtest.h>

namespace cs = caesar;

TEST(ErrorCodeTest, FromEnum)
{
    const auto e = cs::DomainError::DivisionByZero;
    const cs::ErrorCode error_code = e;

    EXPECT_EQ(error_code.value(), static_cast<int>(e));
    EXPECT_EQ(error_code.category(), &cs::domain_error_category);
}

TEST(ErrorCodeTest, AssignEnum)
{
    auto error_code = cs::ErrorCode(cs::DomainError::DivisionByZero);

    const auto e = cs::OutOfRange::OutOfBoundsAccess;
    error_code = e;

    EXPECT_EQ(error_code.value(), static_cast<int>(e));
    EXPECT_EQ(error_code.category(), &cs::out_of_range_category);
}

TEST(ErrorCodeTest, Description)
{
    {
        const cs::ErrorCode error_code = cs::DomainError::DivisionByZero;
        const auto s = std::string(error_code.description());

        EXPECT_EQ(s, "Division by zero");
    }

    {
        const cs::ErrorCode error_code = cs::OutOfRange::OutOfBoundsAccess;
        const auto s = std::string(error_code.description());

        EXPECT_EQ(s, "Out of bounds access attempted");
    }
}

TEST(ErrorCodeTest, Compare)
{
    const cs::ErrorCode error_code1 = cs::DomainError::DivisionByZero;
    const cs::ErrorCode error_code2 = cs::DomainError::DivisionByZero;
    const cs::ErrorCode error_code3 = cs::OutOfRange::OutOfBoundsAccess;

    EXPECT_TRUE(error_code1 == error_code2);
    EXPECT_TRUE(error_code1 != error_code3);
}
