#include "dfa.hpp"
#include "printer.hpp"

#include <algorithm>
#include <deque>
#include <iterator>
#include <utility>
#include <vector>

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

    std::cout << "Final states: [ ";
    for(int const state : m_builder.get_accepting_states()) {
        std::cout << state << ' ';
    }
    std::cout << "]" << std::endl;

    for(auto const& [state, transitions] : autom) {
        std::cout << state << ": ";
        print(transitions);
        std::cout << std::endl;
    }
}

template<typename T, typename U>
static auto as(U const& param) -> T
{
    return static_cast<T>(param);
}

auto dfa::get_reachable_from(int const state) const -> std::set<int>
{
    std::set<int> inserted{};
    std::set<int> reachable{};
    std::deque<int> queue{};

    auto insert = [&inserted, &queue](int const state) -> void {
        if(inserted.count(state) > 0U) {
            return;
        }

        inserted.insert(state);
        queue.push_back(state);
    };

    insert(state);

    for(; !queue.empty(); queue.pop_front()) {
        int const current_state = queue.front();
        auto const& autom = m_builder.get_configuration();

        reachable.insert(current_state);

        for(auto const& transition : autom.at(current_state)) {
            insert(transition.to);
        }
    }

    return reachable;
}

[[nodiscard]] auto operator-(std::set<int> const& left,
                             std::set<int> const& right) -> std::set<int>
{
    std::set<int> result{};

    std::set_difference(left.begin(),
                        left.end(),
                        right.begin(),
                        right.end(),
                        std::inserter(result, result.begin()));

    return result;
}

auto dfa::get_equivalent_states(
    std::map<int, std::vector<fsm::impl::transition>> const& autom) const
    -> std::set<int>
{
    std::map<int, std::map<char, int>> repr{};
    std::set<int> result{};

    for(auto const& [state, transitions] : autom) {
        for(auto const& transition : transitions) {
            repr[state][transition.on] = transition.to;
        }
    }

    auto end_it = repr.end();
    std::advance(end_it, -1);

    auto is_final = [&](int const state) -> bool {
        auto const& finals = m_builder.get_accepting_states();

        return std::find(finals.begin(), finals.end(), state) != finals.end();
    };

    for(auto it = repr.begin(); it != end_it; ++it) {
        auto it2 = it;
        std::advance(it2, 1);

        for(; it2 != repr.end(); ++it2) {
            if(is_final(it->first) == is_final(it2->first) &&
               it->second == it2->second) {
                result.insert(it->first);
                result.insert(it2->first);
            }
        }

        if(!result.empty()) {
            return result;
        }
    }

    return result;
}

auto dfa::remove_equivalent(
    std::map<int, std::vector<fsm::impl::transition>> const& autom,
    std::set<int> const& equiv) const
    -> std::map<int, std::vector<fsm::impl::transition>>
{
    std::map<int, std::vector<fsm::impl::transition>> result{ autom };
    auto it = equiv.begin();
    std::advance(it, 1);

    for(; it != equiv.end(); ++it) {
        result.erase(*it);
    }

    for(auto& [state, transitions] : result) {
        for(auto& transition : transitions) {
            if(equiv.count(transition.to) > 0U) {
                transition.to = *equiv.begin();
            }
        }
    }

    std::cout << "\nRemove equivalent states:\n";
    for(auto const& [state, transitions] : result) {
        std::cout << state << ": ";
        print(transitions);
        std::cout << std::endl;
    }

    return result;
}

auto dfa::minimize() const -> builder
{
    builder result{};
    auto const& autom = m_builder.get_configuration();
    std::set<int> all_states{};

    for(auto const& [state, transitions] : autom) {
        all_states.insert(state);
    }

    auto const unreachable_states =
        all_states - this->get_reachable_from(m_builder.get_starting_state());

    std::cout << "Unreachable: ";
    print(unreachable_states);
    std::cout << std::endl;

    auto new_autom = autom;

    for(int const state : unreachable_states) {
        new_autom.erase(state);
    }

    std::cout << "\nRemove unreachable states:\n";
    for(auto const& [state, transitions] : new_autom) {
        std::cout << state << ": ";
        print(transitions);
        std::cout << std::endl;
    }

    auto equiv = this->get_equivalent_states(new_autom);

    while(!equiv.empty()) {
        std::cout << "\nEquivalent states: ";
        print(equiv);
        std::cout << std::endl;

        new_autom = this->remove_equivalent(new_autom, equiv);
        equiv = this->get_equivalent_states(new_autom);
    }

    result.set_starting_state(m_builder.get_starting_state());

    auto is_final = [&](int const state) -> bool {
        auto const& finals = m_builder.get_accepting_states();

        return std::find(finals.begin(), finals.end(), state) != finals.end();
    };

    for(auto const& [state, transitions] : new_autom) {
        if(is_final(state)) {
            result.set_accepting_state(state);
        }

        for(auto const& transition : transitions) {
            result.add_transition(state, transition.on, transition.to);
        }
    }

    return result;
}

} // namespace fsm
