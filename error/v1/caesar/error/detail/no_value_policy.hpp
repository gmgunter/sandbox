#pragma once

#include "../error.hpp"
#include "../runtime_error.hpp"

#include <boost/outcome/outcome.hpp>

namespace caesar::detail {

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

/**
 * \private
 * A mixin type that defines how invalid access to Expected object members is
 * handled
 */
struct NoValuePolicy : public outcome::policy::base {
    /** \private Called on each access to Expected::value */
    template<class Expected>
    static constexpr void
    wide_value_check(Expected&& self)
    {
        if (not base::_has_value(self)) {
            base::_error(std::forward<Expected>(self)).throw_exception();
        }
    }

    /** \private Called on each access to Expected::error */
    template<class Expected>
    static constexpr void
    wide_error_check(Expected&& self)
    {
        if (not base::_has_error(std::forward<Expected>(self))) {
            Error(RuntimeError::BadExpectedAccess).throw_exception();
        }
    }
};

} // namespace caesar::detail
