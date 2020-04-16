#include "fsm_builder.hpp"

#include <algorithm>

namespace fsm {

auto builder::add_transition(int const from, char const on, int const to)
    -> void
{
    m_automaton[from].emplace_back(on, to);
}

auto builder::set_starting_state(int const state) -> void
{
    m_starting_state = state;
}

auto builder::set_accepting_state(int const state) -> void
{
    m_accepting_states.push_back(state);
}

auto builder::get_configuration() const noexcept -> builder::automaton_t const&
{
    return m_automaton;
}

auto builder::get_accepting_states() const noexcept -> std::vector<int> const&
{
    return m_accepting_states;
}

auto builder::get_starting_state() const noexcept -> int
{
    return m_starting_state;
}

} // namespace fsm
