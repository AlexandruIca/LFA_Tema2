#define MAIN_EXECUTABLE
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

TEST("[LNFA -> NFA]")
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

    fsm::nfa nfa{ lnfa.to_nfa() };

    ASSERT_ACCEPT(nfa, "");
    ASSERT_ACCEPT(nfa, "a");
    ASSERT_ACCEPT(nfa, "b");
    ASSERT_ACCEPT(nfa, "ab");
    ASSERT_ACCEPT(nfa, "bbbb");
    ASSERT_NOT_ACCEPT(nfa, "c");
    ASSERT_NOT_ACCEPT(nfa, "aabbbbcbaab");
}
