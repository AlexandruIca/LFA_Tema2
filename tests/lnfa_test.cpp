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

TEST("[LNFA]")
{
    fsm::builder builder{};

    builder.set_starting_state(0);
    builder.set_accepting_state(7);
    builder.set_accepting_state(8);

    builder.add_transition(0, 'a', 1);
    builder.add_transition(0, 'a', 2);
    builder.add_transition(0, 'b', 2);
    builder.add_transition(1, fsm::lambda, 3);
    builder.add_transition(2, 'c', 4);
    builder.add_transition(3, 'b', 5);
    builder.add_transition(3, fsm::lambda, 4);
    builder.add_transition(4, 'a', 5);
    builder.add_transition(4, 'b', 6);
    builder.add_transition(5, fsm::lambda, 7);
    builder.add_transition(6, 'x', 7);
    builder.add_transition(7, 'y', 7);
    builder.add_transition(7, 'x', 8);
    builder.add_transition(7, 'z', 8);
    builder.add_transition(8, 'y', 6);
    builder.add_transition(8, 'y', 3);

    fsm::lnfa lnfa{ builder };

    lnfa.print_transitions();

    std::vector<std::string> inputs{
        { "abxyyyxyby", "bcax", "bcbxxy", "abyyxz", "abyyxyx" }
    };
    std::vector<int> values{ { 1, 1, 0, 0, 1 } };

    auto index = 0U;
    for(auto const& input : inputs) {
        auto const expected = static_cast<bool>(values[index++]);
        ASSERT(fsm::accepts(lnfa, input) == expected);
        lnfa.reset();
    }
}
