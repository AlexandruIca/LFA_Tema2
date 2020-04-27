#include "dfa.hpp"
#include "printer.hpp"

#include <algorithm>
#include <utility>

namespace fsm {

dfa::dfa(builder const& build)
    : m_builder{ build }
{
    m_current_state = m_builder.get_starting_state();
}

dfa::dfa(builder&& build) noexcept
    : m_builder{ std::move(build) }
{
    m_current_state = m_builder.get_starting_state();
}

auto dfa::next(char const input) -> void
{
    auto const& autom = m_builder.get_configuration();

    for(auto const& transition : autom.at(m_current_state)) {
        if(transition.on == input) {
            m_current_state = transition.to;
            return;
        }
    }

    m_aborted = true;
}

auto dfa::aborted() const noexcept -> bool
{
    return m_aborted;
}

auto dfa::accepted() const noexcept -> bool
{
    auto const& accepting_states = m_builder.get_accepting_states();
    auto const it = std::find(
        accepting_states.begin(), accepting_states.end(), m_current_state);
    return it != accepting_states.end();
}

auto dfa::accepts_lambda() noexcept -> bool
{
    return this->accepted();
}

auto dfa::reset() -> void
{
    m_current_state = m_builder.get_starting_state();
    m_aborted = false;
}

auto dfa::print_transitions() -> void
{
    auto const& autom = m_builder.get_configuration();

    for(auto const& [state, transitions] : autom) {
        std::cout << state << ": ";
        print(transitions);
        std::cout << std::endl;
    }
}

} // namespace fsm
