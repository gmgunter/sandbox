#pragma once

namespace caesar {

/**
 * Error code used to indicate errors that are consequence of attempt to access
 * elements out of a defined range.
 *
 * \see ErrorCode
 */
enum class OutOfRange {
    OutOfBoundsAccess = 1,
};

/** \private implements ErrorCode::category() */
const char*
get_error_category(OutOfRange e) noexcept;

/** \private implements ErrorCode::description() */
const char*
get_error_string(OutOfRange e) noexcept;

} // namespace caesar
