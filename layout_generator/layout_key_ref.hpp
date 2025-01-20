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

struct LayerId {
    uint8_t layer : 4;
    bool    isShift : 1;
    bool    isAlt : 1;
    bool    isHeld : 1;
    uint8_t unused : 1;

    static LayerId const defaultLayer;

    [[nodiscard]] LayerId withShift() const { return {layer, true, false, false}; }
    [[nodiscard]] LayerId withAlt() const { return {layer, false, true, false}; }
    [[nodiscard]] LayerId withHeld() const { return {layer, false, false, true}; }
    [[nodiscard]] bool    isVirtual() const { return isShift || isAlt || isHeld; }

    // operator uint8_t() const { return layer; }
    bool operator==(LayerId const& other) const = default;
};

inline constexpr LayerId LayerId::defaultLayer{0};

struct LayoutKeyRef {
    LayoutKeyRef() = default;
    LayoutKeyRef(LayerId layer, HandSide side, Row row, Column col)
        : layer(layer)
        , side(side)
        , row(row)
        , column(col) {}

    LayoutKeyRef(LayerId layer, KeyRef keyRef)
        : layer{layer}
        , side{keyRef.side}
        , row{static_cast<Row>(keyRef.row)}
        , column{static_cast<Column>(keyRef.col)} {}

    LayerId  layer;
    HandSide side;
    Row      row;
    Column   column;

    [[nodiscard]] operator KeyRef() const {
        return {side, static_cast<uint8_t>(row), static_cast<uint8_t>(column)};
    }

    [[nodiscard]] LayoutKeyRef withShift() const { return {layer.withShift(), side, row, column}; }
    [[nodiscard]] LayoutKeyRef withAlt() const { return {layer.withAlt(), side, row, column}; }
    [[nodiscard]] LayoutKeyRef withHeld() const { return {layer.withHeld(), side, row, column}; }

    bool operator==(LayoutKeyRef const& other) const = default;
};

static_assert(sizeof(LayoutKeyRef) == sizeof(int));
} // namespace finger_tracking
#endif // LAYOUT_KEY_REF_HPP
