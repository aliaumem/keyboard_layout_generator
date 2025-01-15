#ifndef KEYBOARD_LAYOUT_HPP
#define KEYBOARD_LAYOUT_HPP

#include "layout_generator/keyboard_shape.hpp"

#include "finger_keyboard_mapping/hands/finger_desc.hpp"
#include "finger_keyboard_mapping/keyboard/key.hpp"

#include <vector>

namespace finger_tracking {

template <size_t N>
struct KeyboardLayer {
    std::array<Key, N> keys;
};

enum class Row {
    Fn = 0,
    Top,
    Home,
    Bottom,
    Thumb
};

enum class Column {
    IndexExt = 0,
    Index,
    Middle,
    Ring,
    Pinky,
    PinkyExt,
};

enum class Orientation {
    RowMajor,
    ColumnMajor,
};

template <size_t N>
struct KeyboardLayout {
    KeyboardLayout(KeyboardShape<N> const& shape, Orientation shapeOrientation,
                   std::vector<KeyboardLayer<N>> layers, Orientation layersOrientation)
        : m_shape{shape}
        , m_layers{std::move(layers)}
        , m_shapeLookup(
              shapeOrientation == Orientation::ColumnMajor ? columnMajorLookup : rowMajorLookup)
        , m_keyLookup(
              layersOrientation == Orientation::ColumnMajor ? columnMajorLookup : rowMajorLookup) {}

    [[nodiscard]] std::pair<Key, Rectangle> keyAt(int layer, Row row, HandSide side,
                                                  Column col) const {
        Key       key = m_layers[layer].keys[m_keyLookup(row, side, col)];
        Rectangle pos = m_shape.keys[m_shapeLookup(row, side, col)];
        return std::make_pair(key, pos);
    }

    struct iterator;

    iterator layerBegin(size_t layer) const { return {layer * N, this}; }
    iterator layerEnd(size_t layer) const { return {layer * (N + 1), this}; }
    iterator begin() const { return {0, this}; }
    iterator end() const { return {m_layers.size() * 62, this}; }

private:
    static auto columnMajorLookup(Row row, HandSide side, Column col) -> size_t {
        if (row == Row::Thumb)
            return 48 + toLinearThumb(side, col);
        return toLinear(side, col) * 4u + static_cast<size_t>(row);
    }

    static auto rowMajorLookup(Row row, HandSide side, Column col) -> size_t {
        if (row == Row::Thumb)
            return 48 + toLinearThumb(side, col);
        return static_cast<size_t>(row) * 12u + toLinear(side, col);
    }

    static size_t toLinear(HandSide side, Column col) {
        if (side == HandSide::Left)
            return 5u - static_cast<size_t>(col);
        return 6u + static_cast<size_t>(col);
    }

    static size_t toLinearThumb(HandSide side, Column col) {
        if (side == HandSide::Left)
            return 1u - static_cast<size_t>(col);
        return 2u + static_cast<size_t>(col);
    }

    using lookup_fn = size_t (*)(Row, HandSide, Column);
    KeyboardShape<N>              m_shape;
    std::vector<KeyboardLayer<N>> m_layers;
    lookup_fn                     m_keyLookup;
    lookup_fn                     m_shapeLookup;
};
} // namespace finger_tracking

#include "layout_generator/keyboard_layout_iterator.inl"

#endif // KEYBOARD_LAYOUT_HPP
