#include <caesar/error.hpp>

#include <gtest/gtest.h>
#include <string>

namespace cs = caesar;

TEST(ErrorCodeTest, FromEnum)
{
    const auto e = cs::DomainError::DivisionByZero;
    const cs::ErrorCode error_code = e;

    const auto value = static_cast<int>(e);
    EXPECT_EQ(error_code.value(), value);

    const std::string category = "DomainError";
    EXPECT_EQ(error_code.category(), category);
}

TEST(ErrorCodeTest, AssignEnum)
{
    cs::ErrorCode error_code = cs::DomainError::DivisionByZero;

    const auto e = cs::OutOfRange::OutOfBoundsAccess;
    error_code = e;

    const auto value = static_cast<int>(e);
    EXPECT_EQ(error_code.value(), value);

    const std::string category = "OutOfRange";
    EXPECT_EQ(error_code.category(), category);
}

TEST(ErrorCodeTest, Description)
{
    const cs::ErrorCode error_code = cs::DomainError::DivisionByZero;

    const std::string description = "Division by zero";
    EXPECT_EQ(error_code.description(), description);
}

TEST(ErrorCodeTest, Compare)
{
    const cs::ErrorCode error_code1 = cs::DomainError::DivisionByZero;
    const cs::ErrorCode error_code2 = cs::DomainError::DivisionByZero;
    const cs::ErrorCode error_code3 = cs::OutOfRange::OutOfBoundsAccess;

    EXPECT_TRUE(error_code1 == error_code2);
    EXPECT_TRUE(error_code1 != error_code3);
}
