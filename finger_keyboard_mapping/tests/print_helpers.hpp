#ifndef PRINT_HELPERS_HPP
#define PRINT_HELPERS_HPP

#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"
#include "finger_keyboard_mapping/hands/maybe_finger.hpp"

#include <ostream>

inline std::ostream& operator<<(std::ostream& os, finger_tracking::FingerRef const& d) {
    return os << fmt::format("{}", d);
}

inline std::ostream& operator<<(std::ostream& os, std::nullopt_t) {
    return os << "[nullopt]";
}

template <typename T>
std::ostream& operator<<(std::ostream& os, std::optional<T> const& opt) {
    if (opt.has_value())
        os << opt.value();
    else
        os << std::nullopt;
    return os;
}

inline std::ostream& operator<<(std::ostream& os, finger_tracking::MaybeFinger const& f) {
    return os << f.maybeFinger;
}

#endif // PRINT_HELPERS_HPP
