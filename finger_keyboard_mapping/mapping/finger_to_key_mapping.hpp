#ifndef FINGER_TO_KEY_MAPPING_HPP
#define FINGER_TO_KEY_MAPPING_HPP

#include "finger_keyboard_mapping/hands/frame.hpp"
#include "finger_keyboard_mapping/keyboard/keyboard_shape.hpp"
#include "finger_keyboard_mapping/keyboard_timeline.hpp"
#include "finger_keyboard_mapping/keyboard/scancode_key_map.hpp"

namespace finger_tracking {
struct FingerToKeyMapper {
    KeyboardShape const&  shape;
    ScancodeKeyMap const& scancodeKeyMap;

    [[nodiscard]] KeyboardTimeline mapFingersToKeys(std::vector<Frame> const&    frames,
                                                    std::vector<KeyEvent> const& keyEvents,
                                                    std::chrono::milliseconds    timeOffset) const;
};
} // namespace finger_tracking

#endif // FINGER_TO_KEY_MAPPING_HPP
