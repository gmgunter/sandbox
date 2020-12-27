#pragma once

#include "error_category.hpp"

namespace caesar {

/**
 * A type-erased error code
 *
 * ErrorCode facilitates interoperability between any number of domain-specific
 * error codes that an application and its third-party dependencies may
 * generate. It does so by providing a common type that can be used to represent
 * error code enumerations of different types.
 *
 * ErrorCode stores an integer value and a pointer to an object of type
 * ErrorCategory. The ErrorCategory class can be specialized to add support for
 * additional error code enumeration types.
 *
 * \see Error
 * \see ErrorCategory
 */
class ErrorCode {
public:
    /**
     * Construct a new ErrorCode object.
     *
     * \param[in] value     error code value
     * \param[in] category  associated error category
     */
    constexpr ErrorCode(int value, const ErrorCategory* category) noexcept
        : value_(value), category_(category)
    {}

    /**
     * Construct a new ErrorCode object from an error code enumeration object
     * as though by `ErrorCode(make_error_code(e))`.
     *
     * This overload participates in overload resolution only if
     * `IsErrorCodeEnum<ErrorCodeEnum>::value == true`.
     */
    template<class ErrorCodeEnum,
             typename = std::enable_if_t<is_error_code_enum_v<ErrorCodeEnum>>>
    constexpr ErrorCode(ErrorCodeEnum e) : ErrorCode(make_error_code(e))
    {}

    /**
     * Assign the ErrorCode value from an error code enumeration object as
     * though by `*this = make_error_code(e)`.
     *
     * This overload participates in overload resolution only if
     * `IsErrorCodeEnum<ErrorCodeEnum>::value == true`.
     */
    template<class ErrorCodeEnum,
             typename = std::enable_if_t<is_error_code_enum_v<ErrorCodeEnum>>>
    constexpr ErrorCode&
    operator=(ErrorCodeEnum e)
    {
        return *this = make_error_code(e);
    }

    /** Return the error code value. */
    constexpr int
    value() const noexcept
    {
        return value_;
    }

    /** Return a pointer to the associated error category. */
    constexpr const ErrorCategory*
    category() const noexcept
    {
        return category_;
    }

    /** Return a message describing the error code. */
    const char*
    description() const noexcept
    {
        return category()->description(value());
    }

    /** Compare two ErrorCode objects */
    friend constexpr bool
    operator==(const ErrorCode& lhs, const ErrorCode& rhs) noexcept
    {
        return lhs.value() == rhs.value() and
               *lhs.category() == *rhs.category();
    }

    /** \copydoc operator==(const ErrorCode&, const ErrorCode&) */
    friend constexpr bool
    operator!=(const ErrorCode& lhs, const ErrorCode& rhs) noexcept
    {
        return not(lhs == rhs);
    }

    // XXX boost/outcome requires error types to be default-constructible :(
    // this constraint is removed in boost v1.76
    ErrorCode() = default;

private:
    int value_;
    const ErrorCategory* category_;
};

} // namespace caesar
