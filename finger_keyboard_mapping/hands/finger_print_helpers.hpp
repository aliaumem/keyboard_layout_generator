#ifndef FINGER_PRINT_HELPERS_HPP
#define FINGER_PRINT_HELPERS_HPP

#include "finger_keyboard_mapping/hands/both_hands.hpp"
#include <fmt/format.h>

template <typename T>
struct fmt::formatter<finger_tracking::Point_<T>> : fmt::formatter<char> {
    template <class FmtContext>
    auto format(finger_tracking::Point_<T> p, FmtContext& ctx) const {
        return fmt::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

template <>
struct fmt::formatter<finger_tracking::Finger> : fmt::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::Finger finger, FmtContext& ctx) const {
        return fmt::format_to(ctx.out(), "{}", [finger]() -> std::string_view {
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
struct fmt::formatter<finger_tracking::HandSide> : fmt::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::HandSide side, FmtContext& ctx) const {
        return fmt::format_to(ctx.out(), "{}",
                              side == finger_tracking::HandSide::Left ? "Left" : "Right");
    }
};

template <>
struct fmt::formatter<finger_tracking::FingerDesc> : fmt::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::FingerDesc d, FmtContext& ctx) const {
        return fmt::format_to(ctx.out(), "{}, {}", d.side, d.finger);
    }
};

template <>
struct fmt::formatter<finger_tracking::FingerRef> : fmt::formatter<char> {
    template <typename FmtContext>
    auto format(finger_tracking::FingerRef d, FmtContext& ctx) const {
        return fmt::format_to(ctx.out(), "{{{}, {}}}", d.fingerDesc, d.position);
    }
};

#endif // FINGER_PRINT_HELPERS_HPP
