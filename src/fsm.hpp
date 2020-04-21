#ifndef FSM_HPP
#define FSM_HPP
#pragma once

#include <string>

namespace fsm {

class automaton
{
public:
    automaton() = default;
    automaton(automaton const&) = default;
    automaton(automaton&&) noexcept = default;
    virtual ~automaton() noexcept = default;

    auto operator=(automaton const&) -> automaton& = default;
    auto operator=(automaton&&) noexcept -> automaton& = default;

    virtual auto next(char const input) -> void = 0;
    [[nodiscard]] virtual auto aborted() const noexcept -> bool = 0;
    [[nodiscard]] virtual auto accepted() const noexcept -> bool = 0;
    virtual auto reset() -> void = 0;
    virtual auto print_transitions() -> void = 0;
};

[[nodiscard]] auto accepts(automaton& autom, std::string const& input) -> bool;

} // namespace fsm

#endif // !FSM_HPP
