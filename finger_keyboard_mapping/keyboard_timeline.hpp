#ifndef KEYBOARD_TIMELINE_HPP
#define KEYBOARD_TIMELINE_HPP

#include "finger_keyboard_mapping/fingers.hpp"
#include "finger_keyboard_mapping/key.hpp"

#include <variant>
#include <vector>

namespace finger_tracking {
struct KeyboardState {
    std::vector<std::pair<Key, FingerDesc>> pressedKeys;
};

class KeyboardTimeline {
    std::vector<KeyboardState> m_keyboardStates;

    friend struct Builder;

    explicit KeyboardTimeline(std::vector<KeyboardState> states)
        : m_keyboardStates(std::move(states)) {}

public:
    KeyboardState const& operator[](size_t i) const { return m_keyboardStates.at(i); }

    [[nodiscard]] auto begin() const { return m_keyboardStates.begin(); }
    [[nodiscard]] auto end() const { return m_keyboardStates.end(); }

    struct Builder {
        struct PressedEvent {
            Key        key;
            FingerDesc finger;
        };
        struct ReleasedEvent {
            Key key;
        };
        using Event = std::variant<PressedEvent, ReleasedEvent>;

        std::vector<std::vector<Event>> m_frames = {std::vector<Event>{}};

        Builder& pressed(Key key, FingerDesc fingerDesc) {
            m_frames.back().emplace_back(PressedEvent{key, fingerDesc});
            return *this;
        }

        Builder& released(Key key) {
            m_frames.back().emplace_back(ReleasedEvent{key});
            return *this;
        }

        Builder& nextFrame() {
            m_frames.emplace_back();
            return *this;
        }

        KeyboardTimeline build() {
            std::vector<KeyboardState> result;

            KeyboardState current;

            for (auto const& frame : m_frames) {
                for (auto const& event : frame) {
                    if (std::holds_alternative<PressedEvent>(event)) {
                        auto pressedEvent = std::get<PressedEvent>(event);
                        auto it
                            = std::find_if(current.pressedKeys.begin(), current.pressedKeys.end(),
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

            return KeyboardTimeline{std::move(result)};
        }
    };
};
} // namespace finger_tracking

#endif // KEYBOARD_TIMELINE_HPP
