#ifndef FRAME_H
#define FRAME_H

#include "finger_keyboard_mapping/mapping_geometry.hpp"

#include <chrono>
#include <optional>

namespace finger_tracking {
struct Frame {
    std::chrono::milliseconds timestamp = {};
    std::optional<Hand>       leftHand;
    std::optional<Hand>       rightHand;
};
} // namespace finger_tracking

#endif // FRAME_H
