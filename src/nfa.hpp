#ifndef NFA_HPP
#define NFA_HPP
#pragma once

#include "fsm.hpp"
#include "fsm_builder.hpp"
#include "lnfa.hpp"

#include <vector>

namespace fsm {

class nfa final : public automaton
{
private:
    builder m_builder{};
    std::vector<int> m_current_states{};
    bool m_aborted{ false };

public:
    nfa() = delete;
    nfa(nfa const&) = default;
    nfa(nfa&&) noexcept = default;
    ~nfa() noexcept override = default;

    explicit nfa(builder const& build);
    explicit nfa(builder&& build) noexcept;

    auto operator=(nfa const&) -> nfa& = default;
    auto operator=(nfa&&) noexcept -> nfa& = default;

    auto next(char const input) -> void override;
    [[nodiscard]] auto aborted() const noexcept -> bool override;
    [[nodiscard]] auto accepted() const noexcept -> bool override;
    [[nodiscard]] auto accepts_lambda() noexcept -> bool override;
    auto reset() -> void override;
    auto print_transitions() -> void override;

    [[nodiscard]] auto to_dfa() -> builder;
};

} // namespace fsm

#endif // !NFA_HPP
