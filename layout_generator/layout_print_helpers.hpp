#ifndef LAYOUT_PRINT_HELPERS_HPP
#define LAYOUT_PRINT_HELPERS_HPP

#include "layout_generator/key_press.hpp"
#include "finger_keyboard_mapping/hands/finger_print_helpers.hpp"

#include <fmt/format.h>

template <>
struct fmt::formatter<finger_tracking::Row> : fmt::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::Row row, FmtContext& ctx) const {
        return fmt::format_to(ctx.out(), "{}", [row] {
            switch (row) {
            case finger_tracking::Row::Fn: return "Fn";
            case finger_tracking::Row::Top: return "Top";
            case finger_tracking::Row::Home: return "Home";
            case finger_tracking::Row::Bottom: return "Bottom";
            case finger_tracking::Row::Thumb: return "Thumb";
            default: return "invalid row";
            }
        }());
    }
};

template <>
struct fmt::formatter<finger_tracking::Column> : fmt::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::Column col, FmtContext& ctx) const {
        return fmt::format_to(ctx.out(), "{}", [col] {
            switch (col) {
            case finger_tracking::Column::IndexExt: return "IndexExt";
            case finger_tracking::Column::Index: return "Index";
            case finger_tracking::Column::Middle: return "Middle";
            case finger_tracking::Column::Ring: return "Ring";
            case finger_tracking::Column::Pinky: return "Pinky";
            case finger_tracking::Column::PinkyExt: return "PinkyExt";
            default: return "invalid column";
            }
        }());
    }
};
template <>
struct fmt::formatter<finger_tracking::LayerId> : fmt::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::LayerId layer, FmtContext& ctx) const {
        uint8_t id = layer.layer;
        auto    it = fmt::format_to(ctx.out(), "{}", id);
        if (layer.isVirtual())
            it = fmt::format_to(ctx.out(), "({}{}{})", layer.isShift ? "shift" : "",
                                layer.isAlt ? "alt" : "", layer.isHeld ? "held" : "");
        return it;
    }
};

template <>
struct fmt::formatter<finger_tracking::LayoutKeyRef> : fmt::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::LayoutKeyRef value, FmtContext& ctx) const {

        auto handSide = value.side;
        auto row      = value.row;
        auto column   = value.column;

        return fmt::format_to(ctx.out(), "{}. {} {}:{}", value.layer, handSide, row, column);
    }
};

template <>
struct fmt::formatter<finger_tracking::KeyPress> : fmt::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::KeyPress const& value, FmtContext& ctx) const {
        return fmt::format_to(ctx.out(), "{} ({}) {}", value.keyRef, value.finger, value.isPress);
    }
};

#endif // LAYOUT_PRINT_HELPERS_HPP
