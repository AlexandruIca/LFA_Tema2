#include "nfa.hpp"
#include "printer.hpp"

#include <algorithm>
#include <set>
#include <utility>

namespace fsm {

nfa::nfa(builder const& build)
    : m_builder{ build }
{
    m_current_states.push_back(m_builder.get_starting_state());
}

nfa::nfa(builder&& build) noexcept
    : m_builder{ std::move(build) }
{
    m_current_states.push_back(m_builder.get_starting_state());
}

auto nfa::next(char const input) -> void
{
    std::set<int> next_states{};
    auto const& autom = m_builder.get_configuration();

    for(int const current_state : m_current_states) {
        for(auto const& transition : autom.at(current_state)) {
            if(transition.on == input) {
                next_states.insert(transition.to);
            }
        }
    }

    if(next_states.empty()) {
        m_aborted = true;
    }

    m_current_states.clear();
    m_current_states.assign(next_states.begin(), next_states.end());
}

auto nfa::aborted() const noexcept -> bool
{
    return m_aborted;
}

auto nfa::accepted() const noexcept -> bool
{
    auto const& accepting_states = m_builder.get_accepting_states();

    for(int const state : m_current_states) {
        auto it =
            std::find(accepting_states.begin(), accepting_states.end(), state);
        if(it != accepting_states.end()) {
            return true;
        }
    }

    return false;
}

auto nfa::accepts_lambda() noexcept -> bool
{
    return this->accepted();
}

auto nfa::reset() -> void
{
    m_current_states.clear();
    m_current_states.push_back(m_builder.get_starting_state());
    m_aborted = false;
}

auto nfa::print_transitions() -> void
{
    auto const& autom = m_builder.get_configuration();

    for(auto const& [state, transitions] : autom) {
        std::cout << state << ':' << '\t';
        print(transitions);
        std::cout << std::endl;
    }
}

auto nfa::to_dfa() -> builder
{
    builder result{};

    return result;
}

} // namespace fsm
