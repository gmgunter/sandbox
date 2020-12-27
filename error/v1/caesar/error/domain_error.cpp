#include "domain_error.hpp"

#include "error.hpp"

#include <stdexcept>

namespace caesar {

const char*
DomainErrorCategory::description(int value) const noexcept
{
    const auto e = static_cast<ErrorCodeEnum>(value);

    switch (e) {
    case DomainError::DivisionByZero: return "Division by zero";
    }

    return "<unknown>";
}

void
DomainErrorCategory::throw_exception(const Error& error) const
{
    throw std::domain_error(error.message());
}

} // namespace caesar
