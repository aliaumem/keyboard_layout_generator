#ifndef FINGER_TO_KEY_MAPPING_HPP
#define FINGER_TO_KEY_MAPPING_HPP

#include "finger_keyboard_mapping/frame.hpp"
#include "static_vector.hpp"
#include "key.hpp"

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
    explicit KeyboardShape(static_vector<KeyInSpace, 62> keys)
        : m_keys(std::move(keys)) {}

    KeyboardShape(KeyboardShape&&)      = default;
    KeyboardShape(KeyboardShape const&) = delete;

    Rectangle aabb() const;

    [[nodiscard]] std::span<KeyInSpace const> keys() const {
        return std::span<KeyInSpace const>{m_keys.begin(), m_keys.size()};
    }
    std::optional<FingerRef> closestFinger(Key key, BothHands const& hands);

    static KeyboardShape defaultShape();
};

void mapFingersToKeys(std::vector<Frame> frames);
} // namespace finger_tracking

#endif // FINGER_TO_KEY_MAPPING_HPP
