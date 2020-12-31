#pragma once

#include "error.hpp"

#include <experimental/source_location>
#include <tl/expected.hpp>

namespace caesar {

/**
 * A wrapper that may contain an object of type T or an error
 *
 * The Expected class is intended to be used as a return type for operations
 * that may fail. On success, the returned object contains the expected result.
 * In case of failure, it instead contains an object that describes the error
 * encountered.
 *
 * Compared to the use of exceptions, the Expected approach
 *
 * - can be used in device code and can be safely and easily propagated across
 *   thread boundaries
 * - allows for inexpensive local handling of the "bad path" when an operation
 *   fails to return the expected value
 * - improves error visibility in code review by making the possibility of error
 *   explicit in a function's return type
 *
 * Compared to error codes, Expected objects
 *
 * - do not monopolize the return channel
 * - are not easily ignored (if the user wants to retrieve the contained value)
 *
 * The Expected class provides methods for safe and unsafe access to the
 * underlying value. If an instance of the expected type was not stored,
 * attempting to access it via the `Expected::value()` method causes an
 * exception to be thrown. The indirection operators (`Expected::operator*()`
 * and `Expected::operator->()`), however, provide unchecked access to the
 * stored value. The behavior of these methods is undefined if the expected
 * value is not present. (Similarly, attempting to access the error object via
 * `Expected::error()` has undefined behavior if a value was stored instead.)
 *
 * When an Expected object is contextually converted to `bool`, the conversion
 * returns `true` if the object contains a value and `false` if the object
 * contains an error.
 *
 * Expected may not store a reference type.
 *
 * \tparam the expected value type
 *
 * ### Example
 *
 * ```c++
 * Expected<int> safe_divide(int x, int y)
 * {
 *     if (y == 0) {
 *         return DomainError::DivisionByZero;
 *     }
 *     return x / y;
 * }
 *
 * auto res1 = safe_divide(6, 3);
 * auto res2 = safe_divide(6, 0);
 *
 * std::cout << std::boolalpha << bool(res1) << "\n";
 * std::cout << std::boolalpha << bool(res2) << "\n";
 *
 * std::cout << res1.value() << "\n";
 * std::cout << res2.error().error_code().description() << "\n";
 *
 * // std::cout << res2.value() << "\n"; // causes an exception to be thrown
 * ```
 *
 * Possible output:
 *
 * ```
 * true
 * false
 * 2
 * Division by zero
 * ```
 *
 * \see Error
 */
template<class T>
class Expected : public tl::expected<T, Error> {
    using Base = tl::expected<T, Error>;
    using source_location = std::experimental::source_location;

public:
    using Base::Base;
    using Base::operator=;

    Expected() = delete;

    /** Construct a new Expected object containing an error. */
    constexpr Expected(const Error& error) noexcept
        : Base(tl::unexpected<Error>(error))
    {}

    /**
     * Construct a new Expected object containing an error.
     *
     * The source location defaults to the call site where the Expected object
     * was constructed.
     *
     * \param[in] error_code    error code
     * \param[in] origin        source code location where the error occurred
     */
    constexpr Expected(
            const ErrorCode& error_code,
            const source_location& origin = source_location::current()) noexcept
        : Expected(Error(error_code, origin))
    {}

    /**
     * Construct a new Expected object containing an error.
     *
     * The source location defaults to the call site where the Expected object
     * was constructed.
     *
     * This overload participates in overload resolution only if
     * `std::is_constructible_v<ErrorCode, ErrorCodeEnum> == true`.
     *
     * \tparam ErrorCodeEnum
     * an error code enumeration type supported by ErrorCode
     *
     * \param[in] e         error code enumeration object
     * \param[in] origin    source code location where the error occurred
     */
    template<class ErrorCodeEnum,
             typename = std::enable_if_t<
                     std::is_constructible_v<ErrorCode, ErrorCodeEnum>>>
    constexpr Expected(
            ErrorCodeEnum e,
            const source_location& origin = source_location::current()) noexcept
        : Expected(Error(e, origin))
    {}
};

} // namespace caesar
