#ifndef KEY_PRESS_HPP
#define KEY_PRESS_HPP

#include "layout_generator/row_col.hpp"
#include "layout_generator/layout_key_ref.hpp"

#include "finger_keyboard_mapping/mapping_geometry.hpp"
#include "finger_keyboard_mapping/hands/finger_desc.hpp"

#include "absl/hash/hash.h"

namespace finger_tracking {
struct KeyPress {
    LayoutKeyRef keyRef;
    Finger       finger;
    bool         isPress = true;
    Point16      position;

    [[nodiscard]] HandSide   side() const { return keyRef.side; }
    [[nodiscard]] Row        row() const { return static_cast<Row>(keyRef.row); }
    [[nodiscard]] Column     col() const { return static_cast<Column>(keyRef.column); }
    [[nodiscard]] FingerDesc fingerDesc() const { return {side(), finger}; }

    bool operator==(KeyPress const& other) const {
        // Position and finger are redundant since they are fully determined by the LayoutKeyRef
        return keyRef == other.keyRef && isPress == other.isPress;
    };

    template <typename H>
    friend H AbslHashValue(H h, KeyPress const& kp) {
        // Position and finger are redundant since they are fully determined by the LayoutKeyRef
        return H::combine(std::move(h), kp.keyRef.layer, kp.keyRef.side, kp.keyRef.row,
                          kp.keyRef.column, kp.isPress);
    }
};
} // namespace finger_tracking
#endif // KEY_PRESS_HPP
