#pragma once

namespace caesar {

/**
 * Error code used to indicate domain errors, i.e. situations where the inputs
 * are outside of the domain on which an operation is defined.
 *
 * \see ErrorCode
 */
enum class DomainError {
    DivisionByZero = 1,
};

/** \private implements ErrorCode::category() */
const char*
get_error_category(DomainError e) noexcept;

/** \private implements ErrorCode::description() */
const char*
get_error_string(DomainError e) noexcept;

} // namespace caesar
