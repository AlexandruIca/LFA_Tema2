#include "printer.hpp"
#include "lnfa.hpp"

#include <iterator>

namespace fsm {

auto print(std::vector<fsm::impl::transition> const& vec, std::ostream& os)
    -> void
{
    using transition_t = fsm::impl::transition;

    auto print_transition = [&os](transition_t const& t) -> void {
        os << '(';
        if(t.on == fsm::lambda) {
            os << '$';
        }
        else {
            os << t.on;
        }
        os << " -> ";
        os << t.to << ')';
    };

    os << '[';

    if(!vec.empty()) {
        print_transition(vec.front());

        auto it = vec.begin();
        ++it;

        for(; it != vec.end(); ++it) {
            os << ", ";
            print_transition(*it);
        }
    }

    os << ']';
}

auto print(std::set<int> const& set, std::ostream& os) -> void
{
    os << '{';

    if(!set.empty()) {
        os << (*set.begin());

        auto it = set.begin();
        ++it;

        for(; it != set.end(); ++it) {
            os << ", ";
            os << (*it);
        }
    }

    os << '}';
}

} // namespace fsm
