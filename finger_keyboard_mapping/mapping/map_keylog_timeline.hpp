#ifndef MAP_KEYLOG_TIMELINE_HPP
#define MAP_KEYLOG_TIMELINE_HPP

#include "finger_keyboard_mapping/keyboard_timeline.hpp"
#include "finger_keyboard_mapping/keyboard/scancode_key_map.hpp"
#include "finger_keyboard_mapping/keyboard/keyboard_shape.hpp"
#include "finger_keyboard_mapping/hands/frame.hpp"

namespace finger_tracking {
[[nodiscard]] KeyboardTimeline mapForStats(
    ScancodeKeyMap const& scancodeKeyMap, std::vector<Frame> const& frames,
    KeyboardShape const& shape, std::vector<KeyEvent> const& keyEvents,
    std::chrono::milliseconds timeOffset);
} // namespace finger_tracking

#endif // MAP_KEYLOG_TIMELINE_HPP
