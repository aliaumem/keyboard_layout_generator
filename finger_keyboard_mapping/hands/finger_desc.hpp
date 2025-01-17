#ifndef FINGERS_HPP
#define FINGERS_HPP

#include "finger_keyboard_mapping/hands/hand_side.hpp"

namespace finger_tracking {

enum class Finger : std::uint8_t {
    Thumb,
    Index,
    Middle,
    Ring,
    Pinky,
};

inline int operator-(Finger lhs, Finger rhs) {
    return static_cast<int>(lhs) - static_cast<int>(rhs);
}

struct FingerDesc {

    using Side   = HandSide;
    using Finger = Finger;

    Side   side;
    Finger finger;

    constexpr FingerDesc(Side side, Finger finger)
        : side(side)
        , finger(finger) {}

    explicit FingerDesc(int value) {
        side   = value < 5 ? Side::Left : Side::Right;
        finger = [value] {
            switch (value % 5) {
            case 0: return Finger::Thumb;
            case 1: return Finger::Index;
            case 2: return Finger::Middle;
            case 3: return Finger::Ring;
            case 4: return Finger::Pinky;
            default: return Finger::Thumb;
            }
        }();
    }

    constexpr explicit operator int() const {
        return (side == Side::Right ? 5 : 0) + [this] {
            switch (finger) {
            case Finger::Thumb: return 0;
            case Finger::Index: return 1;
            case Finger::Middle: return 2;
            case Finger::Ring: return 3;
            case Finger::Pinky: return 4;
            default: return 0;
            }
        }();
    }

    constexpr bool operator==(FingerDesc const&) const = default;
};

static_assert(static_cast<int>(FingerDesc{FingerDesc::Side::Right, FingerDesc::Finger::Pinky})
              == 9);
} // namespace finger_tracking

#endif // FINGERS_HPP
