#pragma once

#include "error_category.hpp"
#include "error_code.hpp"

namespace caesar {

/**
 * Error code used to indicate errors that are consequence of attempt to access
 * elements out of defined range.
 *
 * \see OutOfRangeCategory
 * \see ErrorCode
 */
enum class OutOfRange {
    OutOfBoundsAccess = 1,
};

/**
 * Error category associated with out-of-range errors
 *
 * \see OutOfRange
 * \see ErrorCategory
 */
class OutOfRangeCategory : public ErrorCategory {
    using Base = ErrorCategory;

public:
    /** The associated error code enumeration type */
    using ErrorCodeEnum = OutOfRange;

    /** \copydoc ErrorCategory::name() */
    const char*
    name() const noexcept override
    {
        return "OutOfRange";
    }

    /** \copydoc ErrorCategory::description(int) */
    const char*
    description(int value) const noexcept override;

    /** \copydoc ErrorCategory::throw_exception(const Error&) */
    [[noreturn]] void
    throw_exception(const Error& error) const override;
};

/** Singleton instance of the class */
static constexpr OutOfRangeCategory out_of_range_category;

/** \private Enables implicit construction of ErrorCode from OutOfRange */
template<>
struct IsErrorCodeEnum<OutOfRange> : public std::true_type {};

/** \private Enables implicit construction of ErrorCode from OutOfRange */
constexpr ErrorCode
make_error_code(OutOfRange e) noexcept
{
    return {static_cast<int>(e), &out_of_range_category};
}

} // namespace caesar
