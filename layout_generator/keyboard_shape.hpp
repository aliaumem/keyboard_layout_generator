#ifndef KEYBOARD_SHAPE_HPP
#define KEYBOARD_SHAPE_HPP

#include "finger_keyboard_mapping/mapping_geometry.hpp"

#include <array>

namespace finger_tracking {
template <size_t N>
struct KeyboardShape {
    std::array<Rectangle, N> keys;
};
} // namespace finger_tracking
#endif // KEYBOARD_SHAPE_HPP
