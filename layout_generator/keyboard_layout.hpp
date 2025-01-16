#ifndef KEYBOARD_LAYOUT_HPP
#define KEYBOARD_LAYOUT_HPP

#include "finger_keyboard_mapping/hands/finger_desc.hpp"
#include "layout_generator/layout_key_ref.hpp"
#include "layout_generator/keyboard_shape.hpp"

#include "finger_keyboard_mapping/keyboard/key.hpp"

#include <vector>

namespace finger_tracking {

template <size_t N>
struct KeyboardLayer {
    std::array<Key, N> keys;
};

template <size_t N>
struct KeyboardLayout {
    KeyboardLayout(KeyboardShape<N> const& shape, std::vector<KeyboardLayer<N>> layers)
        : m_shape{shape}
        , m_layers{std::move(layers)} {}

    [[nodiscard]] std::pair<Key, Rectangle> keyAt(std::uint8_t layer,
                                                  std::size_t  indexInLayer) const {
        Key       key = m_layers[layer].keys[indexInLayer];
        Rectangle pos = m_shape.keys[indexInLayer];
        return std::make_pair(key, pos);
    }

    [[nodiscard]] std::pair<Key, Rectangle> keyAt(LayoutKeyRef keyRef) const {
        auto      layerIndex = m_shape.indexInLayer(keyRef);
        Key       key        = m_layers[keyRef.layer].keys[layerIndex];
        Rectangle pos        = m_shape.keys[layerIndex];
        return std::make_pair(key, pos);
    }

    [[nodiscard]] Finger fingerFor(LayoutKeyRef keyRef) const { return m_shape.fingerFor(keyRef); }

    struct iterator;

    iterator layerBegin(size_t layer) const { return {layer * N, this}; }
    iterator layerEnd(size_t layer) const { return {layer * (N + 1), this}; }
    iterator begin() const { return {0, this}; }
    iterator end() const { return {m_layers.size() * N, this}; }

    LayoutKeyRef toKeyRef(iterator it) const {
        KeyRef keyRef = m_shape.atIndex(it.index % N);
        return LayoutKeyRef{
            .layer  = static_cast<std::uint8_t>(it.index / N),
            .side   = keyRef.side,
            .row    = keyRef.row,
            .column = keyRef.col,
        };
    }

private:
    KeyboardShape<N>              m_shape;
    std::vector<KeyboardLayer<N>> m_layers;
};

using TargetKeyboardLayout = KeyboardLayout<52>;
} // namespace finger_tracking

#include "layout_generator/keyboard_layout_iterator.inl"

#endif // KEYBOARD_LAYOUT_HPP
