#pragma once

#include "domain_error.hpp"
#include "out_of_range.hpp"

#include <variant>

namespace caesar {

/**
 * A type-erased error code
 *
 * ErrorCode facilitates interoperability between any number of domain-specific
 * error codes that an application and its third-party dependencies may
 * need to support. It does so by providing a common type that can be used to
 * represent error code enumerations of different types.
 *
 * Unlike
 * [`std::error_code`](https://en.cppreference.com/w/cpp/error/error_code),
 * ErrorCode uses a variant/visitor idiom rather than dynamic polymorphism to
 * implement type erasure. This makes extending ErrorCode more intrusive (you
 * need to actually modify the class definition in order to support additional
 * error code enum types). However, ErrorCode boasts a number of beneficial
 * properties compared to `std::error_code`:
 *
 * - ErrorCode objects can be constructed in device code and can be safely
 *   passed between the host and device
 * - Supporting custom error codes requires significantly less boilerplate
 * - ErrorCode is a *LiteralType* and most operations are `constexpr`
 *
 * ### Example
 *
 * ```c++
 * ErrorCode ec = DomainError::DivisionByZero;
 *
 * std::cout << ec.category() << "\n";    // "DomainError"
 * std::cout << ec.description() << "\n"; // "Division by zero"
 * ```
 *
 * \see Error
 */
class ErrorCode : private std::variant<DomainError, OutOfRange> {
    using Base = std::variant<DomainError, OutOfRange>;

public:
    /** Integral type that can be used to represent error code values */
    using value_type = int;

    using Base::Base;
    using Base::operator=;

    ErrorCode() = delete;

    /** Return the error code value. */
    constexpr value_type
    value() const noexcept
    {
        return std::visit([](auto arg) { return static_cast<value_type>(arg); },
                          base());
    }

    /** Return the associated error category name. */
    const char*
    category() const noexcept
    {
        return std::visit([](auto arg) { return get_error_category(arg); },
                          base());
    }

    /** Return a message describing the error code. */
    const char*
    description() const noexcept
    {
        return std::visit([](auto arg) { return get_error_string(arg); },
                          base());
    }

    /** Compare two ErrorCode objects */
    friend constexpr bool
    operator==(const ErrorCode& lhs, const ErrorCode& rhs) noexcept
    {
        return lhs.base() == rhs.base();
    }

    /** \copydoc operator==(const ErrorCode&, const ErrorCode&) */
    friend constexpr bool
    operator!=(const ErrorCode& lhs, const ErrorCode& rhs) noexcept
    {
        return not(lhs == rhs);
    }

private:
    constexpr const Base&
    base() const noexcept
    {
        return static_cast<const Base&>(*this);
    }
};

} // namespace caesar
