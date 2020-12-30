#include <caesar/error.hpp>

#include <gmock/gmock.h>

namespace cs = caesar;

TEST(ErrorTest, FromErrorCode)
{
    const auto error_code = cs::DomainError::DivisionByZero;
    const cs::Error error = error_code;

    EXPECT_EQ(error.error_code(), error_code);
    EXPECT_EQ(error.line(), 10);
    EXPECT_THAT(error.file(), testing::EndsWith("error_test.cpp"));
}

TEST(ErrorTest, AssignErrorCode)
{
    cs::Error error = cs::DomainError::DivisionByZero;

    const auto error_code = cs::OutOfRange::OutOfBoundsAccess;
    error = error_code;

    EXPECT_EQ(error.error_code(), error_code);
    EXPECT_EQ(error.line(), 22);
    EXPECT_THAT(error.file(), testing::EndsWith("error_test.cpp"));
}
