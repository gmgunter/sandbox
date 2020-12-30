#pragma once

#include "error_code.hpp"

#include <experimental/source_location>
#include <type_traits>

namespace caesar {

/**
 * Describes an error encountered during processing
 *
 * The Error class stores an error code along with contextual information about
 * where the error originated from in the source code (filename and line
 * number). When constructed from an ErrorCode (or any supported error code
 * enumeration type), by default, the source location information is populated
 * based on the call site where the Error object was constructed.
 *
 * \see ErrorCode
 * \see Expected
 */
class Error {
    using source_location = std::experimental::source_location;

public:
    /**
     * Construct a new Error object.
     *
     * \param[in] error_code    error code
     * \param[in] file          source code filename where the error occurred
     * \param[in] line          source code line number where the error occurred
     */
    constexpr Error(const ErrorCode& error_code,
                    const char* file,
                    int line) noexcept
        : error_code_(error_code), file_(file), line_(line)
    {}

    /**
     * Construct a new Error object.
     *
     * The source location defaults to the call site where the Error object was
     * constructed.
     *
     * \param[in] error_code    error code
     * \param[in] origin        source code location where the error occurred
     */
    constexpr Error(
            const ErrorCode& error_code,
            const source_location& origin = source_location::current()) noexcept
        : Error(error_code, origin.file_name(), static_cast<int>(origin.line()))
    {}

    /**
     * Construct a new Error object.
     *
     * The source location defaults to the call site where the Error object was
     * constructed.
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
    constexpr Error(
            ErrorCodeEnum e,
            const source_location& origin = source_location::current()) noexcept
        : Error(ErrorCode(e), origin)
    {}

    /** Return the error code. */
    constexpr const ErrorCode&
    error_code() const noexcept
    {
        return error_code_;
    }

    /** Return the source code filename where the error occurred. */
    constexpr const char*
    file() const noexcept
    {
        return file_;
    }

    /** Return the source code line number where the error occurred. */
    constexpr int
    line() const noexcept
    {
        return line_;
    }

private:
    ErrorCode error_code_;
    const char* file_;
    int line_;
};

} // namespace caesar
