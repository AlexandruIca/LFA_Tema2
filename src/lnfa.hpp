#ifndef LAMBDA_NFA_HPP
#define LAMBDA_NFA_HPP
#pragma once

#include "fsm.hpp"
#include "fsm_builder.hpp"

#include <set>

namespace fsm {

inline constexpr char lambda = '\0';

class lnfa final : public automaton
{
private:
    builder m_builder{};
    std::vector<int> m_current_states{};
    bool m_aborted{ false };

    auto check_lambda(std::set<int>& to_check, int const state) -> void;

public:
    lnfa() = default;
    lnfa(lnfa const&) = default;
    lnfa(lnfa&&) noexcept = default;
    ~lnfa() noexcept override = default;

    explicit lnfa(builder const& build);
    explicit lnfa(builder&& build) noexcept;

    auto operator=(lnfa const&) -> lnfa& = default;
    auto operator=(lnfa&&) noexcept -> lnfa& = default;

    auto next(char const input) -> void override;
    [[nodiscard]] auto aborted() const noexcept -> bool override;
    [[nodiscard]] auto accepted() const noexcept -> bool override;
    auto reset() -> void override;
};

} // namespace fsm

#endif // !LAMBDA_NFA_HPP
