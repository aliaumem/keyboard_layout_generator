#ifndef KEYBOARD_TIMELINE_HPP
#define KEYBOARD_TIMELINE_HPP

#include "finger_keyboard_mapping/hands/finger_desc.hpp"
#include "finger_keyboard_mapping/keyboard/key.hpp"

#include <chrono>
#include <numeric>
#include <variant>
#include <vector>

namespace finger_tracking {
struct KeyboardState {
    std::chrono::milliseconds               timestamp;
    std::vector<std::pair<Key, FingerDesc>> pressedKeys;
};

class KeyboardTimeline {
    std::vector<KeyboardState> m_keyboardStates;
    size_t                     m_eventCount    = 0;
    long                       m_totalDistance = 0;

    friend struct Builder;

    explicit KeyboardTimeline(std::vector<KeyboardState> states, size_t eventCount,
                              long totalDistance)
        : m_keyboardStates(std::move(states))
        , m_eventCount(eventCount)
        , m_totalDistance(totalDistance) {}

public:
    KeyboardTimeline(std::vector<KeyboardState> states)
        : m_keyboardStates(std::move(states)) {}

    KeyboardState const& operator[](size_t i) const { return m_keyboardStates.at(i); }

    [[nodiscard]] auto begin() const { return m_keyboardStates.begin(); }
    [[nodiscard]] auto end() const { return m_keyboardStates.end(); }

    [[nodiscard]] size_t size() const { return m_keyboardStates.size(); }
    [[nodiscard]] size_t totalDistance() const { return m_totalDistance; }
    [[nodiscard]] size_t eventCount() const { return m_eventCount; }
    [[nodiscard]] size_t totalPressedKeyFrameCount() const {
        return std::accumulate(
            m_keyboardStates.begin(), m_keyboardStates.end(), 0ull,
            [](size_t acc, KeyboardState const& state) { return acc + state.pressedKeys.size(); });
    }

    struct Builder {
        struct PressedEvent {
            Key        key;
            FingerDesc finger;
        };
        struct ReleasedEvent {
            Key key;
        };
        using Event = std::variant<PressedEvent, ReleasedEvent>;

        std::vector<std::vector<Event>>        m_frames     = {};
        std::vector<std::chrono::milliseconds> m_timestamps = {};
        long                                   m_distance   = 0;

        [[nodiscard]] size_t eventCount() const {
            return std::accumulate(
                m_frames.begin(), m_frames.end(), 0ull,
                [](size_t acc, std::vector<Event> const& state) { return acc + state.size(); });
        }

        Builder& pressed(Key key, FingerDesc fingerDesc) {
            m_frames.back().emplace_back(PressedEvent{key, fingerDesc});
            return *this;
        }

        Builder& released(Key key) {
            m_frames.back().emplace_back(ReleasedEvent{key});
            return *this;
        }

        Builder& nextFrame(std::chrono::milliseconds timestamp) {
            m_frames.emplace_back();
            m_timestamps.emplace_back(timestamp);
            return *this;
        }

        KeyboardTimeline build();

        void addDistance(int distance) { m_distance += distance; }
    };
};
} // namespace finger_tracking

#endif // KEYBOARD_TIMELINE_HPP
