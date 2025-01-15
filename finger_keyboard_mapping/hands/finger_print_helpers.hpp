#ifndef FINGER_PRINT_HELPERS_HPP
#define FINGER_PRINT_HELPERS_HPP

#include "finger_keyboard_mapping/hands/both_hands.hpp"
#include <format>

template <>
struct std::formatter<finger_tracking::Point> : std::formatter<char> {
    template <class FmtContext>
    auto format(finger_tracking::Point p, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

template <>
struct std::formatter<finger_tracking::Finger> : std::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::Finger finger, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}", [finger]() -> std::string_view {
            using enum finger_tracking::Finger;
            switch (finger) {
            case Thumb: return "Thumb";
            case Index: return "Index";
            case Middle: return "Middle";
            case Ring: return "Ring";
            case Pinky: return "Pinky";
            default: return "???";
            }
        }());
    }
};

template <>
struct std::formatter<finger_tracking::HandSide> : std::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::FingerDesc::Side side, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}",
                              side == finger_tracking::HandSide::Left ? "Left" : "Right");
    }
};

template <>
struct std::formatter<finger_tracking::FingerDesc> : std::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::FingerDesc d, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{}, {}", d.side, d.finger);
    }
};

template <>
struct std::formatter<finger_tracking::FingerRef> : std::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::FingerRef d, FmtContext& ctx) const {
        return std::format_to(ctx.out(), "{{{}, {}}}", d.fingerDesc, d.position);
    }
};

#endif // FINGER_PRINT_HELPERS_HPP
