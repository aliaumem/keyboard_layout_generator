#ifndef KEY_PRINTER_HELPER_HPP
#define KEY_PRINTER_HELPER_HPP

#include "finger_keyboard_mapping/keyboard/key.hpp"

#include <fmt/format.h>

template <>
struct fmt::formatter<finger_tracking::Key> : fmt::formatter<std::string_view> {
    template <typename FmtContext>
    auto format(finger_tracking::Key key, FmtContext& ctx) const {
        return fmt::format_to(ctx.out(), "{}", key.name);
    }
};

#endif // KEY_PRINTER_HELPER_HPP
