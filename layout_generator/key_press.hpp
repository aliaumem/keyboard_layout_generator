#ifndef KEY_PRESS_HPP
#define KEY_PRESS_HPP

#include "layout_generator/layout_key_ref.hpp"

#include "finger_keyboard_mapping/mapping_geometry.hpp"
#include "finger_keyboard_mapping/hands/finger_desc.hpp"

namespace finger_tracking {
struct KeyPress {
    LayoutKeyRef keyRef;
    Finger       finger;
    bool         isPress = true;
    Point16      position;

    [[nodiscard]] HandSide   side() const { return keyRef.side; }
    [[nodiscard]] FingerDesc fingerDesc() const { return {side(), finger}; }

    bool operator==(KeyPress const& other) const = default;
};
} // namespace finger_tracking
#endif // KEY_PRESS_HPP
