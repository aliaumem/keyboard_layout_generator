#ifndef FINGER_TO_KEY_MAPPING_HPP
#define FINGER_TO_KEY_MAPPING_HPP

#include "finger_keyboard_mapping/frame.hpp"
#include "finger_keyboard_mapping/key.hpp"
#include "finger_keyboard_mapping/keyboard_timeline.hpp"
#include "finger_keyboard_mapping/scancode_key_map.hpp"

#include <span>

namespace finger_tracking {

struct KeyInSpace {
    Key       key;
    Rectangle aabb{};

    bool operator==(Key const& other) const { return key == other; }
};

class KeyboardShape {
    static_vector<KeyInSpace, 62> m_keys;

public:
    explicit KeyboardShape(static_vector<KeyInSpace, 62> const& keys)
        : m_keys(keys) {}

    KeyboardShape(KeyboardShape&&)      = default;
    KeyboardShape(KeyboardShape const&) = delete;

    [[nodiscard]] Rectangle aabb() const;

    [[nodiscard]] std::span<KeyInSpace const> keys() const {
        return std::span{m_keys.begin(), m_keys.size()};
    }
    [[nodiscard]] std::optional<FingerRef> closestFinger(Key key, BothHands const& hands) const;

    static KeyboardShape defaultShape();
};

KeyboardTimeline mapFingersToKeys(std::vector<Frame> const&    frames,
                                  std::vector<KeyEvent> const& keyEvents,
                                  KeyboardShape const& shape, ScancodeKeyMap const& scancodeKeyMap);
} // namespace finger_tracking

#endif // FINGER_TO_KEY_MAPPING_HPP
