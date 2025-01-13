#include "keyboard_timeline.hpp"

namespace finger_tracking {

KeyboardTimeline KeyboardTimeline::Builder::build() {
    std::vector<KeyboardState> result;

    KeyboardState current;
    auto          timestampIt = m_timestamps.begin();

    for (auto const& frame : m_frames) {
        current.timestamp = *timestampIt++;
        for (auto const& event : frame) {
            if (std::holds_alternative<PressedEvent>(event)) {
                auto pressedEvent = std::get<PressedEvent>(event);
                auto it = std::find_if(current.pressedKeys.begin(), current.pressedKeys.end(),
                                       [&pressedEvent](auto const& pressedKey) {
                                           return pressedKey.first == pressedEvent.key;
                                       });
                if (it == current.pressedKeys.end())
                    current.pressedKeys.emplace_back(pressedEvent.key, pressedEvent.finger);
                else
                    it->second = pressedEvent.finger;
            } else {
                auto key = std::get<ReleasedEvent>(event).key;
                auto it  = std::find_if(
                    current.pressedKeys.begin(), current.pressedKeys.end(),
                    [&key](auto const& pressedKey) { return pressedKey.first == key; });
                if (it != current.pressedKeys.end())
                    current.pressedKeys.erase(it);
            }
        }

        result.emplace_back(current);
    }

    return KeyboardTimeline{std::move(result), eventCount(), m_distance};
}
} // namespace finger_tracking
