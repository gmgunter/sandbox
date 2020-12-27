#pragma once

#include "detail/no_value_policy.hpp"
#include "error.hpp"

#include <boost/outcome/outcome.hpp>
#include <utility>

namespace caesar {

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

/**
 * A wrapper that may contain an object of type T or an Error
 *
 * Expected is intended to be used as a return type for operations that may
 * fail. On success, the return value contains the expected result. In case of
 * failure, it instead contains an object that describes the error encountered.
 *
 * Attempting to access the underlying value (or error) if it was not contained
 * causes an exception to be thrown.
 *
 * When an Expected object is contextually converted to bool, the conversion
 * returns `true` if the object contains a value and `false` if the object
 * contains an error.
 *
 * \see Error
 */
template<class T>
class Expected : public outcome::basic_result<T, Error, detail::NoValuePolicy> {
    using Base = outcome::basic_result<T, Error, detail::NoValuePolicy>;

public:
    using Base::Base;

    /** Access the contained value, if present. */
     constexpr T*
     operator->()
    {
        return &Base::value();
    }

    /** \copydoc Expected::operator->() */
     constexpr const T*
     operator->() const
    {
        return &Base::value();
    }

    /** Access the contained value, if present. */
     constexpr T&
     operator*()
    {
        return Base::value();
    }

    /** \copydoc Expected::operator*() */
     constexpr const T&
     operator*() const
    {
        return Base::value();
    }

#if 0
    // Declare these members for the purpose of documenting them - they actually
    // exist in a derived-from class.

    /** */
    template<class U = T>
    constexpr Expected(U&& value);

    constexpr Expected(const Error& error);

    /** \copydoc Expected::has_value() */
    explicit constexpr operator bool() const noexcept;

    /** Checks whether the object contains a value. */
    constexpr bool
    has_value() const noexcept;

    /** Checks whether the object contains an error. */
    constexpr bool
    has_error() const noexcept;

    /** Access the contained value, if present. */
    constexpr T&
    value();

    /** \copydoc Expected::value() */
    constexpr const T&
    value() const;

    /** Access the contained error, if present. */
    constexpr Error&
    error();

    /** \copydoc Expected::error() */
    constexpr const Error&
    error() const;
#endif
};

} // namespace caesar
