#include "lnfa.hpp"
#include "printer.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

namespace fsm {

lnfa::lnfa(builder const& build)
    : m_builder{ build }
{
    m_current_states.push_back(m_builder.get_starting_state());
}

lnfa::lnfa(builder&& build) noexcept
    : m_builder{ std::move(build) }
{
    m_current_states.push_back(m_builder.get_starting_state());
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

auto lnfa::lambda_suffix(int const from) -> std::set<int>
{
    std::set<int> result{};

    this->check_lambda(result, from);

    return result;
}

auto lnfa::can_go_to(std::set<int> const& input, char const on) const
    -> std::set<int>
{
    std::set<int> result{};
    auto const& autom = m_builder.get_configuration();

    for(int const state : input) {
        auto const& transitions = autom.at(state);

        for(auto const& transition : transitions) {
            if(transition.on == on) {
                result.insert(transition.to);
            }
        }
    }

    return result;
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

auto lnfa::accepts_lambda() noexcept -> bool
{
    auto const& states = this->lambda_suffix(m_builder.get_starting_state());
    auto const& final_states = m_builder.get_accepting_states();

    for(int const state : states) {
        auto const it =
            std::find(final_states.begin(), final_states.end(), state);

        if(it != final_states.end()) {
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

auto lnfa::print_transitions() -> void
{
    auto const& autom = m_builder.get_configuration();

    for(auto const& [state, transitions] : autom) {
        std::cout << state << ':' << '\t';
        print(transitions);
        std::cout << std::endl;
    }
}

auto lnfa::print_enclosing(
    std::map<char, std::vector<std::set<int>>> const& enclosing) -> void
{
    // auto const& autom = m_builder.get_configuration();

    for(char const ch : m_builder.get_alphabet()) {
        std::cout << ch << ":\n";
        auto size = enclosing.at(ch).size();

        for(auto i = 0U; i < size; ++i) {
            std::cout << i << ": ";
            print(enclosing.at(ch)[i]);
            std::cout << std::endl;
        }
    }
}

auto lnfa::get_identical_states(
    std::map<char, std::vector<std::set<int>>> const& enclosing) const
    -> std::set<int>
{
    auto is_final = [this](unsigned const state) -> bool {
        auto const it = std::find(m_all_final_states.begin(),
                                  m_all_final_states.end(),
                                  static_cast<int>(state));
        return it != m_all_final_states.end();
    };

    std::set<int> result{};
    // path[i] will be where does state i point to on each char of the alphabet
    // so path[i][j] is where does state i point to on character j
    // two paths are identical if path[i] == path[j]
    //  and
    // is_final(i) == is_final(j)
    std::vector<std::vector<std::set<int>>> path{};
    auto const& autom = m_builder.get_configuration();

    for(auto i = 0U; i < autom.size(); ++i) {
        path.emplace_back();

        for(char const ch : m_builder.get_alphabet()) {
            path.back().emplace_back(enclosing.at(ch)[i]);
        }
    }

    for(auto i = 0U; i < autom.size() - 1; ++i) {
        bool found{ false };

        for(auto j = i + 1; j < autom.size(); ++j) {
            if(is_final(i) == is_final(j) && path[i] == path[j]) {
                result.insert(static_cast<int>(i));
                result.insert(static_cast<int>(j));
                found = true;
            }
        }

        if(found) {
            return result;
        }
    }

    return result;
}

auto lnfa::rename_redundant_states(std::set<int>& input,
                                   std::set<int> const& redundant) const -> void
{
    if(redundant.empty()) {
        return;
    }

    std::set<int> new_set{};

    for(auto it = input.begin(); it != input.end(); ++it) {
        if(redundant.count(*it) > 0U) {
            new_set.insert(*redundant.begin());
        }
        else {
            new_set.insert(*it);
        }
    }

    input = new_set;
}

[[nodiscard]] static auto get_new_index(int const state,
                                        std::set<int> const& redundant) -> int
{
    if(redundant.empty()) {
        return state;
    }

    int index{ 0 };
    auto it = redundant.begin();
    ++it;

    for(; it != redundant.end(); ++it) {
        if(*it == state) {
            return *redundant.begin();
        }
        if(*it < state) {
            ++index;
        }
    }

    return state - index;
}

auto lnfa::build_nfa(
    fsm::builder& build,
    std::map<char, std::vector<std::set<int>>> const& enclosing,
    std::set<int> const& redundant) -> void
{
    auto new_idx = [&redundant](int const state) -> int {
        return get_new_index(state, redundant);
    };

    build.set_starting_state(new_idx(m_builder.get_starting_state()));

    std::set<int> unique_final_states{};
    for(int const state : m_all_final_states) {
        unique_final_states.insert(new_idx(state));
    }
    for(int const state : unique_final_states) {
        build.set_accepting_state(state);
    }

    for(auto const& [ch, states] : enclosing) {
        for(auto i = 0U; i < states.size(); ++i) {
            for(int const state : states[i]) {
                build.add_transition(static_cast<int>(i), ch, new_idx(state));
            }
        }
    }

    auto const& autom = build.get_configuration();

    std::cout << "NFA:\n";
    for(auto const& [state, transitions] : autom) {
        std::cout << state << ": ";
        print(transitions);
        std::cout << std::endl;
    }
}

auto lnfa::to_nfa() -> builder
{
    builder result{};
    std::vector<std::set<int>> path{};
    std::map<char, std::vector<std::set<int>>> enclosing{};
    auto const& autom = m_builder.get_configuration();
    auto const& final_states = m_builder.get_accepting_states();

    path.resize(autom.size());
    m_all_final_states.insert(final_states.begin(), final_states.end());

    auto is_final = [this](std::set<int> const& set) -> bool {
        auto const& finals = m_builder.get_accepting_states();

        for(int const state : set) {
            auto const it = std::find(finals.begin(), finals.end(), state);

            if(it != finals.end()) {
                return true;
            }
        }

        return false;
    };

    for(auto i = 0U; i < autom.size(); ++i) {
        path[i] = this->lambda_suffix(static_cast<int>(i));

        if(is_final(path[i])) {
            m_all_final_states.insert(static_cast<int>(i));
        }

        for(char const ch : m_builder.get_alphabet()) {
            auto const& states = this->can_go_to(path[i], ch);
            std::set<int> final_path{};

            for(int const state : states) {
                auto const& tmp = this->lambda_suffix(state);
                final_path.insert(tmp.begin(), tmp.end());
            }

            enclosing[ch].push_back(std::move(final_path));
        }
    }

    std::cout << "Testing enclosing: " << std::endl;
    this->print_enclosing(enclosing);

    auto const& identical_states = this->get_identical_states(enclosing);

    std::cout << "Identical states: " << std::endl;
    print(identical_states);
    std::cout << std::endl << std::endl;

    std::cout << "Final states: \n";
    for(auto const s : m_all_final_states) {
        std::cout << s << ' ';
    }
    std::cout << std::endl;

    for(auto i = identical_states.size() - 1; i > 0U; --i) {
        auto it = identical_states.begin();
        std::advance(it, i);
        int const state = *it;

        for(char const ch : m_builder.get_alphabet()) {
            auto iter = enclosing.at(ch).begin();
            std::advance(iter, state);
            enclosing.at(ch).erase(iter);
        }
    }

    for(char const ch : m_builder.get_alphabet()) {
        for(auto& set : enclosing.at(ch)) {
            this->rename_redundant_states(set, identical_states);
        }
    }

    std::cout << "After removal: \n";
    this->print_enclosing(enclosing);
    std::cout << std::endl;

    this->build_nfa(result, enclosing, identical_states);

    return result;
}

} // namespace fsm
