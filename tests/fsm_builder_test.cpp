#define MAIN_EXECUTABLE
#include "fsm_builder.hpp"
#include "lnfa.hpp"
#include "test.hpp"

using vec = std::vector<int>;

template<typename T, typename U>
[[nodiscard]] auto eq(T const& a, U const& b) noexcept -> bool
{
    return a == b;
}

TEST("[FSM Builder]")
{
    using fsm::lambda;
    fsm::builder builder{};

    builder.set_starting_state(0);
    builder.set_accepting_state(7);
    builder.set_accepting_state(8);

    ASSERT(builder.get_starting_state() == 0);
    ASSERT(eq(builder.get_accepting_states(), vec({ 7, 8 })));

    builder.add_transition(0, 'a', 1);
    builder.add_transition(0, 'a', 2);
    builder.add_transition(0, 'b', 2);
    builder.add_transition(1, lambda, 3);
    builder.add_transition(2, 'c', 4);
    builder.add_transition(3, 'b', 5);
    builder.add_transition(3, lambda, 4);
    builder.add_transition(4, 'a', 5);
    builder.add_transition(4, 'b', 6);
    builder.add_transition(5, lambda, 7);
    builder.add_transition(6, 'x', 7);
    builder.add_transition(7, 'y', 7);
    builder.add_transition(7, 'x', 8);
    builder.add_transition(7, 'z', 8);
    builder.add_transition(8, 'y', 6);
    builder.add_transition(8, 'y', 3);

    ASSERT(builder.get_alphabet() == "abcxyz");

    auto const& config = builder.get_configuration();

    ASSERT(config.at(0).size() == 3);
    ASSERT(config.at(1).size() == 1);
    ASSERT(config.at(2).size() == 1);
    ASSERT(config.at(3).size() == 2);
    ASSERT(config.at(4).size() == 2);
    ASSERT(config.at(5).size() == 1);
    ASSERT(config.at(6).size() == 1);
    ASSERT(config.at(7).size() == 3);
    ASSERT(config.at(8).size() == 2);
}
