#pragma once

#include "proof/connectives.hpp"

/// Compile-time traits for identifying logical connective types.

namespace Proof {
    template<typename T>
    struct IsAnd {
    static constexpr bool value = false;
    };

    template<typename P, typename Q>
    struct IsAnd<And<P, Q>>{
    static constexpr bool value = true;
    };

    template<typename T>
    struct IsOr {
    static constexpr bool value = false;
    };

    template<typename P, typename Q, bool B>
    struct IsOr<Or<P, Q, B>> {
    static constexpr bool value = true;
    };

    template<typename T>
    inline constexpr bool is_and_v = IsAnd<T>::value;

    template<typename T>
    inline constexpr bool is_or_v = IsOr<T>::value;
}