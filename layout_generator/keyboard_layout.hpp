#ifndef KEYBOARD_LAYOUT_HPP
#define KEYBOARD_LAYOUT_HPP

#include "finger_keyboard_mapping/hands/finger_desc.hpp"
#include "layout_generator/row_col.hpp"
#include "layout_generator/layout_key_ref.hpp"
#include "layout_generator/keyboard_shape.hpp"

#include "finger_keyboard_mapping/keyboard/key.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace finger_tracking {
struct LayerJumpKey {
    Key          key;
    std::uint8_t targetLayer;
};

template <size_t N>
struct KeyboardLayer {
    std::array<Key, N> keys;
    std::array<Key, N> keysHeld;
};

template <size_t N>
struct KeyboardLayout {
    using shape_type = KeyboardShape<N>;
    using layer_type = KeyboardLayer<N>;

    KeyboardLayout(shape_type const& shape, std::vector<layer_type> layers,
                   std::vector<LayerJumpKey> jumpKeys)
        : m_shape{shape}
        , m_layers{std::move(layers)}
        , m_layerJumpKeys{std::move(jumpKeys)} {}

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

    [[nodiscard]] Key heldKeyAt(LayoutKeyRef keyRef) const {
        auto layerIndex = m_shape.indexInLayer(keyRef);
        return m_layers[keyRef.layer].keysHeld[layerIndex];
    }

    [[nodiscard]] Finger fingerFor(LayoutKeyRef keyRef) const { return m_shape.fingerFor(keyRef); }

    [[nodiscard]] Key layerTransitionKey(uint8_t layer) const {
        auto it = std::find_if(m_layerJumpKeys.begin(), m_layerJumpKeys.end(), [&](auto& key) {
            return key.targetLayer == layer;
        });
        if (it == m_layerJumpKeys.end())
            throw std::invalid_argument("Unknown layer transition" + std::to_string(layer));
        return it->key;
    }

    struct iterator;

    auto     size() const { return m_layers.size() * N; }
    iterator layerBegin(size_t layer) const { return {layer * N, this}; }
    iterator layerEnd(size_t layer) const { return {layer * (N + 1), this}; }
    iterator begin() const { return {0, this}; }
    iterator end() const { return {m_layers.size() * N, this}; }

    LayoutKeyRef toKeyRef(iterator it) const {
        KeyRef keyRef = m_shape.atIndex(it.index % N);
        return LayoutKeyRef{
            static_cast<std::uint8_t>(it.index / N),
            keyRef.side,
            static_cast<Row>(keyRef.row),
            static_cast<Column>(keyRef.col),
        };
    }

private:
    shape_type                m_shape;
    std::vector<layer_type>   m_layers;
    std::vector<LayerJumpKey> m_layerJumpKeys;
};

using TargetKeyboardLayout = KeyboardLayout<52>;
} // namespace finger_tracking

#include "layout_generator/keyboard_layout_iterator.inl"

#endif // KEYBOARD_LAYOUT_HPP
