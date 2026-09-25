# Proof Assistant

*A simple compile-time proof assistant concept using C++23 template metaprogramming and the Curry-Howard correspondence.*

---

```cpp
struct Sunny{};
struct Hot{};

constexpr Hot sunny_implies_hot(Sunny) {
    return {};
}

int main() {
    Sunny sunny{};

    static_assert(Proof::Proves<decltype(sunny_imples_hot), Sunny, Hot>)

    Hot hot = Proof::Apply(sunny_implies_hot, sunny); 
    
    auto sunny_and_hot = Proof:prove_and(sunny, hot);

    static_assert(Proof::is_and_v<decltype(sunny_and_hot)>);
}
```
The Curry-Howard correspondence is a concept from types and programming languages theory that demonstrates the connection between formal logic and programming: propositions correspond to types, and proofs correspond to values of those types. In the example above, `Sunny` and `Hot` are propositions, while `sunny_implies_hot` represents a proof of `Sunny → Hot`. `Proof::Proves` verifies that implication at compile time, and `Proof::Apply` uses modus poens to derive a proof of `Hot`. The resulting proofs can then be composed into larger propositions, such as `Sunny ∧ Hot`.

The library uses the C++ compiler itself as the proof checker. Invalid proof constructions and ill-formed logical statements fail during compilation, while a successful build indicates that the encoded proofs satisfy the required logical constraints.

$$
\mathrm{Sunny}, \quad
\mathrm{Sunny} \rightarrow \mathrm{Hot}
\;\vdash\;
\mathrm{Hot}
$$

## Library Overview

### `proposition.hpp`

Defines 'Contradiction', the library's representation of logical falsehood (⊥). Its deleted constructor makes it intentionally uninhabitable.

Atomic propositions (axioms) are ordinary user-defined C++ types:

```cpp
struct Sunny{}; // proposition
Sunny sunny{}; // proof/assumption of Sunny
```

### `connectives.hpp`

Defines conjunction (logical AND) and disjunction (logical OR).

- `And<P, Q>` represents `P ∧ Q`.
- `prove_and` performs conjunction introduction.
- `and_left` and `and_right` perform conjunction elimination
- `Or<P, Q, B>` represnts `P ∨ Q`

Unlike `And<P, Q>`, a proof of `Or<P, Q>` must not require proofs of both propositions. `Or` therefore stores only the proven branch. The IsLeft template parameter records which branch is inhabited, allowing `std::conditional_t` to select the stored proof type at compile time. This preserves the logical distinction between conjunction ("both"), and disjunction ("at least one").

### `theorems.hpp`

Implements reusable proofs and transformations, including

- Conjunction commutativity
- Conjunction associativity
- Implication composition
- Contraposition
- De Morgan's law

The theorem functions all construnct callable proofs of logical implictations. These callables can then be passed to `Proof::Apply` together with a proof satisfying the theorem's premise, producing a proof of its conclusion.

```cpp
auto commutativity = Proof::theorem_and_commutative<Sunny, Hot>();

auto hot_and_sunny = Proof::Apply(commutativity, sunny_and_hot);

static_assert(std::same_as<decltype(hot_and_sunny), Proof::And<Hot, Sunny>)
```

The library's API is designed to model the underlying mathematics directly. Proofs of implications are represented as callables, allowing theorem application and proof composition to correspond closely to function application and composition in C++.

```cpp
struct Sunny{};
struct Hot{};
struct ACWorking;

constexpr Hot sunny_implies_hot(Sunny) {
    return {};
}

constexpr ACWorking hot_impiles_ac_working(Hot) {
    return {};
}

Sunny sunny{};

auto sunny_implies_ac_working = Proof::compose(sunny_implies_hot, hot_implies_ac_working);
static_assert(Proof::Proves<decltype(sunny_implies_ac_working)>, Sunny, ACWorking);
ACWorking ac_working = Proof::Apply(sunny_implies_ac_working, sunny);
```

### `inspection.hpp`

Provides compile-time traits for identifying prosition structures.

- `is_and_v`
- `is_or_v`

### `tree.hpp`

Treats nested proposition types as compile-time syntax trees, providing:

- `depth_v`
- `node_count_v`
- `leaf_count_v`
- `contains_v`

        And
       /   \
      Q     Or
           / \
          P   R

The above tree is a representation of `And<Q, Or<P, R, true>>`. `true` is disjunction branch metadata, and therefore is not treated as a child in the proposition syntax tree.

---

## Future Considerations

### Compiler-backed Python frontend

The repository contains experimental pybind11 infrastructure for exposing C++ types to Python. However, Python values are created at runtime and therefore cannot directly instantiate arbitrary C++ template types after compilation. A future frontend could instead translate Python proof expressions into C++ source and invoke the compiler as a proof-checking backend. This would preserve the existing compile-time proof machinery while providing a more ergonomic Python interface.

### Additional logical constructs

The current implementation focuses on a small set of propositional logic operations and theorem transformations. The type-level representation could be extended with additional connectives, inference rules, and reusable theorems.

### Compile-time performance analysis

Because proof checking is performed through template instantiation, larger proposition trees could be used to investigate compilation cost. Potential experiments could include comparing GCC and Clang compile times, template-instantiation behavior, and compiler memory usage as proof complexity increases.




