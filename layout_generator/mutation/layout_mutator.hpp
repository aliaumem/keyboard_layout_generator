#ifndef LAYOUT_MUTATOR_HPP
#define LAYOUT_MUTATOR_HPP

#include <layout_generator/keyboard_layout.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/algorithm/count.hpp>

namespace finger_tracking {
template <size_t N>
class LayoutMutator {
public:
    KeyboardLayout<N>& m_layout;
    void               swapKeys(LayoutKeyRef lhs, LayoutKeyRef rhs) {
        auto const& shape = m_layout.m_shape;
        std::swap(m_layout.m_layers[lhs.layer.layer].keys[shape.indexInLayer(lhs)],
                                m_layout.m_layers[rhs.layer.layer].keys[shape.indexInLayer(rhs)]);
    }
    void copyKeyTo(LayoutKeyRef src, LayoutKeyRef dst) {
        m_layout.m_layers[dst.layer.layer].keys[m_layout.m_shape.indexInLayer(dst)]
            = m_layout.keyAt(src);
    }

    bool isWritable(LayoutKeyRef src) const {
        return !src.layer.isModified() && !m_layout.isLockedAt(src);
    }
    bool canCopyTo(LayoutKeyRef dst) const {
        if (!isWritable(dst))
            return false;

        auto dstKey = m_layout.keyAt(dst);
        if (dstKey.isEmpty())
            return true;

        auto occurrences
            = ranges::count(m_layout.m_layers | ranges::views::transform(&KeyboardLayer<N>::keys)
                                | ranges::views::join,
                            dstKey);
        return occurrences > 1;
    }
    bool canSwapKeys(LayoutKeyRef lhs, LayoutKeyRef rhs) const {
        return isWritable(lhs) && isWritable(rhs);
    }
};
} // namespace finger_tracking

#endif // LAYOUT_MUTATOR_HPP
