#ifndef FINGERS_HPP
#define FINGERS_HPP

namespace finger_tracking {
struct FingerDesc {
    enum class Side {
        Left,
        Right,
    };
    enum class Finger {
        Thumb,
        Index,
        Middle,
        Ring,
        Pinky,
    };

    Side   side;
    Finger finger;

    FingerDesc(Side side, Finger finger)
        : side(side)
        , finger(finger) {}

    explicit FingerDesc(int value) {
        side   = value / 5 == 0 ? Side::Left : Side::Right;
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

    explicit operator int() const {
        return side == Side::Right ? 5 : 0 + [this] {
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

    bool operator==(FingerDesc const&) const = default;
};
} // namespace finger_tracking

#endif // FINGERS_HPP
