#pragma once

#include "proof/proposition.hpp"
#include <proof/connectives.hpp>
#include <proof/inference.hpp>

namespace Proof {

    /// Conjunction commutativity: P ∧ Q → Q ∧ P.
    /// Swaps the proofs contained in a conjunction.

    template<typename P, typename Q>
    constexpr auto theorem_and_commutative() {
        return [](And<P, Q> premise) {
            P p = and_left(premise);
            Q q = and_right(premise);

            return prove_and(q, p);
        };
    }
    
    /// Implication composition: (P → Q), (Q → R) ⊢ (P → R).
    /// Composes two proofs by passing the result of F into G.
   
    template<typename F, typename G>
    constexpr auto compose(F f, G g) {
        return [f, g](auto p) {
            return g(f(p));
        };
    }

    /// Conjunction associativity: P ∧ (Q ∧ R) → (P ∧ Q) ∧ R.
    /// Reassociates a nested conjunction.

    template<typename P, typename Q, typename R>
    constexpr auto theorem_and_assoc() {
        return [](And<P, And<Q, R>> premise) {
            P p = and_left(premise);
            And<Q, R> qr = and_right(premise);
            Q q = and_left(qr);
            R r = and_right(qr);

            return prove_and(prove_and(p, q), r);
        };
    }    


    /// Constructs a proof of contraposition:
    /// (P → Q) → (¬Q → ¬P).
    ///
    /// Given a proof that P implies Q, produces a transformation
    /// from a proof of ¬Q into a proof of ¬P.

    /// Final proof of ¬P obtained by composing P → Q with Q → ⊥.
    
    template<typename P, typename F, typename G>
    struct NegationProof {
        F f;
        G g;

        constexpr Contradiction operator()(P p) const {
            return g(f(p));
        }
    };
    
    /// Intermediate stage of contraposition after receiving P → Q.

    template<typename P, typename F>
    struct ContrapositiveStep {
        F f;

        template<typename G>
        constexpr auto operator()(G g) const {
            return NegationProof<P, F, G>{f, g};
        }
    };

    /// Callable proof of (P → Q) → (¬Q → ¬P).

    template<typename P, typename Q>
    struct Contrapositive {
        template<typename F>
        requires Proves<F, P, Q>
        constexpr auto operator()(F f) const {
            return ContrapositiveStep<P, F>{f};
        }
    };


   template<typename P, typename Q>
    constexpr auto theorem_contrapositive() {
            return Contrapositive<P, Q>{
        };
    }

    /// Applies contraposition directly:
    /// P → Q, ¬Q ⊢ ¬P.
    
    template<typename P, typename Q, typename F, typename G>
    requires Proves<F, P, Q> && ProvesNot<G, Q>
    constexpr auto contrapositive(F f, G g) {
        return theorem_contrapositive<P, Q>()(f)(g);
    }

    /// De Morgan: ¬P ∨ ¬Q → ¬(P ∧ Q).
    /// Handles the case where the disjunction contains a proof of ¬P.

    template<typename P, typename Q, typename NP, typename NQ>
    requires ProvesNot<NP, P>
    constexpr auto theorem_demorgan(Or<NP, NQ, true> o) {
        NP not_p = o.proof;

        return [not_p](And<P, Q> a) -> Contradiction {
            return not_p(and_left(a));
        };
    }

    /// De Morgan: ¬P ∨ ¬Q → ¬(P ∧ Q).
    /// Handles the case where the disjunction contains a proof of ¬Q.

    template<typename P, typename Q, typename NP, typename NQ>
    requires ProvesNot<NQ, Q>
    constexpr auto theorem_demorgan(Or<NP, NQ, false> o) {
        NQ not_q = o.proof;

        return [not_q](And<P, Q> a) -> Contradiction {
            return not_q(and_right(a));
        };
    }
}
