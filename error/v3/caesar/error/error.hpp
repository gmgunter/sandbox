#pragma once

#include "source_location.hpp"

#include <string>

namespace caesar {

/// A generic runtime error
class [[nodiscard]] Error {
public:
    /// Construct a new `Error` object.
    ///
    /// Parameters
    ///    `location` - source location where the error originated
    constexpr Error(const source_location& location) noexcept
        : Error{location.file_name(), location.line()}
    {}

    /// Construct a new `Error` object.
    ///
    /// Parameters
    ///    `source_file` - name of the source file where the error originated
    ///    `source_line` - line number where the error originated
    constexpr Error(const char* source_file,
                    std::uint_least32_t source_line) noexcept
        : source_file_{source_file}, source_line_{source_line}
    {}

    /// Return a description of the error.
    [[nodiscard]] std::string
    description() const
    {
        return "Unknown error encountered";
    }

    /// Get the name of the source file where the error originated.
    [[nodiscard]] constexpr const char*
    source_file() const noexcept
    {
        return source_file_;
    }

    /// Get the line number where the error originated.
    [[nodiscard]] constexpr std::uint_least32_t
    source_line() const noexcept
    {
        return source_line_;
    }

    /// Translates the error into an thrown exception object.
    [[noreturn]] void
    throw_exception() const;

protected:
    /// Returning a formatted string describing the error, including information
    /// about its source location.
    [[nodiscard]] std::string
    error_message() const;

private:
    const char* source_file_;
    std::uint_least32_t source_line_;
};

} // namespace caesar
