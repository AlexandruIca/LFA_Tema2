#ifndef TRANSITION_HPP
#define TRANSITION_HPP
#pragma once

namespace fsm::impl {

class transition
{
private:
    int to{ 0 };
    char on{ '\0' };

public:
    transition() noexcept = default;
    transition(transition const&) noexcept = default;
    transition(transition&&) noexcept = default;
    ~transition() noexcept = default;

    explicit transition(char const on, int const to);

    auto operator=(transition const&) noexcept -> transition& = default;
    auto operator=(transition&&) noexcept -> transition& = default;
};

} // namespace fsm::impl

#endif // !TRANSITION_HPP
