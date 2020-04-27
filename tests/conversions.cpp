#define MAIN_EXECUTABLE
#include "dfa.hpp"
#include "fsm_builder.hpp"
#include "lnfa.hpp"
#include "nfa.hpp"
#include "test.hpp"

#include <string>

#define ASSERT_ACCEPT(autom, input)                                            \
    ASSERT(fsm::accepts(autom, input));                                        \
    (autom).reset()

#define ASSERT_NOT_ACCEPT(autom, input)                                        \
    ASSERT(!fsm::accepts(autom, input));                                       \
    (autom).reset()

using vec = std::vector<int>;

template<typename T, typename U>
[[nodiscard]] auto eq(T const& a, U const& b) noexcept -> bool
{
    return a == b;
}

TEST("[LNFA -> NFA -> DFA -> Min-DFA]")
{
    using fsm::lambda;
    fsm::builder builder{};

    builder.set_starting_state(0);
    builder.set_accepting_state(2);
    builder.set_accepting_state(6);

    builder.add_transition(0, 'a', 0);
    builder.add_transition(0, 'a', 1);
    builder.add_transition(0, 'b', 2);
    builder.add_transition(0, lambda, 2);
    builder.add_transition(0, lambda, 3);
    builder.add_transition(1, lambda, 2);
    builder.add_transition(2, 'a', 3);
    builder.add_transition(2, lambda, 4);
    builder.add_transition(3, 'b', 3);
    builder.add_transition(3, lambda, 5);
    builder.add_transition(3, 'a', 6);
    builder.add_transition(3, 'b', 6);
    builder.add_transition(4, 'b', 5);
    builder.add_transition(4, 'a', 6);
    builder.add_transition(4, lambda, 6);
    builder.add_transition(5, lambda, 2);
    builder.add_transition(5, 'b', 2);
    builder.add_transition(5, lambda, 6);
    builder.add_transition(5, 'a', 6);
    builder.add_transition(6, 'b', 6);

    ASSERT(builder.get_alphabet() == "ab");

    fsm::lnfa lnfa{ builder };

    ASSERT_ACCEPT(lnfa, "");
    ASSERT_ACCEPT(lnfa, "a");
    ASSERT_ACCEPT(lnfa, "b");
    ASSERT_ACCEPT(lnfa, "ab");
    ASSERT_ACCEPT(lnfa, "bbbb");
    ASSERT_NOT_ACCEPT(lnfa, "c");
    ASSERT_NOT_ACCEPT(lnfa, "aabbbbcbaab");

    std::cout << "LNFA:\n";
    lnfa.print_transitions();

    fsm::nfa nfa{ lnfa.to_nfa() };

    ASSERT_ACCEPT(nfa, "");
    ASSERT_ACCEPT(nfa, "a");
    ASSERT_ACCEPT(nfa, "b");
    ASSERT_ACCEPT(nfa, "ab");
    ASSERT_ACCEPT(nfa, "bbbb");
    ASSERT_NOT_ACCEPT(nfa, "c");
    ASSERT_NOT_ACCEPT(nfa, "aabbbbcbaab");

    std::cout << "\nNFA:\n";
    nfa.print_transitions();

    fsm::dfa dfa{ nfa.to_dfa() };

    ASSERT_ACCEPT(dfa, "");
    ASSERT_ACCEPT(dfa, "a");
    ASSERT_ACCEPT(dfa, "b");
    ASSERT_ACCEPT(dfa, "ab");
    ASSERT_ACCEPT(dfa, "bbbb");
    ASSERT_NOT_ACCEPT(dfa, "c");
    ASSERT_NOT_ACCEPT(dfa, "aabbbbcbaab");

    std::cout << "\nDFA:\n";
    dfa.print_transitions();

    fsm::dfa min_dfa{ dfa.minimize() };

    ASSERT_ACCEPT(min_dfa, "");
    ASSERT_ACCEPT(min_dfa, "a");
    ASSERT_ACCEPT(min_dfa, "b");
    ASSERT_ACCEPT(min_dfa, "ab");
    ASSERT_ACCEPT(min_dfa, "bbbb");
    ASSERT_NOT_ACCEPT(min_dfa, "c");
    ASSERT_NOT_ACCEPT(min_dfa, "aabbbbcbaab");

    std::cout << "\nMin-DFA:\n";
    min_dfa.print_transitions();
}

TEST("[NFA -> DFA -> Min-DFA]")
{
    fsm::builder builder{};

    builder.set_starting_state(0);
    builder.set_accepting_state(2);

    builder.add_transition(0, 'a', 0);
    builder.add_transition(0, 'a', 1);
    builder.add_transition(1, 'a', 1);
    builder.add_transition(1, 'b', 1);
    builder.add_transition(1, 'b', 2);
    builder.add_transition(2, 'a', 1);
    builder.add_transition(2, 'b', 2);

    fsm::nfa nfa{ builder };

    ASSERT_NOT_ACCEPT(nfa, "");
    ASSERT_NOT_ACCEPT(nfa, "b");
    ASSERT_ACCEPT(nfa, "ab");
    ASSERT_ACCEPT(nfa, "aaaabbbbb");
    ASSERT_NOT_ACCEPT(nfa, "aaaabbbbba");

    fsm::dfa dfa{ nfa.to_dfa() };

    ASSERT_NOT_ACCEPT(dfa, "");
    ASSERT_NOT_ACCEPT(dfa, "b");
    ASSERT_ACCEPT(dfa, "ab");
    ASSERT_ACCEPT(dfa, "aaaabbbbb");
    ASSERT_ACCEPT(dfa, "aaaabbbbbab");

    fsm::dfa min_dfa{ dfa.minimize() };

    ASSERT_NOT_ACCEPT(min_dfa, "");
    ASSERT_NOT_ACCEPT(min_dfa, "b");
    ASSERT_ACCEPT(min_dfa, "ab");
    ASSERT_ACCEPT(min_dfa, "aaaabbbbb");
    ASSERT_ACCEPT(min_dfa, "aaaabbbbbab");
}

TEST("[DFA -> Min-DFA]")
{
    fsm::builder builder{};

    builder.set_starting_state(0);
    builder.set_accepting_state(2);
    builder.set_accepting_state(3);
    builder.set_accepting_state(4);

    builder.add_transition(0, 'a', 1);
    builder.add_transition(0, 'b', 2);
    builder.add_transition(1, 'a', 0);
    builder.add_transition(1, 'b', 3);
    builder.add_transition(2, 'a', 4);
    builder.add_transition(2, 'b', 5);
    builder.add_transition(3, 'a', 4);
    builder.add_transition(3, 'b', 5);
    builder.add_transition(4, 'a', 4);
    builder.add_transition(4, 'b', 5);
    builder.add_transition(5, 'a', 5);
    builder.add_transition(5, 'b', 5);

    fsm::dfa dfa{ builder };

    ASSERT_NOT_ACCEPT(dfa, "");
    ASSERT_ACCEPT(dfa, "ab");
    ASSERT_NOT_ACCEPT(dfa, "bb");
    ASSERT_ACCEPT(dfa, "ba");
    ASSERT_ACCEPT(dfa, "abaaa");
    ASSERT_NOT_ACCEPT(dfa, "abaaab");

    fsm::dfa min_dfa{ dfa.minimize() };

    ASSERT_NOT_ACCEPT(min_dfa, "");
    ASSERT_ACCEPT(min_dfa, "ab");
    ASSERT_NOT_ACCEPT(min_dfa, "bb");
    ASSERT_ACCEPT(min_dfa, "ba");
    ASSERT_ACCEPT(min_dfa, "abaaa");
    ASSERT_NOT_ACCEPT(min_dfa, "abaaab");
}
