#pragma once

#include "error_category.hpp"
#include "error_code.hpp"

namespace caesar {

/**
 * Error code used to indicate domain errors, i.e. situations where the inputs
 * are outside of the domain on which an operation is defined.
 *
 * \see DomainErrorCategory
 * \see ErrorCode
 */
enum class DomainError {
    DivisionByZero = 1,
};

/**
 * Error category associated with domain errors
 *
 * \see DomainError
 * \see ErrorCategory
 */
class DomainErrorCategory : public ErrorCategory {
    using Base = ErrorCategory;

public:
    /** The associated error code enumeration type */
    using ErrorCodeEnum = DomainError;

    /** \copydoc ErrorCategory::name() */
    const char*
    name() const noexcept override
    {
        return "DomainError";
    }

    /** \copydoc ErrorCategory::description(int) */
    const char*
    description(int value) const noexcept override;

    /** \copydoc ErrorCategory::throw_exception(const Error&) */
    [[noreturn]] void
    throw_exception(const Error& error) const override;
};

/** Singleton instance of the class */
static constexpr DomainErrorCategory domain_error_category;

/** \private Enables implicit construction of ErrorCode from DomainError */
template<>
struct IsErrorCodeEnum<DomainError> : public std::true_type {};

/** \private Enables implicit construction of ErrorCode from DomainError */
constexpr ErrorCode
make_error_code(DomainError e) noexcept
{
    return {static_cast<int>(e), &domain_error_category};
}

} // namespace caesar
