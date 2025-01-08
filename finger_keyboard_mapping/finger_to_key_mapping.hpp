#ifndef FINGER_TO_KEY_MAPPING_HPP
#define FINGER_TO_KEY_MAPPING_HPP

#include "finger_keyboard_mapping/frame.hpp"

#include <span>
#include <vector>

namespace finger_tracking {
struct Key {
    std::string_view name;
};

struct KeyInSpace {
    Key       key;
    Rectangle aabb{};
};

class KeyboardShape {
    std::vector<KeyInSpace> m_keys;

public:
    explicit KeyboardShape(std::vector<KeyInSpace> keys)
        : m_keys(std::move(keys)) {}

    KeyboardShape(KeyboardShape&&)      = default;
    KeyboardShape(KeyboardShape const&) = delete;

    [[nodiscard]] std::span<KeyInSpace const> keys() const { return m_keys; }

    static KeyboardShape defaultShape();
};

void mapFingersToKeys(std::vector<Frame> frames, Corners const& corners);
} // namespace finger_tracking

#endif // FINGER_TO_KEY_MAPPING_HPP
