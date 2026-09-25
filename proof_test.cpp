#include <concepts>
#include <proof/proposition.hpp>
#include <proof/connectives.hpp>
#include <proof/inference.hpp>
#include <proof/inspection.hpp>
#include <proof/theorems.hpp>
#include <proof/tree.hpp>

struct Rain {};
struct Wet{};
struct Slippery{};

// Rain -> Wet
constexpr Wet rain_implies_wet(Rain) {
    return {};
}

// Wet -> Slippery
constexpr Slippery wet_implies_slippery(Wet) {
    return {};
}

constexpr Proof::Contradiction not_wet(Wet) {
    throw 0; // Cannot return Contradiction directly as it's unconstructable
}

int main() {
    // Define Axiom
    Rain rain{};
    
    // Check if the proof is possible using Proves
    static_assert(Proof::Proves<decltype(rain_implies_wet), Rain, Wet>);

    // Use modus poens to derive proof of wet from proof of rain and proof of rain -> wet
    Wet wet = Proof::Apply(rain_implies_wet, rain);

    // Conjunction introduction
    auto rain_and_wet = Proof::prove_and(rain, wet);
    
    // Conjunction elimination
    auto left_rain = Proof::and_left(rain_and_wet);
    [[maybe_unused]] auto right_wet = Proof::and_right(rain_and_wet);

    // Disjunction
    auto rain_or_wet = Proof::or_left<Wet>(rain);
    auto wet_or_rain = Proof::or_right<Rain>(wet);

    // Theorem: Commutativity of Conjunction
    auto commutativity = Proof::theorem_and_commutative<Rain, Wet>();
    auto wet_and_rain = Proof::Apply(commutativity, rain_and_wet);

    // Theorem: Composability of Implications
    auto rain_implies_slippery = Proof::compose(rain_implies_wet, wet_implies_slippery);
    static_assert(Proof::Proves<decltype(rain_implies_slippery), Rain, Slippery>);
    Slippery slippery = Proof::Apply(rain_implies_slippery, rain);

    // Theorem: Associativity of Conjunction
    auto wet_and_slippery = Proof::prove_and(wet, slippery);
    auto rain_and_wet_and_slippery = Proof::prove_and(rain, wet_and_slippery);
    auto associativity = Proof::theorem_and_assoc<Rain, Wet, Slippery>();
    auto associated = Proof::Apply(associativity, rain_and_wet_and_slippery);
    static_assert(std::same_as<decltype(associated), Proof::And<Proof::And<Rain, Wet>, Slippery>>);

    // Theorem: Implication Contrapositive
    static_assert(Proof::ProvesNot<decltype(not_wet), Wet>);
    auto contrapositive = Proof::theorem_contrapositive<Rain, Wet>();
    auto step = contrapositive(rain_implies_wet);
    auto not_rain = step(not_wet);
    static_assert(Proof::ProvesNot<decltype(not_rain), Rain>);

    // Theorem: DeMorgan's Laws
    auto not_rain_or_not_wet = Proof::or_left<decltype(not_wet) >(not_rain);
    auto not_rain_or_wet_together = Proof::theorem_demorgan<Rain, Wet>(not_rain_or_not_wet);
    static_assert(Proof::ProvesNot<decltype(not_rain_or_wet_together), Proof::And<Rain, Wet>>);

    auto not_wet_or_not_rain = Proof::or_right<decltype(not_rain)>(not_wet);
    auto not_wet_or_not_rain_together = Proof::theorem_demorgan<Rain, Wet>(not_wet_or_not_rain);
    static_assert(Proof::ProvesNot<decltype(not_wet_or_not_rain_together), Proof::And<Rain, Wet>>);

    // Inspections Test:
    static_assert(!Proof::IsAnd<decltype(rain_or_wet)>::value);
    static_assert(Proof::IsAnd<decltype(rain_and_wet)>::value);
    static_assert(!Proof::IsOr<decltype(rain_and_wet)>::value);
    static_assert(Proof::IsOr<decltype(rain_or_wet)>::value);

    // Inspection Convenience Test:
    static_assert(Proof::is_and_v<decltype(rain_and_wet)>);
    static_assert(Proof::is_or_v<decltype(rain_or_wet)>);

    // Tree Depth Test:
    static_assert(Proof::Depth<decltype(left_rain)>::value == 1);
    static_assert(Proof::Depth<decltype(wet_and_rain)>::value == 2);

    auto slippery_and_wet = Proof::prove_and(slippery, wet);
    auto wet_and_rain_and_slippery_and_wet = Proof::prove_and(wet_and_rain, slippery_and_wet);
    static_assert(Proof::Depth<decltype(wet_and_rain_and_slippery_and_wet)>::value == 3);


    static_assert(Proof::Depth<decltype(wet_or_rain)>::value == 2);

    auto massive_disjunction = Proof::or_left<decltype(wet_and_rain_and_slippery_and_wet)>(not_rain_or_not_wet);
    auto even_bigger_conjunction = Proof::prove_and(massive_disjunction, not_wet_or_not_rain_together);
    
    using monstrosity = decltype(even_bigger_conjunction);

    static_assert(Proof::Depth<monstrosity>::value == 5);

    // depth_v Test
    static_assert(Proof::depth_v<monstrosity> == 5);

    // node_count_v Test
    static_assert(Proof::node_count_v<monstrosity> == 13);

    // leaf_count_v Test
    static_assert(Proof::leaf_count_v<monstrosity> == 7);

    // contains_v Test
    struct Snow{};
    
    static_assert(Proof::contains_v<monstrosity, Wet>); // Single Axiom
    static_assert(Proof::contains_v<monstrosity, decltype(massive_disjunction)>); // Compound Subtree
    static_assert(!Proof::contains_v<monstrosity, Snow>); // Does Not Contain
}   


