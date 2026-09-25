#pragma once

#include <concepts>
#include "proof/proposition.hpp"

namespace Proof {

    /// Implication: P → Q.
    /// Satisfied when F maps a proof of P to a proof of Q.

    template<typename F, typename P, typename Q>
    concept Proves = requires(F f, P p) {
    f(p);
    requires std::same_as<decltype(f(p)), Q>;
    };

    /// Modus ponens: P, P → Q ⊢ Q.
    /// Applies a proof of P → Q to a proof of P.

    template<typename F, typename P>
    constexpr auto Apply(F f, P p) -> decltype(f(p)) {
    return f(p);
    }

    /// Negation: ¬P ≡ P → ⊥.
    /// Satisfied when F maps a proof of P to contradiction.

    template<typename F, typename P>
    concept ProvesNot = Proves<F, P, Contradiction>;
}