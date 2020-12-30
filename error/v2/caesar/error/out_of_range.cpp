#include "out_of_range.hpp"

namespace caesar {

const char* get_error_category(OutOfRange) noexcept { return "OutOfRange"; }

const char*
get_error_string(OutOfRange e) noexcept
{
    switch (e) {
    case OutOfRange::OutOfBoundsAccess: return "Out of bounds access attempted";
    }

    return "<unknown>";
}

} // namespace caesar
