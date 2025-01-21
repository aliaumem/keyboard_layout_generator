#ifndef LAYOUT_MUTATOR_HPP
#define LAYOUT_MUTATOR_HPP

#include "layout_generator/keyboard_layout.hpp"
#include <range/v3/view/transform.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/algorithm/count.hpp>

#include "layout_generator/layout_print_helpers.hpp"
#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

#include <random>

namespace finger_tracking {
template <size_t N>
class LayoutMutator {
public:
    LayoutMutator(KeyboardLayout<N>& layout)
        : m_layout{layout} {}

    void swapKeys(LayoutKeyRef lhs, LayoutKeyRef rhs) {
        auto const& shape = m_layout.m_shape;
        std::swap(m_layout.m_layers[lhs.layer.layer].keys[shape.indexInLayer(lhs)],
                  m_layout.m_layers[rhs.layer.layer].keys[shape.indexInLayer(rhs)]);
    }
    void copyKeyTo(LayoutKeyRef src, LayoutKeyRef dst) {
        m_layout.m_layers[dst.layer.layer].keys[m_layout.m_shape.indexInLayer(dst)]
            = m_layout.keyAt(src);
    }

    bool isWritable(LayoutKeyRef src) const {
        return !src.layer.isVirtual() && !m_layout.isLockedAt(src);
    }
    bool isKeyUnique(auto dstKey) const {
        return ranges::count(m_layout.m_layers | ranges::views::transform(&KeyboardLayer<N>::keys)
                                 | ranges::views::join,
                             dstKey)
            <= 1;
    }

    bool canCopyTo(LayoutKeyRef dst) const {
        if (!isWritable(dst))
            return false;

        auto dstKey = m_layout.keyAt(dst);
        if (dstKey.isEmpty())
            return true;

        return !isKeyUnique(dstKey);
    }
    bool canSwapKeys(LayoutKeyRef lhs, LayoutKeyRef rhs) const {
        return isWritable(lhs) && isWritable(rhs);
    }

    void performNSwapsOrCopies(size_t num, std::mt19937_64& generator) {
        uint16_t                            flagMean = std::numeric_limits<uint16_t>::max() / 2u;
        std::poisson_distribution<uint16_t> flagDistribution{static_cast<double>(flagMean)};
        std::uniform_int_distribution<uint16_t> layerIdDistribution{
            0, static_cast<uint16_t>(m_layout.m_layers.size() - 1)};
        std::uniform_int_distribution<uint16_t> indexInLayerDistribution{0, N - 1};

        for (size_t i = 0; i < num;) {
            auto lhsId = static_cast<uint8_t>(layerIdDistribution(generator));
            auto rhsId = static_cast<uint8_t>(layerIdDistribution(generator));
            auto flag  = flagDistribution(generator);

            LayerId lhsLayer{
                .layer = lhsId, .isShift = flag - flagMean > 150, .isAlt = flag - flagMean < -150};
            LayerId      rhsLayer{rhsId};
            LayoutKeyRef lhs{lhsLayer,
                             m_layout.m_shape.atIndex(indexInLayerDistribution(generator))};
            LayoutKeyRef rhs{rhsLayer,
                             m_layout.m_shape.atIndex(indexInLayerDistribution(generator))};

            /*fmt::println("{} {} -  '{}' {}   \t\t\tto\t\t\t '{}' {}", i,
                         (canSwapKeys(lhs, rhs) ? "swap"
                          : canCopyTo(rhs)      ? "cpLR"
                          : canCopyTo(lhs)      ? "cpRL"
                                                : "none"),
                         m_layout.keyAt(lhs), lhs, m_layout.keyAt(rhs), rhs);*/

            if (m_layout.keyAt(lhs).isEmpty() && m_layout.keyAt(rhs).isEmpty())
                continue;

            if (canSwapKeys(lhs, rhs))
                swapKeys(lhs, rhs);
            else if (canCopyTo(rhs))
                copyKeyTo(lhs, rhs);
            else if (canCopyTo(lhs))
                copyKeyTo(rhs, lhs);
            else
                continue;
            ++i;
        }
    }

private:
    KeyboardLayout<N>& m_layout;
};
} // namespace finger_tracking

#endif // LAYOUT_MUTATOR_HPP
