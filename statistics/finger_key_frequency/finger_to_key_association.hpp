#ifndef FINGER_TO_KEY_ASSOCIATION_HPP
#define FINGER_TO_KEY_ASSOCIATION_HPP

#include "statistics/finger_key_frequency/finger_to_key_map.hpp"

#include "finger_keyboard_mapping/keyboard_timeline.hpp"
#include "finger_keyboard_mapping/keyboard/scancode_key_map.hpp"

namespace finger_tracking {
struct FingerToKeyAssociation {
    ScancodeKeyMap keyMap{};

    [[nodiscard]] auto computeKeyToFingerAssociation(KeyboardTimeline const& timeline) const
        -> FingerToKeyMap {
        FingerToKeyMap fingerToKeyMap{};

        for (auto const& state : timeline) {
            if (state.pressedKeys.empty())
                throw std::runtime_error(
                    std::format("No keys pressed at timestamp {}", state.timestamp));
            auto const& [key, finger] = state.pressedKeys.front();
            size_t keyIndex           = keyMap.keyIndex(key);
            fingerToKeyMap[{keyIndex, finger}]++;
        }

        return fingerToKeyMap;
    }

    void print(FingerToKeyMap const& association) const;
};
} // namespace finger_tracking

#endif // FINGER_TO_KEY_ASSOCIATION_HPP
