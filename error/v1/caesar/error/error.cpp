#include "error.hpp"

#include <fmt/core.h>

namespace caesar {

std::string
Error::message() const
{
    return fmt::format("{}:{}: {}: {}", file(), line(),
                       error_code().category()->name(),
                       error_code().description());
}

void
Error::throw_exception() const
{
    error_code().category()->throw_exception(*this);
}

} // namespace caesar
