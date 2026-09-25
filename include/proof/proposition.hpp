#pragma once

/// Falsehood (⊥): a proposition with no proof.
/// Used as the target of negation, where ¬P ≡ P → ⊥.

namespace Proof {

    struct Contradiction {
        Contradiction() = delete;
    };
}