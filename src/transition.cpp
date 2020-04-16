#include "transition.hpp"

namespace fsm::impl {

transition::transition(char const on_, int const to_)
    : to{ to_ }
    , on{ on_ }
{
    static_cast<void>(to);
    static_cast<void>(on);
}

} // namespace fsm::impl
