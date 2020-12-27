#include "runtime_error.hpp"

#include "error.hpp"

#include <stdexcept>

namespace caesar {

const char*
RuntimeErrorCategory::description(int value) const noexcept
{
    const auto e = static_cast<ErrorCodeEnum>(value);

    switch (e) {
    case RuntimeError::BadExpectedAccess:
        return "Invalid access to Expected::error";
    }

    return "<unknown>";
}

void
RuntimeErrorCategory::throw_exception(const Error& error) const
{
    throw std::runtime_error(error.message());
}

} // namespace caesar
