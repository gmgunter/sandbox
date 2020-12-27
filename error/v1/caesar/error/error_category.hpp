#pragma once

#include <type_traits>

namespace caesar {

class Error;

/**
 * Base class for specific error category types
 *
 * Each derived class defines an interface between a unique error code
 * enumeration type and the type-erased ErrorCode class.
 *
 * The objects of derived error category classes are treated as singletons -
 * they are passed by pointer and compared based on their address in memory.
 *
 * \see Error
 * \see ErrorCode
 */
class ErrorCategory {
public:
    /** Get a string representation of the error category name. */
    virtual const char*
    name() const noexcept = 0;

    /** Return a message describing the error code with the specified value. */
    virtual const char*
    description(int value) const noexcept = 0;

    /** Throw an exception object corresponding to the specified error. */
    virtual void
    throw_exception(const Error& error) const = 0;

    /**
     * Compare two ErrorCategory objects.
     *
     * ErrorCategory objects are assumed to be singletons and are considered
     * equal only if they are the same object.
     */
    friend constexpr bool
    operator==(const ErrorCategory& lhs, const ErrorCategory& rhs) noexcept
    {
        return &lhs == &rhs;
    }

    /** \copydoc operator==(const ErrorCategory&, const ErrorCategory&) */
    friend constexpr bool
    operator!=(const ErrorCategory& lhs, const ErrorCategory& rhs) noexcept
    {
        return not(lhs == rhs);
    }

protected:
    ~ErrorCategory() = default;
};

/**
 * Checks whether T is an error code enumeration type.
 *
 * The template may be specialized for additional types to indicate that the
 * type may be converted to ErrorCode.
 *
 * \see ErrorCode
 */
template<class T>
struct IsErrorCodeEnum : public std::false_type {};

/** Helper variable template for IsErrorCodeEnum */
template<class T>
inline constexpr bool is_error_code_enum_v = IsErrorCodeEnum<T>::value;

} // namespace caesar
