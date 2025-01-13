#ifndef KEYBOARD_SHAPE_HPP
#define KEYBOARD_SHAPE_HPP

#include "finger_keyboard_mapping/keyboard/key.hpp"
#include "finger_keyboard_mapping/mapping_geometry.hpp"

#include "finger_keyboard_mapping/static_vector.hpp"
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

    static KeyboardShape defaultShape();
};
} // namespace finger_tracking

#endif // KEYBOARD_SHAPE_HPP
