#ifndef FORMAT_HPP
#define FORMAT_HPP
#pragma once

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

template<typename... Args>
[[nodiscard]] auto format(Args&&... args) -> std::string
{
    static thread_local std::stringstream ss;
    std::string result{};

    ((result += (ss << std::forward<Args>(args), ss.str()), ss.str("")), ...);

    return result;
}

template<typename... Args>
auto print_to(std::ostream& os, Args&&... args) -> void
{
    os << format(std::forward<Args>(args)...);
}

template<typename... Args>
auto println_to(std::ostream& os, Args&&... args) -> void
{
    os << format(std::forward<Args>(args)...) << std::endl;
}

template<typename... Args>
auto print(Args&&... args) -> void
{
    print_to(std::cout, std::forward<Args>(args)...);
}

template<typename... Args>
auto println(Args&&... args) -> void
{
    println_to(std::cout, std::forward<Args>(args)...);
}

#endif // !FORMAT_HPP
