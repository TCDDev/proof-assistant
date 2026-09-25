/**
 * Compile-time utilities for inspecting proposition syntax trees.
 *
 * Proposition types form a tree through nested logical connectives:
 *
 *              And
 *             /   \
 *            Q     Or
 *                 /  \
 *               P     R
 *
 * represented as:
 *
 *     And<Q, Or<P, R, true>>
 *
 * Atomic propositions form leaves, while logical connectives form
 * internal nodes. The utilities below inspect this structure entirely
 * at compile time.
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <proof/connectives.hpp>


namespace Proof {

    /// Computes the depth of a proposition tree.
    /// Atomic propositions have depth 1.

    template<typename T>
    struct Depth {
        static constexpr std::size_t value = 1;
    };

    template<typename P, typename Q>
    struct Depth<And<P, Q>> {
        static constexpr std::size_t value = 1 + std::max(Depth<P>::value, Depth<Q>::value);
    };

    template<typename P, typename Q, bool B>
    struct Depth<Or<P, Q, B>> {
        static constexpr std::size_t value = 1 + std::max(Depth<P>::value, Depth<Q>::value);
    };

    template<typename T>
    inline constexpr std::size_t depth_v = Depth<T>::value;


    /// Counts all nodes in a proposition tree,
    /// including both connectives and atomic propositions.

    template<typename T>
    struct NodeCount {
        static constexpr std::size_t value = 1;
    };

    template<typename P, typename Q>
    struct NodeCount<And<P, Q>> {
        static constexpr std::size_t value = 1 + NodeCount<P>::value + NodeCount<Q>::value;
    };

    template<typename P, typename Q, bool B>
    struct NodeCount<Or<P, Q, B>> {
        static constexpr std::size_t value = 1 + NodeCount<P>::value + NodeCount<Q>::value;
    };

    template<typename T>
    inline constexpr std::size_t node_count_v = NodeCount<T>::value;

    /// Counts the atomic propositions (leaves) in a proposition tree.
    
    template<typename T>
    struct LeafCount {
        static constexpr std::size_t value = 1; // Primary template is base case; treat anything non-specialized as a leaf.
    };

    // Template specialization performs the compile-time branching, so no explicit atomic-proposition check is needed.
    // Each branch of And and Or must contribute at least one leaf each. And and Or themselves don't contribute leaves.

    template<typename P, typename Q>
    struct LeafCount<And<P, Q>> {
        static constexpr std::size_t value = LeafCount<P>::value + LeafCount<Q>::value;
    };

    template<typename P, typename Q, bool B>
    struct LeafCount<Or<P, Q, B>> {
        static constexpr std::size_t value = LeafCount<P>::value + LeafCount<Q>::value;
    };

    template<typename T>
    inline constexpr std::size_t leaf_count_v = LeafCount<T>::value;

    /// Determines whether X occurs anywhere in the proposition tree T,
    /// including T itself.
    
    template<typename T, typename X>
    struct Contains{
        static constexpr bool value = std::is_same_v<T, X>;
    };

    template<typename P, typename Q, typename X>
    struct Contains<And<P, Q>, X> {
        static constexpr bool value = std::is_same_v<And<P, Q>, X> ||
                                            Contains<P, X>::value ||
                                            Contains<Q, X>::value;

    };

    template<typename P, typename Q, typename X, bool B>
    struct Contains<Or<P, Q, B>, X> {
        static constexpr bool value = std::is_same_v<Or<P, Q, B>, X> ||
                                            Contains<P, X>::value ||
                                            Contains<Q, X>::value;
    };

    template<typename T, typename X>
    inline constexpr bool contains_v = Contains<T, X>::value;
}