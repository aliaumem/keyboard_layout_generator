#ifndef LAYOUT_MUTATOR_HPP
#define LAYOUT_MUTATOR_HPP

#include <layout_generator/keyboard_layout.hpp>

namespace finger_tracking {
template <size_t N>
class LayoutMutator {
public:
    KeyboardLayout<N>& m_layout;
    void               swapKeys(LayoutKeyRef lhs, LayoutKeyRef rhs) {
        auto const& shape = m_layout.m_shape;
        std::swap(m_layout.m_layers[lhs.layer].keys[shape.indexInLayer(lhs)],
                                m_layout.m_layers[rhs.layer].keys[shape.indexInLayer(rhs)]);
    }
};
} // namespace finger_tracking

#endif // LAYOUT_MUTATOR_HPP
