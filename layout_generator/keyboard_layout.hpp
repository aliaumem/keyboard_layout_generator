#ifndef KEYBOARD_LAYOUT_HPP
#define KEYBOARD_LAYOUT_HPP

#include "finger_keyboard_mapping/static_vector.hpp"
#include "finger_keyboard_mapping/hands/finger_desc.hpp"
#include "layout_generator/row_col.hpp"
#include "layout_generator/layout_key_ref.hpp"
#include "layout_generator/keyboard_shape.hpp"
#include "layout_generator/to_shifted_keys.hpp"

#include "finger_keyboard_mapping/keyboard/key.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace finger_tracking {
struct LayerJumpKey {
    Key     key;
    LayerId targetLayer;
};

template <size_t N>
struct KeyboardLayer {
    std::array<Key, N>        keys;
    std::array<bool, N>       lockedKeysMask;
    std::array<Key, N>        keysHeld;
    static_vector<LayerId, 2> relatedLayerIndices;
};

template <size_t N>
class LayoutMutator;

template <size_t N>
struct KeyboardLayout {
    using shape_type = KeyboardShape<N>;
    using layer_type = KeyboardLayer<N>;

    friend class LayoutMutator<N>;

    KeyboardLayout(shape_type const& shape, std::vector<layer_type> layers,
                   std::vector<LayerJumpKey> jumpKeys)
        : m_shape{shape}
        , m_layers{std::move(layers)}
        , m_layerJumpKeys{std::move(jumpKeys)} {}

    [[nodiscard]] Key keyAt(LayoutKeyRef keyRef) const {
        auto layerIndex = m_shape.indexInLayer(keyRef);
        if (keyRef.layer.isHeld)
            return m_layers[keyRef.layer.layer].keysHeld[layerIndex];
        else if (keyRef.layer.isShift)
            return toShiftedKey(m_layers[keyRef.layer.layer].keys[layerIndex]);
        else if (keyRef.layer.isAlt)
            return toAltKey(m_layers[keyRef.layer.layer].keys[layerIndex]);
        return m_layers[keyRef.layer.layer].keys[layerIndex];
    }

    [[nodiscard]] bool isLockedAt(LayoutKeyRef keyRef) const {
        auto layerIndex = m_shape.indexInLayer(keyRef);
        bool isLocked   = m_layers[keyRef.layer.layer].lockedKeysMask[layerIndex];
        return isLocked;
    }

    [[nodiscard]] Finger fingerFor(LayoutKeyRef keyRef) const { return m_shape.fingerFor(keyRef); }

    [[nodiscard]] Key layerTransitionKey(LayerId layer) const {
        if (layer.isShift)
            return Key{"LSft"};
        if (layer.isAlt)
            return Key{"RAlt"};

        auto it = std::find_if(m_layerJumpKeys.begin(), m_layerJumpKeys.end(), [&](auto& key) {
            return key.targetLayer == layer;
        });
        if (it == m_layerJumpKeys.end())
            throw std::invalid_argument("Unknown layer transition" + std::to_string(layer.layer));
        return it->key;
    }

    struct iterator;

    auto     size() const { return m_layers.size() * N; }
    iterator layerBegin(size_t layer) const { return {layer * N, this}; }
    iterator layerEnd(size_t layer) const { return {(layer + 1) * N, this}; }
    iterator begin() const { return {0, this}; }
    iterator end() const { return {m_layers.size() * N, this}; }

    LayoutKeyRef toKeyRef(iterator it) const {
        KeyRef keyRef = m_shape.atIndex(it.index % N);
        return LayoutKeyRef{LayerId{static_cast<std::uint8_t>(it.index / N)}, keyRef};
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
