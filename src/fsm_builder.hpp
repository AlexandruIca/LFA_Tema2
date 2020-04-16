#ifndef FSM_BUILDER_HPP
#define FSM_BUILDER_HPP
#pragma once

#include <map>
#include <vector>

#include "transition.hpp"

namespace fsm {

class builder
{
private:
    using transition_t = fsm::impl::transition;
    using automaton_t = std::map<int, std::vector<transition_t>>;

    automaton_t m_automaton{};
    std::vector<int> m_accepting_states{};
    int m_starting_state{ 0 };

public:
    builder() = default;
    builder(builder const&) = default;
    builder(builder&&) noexcept = default;
    ~builder() noexcept = default;

    auto operator=(builder const&) -> builder& = default;
    auto operator=(builder&&) noexcept -> builder& = default;

    auto add_transition(int const from, char const on, int const to) -> void;
    auto set_starting_state(int const state) -> void;
    auto set_accepting_state(int const state) -> void;

    [[nodiscard]] auto get_configuration() const noexcept -> automaton_t const&;
    [[nodiscard]] auto get_accepting_states() const noexcept
        -> std::vector<int> const&;
    [[nodiscard]] auto get_starting_state() const noexcept -> int;
};

} // namespace fsm

#endif // !FSM_BUILDER_HPP
