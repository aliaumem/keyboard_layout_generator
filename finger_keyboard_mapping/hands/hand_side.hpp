#ifndef HAND_SIDE_HPP
#define HAND_SIDE_HPP

#include <cstdint>

namespace finger_tracking {
enum class HandSide : std::uint8_t {
    Left,
    Right,
};
} // namespace finger_tracking

#endif // HAND_SIDE_HPP
