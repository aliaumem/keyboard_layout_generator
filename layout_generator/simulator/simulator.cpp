#include "simulator.hpp"

#include "layout_generator/keyboard_layout.hpp"
#include "finger_keyboard_mapping/keyboard/key_print_helpers.hpp"

#include <numeric>
#include <optional>
#include <ranges>
#include <format>

namespace finger_tracking {
float Simulator::computePenalties(std::vector<Quartad> const& quartads) const {
    return std::accumulate(
        quartads.begin(), quartads.end(), 0.f, [this](float acc, Quartad const& quartad) {
            auto seq = sequenceForQuartad(quartad);
            return std::accumulate(m_penalties.begin(), m_penalties.end(), 0.f,
                                   [&seq](float innerAcc, auto const& penalty) {
                                       return innerAcc + penalty(seq);
                                   })
                 + acc;
        });
}
void Simulator::emplaceKeyRefInSequence(KeyLayoutSequence& sequence, LayoutKeyRef keyRef,
                                        bool isPress) const {
    sequence.emplace_back(keyRef, m_layout.fingerFor(keyRef),
                          m_layout.keyAt(keyRef).second.center(), isPress);
}

void Simulator::insertLayoutChangeSequence(std::uint8_t fromLayer, KeyLayoutSequence& sequence,
                                           LayoutKeyRef keyRef) const {
    if (fromLayer != 0) {
        auto extraKeyRef = m_layout.layerTransitionKey(fromLayer);
        emplaceKeyRefInSequence(sequence, extraKeyRef, false);
    }
    if (keyRef.layer != 0) {
        auto extraKeyRef = m_layout.layerTransitionKey(keyRef.layer);
        emplaceKeyRefInSequence(sequence, extraKeyRef, true);
    }
}

KeyLayoutSequence Simulator::sequenceForKey(std::uint8_t fromLayer, Key const& key) const {
    KeyLayoutSequence sequence;

    auto keysMatch = [&key](auto const& pairs) -> bool { return pairs.first == key; };
    auto it        = std::ranges::find_if(m_layout, keysMatch);
    if (it == m_layout.end())
        throw std::invalid_argument{std::format("Key '{}' does not exist in layout", key)};

    auto keyRef = m_layout.toKeyRef(it);

    if (keyRef.layer != fromLayer)
        insertLayoutChangeSequence(fromLayer, sequence, keyRef);

    emplaceKeyRefInSequence(sequence, keyRef);

    return sequence;
}

KeyLayoutSequence Simulator::sequenceForQuartad(Quartad const& quartad) const {
    KeyLayoutSequence sequence;
    for (auto const& key : quartad | std::views::reverse) {
        if (!key.isValid())
            continue;

        auto keysMatch = [&key](auto const& pairs) -> bool { return pairs.first == key; };

        auto it = std::ranges::find_if(m_layout, keysMatch);
        if (it == m_layout.end())
            throw std::invalid_argument{std::format("Key '{}' does not exist in layout", key)};

        auto keyRef = m_layout.toKeyRef(it);
        sequence.emplace_back(keyRef, m_layout.fingerFor(keyRef),
                              m_layout.keyAt(keyRef).second.center());
    }
    return sequence;
}
} // namespace finger_tracking
