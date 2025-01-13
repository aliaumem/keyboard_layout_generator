#ifndef CLOSEST_FINGER_MAPPING_HPP
#define CLOSEST_FINGER_MAPPING_HPP

#include "finger_keyboard_mapping/keyboard/keyboard_shape.hpp"
#include "finger_keyboard_mapping/hands/maybe_finger.hpp"

namespace finger_tracking {

[[nodiscard]] MaybeFinger closestFinger(KeyboardShape const& shape, Key key,
                                        BothHands const& hands);
}

#endif // CLOSEST_FINGER_MAPPING_HPP
