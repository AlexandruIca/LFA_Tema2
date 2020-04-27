#include "nfa.hpp"
#include "printer.hpp"
#include "transition.hpp"

#include <algorithm>
#include <deque>
#include <map>
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
        std::cout << state << ": ";
        print(transitions);
        std::cout << std::endl;
    }
}

auto nfa::to_dfa() -> builder
{
    builder result{};
    auto const& autom = m_builder.get_configuration();
    std::deque<std::set<int>> additional_states{};
    std::map<int, std::map<char, std::set<int>>> new_autom{};
    int max_state{ 0 };
    int max_state_copy{ 0 };

    auto already_seen = [&additional_states](std::set<int> const& set) {
        return std::find(additional_states.begin(),
                         additional_states.end(),
                         set) != additional_states.end();
    };

    for(auto const& [state, transitions] : autom) {
        if(state > max_state) {
            max_state = state;
            max_state_copy = max_state;
        }

        for(char const ch : m_builder.get_alphabet()) {
            std::set<int> path{};

            for(auto const& transition : transitions) {
                if(transition.on == ch) {
                    path.insert(transition.to);
                }
            }

            if(path.size() > 1U && !already_seen(path)) {
                additional_states.push_back(path);
            }

            new_autom[state][ch] = path;
        }
    }

    std::cout << "\nAdditional states:\n";
    for(auto const& set : additional_states) {
        print(set);
        std::cout << ' ';
    }
    std::cout << std::endl << std::endl;

    std::cout << "New autom:\n";
    for(auto const& [state, transitions] : new_autom) {
        std::cout << state << ":\n";

        for(auto const& [ch, path] : transitions) {
            std::cout << ch << " -> ";
            print(path);
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    std::deque<std::set<int>> additional_states_copy{ additional_states };

    while(!additional_states_copy.empty()) {
        auto const& to_check = additional_states_copy.front();
        ++max_state;

        for(char const ch : m_builder.get_alphabet()) {
            std::set<int> path{};
            for(int const state : to_check) {
                for(auto const& transition : autom.at(state)) {
                    if(transition.on == ch) {
                        path.insert(transition.to);
                    }
                }
            }

            if(path.size() > 1U && !already_seen(path)) {
                additional_states.push_back(path);
                additional_states_copy.push_back(path);
            }

            new_autom[max_state][ch] = path;
        }

        additional_states_copy.pop_front();
    }

    std::cout << "\nAdditional states:\n";
    for(auto const& set : additional_states) {
        print(set);
        std::cout << ' ';
    }
    std::cout << std::endl << std::endl;

    auto index = [&additional_states,
                  max_state_copy](std::set<int> const& state) -> int {
        if(state.empty()) {
            return -1;
        }
        if(state.size() == 1U) {
            return *state.begin();
        }
        auto it = std::find(
            additional_states.begin(), additional_states.end(), state);

        return static_cast<int>(std::distance(additional_states.begin(), it)) +
               max_state_copy + 1;
    };

    std::cout << "New autom:\n";
    for(auto const& [state, transitions] : new_autom) {
        std::cout << state << ":\n";

        for(auto const& [ch, path] : transitions) {
            std::cout << ch << " -> ";
            if(path.empty()) {
                std::cout << "{}";
            }
            else if(path.size() == 1U) {
                std::cout << *path.begin();
            }
            else {
                std::cout << index(path);
            }
            // print(path);
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    auto is_final = [this](std::set<int> const& set) -> bool {
        auto const& accepting_states = m_builder.get_accepting_states();
        for(int const state : set) {
            auto it = std::find(
                accepting_states.begin(), accepting_states.end(), state);

            if(it != accepting_states.end()) {
                return true;
            }
        }

        return false;
    };

    result.set_starting_state(m_builder.get_starting_state());

    auto const& old_final_states = m_builder.get_accepting_states();
    std::set<int> new_final_states{};

    if(!old_final_states.empty()) {
        new_final_states.insert(old_final_states.begin(),
                                old_final_states.end());
    }

    for(auto const& [state, transitions] : new_autom) {
        for(auto const& [ch, path] : transitions) {
            if(path.size() > 1U && is_final(path)) {
                new_final_states.insert(state);
            }

            auto const idx = index(path);
            if(idx > 0) {
                result.add_transition(state, ch, idx);
            }
        }
    }

    std::cout << "New final states: ";
    print(new_final_states);
    std::cout << std::endl;
    for(auto const& final_state : new_final_states) {
        result.set_accepting_state(final_state);
    }

    return result;
}

} // namespace fsm
