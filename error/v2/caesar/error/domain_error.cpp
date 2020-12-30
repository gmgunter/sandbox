#include "domain_error.hpp"

namespace caesar {

const char* get_error_category(DomainError) noexcept { return "DomainError"; }

const char*
get_error_string(DomainError e) noexcept
{
    switch (e) {
    case DomainError::DivisionByZero: return "Division by zero";
    }

    return "<unknown>";
}

} // namespace caesar
