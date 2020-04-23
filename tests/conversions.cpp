#define MAIN_EXECUTABLE
#include "fsm_builder.hpp"
#include "lnfa.hpp"
#include "test.hpp"

#include <string>

using vec = std::vector<int>;

template<typename T, typename U>
[[nodiscard]] auto eq(T const& a, U const& b) noexcept -> bool
{
    return a == b;
}

TEST("[LNFA -> NFA]")
{
    fsm::builder builder{};

    builder.set_starting_state(0);
    builder.set_accepting_state(2);
    builder.set_accepting_state(6);

    builder.add_transition(0, 'a', 0);
    builder.add_transition(0, 'a', 1);
    builder.add_transition(0, 'b', 2);
    builder.add_transition(0, fsm::lambda, 2);
    builder.add_transition(0, fsm::lambda, 3);
    builder.add_transition(1, fsm::lambda, 2);
    builder.add_transition(2, 'a', 3);
    builder.add_transition(2, fsm::lambda, 4);
    builder.add_transition(3, 'b', 3);
    builder.add_transition(3, fsm::lambda, 5);
    builder.add_transition(3, 'a', 6);
    builder.add_transition(3, 'b', 6);
    builder.add_transition(4, 'b', 5);
    builder.add_transition(4, 'a', 6);
    builder.add_transition(4, fsm::lambda, 6);
    builder.add_transition(5, fsm::lambda, 2);
    builder.add_transition(5, 'b', 2);
    builder.add_transition(5, fsm::lambda, 6);
    builder.add_transition(5, 'a', 6);
    builder.add_transition(6, 'b', 6);

    ASSERT(builder.get_alphabet() == "ab");

    fsm::lnfa lnfa{ builder };

    ASSERT(fsm::accepts(lnfa, ""));
    ASSERT(fsm::accepts(lnfa, "a"));
    ASSERT(fsm::accepts(lnfa, "b"));
    ASSERT(fsm::accepts(lnfa, "ab"));
    ASSERT(fsm::accepts(lnfa, "bbbb"));
    ASSERT(!fsm::accepts(lnfa, "c"));
    ASSERT(!fsm::accepts(lnfa, "aabbbbcbaab"));

    static_cast<void>(lnfa.to_nfa());
}
