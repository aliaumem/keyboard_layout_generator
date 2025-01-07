#ifndef FINGER_TO_KEY_MAPPING_HPP
#define FINGER_TO_KEY_MAPPING_HPP

#include "finger_keyboard_mapping/frame.hpp"

#include <vector>

namespace finger_tracking {
void mapFingersToKeys(std::vector<Frame> frames, Corners corners);
}

#endif // FINGER_TO_KEY_MAPPING_HPP
