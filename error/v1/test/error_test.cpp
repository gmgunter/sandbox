#include <caesar/error.hpp>

#include <gmock/gmock.h>
#include <stdexcept>

namespace cs = caesar;

TEST(ErrorTest, FromErrorCode)
{
    const auto error_code = cs::DomainError::DivisionByZero;
    const auto error = cs::Error(error_code);

    EXPECT_EQ(error.error_code(), error_code);
    EXPECT_EQ(error.line(), 11);
    EXPECT_THAT(error.file(), testing::EndsWith("error_test.cpp"));
}

TEST(ErrorTest, Message)
{
    const auto error_code = cs::DomainError::DivisionByZero;
    const auto error = cs::Error(error_code);

    const auto s = "error_test.cpp:21: DomainError: Division by zero";
    EXPECT_THAT(error.message(), testing::EndsWith(s));
}

TEST(ErrorTest, ThrowException)
{
    {
        const auto error_code = cs::DomainError::DivisionByZero;
        const auto error = cs::Error(error_code);

        EXPECT_THROW({ error.throw_exception(); }, std::domain_error);
    }

    {
        const auto error_code = cs::OutOfRange::OutOfBoundsAccess;
        const auto error = cs::Error(error_code);

        EXPECT_THROW({ error.throw_exception(); }, std::out_of_range);
    }
}
