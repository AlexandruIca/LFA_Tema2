#ifndef DFA_HPP
#define DFA_HPP
#pragma once

#include "fsm.hpp"
#include "fsm_builder.hpp"

namespace fsm {

class dfa final : public automaton
{
private:
    builder m_builder{};
    int m_current_state{ 0 };
    bool m_aborted{ false };

public:
    dfa() = delete;
    dfa(dfa const&) = default;
    dfa(dfa&&) noexcept = default;
    ~dfa() noexcept override = default;

    explicit dfa(builder const& build);
    explicit dfa(builder&& build) noexcept;

    auto operator=(dfa const&) -> dfa& = default;
    auto operator=(dfa&&) noexcept -> dfa& = default;

    auto next(char const input) -> void override;
    [[nodiscard]] auto aborted() const noexcept -> bool override;
    [[nodiscard]] auto accepted() const noexcept -> bool override;
    [[nodiscard]] auto accepts_lambda() noexcept -> bool override;
    auto reset() -> void override;
    auto print_transitions() -> void override;
};

} // namespace fsm

#endif // !DFA_HPP
