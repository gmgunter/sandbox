#pragma once

#include "error_code.hpp"

#include <experimental/source_location>
#include <string>

namespace caesar {

/**
 * Encapsulates an error code as well as contextual information about its point
 * of origin
 *
 * \see ErrorCategory
 * \see ErrorCode
 */
class Error {
    using source_location = std::experimental::source_location;

public:
    /**
     * Construct a new Error object.
     *
     * \param[in] error_code    error code
     * \param[in] file          source file where the error originated
     * \param[in] line          source line where the error originated
     */
    constexpr Error(const ErrorCode& error_code,
                    const char* file,
                    int line) noexcept
        : error_code_(error_code), file_(file), line_(line)
    {}

    /**
     * Construct a new Error object.
     *
     * \param[in] error_code    error code
     * \param[in] origin        source location where the error originated
     */
    explicit constexpr Error(
            const ErrorCode& error_code,
            const source_location& origin = source_location::current()) noexcept
        : Error(error_code, origin.file_name(), static_cast<int>(origin.line()))
    {}

    /** Return the error code. */
    constexpr const ErrorCode&
    error_code() const noexcept
    {
        return error_code_;
    }

    /** Return the source file where the error originated. */
    constexpr const char*
    file() const noexcept
    {
        return file_;
    }

    /** Return the source line where the error originated. */
    constexpr int
    line() const noexcept
    {
        return line_;
    }

    /** Return an explanatory message. */
    std::string
    message() const;

    /** Throw an exception object corresponding to the error. */
    void
    throw_exception() const;

    // XXX boost/outcome requires error types to be default-constructible :(
    // this constraint is removed in boost v1.76
    Error() = default;

private:
    ErrorCode error_code_;
    const char* file_;
    int line_;
};

} // namespace caesar
