#pragma once

#include <type_traits>
#include <variant>

namespace caesar {

/// Represents the result of an operation that may fail
///
/// At any given time, an `Expected<T, Errors...>` object may contain a value of
/// type `T` or an error object from among its `Errors` types. It may be used to
/// return and propagate errors from functions that could fail in predictable
/// and recoverable ways. If no error occurred, the returned object contains the
/// expected result. In case of failure, it instead contains an object that
/// describes the error encountered.
///
/// An expected object is not permitted to hold references, C-style arrays, or
/// the type `void`. The set of `Errors` types must not be empty and must not
/// contain `T`.
///
/// Template parameters
///    `T` - expected value type
///    `Errors` - possible error type(s)
template<class T, class... Errors>
class [[nodiscard]] Expected : public std::variant<T, Errors...> {
    using Base = std::variant<T, Errors...>;

    // `Errors` must not be empty.
    static_assert(sizeof...(Errors) > 0);

    // `Errors` must not contain `T`.
    static_assert((not std::is_same_v<T, Errors> and ...));

public:
    Expected() = delete;

//    template<class... OtherErrors>
//    operator Expected<T, OtherErrors...>() const;
//
//    /// Access the stored value, if present.
//    constexpr const T*
//    operator->() const
//    {
//    }
//
//    constexpr T*
//    operator->()
//    {
//    }
//
//    constexpr const T&
//    operator*() const&
//    {
//    }
//
//    constexpr T&
//    operator*() &
//    {
//    }
//
//    constexpr const T&&
//    operator*() const&&
//    {
//    }
//
//    constexpr T&&
//    operator*() &&
//    {
//    }

    constexpr explicit operator bool() const noexcept
    {
        return has_value();
    }

    constexpr bool has_value() const noexcept
    {
        return index() == 0;
    }

    template<class Error>
    constexpr bool has_error() const noexcept
    {
        static_assert((std::is_same_v<Error, Errors> or ...));
        return std::holds_alternative<Error>(base());
    }

protected:
    constexpr const Base&
    base() const noexcept
    {
        return static_cast<const Base&>(*this);
    }

    constexpr Base&
    base() noexcept
    {
        return static_cast<Base&>(*this);
    }
};

} // namespace caesar
