#ifndef FRAME_H
#define FRAME_H

#include "finger_keyboard_mapping/both_hands.hpp"

#include <chrono>

namespace finger_tracking {
struct Frame {
    std::chrono::milliseconds timestamp = {};
    BothHands                 hands     = {};
};
} // namespace finger_tracking

#endif // FRAME_H
