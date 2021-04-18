#include "error.hpp"

#include <fmt/core.h>
#include <stdexcept>

namespace caesar {

void
Error::throw_exception() const
{
    throw std::runtime_error(error_message());
}

std::string
Error::error_message() const
{
    return fmt::format("{}:{}: {}", source_file(), source_line(),
                       description());
}

} // namespace caesar
