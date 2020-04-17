#include "lnfa.hpp"

#include <algorithm>
#include <utility>

namespace fsm {

lnfa::lnfa(builder const& build)
    : m_builder{ build }
{
}

lnfa::lnfa(builder&& build) noexcept
    : m_builder{ std::move(build) }
{
}

auto lnfa::check_lambda(std::set<int>& to_check, int const state) -> void
{
    std::vector<int> with_lambda({ state });
    std::size_t index{ 0 };

    auto const& autom = m_builder.get_configuration();

    do {
        for(auto const& transition : autom.at(with_lambda[index++])) {
            if(transition.on == lambda) {
                with_lambda.push_back(transition.to);
            }
        }
    } while(index < with_lambda.size());

    for(int const s : with_lambda) {
        to_check.insert(s);
    }
}

auto lnfa::next(char const input) -> void
{
    std::set<int> to_check{};
    auto const& autom = m_builder.get_configuration();
    int const starting_state = m_builder.get_starting_state();

    // case when there is a lambda transition from the starting state
    bool first_time = m_current_states.size() == 1U &&
                      m_current_states.back() == starting_state;

    if(first_time) {
        this->check_lambda(to_check, starting_state);

        for(auto const& state : to_check) {
            m_current_states.push_back(state);
        }

        to_check.clear();
    }

    for(auto const& current_state : m_current_states) {
        for(auto const& transition : autom.at(current_state)) {
            if(transition.on == input) {
                to_check.insert(transition.to);
                this->check_lambda(to_check, transition.to);
            }
        }
    }

    if(to_check.empty()) {
        m_aborted = true;
        return;
    }

    m_current_states.clear();
    m_current_states.assign(to_check.begin(), to_check.end());
}

auto lnfa::aborted() const noexcept -> bool
{
    return m_aborted;
}

auto lnfa::accepted() const noexcept -> bool
{
    auto const& accepting_states = m_builder.get_accepting_states();

    for(auto const& state : m_current_states) {
        auto it =
            std::find(accepting_states.begin(), accepting_states.end(), state);
        if(it != accepting_states.end()) {
            return true;
        }
    }

    return false;
}

auto lnfa::reset() -> void
{
    m_current_states.clear();
    m_current_states.push_back(m_builder.get_starting_state());
    m_aborted = false;
}

} // namespace fsm
