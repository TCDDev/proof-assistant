#pragma once

#include <type_traits>

namespace Proof {

        /// Conjunction: P, Q ⊢ P ∧ Q
        /// Constructs a proof of P ∧ Q from proofs of P and Q.    
        
        template<typename P, typename Q>
        struct And {
        P left;
        Q right;

        constexpr And(P p, Q q) : left(p), right(q) {};
        };

        /// Conjunction introduction: P, Q ⊢ P ∧ Q.
        
        template<typename P, typename Q>
        constexpr And<P, Q> prove_and(P p, Q q) {
            return And<P, Q>{p, q};
        }

        /// Left conjunction elimination: P ∧ Q ⊢ P.

        template<typename P, typename Q>
        constexpr P and_left(And<P, Q> a) {
            return a.left;
        }

        template<typename P, typename Q>
        constexpr Q and_right(And<P, Q> a) {
            return a.right;
        }


        /// Disjunction: P ⊢ P ∨ Q or Q ⊢ P ∨ Q.
        /// Represents a proof of P ∨ Q by storing a proof of one branch.

        template<typename P, typename Q, bool IsLeft>
        struct Or {
        using proof_t = std::conditional_t<IsLeft, P, Q>;
        
        proof_t proof;

        constexpr explicit Or(proof_t proof) : proof(proof) {};
        };

        /// Left disjunction introduction: P ⊢ P ∨ Q.

        template<typename Q, typename P>
        constexpr Or<P, Q, true> or_left(P p) {
            using Result = Or<P, Q, true>;
            return Result{p};
        }

        /// Right disjunction introduction: Q ⊢ P ∨ Q.

        template<typename P, typename Q>
        constexpr Or<P, Q, false> or_right(Q q) {
            using Result = Or<P, Q, false>;
            return Result{q};
        }

}   