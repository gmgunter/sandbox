#pragma once

#include <limits>
#include <type_traits>

namespace caesar {

/// Checks whether `T` is a (built-in or user-defined) arithmetic type.
///
/// Formally, this checks whether [std::numeric_limits]() is specialized for
/// `T`. This allows checking for user-defined types in addition to built-in
/// types since `std::numeric_limits`, unlike [std::is_arithmetic](), may be
/// specialized for user-defined types.
template<typename T>
struct IsArithmetic
    : public std::bool_constant<std::numeric_limits<T>::is_specialized> {};

/// Helper variable template for [IsArithmetic]()
template<typename T>
inline constexpr bool is_arithmetic_v = IsArithmetic<T>::value;

} // namespace caesar
