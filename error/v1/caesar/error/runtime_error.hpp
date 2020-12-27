#pragma once

#include "error_category.hpp"
#include "error_code.hpp"

namespace caesar {

/**
 * Error code used to indicate
 *
 * \see RuntimeErrorCategory
 * \see ErrorCode
 */
enum class RuntimeError {
    BadExpectedAccess = 1,
};

/**
 * Error category associated with runtime errors
 *
 * \see RuntimeError
 * \see ErrorCategory
 */
class RuntimeErrorCategory : public ErrorCategory {
    using Base = ErrorCategory;

public:
    /** The associated error code enumeration type */
    using ErrorCodeEnum = RuntimeError;

    /** \copydoc ErrorCategory::name() */
    const char*
    name() const noexcept override
    {
        return "RuntimeError";
    }

    /** \copydoc ErrorCategory::description(int) */
    const char*
    description(int value) const noexcept override;

    /** \copydoc ErrorCategory::throw_exception(const Error&) */
    [[noreturn]] void
    throw_exception(const Error& error) const override;
};

/** Singleton instance of the class */
static constexpr RuntimeErrorCategory runtime_error_category;

/** \private Enables implicit construction of ErrorCode from RuntimeError */
template<>
struct IsErrorCodeEnum<RuntimeError> : public std::true_type {};

/** \private Enables implicit construction of ErrorCode from RuntimeError */
constexpr ErrorCode
make_error_code(RuntimeError e) noexcept
{
    return {static_cast<int>(e), &runtime_error_category};
}

} // namespace caesar
