#include "finger_to_key_mapping.hpp"

#include "finger_keyboard_mapping/mapping/closest_finger_mapping.hpp"

#include "finger_keyboard_mapping/keyboard_timeline.hpp"
#include "finger_keyboard_mapping/keyboard/scancode_key_map.hpp"

namespace finger_tracking {
auto FingerToKeyMapper::mapFingersToKeys(
    std::vector<Frame> const& frames, std::vector<KeyEvent> const& keyEvents,
    std::chrono::milliseconds timeOffset) const -> KeyboardTimeline {
    using std::chrono::milliseconds;

    KeyboardTimeline::Builder builder;

    auto prevEvent = keyEvents.begin();
    for (auto const& frame : frames) {
        while (prevEvent != keyEvents.end()
               && prevEvent->timestamp < frame.timestamp + timeOffset) {
            auto key         = scancodeKeyMap.scanCodeToKey(prevEvent->code);
            auto maybeFinger = closestFinger(shape, key, frame.hands);
            if (prevEvent->isPressed) {
                if (maybeFinger.has_value()) {
                    builder.addDistance(maybeFinger.distance);
                    builder.pressed(key, maybeFinger->fingerDesc);
                }
            } else
                builder.released(key);

            ++prevEvent;
        }

        builder.nextFrame(frame.timestamp);
    }

    return builder.build();
}
} // namespace finger_tracking
