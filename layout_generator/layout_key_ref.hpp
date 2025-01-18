#ifndef LAYOUT_KEY_REF_HPP
#define LAYOUT_KEY_REF_HPP

#include "finger_keyboard_mapping/hands/hand_side.hpp"
#include "row_col.hpp"

#include <cstddef>

namespace finger_tracking {
struct KeyRef {
    HandSide     side;
    std::uint8_t row;
    std::uint8_t col;

    bool operator==(KeyRef const& other) const = default;
};

struct LayoutKeyRef {
    LayoutKeyRef() = default;
    LayoutKeyRef(std::uint8_t layer, HandSide side, Row row, Column col)
        : layer(layer)
        , side(side)
        , row(row)
        , column(col) {}

    std::uint8_t layer;
    HandSide     side;
    Row          row;
    Column       column;

    [[nodiscard]] operator KeyRef() const {
        return {side, static_cast<uint8_t>(row), static_cast<uint8_t>(column)};
    }

    bool operator==(LayoutKeyRef const& other) const = default;
};

static_assert(sizeof(LayoutKeyRef) <= sizeof(std::size_t));
} // namespace finger_tracking
#endif // LAYOUT_KEY_REF_HPP
