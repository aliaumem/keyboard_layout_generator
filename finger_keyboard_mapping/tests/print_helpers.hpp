#ifndef PRINT_HELPERS_HPP
#define PRINT_HELPERS_HPP

#include "finger_keyboard_mapping/finger_print_helpers.hpp"

#include <ostream>

std::ostream& operator<<(std::ostream& os, finger_tracking::FingerRef const& d) {
    return os << std::format("{}", d);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, std::optional<T> const& opt) {
    if (opt.has_value())
        os << opt.value();
    else
        os << "[nullopt]";
    return os;
}

#endif // PRINT_HELPERS_HPP
