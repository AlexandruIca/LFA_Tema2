#include "fsm.hpp"

#include <utility>

namespace fsm {

auto accepts(automaton& autom, std::string const& input) -> bool
{
    if(input.empty()) {
        return autom.accepted();
    }

    for(char const tok : input) {
        autom.next(tok);

        if(autom.aborted()) {
            return false;
        }
    }

    return autom.accepted();
}

} // namespace fsm
