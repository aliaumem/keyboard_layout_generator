#ifndef KEY_PRESS_HPP
#define KEY_PRESS_HPP

#include "layout_generator/row_col.hpp"
#include "layout_generator/layout_key_ref.hpp"

#include "finger_keyboard_mapping/mapping_geometry.hpp"
#include "finger_keyboard_mapping/hands/finger_desc.hpp"

namespace finger_tracking {
struct KeyPress {
    KeyPress() = default;
    KeyPress(LayoutKeyRef keyRef, Finger finger, bool isPress = true)
        : keyRef{keyRef}
        , isPress{isPress} //
        , finger{finger}   //
    {}

    LayoutKeyRef keyRef;
    bool         isPress : 1 = true;
    Finger       finger : 3;
    std::uint8_t unused : 4;

    [[nodiscard]] HandSide   side() const { return keyRef.side; }
    [[nodiscard]] Row        row() const { return keyRef.row; }
    [[nodiscard]] Column     col() const { return keyRef.column; }
    [[nodiscard]] FingerDesc fingerDesc() const { return {side(), finger}; }

    bool operator==(KeyPress const& other) const {
        // Finger is redundant since is fully determined by the LayoutKeyRef
        return keyRef == other.keyRef && isPress == other.isPress;
    }
};

static_assert(sizeof(KeyPress) == 3);
} // namespace finger_tracking
#endif // KEY_PRESS_HPP
