#ifndef VOYAGER_SHAPE_HPP
#define VOYAGER_SHAPE_HPP

#include "finger_keyboard_mapping/keyboard/keyboard_shape.hpp"

namespace finger_tracking {
KeyboardShape_<52> voyagerShape(static_vector<Key, 52> const& keys);
}

#endif // VOYAGER_SHAPE_HPP
