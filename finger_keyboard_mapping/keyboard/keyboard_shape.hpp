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

template <size_t N = 62>
class KeyboardShape_ {
    static_vector<KeyInSpace, N> m_keys;

public:
    explicit KeyboardShape_(static_vector<KeyInSpace, N> const& keys)
        : m_keys(keys) {}

    KeyboardShape_(KeyboardShape_&&)      = default;
    KeyboardShape_(KeyboardShape_ const&) = delete;

    [[nodiscard]] Rectangle aabb() const;

    [[nodiscard]] std::span<KeyInSpace const> keys() const {
        return std::span{m_keys.begin(), m_keys.size()};
    }

    [[nodiscard]] Rectangle keyAABB(Key const& key) const {
        auto it = std::find_if(m_keys.begin(), m_keys.end(), [key](KeyInSpace const& key_in_space) {
            return key == key_in_space.key;
        });
        return it == m_keys.end() ? Rectangle{} : it->aabb;
    }

    static KeyboardShape_<> defaultShape();
};

using KeyboardShape = KeyboardShape_<>;
} // namespace finger_tracking

#endif // KEYBOARD_SHAPE_HPP
