#ifndef PRINTER_HPP
#define PRINTER_HPP
#pragma once

#include "transition.hpp"

#include <iostream>
#include <vector>

namespace fsm {

auto print(std::vector<fsm::impl::transition> const& vec,
           std::ostream& os = std::cout) -> void;

} // namespace fsm

#endif // !PRINTER_HPP
