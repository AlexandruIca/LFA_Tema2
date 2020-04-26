#ifndef LAMBDA_NFA_HPP
#define LAMBDA_NFA_HPP
#pragma once

#include "fsm.hpp"
#include "fsm_builder.hpp"

#include <map>
#include <set>
#include <utility>

namespace fsm {

inline constexpr char lambda = '\0';

class lnfa final : public automaton
{
private:
    builder m_builder{};
    std::vector<int> m_current_states{};
    // Final states after lambda enclosing
    std::set<int> m_all_final_states{};
    bool m_aborted{ false };

    auto check_lambda(std::set<int>& to_check, int const state) -> void;
    [[nodiscard]] auto lambda_suffix(int const from) -> std::set<int>;
    [[nodiscard]] auto can_go_to(std::set<int> const& input,
                                 char const on) const -> std::set<int>;
    auto
    print_enclosing(std::map<char, std::vector<std::set<int>>> const& enclosing)
        -> void;
    [[nodiscard]] auto get_identical_states(
        std::map<char, std::vector<std::set<int>>> const& enclosing) const
        -> std::set<int>;
    auto rename_redundant_states(std::set<int>& input,
                                 std::set<int> const& redundant) const -> void;
    auto build_nfa(builder& build,
                   std::map<char, std::vector<std::set<int>>> const& enclosing,
                   std::set<int> const& redundant) -> void;

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
    [[nodiscard]] auto accepts_lambda() noexcept -> bool override;
    auto reset() -> void override;
    auto print_transitions() -> void override;

    [[nodiscard]] auto to_nfa() -> builder;
};

} // namespace fsm

#endif // !LAMBDA_NFA_HPP
