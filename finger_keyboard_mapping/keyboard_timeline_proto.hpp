#ifndef KEYBOARD_TIMELINE_PROTO_HPP
#define KEYBOARD_TIMELINE_PROTO_HPP

#include "finger_keyboard_mapping/keyboard_timeline.hpp"
#include "finger_keyboard_mapping/keyboard_timeline.pb.h"

namespace finger_tracking {
inline proto::KeyboardTimeline cast(KeyboardTimeline const& timeline) {
    proto::KeyboardTimeline result;
    for (auto const& state : timeline) {
        auto& frame = *result.add_frames();
        frame.set_timestamp_ms(state.timestamp.count());
        for (auto const& [key, finger] : state.pressedKeys) {
            auto& pressedKey = *frame.add_pressed_keys();
            pressedKey.set_key(key.name);
            pressedKey.set_finger(static_cast<int>(finger));
        }
    }

    return result;
}

inline KeyboardTimeline cast(proto::KeyboardTimeline const& timeline) {
    std::vector<KeyboardState> states;
    for (auto const& state : timeline.frames()) {
        states.emplace_back(std::chrono::milliseconds{state.timestamp_ms()},
                            std::vector<std::pair<Key, FingerDesc>>{});
        for (auto const& pressedKey : state.pressed_keys()) {
            states.back().pressedKeys.emplace_back(
                pressedKey.key(), FingerDesc{pressedKey.finger()});
        }
    }

    return KeyboardTimeline{std::move(states)};
}
} // namespace finger_tracking

#endif // KEYBOARD_TIMELINE_PROTO_HPP
