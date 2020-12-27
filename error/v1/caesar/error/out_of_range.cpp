#include "out_of_range.hpp"

#include "error.hpp"

#include <stdexcept>

namespace caesar {

const char*
OutOfRangeCategory::description(int value) const noexcept
{
    const auto e = static_cast<ErrorCodeEnum>(value);

    switch (e) {
    case OutOfRange::OutOfBoundsAccess: return "Out of bounds access attempted";
    }

    return "<unknown>";
}

void
OutOfRangeCategory::throw_exception(const Error& error) const
{
    throw std::out_of_range(error.message());
}

} // namespace caesar
